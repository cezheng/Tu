#include "RiotAPI.h"
#include "XPFService/Utils/CurlRequest.h"
#include "XPFService/Utils/Exception.h"
#include "XPFService/Utils/StringUtil.h"
#include <iostream>

using namespace XPF;

NS_RIOT_BEGIN

const char* RiotAPI::Champion::version = "v1.2";
const char* RiotAPI::Game::version = "v1.3";
const char* RiotAPI::League::version = "v2.5";
const char* RiotAPI::LOLStaticData::version = "v1.2";
const char* RiotAPI::LOLStatus::version = "v1.0";
const char* RiotAPI::Match::version = "v2.2";
const char* RiotAPI::MatchHistory::version = "v2.2";
const char* RiotAPI::Stats::version = "v1.3";
const char* RiotAPI::Summoner::version = "v1.4";
const char* RiotAPI::Team::version = "v2.4";

//endPointTable: Index of URLPattern of RiotAPI.
const std::unordered_map<RiotAPI::EndPoint, URLPattern, std::hash<short>> RiotAPI::endPointTable = {
    {CHAMPION_ALL, {
         "/api/lol/{region}/{version}/champion",
        Champion::version
    }},
    {CHAMPION_BY_ID, {
        "/api/lol/{region}/{version}/champion/{id}",
        Champion::version
    }},
    {GAME_RECENT, {
        "/api/lol/{region}/{version}/game/by-summoner/{summonerId}/recent",
        Game::version
    }},
    {LEAGUE_BY_SUMMONER_IDS, {
        "/api/lol/{region}/{version}/league/by-summoner/{summonerIds}",
        League::version
    }},
    {LEAGUE_ENTRY_BY_SUMMONER_IDS, {
        "/api/lol/{region}/{version}/league/by-summoner/{summonerIds}/entry",
        League::version
    }},
    {LEAGUE_BY_TEAM_IDS, {
        "/api/lol/{region}/{version}/league/by-team/{teamIds}",
        League::version
    }},
    {LEAGUE_ENTRY_BY_TEAM_IDS, {
        "/api/lol/{region}/{version}/league/by-team/{teamIds}/entry",
        League::version
    }},
    {LEAGUE_CHALLENGER, {
        "/api/lol/{region}/{version}/league/challenger",
        League::version
    }},
    {LOL_STATIC_DATA_CHAMPION_LIST, {
        "/api/lol/static-data/{region}/{version}/champion",
        LOLStaticData::version,
        {"locale", "version", "dataById", "champData"}
    }},
    {LOL_STATIC_DATA_CHAMPION_BY_ID, {
        "/api/lol/static-data/{region}/{version}/champion/{id}",
        LOLStaticData::version,
        {"locale", "version", "dataById", "champData"}
    }},
    {LOL_STATIC_DATA_ITEM_LIST, {
        "/api/lol/static-data/{region}/{version}/item",
        LOLStaticData::version,
        {"locale", "version", "itemListData"}
    }},
    {LOL_STATUS_SHARD_LIST, {
        "/shards",
        LOLStatus::version,
        {},
        {}
    }},
    {LOL_STATUS_SHARD_BY_REGION, {
        "/shards/{region}",
        LOLStatus::version,
        {},
        {}
    }},
    {SUMMONER_BY_NAMES, {
        "/api/lol/{region}/{version}/summoner/by-name/{summonerNames}",
        Summoner::version
    }},
    {SUMMONER_BY_IDS, {
        "/api/lol/{region}/{version}/summoner/{summonerIds}",
        Summoner::version
    }},
    {SUMMONER_MASTERIES_BY_IDS, {
        "/api/lol/{region}/{version}/summoner/{summonerIds}/masteries",
        Summoner::version
    }},
    {SUMMONER_NAME_BY_IDS, {
        "/api/lol/{region}/{version}/summoner/{summonerIds}/name",
        Summoner::version
    }},
    {SUMMONER_RUNES_BY_IDS, {
        "/api/lol/{region}/{version}/summoner/{summonerIds}/runes",
        Summoner::version
    }}
};

// Return RiotAPI base url address
std::string RiotAPI::makeBaseUrl(RiotAPI::EndPoint endpoint) const {
    char buf[MAX_URL_LEN];
    switch (endpoint) {
        case LOL_STATUS_SHARD_LIST:
        case LOL_STATUS_SHARD_BY_REGION:
            sprintf(buf, "http://status.leagueoflegends.com");
            break;
        default:
            sprintf(buf, "https://%s.api.pvp.net", regionStrings[_region]);
            break;
    }
    return buf;
}

// Construct
RiotAPI::RiotAPI(const std::string & apiKey, Region region) : _apiKey(apiKey), _region(region) {
}

// Return RiotAPI url address
std::string RiotAPI::getURL(EndPoint endPoint, URLPattern::Param && params, const URLPattern::Param & queryParams) const {
    auto pattern = endPointTable.at(endPoint);
    URLPattern::Param forwardParams(queryParams);
    if (pattern.requires("api_key")) {
        forwardParams["api_key"] = _apiKey;
    }
    return makeBaseUrl(endPoint) + endPointTable.at(endPoint).generateURL(_region, std::move(params), std::move(forwardParams));
}

void RiotAPI::setAPIKey(const std::string &key) {
    _apiKey = key;
}

void RiotAPI::setRegion(Riot::Region region) {
    _region = region;
}

//Riot APIs

/**
 * Get List of All Champions
 * @return List[ChampionDto]
 */
Json RiotAPI::getAllChampionStatus() {
    CurlRequest request;
    auto res = request.request(getURL(CHAMPION_ALL));
    return Json(res.data.c_str());
}

/**
 * Get Champion Information by Id
 * @param  Champion Id
 * @return ChampionDto
 */
Json RiotAPI::getChampionStatusById(long championId) {
    CurlRequest request;
    auto res = request.request(getURL(CHAMPION_BY_ID, {
        {"id", std::to_string(championId)}
    }));
    std::string err;
    return Json::parse(res.data, err);
}

/**
 * Get Recent 10 Games by Summoner Id
 * @param  Summoner Id
 * @return RecentGameDto [Set[GameDto], long]
 */
Json RiotAPI::getRecentGamesBySummonerId(const std::string & id) {
    CurlRequest request;
    auto res = request.request(getURL(GAME_RECENT, {
        {"summonerId", id}
    }));
    std::string err;
    return Json::parse(res.data, err);
}

/**
 * Get Max 40 Summoners Information by Name
 * @param  List[string]
 * @return Map[string, SummnerDto]
 */
Json RiotAPI::getSummonerByNames(const Json & names) {
    const static std::size_t limit = 40;
    std::size_t n = std::min(limit, names.array_items().size());
    std::string namesStr = StringUtil::join(names.array_items(), ",", (int)n);
    CURL* curl = curl_easy_init();

    if (curl) {
        char * output = curl_easy_escape(curl, namesStr.c_str(), (int)namesStr.length());
        if (output) {
            namesStr = output;
            curl_free(output);
        }
    }
    CurlRequest request;
    CurlResponse res = request.request(getURL(SUMMONER_BY_NAMES, {
        {"summonerNames", namesStr}
    }));
    std::string err;
    return Json::parse(res.data, err);
}

/**
 * Get Max 40 Summoners Information by Name
 * @param  List[string]
 * @return Map[string, SummonerDto]
 */
Json RiotAPI::getSummonerByIds(const Json & ids) {
    const static std::size_t limit = 40;
    std::size_t n = std::min(limit, ids.array_items().size());
    std::string idsStr = StringUtil::join(ids.array_items(), ",", (int)n);
    CurlRequest request;
    CurlResponse res = request.request(getURL(SUMMONER_BY_IDS, {
        {"summonerIds", idsStr}
    }));
    std::string err;
    return Json::parse(res.data, err);
}

/**
 * Get List of Statics Info of Champions
 * @param  Optional Param [local, version, dataById, champData]
 * @return ChampionListDto [Map[string, ChampionDto], string, ...]
 */
Json RiotAPI::getChampionList(const URLPattern::Param & optionalParams) {
    CurlRequest request;
    CurlResponse res = request.request(getURL(LOL_STATIC_DATA_CHAMPION_LIST, {}, optionalParams));
    std::string err;
    return Json::parse(res.data, err);
}

/**
 * Get Statics Info of Champion by Champion Id
 * @param  Champion Id, Optional Param [local, version, dataById, champData]
 * @return ChampionDto
 */
Json RiotAPI::getChampionById(long championId, const URLPattern::Param & optionalParams) {
    CurlRequest request;
    CurlResponse res = request.request(getURL(LOL_STATIC_DATA_CHAMPION_BY_ID, {
        {"id", std::to_string(championId)}
    }));
    std::string err;
    return Json::parse(res.data, err);
}

Json RiotAPI::getItemList(const URLPattern::Param & optionalParams) {
    CurlRequest request;
    CurlResponse res = request.request(getURL(LOL_STATIC_DATA_ITEM_LIST, {}, optionalParams));
    std::string err;
    return Json::parse(res.data, err);
}

Json RiotAPI::getShardByRegion(const std::string & region) {
    CurlRequest request;
    auto res = request.request(getURL(LOL_STATUS_SHARD_BY_REGION, {
        {"region", region}
    }));
    std::string err;
    return Json::parse(res.data, err);
}

//API Holder
RiotAPIHolder::~RiotAPIHolder() {
    for (auto & kv : _apis) {
        delete kv.second;
    }
}

RiotAPI* RiotAPIHolder::getAPIByRegion(Riot::Region region) {
    if (_apis.find(region) == _apis.end()) {
        RiotAPI* newInstance = new RiotAPI("a6ef5db9-1e5f-4bc1-aad0-0cdeb42821e7");
        REQUIRE(newInstance != nullptr, "RiotAPI allocation failed");
        _apis[region] = newInstance;
    }
    return _apis[region];
}

NS_RIOT_END
