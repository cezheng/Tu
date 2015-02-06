#include "RiotService.h"
#include <sstream>
#include "XPFService/Utils/CurlRequest.h"
#include "XPFService/Utils/FileUtil.h"
#include "XPFService/Utils/KVS.h"
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
    Json summoners = future1.get();
    future2.get();
    future3.get();
    
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
        _onRead(matchGroupsToArray(matchGroups));
    }
    std::vector<std::future<void>> futures;
    for (auto & kv : diff) {
        futures.push_back(std::async(std::launch::async, [&]() {
            Json newInfo = api->getRecentGamesBySummonerId(kv.first);
            if (!newInfo.is_null()) {
                Json::array modifiedMatches;
                for (auto & match : newInfo["games"].array_items()) {
                    auto item = match.object_items();
                    item["summonerId"] = kv.first;
                    item["name"] = kv.second["name"].string_value();
                    item["champData"] = _service->_assetManager.getChampionImageInfo(item["championId"].int_value());
                    modifiedMatches.push_back(item);
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
            info["profileImagePath"] = _service->_assetManager.getProfileIconPath(kv.second["profileIconId"].int_value(), _service->_region);;
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
