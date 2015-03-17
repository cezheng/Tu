#include "RiotService.h"
#include <sstream>
#include "XPFService/Utils/CurlRequest.h"
#include "XPFService/Utils/FileUtil.h"
#include "XPFService/Utils/StringUtil.h"
#include "XPFService/Utils/KVS.h"
#include "XPFService/Utils/ImageCache.h"
#include <future>

using Riot::RiotAPI;
using Riot::RiotAPIHolder;

NS_XPF_BEGIN

static std::string summonerCacheNameSpace = "summonerCache";

Json RiotService::GetSummonerByNames::internalCall() {
    RiotAPI* api = RiotAPIHolder::getInstance()->getAPIByRegion(_service->_region);
    return api->getSummonerByNames(_params["names"]);
}

Json RiotService::GetServiceStatusByRegion::internalCall() {
    RiotAPI* api = RiotAPIHolder::getInstance()->getAPIByRegion(_service->_region);
    Json status = api->getShardByRegion(_params["region"].string_value());
    return Json(status);
}

Json RiotService::InitSelfData::internalCall() {
    static const std::string selfDataNameSpace = "riotSelfData";
    static const std::string leagueEntryKey = "leagueEntry";
    static const std::string summonerInfoKey = "summonerInfo";
    static const std::string summonerIconPathKey = "summonerIconPath";
    
    std::string err;
    
    RiotAPI* api = RiotAPIHolder::getInstance()->getAPIByRegion(_service->_region);
    std::string name = _params["name"].string_value();
    auto & kvs = (*KVS::getInstance())[selfDataNameSpace];
    Json summonerInfo, leagueEntry;
    kvs.getJson(summonerInfoKey, summonerInfo);
    if (name.empty() && summonerInfo.has_shape({{"name", Json::STRING}}, err)) {
        name = summonerInfo["name"].string_value();
    }
    if (name.empty()) {
        return Json();
    }
    std::string lowerCaseName = StringUtil::toLower(name);
    Json fetchedSummonerInfo = api->getSummonerByNames(Json::array{name})[lowerCaseName];
    bool isUpdated = false;
    if (summonerInfo["name"].string_value() != name) {
        kvs.clear();
        isUpdated = true;
    }
    isUpdated = isUpdated || summonerInfo["revisionDate"].number_value() < fetchedSummonerInfo["revisionDate"].number_value();
    if (isUpdated) {
        int summonerId = fetchedSummonerInfo["id"].int_value();
        leagueEntry = api->getLeagueEntryBySummonerIds(Json::array{summonerId})[std::to_string(summonerId)];
        kvs.set(summonerInfoKey, fetchedSummonerInfo.dump());
        kvs.set(leagueEntryKey, leagueEntry.dump());
    } else {
        kvs.getJson(leagueEntryKey, leagueEntry);
    }
    std::string summonerIconPath = _service->_assetManager.getProfileIconPath(fetchedSummonerInfo["profileIconId"].int_value(), _service->_region);

    return Json({
        {summonerInfoKey, fetchedSummonerInfo},
        {leagueEntryKey, leagueEntry},
        {summonerIconPathKey, summonerIconPath}
    });
}

Json RiotService::GetMatchFeedByIds::internalCall() {
    // internal methods
    auto sortByDateCmp = [](const Json & a, const Json & b) -> bool {
        return a[0]["createDate"].number_value() > b[0]["createDate"].number_value();
    };
    auto insertMatchesToGroups = [](std::map<int, Json::array> & matchGroups, const Json::array & toInsert) {
        for (auto & match : toInsert) {
            matchGroups[match["gameId"].int_value()].push_back(match);
        }
    };
    
    auto matchGroupsToArray = [&sortByDateCmp](const std::map<int, Json::array> & matchGroups) -> Json::array {
        Json::array ret;
        for (auto rit = matchGroups.rbegin(); rit != matchGroups.rend(); rit++) {
            ret.push_back(rit->second);
        }
        std::sort(ret.begin(), ret.end(), sortByDateCmp);
        return ret;
    };
    
    //business logic
    
    //these ugly futures are temporary will be moved to somewhere else once I have time
    RiotAPI* api = RiotAPIHolder::getInstance()->getAPIByRegion(_service->_region);
    auto future1 = std::async(std::launch::async, [this, api]() {
        return api->getSummonerByIds(_params["ids"]);
    });
    auto future2 = std::async(std::launch::async, [this]() {
        _service->_assetManager.updateVersionInfo(_service->_region);
    });
    auto future3 = std::async(std::launch::async, [this]() {
        _service->_assetManager.updateChampionImageInfoList(_service->_region);
    });
    auto future4 = std::async(std::launch::async, [this]() {
        _service->_assetManager.updateItemImageInfoList(_service->_region);
    });
    Json summoners = future1.get();
    future2.get();
    future3.get();
    future4.get();
    
    ImageCache* imageCache = ImageCache::getInstance();
    Json::object diff;
    Json::array matches;
    std::map<int, Json::array> matchGroups;
    std::string err;
    
    for (auto & kv : summoners.object_items()) {
        Json cache = _service->getSummonerInfoCache(kv.first);
        if (cache.is_null() || cache["revisionDate"].number_value() < kv.second["revisionDate"].number_value()) {
            diff[kv.first] = kv.second;
        } else {
            insertMatchesToGroups(matchGroups, cache["games"].array_items());
        }
    }
    if (!matchGroups.empty()) {
        for (auto & matchKV : matchGroups) {
            for (auto & player : matchKV.second) {
                //cache champion images
                auto & championData = player["champData"];
                imageCache->putRiotSprite(championData["sprite_path"].string_value(), championData["image"], ISize(24, 24));
                //cache item images
                auto & itemsData = player["itemsData"].array_items();
                for (auto & item : itemsData) {
                    imageCache->putRiotSprite(item["sprite_path"].string_value(), item["image"]);
                }
            }
        }
        _onRead(matchGroupsToArray(matchGroups));
    }
    std::vector<std::future<void>> futures;
    for (auto & kv : diff) {
        futures.push_back(std::async(std::launch::async, [&]() {
            Json newInfo = api->getRecentGamesBySummonerId(kv.first);
            if (!newInfo.is_null()) {
                Json::array modifiedMatches;
                for (auto & match : newInfo["games"].array_items()) {
                    auto playerMatch = match.object_items();
                    playerMatch["summonerId"] = kv.first;
                    playerMatch["name"] = kv.second["name"].string_value();
                    playerMatch["champData"] = _service->_assetManager.getChampionImageInfo(playerMatch["championId"].int_value(), _service->_region);
                    std::vector<int> ids {
                        playerMatch["stats"]["item0"].int_value(),
                        playerMatch["stats"]["item1"].int_value(),
                        playerMatch["stats"]["item2"].int_value(),
                        playerMatch["stats"]["item3"].int_value(),
                        playerMatch["stats"]["item4"].int_value(),
                        playerMatch["stats"]["item5"].int_value(),
                        playerMatch["stats"]["item6"].int_value()
                    };
                    playerMatch["itemsData"] = _service->_assetManager.getItemImageInfo(ids, _service->_region);
                    modifiedMatches.push_back(playerMatch);
                    
                    //cache champion images
                    auto & championData = playerMatch["champData"];
                    imageCache->putRiotSprite(championData["sprite_path"].string_value(), championData["image"]);
                    //cache item images
                    auto & itemsData = playerMatch["itemsData"].array_items();
                    for (auto & item : itemsData) {
                        imageCache->putRiotSprite(item["sprite_path"].string_value(), item["image"], ISize(24, 24));
                    }
                }
                insertMatchesToGroups(matchGroups, modifiedMatches);
                
                Json::object updateObject(kv.second.object_items());
                updateObject["games"] = modifiedMatches;
                _service->saveSummonerInfo(kv.first, Json(updateObject));
            }
            _onRead(matchGroupsToArray(matchGroups));
        }));
    }
    for (auto & future : futures) {
        future.get();
    }
    
    return Json(matchGroupsToArray(matchGroups));
}

Json RiotService::GetProfileByIds::internalCall() {
    auto future1 = std::async(std::launch::async, [this]() {
        RiotAPI* api = RiotAPIHolder::getInstance()->getAPIByRegion(_service->_region);
        return api->getSummonerByIds(_params["ids"]);
    });
    auto future2 = std::async(std::launch::async, [this]() {
        _service->_assetManager.updateVersionInfo(_service->_region);
    });
    Json summoners = future1.get();
    future2.get();
    std::vector<std::future<void>> futures;
    for (auto & kv : summoners.object_items()) {
        futures.push_back(std::async(std::launch::async, [this, kv]() {
            Json::object info(kv.second.object_items());
            info["profileImagePath"] = _service->_assetManager.getProfileIconPath(kv.second["profileIconId"].int_value(), _service->_region);
            _onRead(Json(info));
        }));
    }
    for (auto & future : futures) {
        future.get();
    }
    return Json();
}

//----

RiotService::RiotService(Riot::Region region) : _region(region) {
}

void RiotService::setRegion(Riot::Region region) {
    _region = region;
}

bool RiotService::saveSummonerInfo(const std::string & summonerId, const Json & res) {
    std::string key = std::string(Riot::regionStrings[_region]) + "#" + summonerId;
    auto & kvs = (*KVS::getInstance());
    return kvs[summonerCacheNameSpace].set(key, res.dump());
}

Json RiotService::getSummonerInfoCache(const std::string & summonerId) {
    auto & kvs = (*KVS::getInstance());
    std::string key = std::string(Riot::regionStrings[_region]) + "#" + summonerId;
    std::string value;
    std::string err;
    if(kvs[summonerCacheNameSpace].get(key, value)) {
        return Json::parse(value, err);
    }
    return Json(nullptr);
}

NS_XPF_END
