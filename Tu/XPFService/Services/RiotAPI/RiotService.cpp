#include "RiotService.h"
#include <sstream>
#include "Backend/Utils/CurlRequest.h"
#include "XPFService/Utils/FileUtil.h"
#include "XPFService/Utils/KVS.h"
#include <future>

NS_XPF_BEGIN

static std::string summonerCacheNameSpace = "summonerCache";

Json RiotService::GetSummonerByNames::internalCall() {
    return _service->_api.getSummonerByNames(_params["names"]);
}

Json RiotService::GetMatchFeedByIds::internalCall() {
    Json summoners = _service->_api.getSummonerByIds(_params["ids"]);
    Json::object diff;
    Json::array matches;
    std::string err;
    auto cmp = [](const Json & a, const Json & b) {
        return a["createDate"].number_value() > b["createDate"].number_value();
    };
    for (auto & kv : summoners.object_items()) {
        Json cache = _service->getSummonerInfoCache(kv.first);
        if (cache.is_null() || cache["revisionDate"].int_value() < kv.second["revisionDate"].int_value()) {
            diff[kv.first] = kv.second;
        } else {
            matches.insert(matches.end(), cache["games"].array_items().begin(), cache["games"].array_items().end());
        }
    }
    std::sort(matches.begin(), matches.end(), cmp);
    if (!matches.empty()) {
        _onRead(matches);
    }
    for (auto & kv : diff) {
        auto future = std::async(std::launch::async, [this, kv, cmp, &matches](){
            printf("kv %s\n", kv.first.c_str());
            Json newInfo = _service->_api.getRecentGamesBySummonerId(kv.first);
            if (!newInfo.is_null()) {
                Json::array modifiedMatches;
                for (auto & match : newInfo["games"].array_items()) {
                    auto item = match.object_items();
                    item["summonerId"] = kv.first;
                    item["name"] = kv.second["name"].string_value();
                    modifiedMatches.push_back(item);
                }
                matches.insert(matches.end(), modifiedMatches.begin(), modifiedMatches.end());
                sort(matches.begin(), matches.end(), cmp);
                
                Json::object updateObject(kv.second.object_items());
                updateObject["games"] = modifiedMatches;
                _service->saveSummonerInfo(kv.first, Json(updateObject));
            }
            _onRead(matches);
        });
    }
    return Json(matches);
}

Json RiotService::GetServiceStatusByRegion::internalCall() {
    Json status = _service->_api.getShardByRegion(_params["region"].string_value());
    return Json(status);
}

//----

RiotService::RiotService(std::string apiKey, Riot::Region region) : _apiKey(apiKey), _region(region), _api(apiKey, region) {
}

RiotService* RiotService::constructInstance() {
    return new RiotService("a6ef5db9-1e5f-4bc1-aad0-0cdeb42821e7");
}

void RiotService::setRegion(Riot::Region region) {
    _region = region;
}

void RiotService::setApiKey(std::string apiKey) {
    _apiKey = apiKey;
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
