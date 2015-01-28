#include "RiotAPI.h"
#include "Backend/Utils/CurlRequest.h"
#include <iostream>

NS_RIOT_BEGIN
#define MAX_URL_LEN 4096
#define TOKEN_BUFF_LEN 128

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

const char* regionStrings[] = {"br", "eune", "euw", "kr", "lan", "las", "na", "oce", "ru", "tr"};

const std::unordered_map<RiotAPI::EndPoint, APIURL, std::hash<short>> RiotAPI::endPointTable = {
    {CHAMPION_ALL,
        {"/api/lol/{region}/{version}/champion", Champion::version}},
    {CHAMPION_BY_ID,
        {"/api/lol/{region}/{version}/champion/{id}", Champion::version}},
    {GAME_RECENT,
        {"/api/lol/{region}/{version}/game/by-summoner/{summonerId}/recent", Game::version}},
    {LEAGUE_BY_SUMMONER_IDS,
        {"/api/lol/{region}/{version}/league/by-summoner/{summonerIds}", League::version}},
    {LEAGUE_ENTRY_BY_SUMMONER_IDS,
        {"/api/lol/{region}/{version}/league/by-summoner/{summonerIds}/entry", League::version}},
    {LEAGUE_BY_TEAM_IDS,
        {"/api/lol/{region}/{version}/league/by-team/{teamIds}", League::version}},
    {LEAGUE_ENTRY_BY_TEAM_IDS,
        {"/api/lol/{region}/{version}/league/by-team/{teamIds}/entry", League::version}},
    {LEAGUE_CHALLENGER,
        {"/api/lol/{region}/{version}/league/challenger", League::version}},
    {LOL_STATIC_DATA_CHAMPION,
        {"/api/lol/static-data/{region}/{version}/champion", LOLStaticData::version}},
    {LOL_STATUS_SHARD_LIST,
        {"/shards", LOLStatus::version}},
    {LOL_STATUS_SHARD_BY_REGION,
        {"/shards/{region}", LOLStatus::version}},
    {SUMMONER_BY_NAMES,
        {"/api/lol/{region}/{version}/summoner/by-name/{summonerNames}", Summoner::version}},
    {SUMMONER_BY_IDS,
        {"/api/lol/{region}/{version}/summoner/{summonerIds}", Summoner::version}},
    {SUMMONER_MASTERIES_BY_IDS,
        {"/api/lol/{region}/{version}/summoner/{summonerIds}/masteries", Summoner::version}},
    {SUMMONER_NAME_BY_IDS,
        {"/api/lol/{region}/{version}/summoner/{summonerIds}/name", Summoner::version}},
    {SUMMONER_RUNES_BY_IDS,
        {"/api/lol/{region}/{version}/summoner/{summonerIds}/runes", Summoner::version}},
};

APIURL::APIURL(const char* pattern, const char* version) {
    char buf[TOKEN_BUFF_LEN];
    char * start = (char*)pattern;
    char * end = start;
    while (true) {
        if (*end == '{' || *end == '\0') {
            if (start < end) {
                memcpy(buf, start, end - start);
                buf[end - start] = '\0';
                elements.push_back({URLElement::STRING, buf});
            }
            start = end + 1;
        } else if(*end == '}') {
            memcpy(buf, start, end - start);
            buf[end - start] = '\0';
            if (strcmp(buf, "version") == 0) {
                elements.push_back({URLElement::STRING, version});
            } else if (strcmp(buf, "region") == 0) {
                elements.push_back({URLElement::REGION, ""});
            } else {
                elements.push_back({URLElement::PARAMETER, buf});
            }
            start = end + 1;
        }
        if (*end == '\0') {
            break;
        }
        end++;
    }
}

std::string APIURL::getEndPointURL(Region region, const std::string& apiKey, const std::unordered_map<std::string, std::string> & params) const {
    char buf[MAX_URL_LEN];
    std::size_t at = 0;
    int written = 0;
    for (auto & e : elements) {
        switch (e.type) {
            case URLElement::STRING:
                written = sprintf(buf + at, "%s", e.value.c_str());
                break;
            case URLElement::REGION:
                written = sprintf(buf + at, "%s%s", regionStrings[region], e.value.c_str());
                break;
            case URLElement::PARAMETER:
                written = sprintf(buf + at, "%s", params.at(e.value).c_str());
                break;
            default:
                break;
        }
        if (written < 0) {
            std::runtime_error("sprintf failed in APIURL::getURL");
        } else {
            at += written;
        }
    }
    if(!apiKey.empty()) {
        sprintf(buf + at, "?api_key=%s", apiKey.c_str());
    }
    return buf;
}

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

RiotAPI::RiotAPI(const std::string & apiKey, Region region) : _apiKey(apiKey), _region(region) {
}

std::string RiotAPI::getURL(EndPoint endPoint, const std::unordered_map<std::string, std::string> & params, bool appendApiKey) const {
    return makeBaseUrl(endPoint) + endPointTable.at(endPoint).getEndPointURL(_region, appendApiKey ? _apiKey : "", params);
}

void RiotAPI::setAPIKey(const std::string &key) {
    _apiKey = key;
}

void RiotAPI::setRegion(Riot::Region region) {
    _region = region;
}

//Riot APIs

Json RiotAPI::getAllChampionStatus() {
    CurlRequest request;
    auto res = request.request(getURL(CHAMPION_ALL));
    return Json(res.data.c_str());
}

Json RiotAPI::getChampionStatus(long championId) {
    CurlRequest request;
    auto res = request.request(getURL(CHAMPION_BY_ID, {
        {"id", std::to_string(championId)}
    }));
    std::string err;
    return Json::parse(res.data, err);
}

Json RiotAPI::getSummonerByNames(const Json & names) {
    const static std::size_t limit = 40;
    std::size_t n = std::min(limit, names.array_items().size());
    std::size_t count = 0;
    std::stringstream namess;
    for (int i = 0; i < n; i++) {
        namess << names[i].string_value();
        if (++count == n) {
            break;
        }
        namess << ',';
    }
    CURL* curl = curl_easy_init();
    std::string namesStr = namess.str();
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

Json RiotAPI::getShardByRegion(std::string region) {
    CurlRequest request;
    auto res = request.request(getURL(LOL_STATUS_SHARD_BY_REGION, {
        {"region", region}
    }, false));
    std::string err;
    return Json::parse(res.data, err);
}

NS_RIOT_END