#include "RiotAPI.h"
#include "../Backend/Utils/CurlRequest.h"
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
    {RiotAPI::CHAMPION_ALL,
        {"/api/lol/{region}/{version}/champion", Champion::version}},
    {RiotAPI::CHAMPION_BY_ID,
        {"/api/lol/{region}/{version}/champion/{id}", Champion::version}},
    {RiotAPI::GAME_RECENT,
        {"/api/lol/{region}/{version}/game/by-summoner/{summonerId}/recent", Game::version}},
    {RiotAPI::LEAGUE_BY_SUMMONER_IDS,
        {"/api/lol/{region}/{version}/league/by-summoner/{summonerIds}", League::version}},
    {LEAGUE_ENTRY_BY_SUMMONER_IDS,
        {"/api/lol/{region}/{version}/league/by-summoner/{summonerIds}/entry", League::version}},
    {RiotAPI::LEAGUE_BY_TEAM_IDS,
        {"/api/lol/{region}/{version}/league/by-team/{teamIds}", League::version}},
    {RiotAPI::LEAGUE_ENTRY_BY_TEAM_IDS,
        {"/api/lol/{region}/{version}/league/by-team/{teamIds}/entry", League::version}},
    {RiotAPI::LEAGUE_CHALLENGER,
        {"/api/lol/{region}/{version}/league/challenger", League::version}},
    {RiotAPI::LOL_STATIC_DATA_CHAMPION,
        {"/api/lol/static-data/{region}/{version}/champion", LOLStaticData::version}},
    {RiotAPI::LOL_STATUS,
        {"", LOLStatus::version}},
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

std::string APIURL::getURL(Region region, const std::string& apiKey, const std::unordered_map<std::string, std::string> & params) const {
    char buf[MAX_URL_LEN];
    makeBaseURL(region, buf);
    std::size_t at = strlen(buf);
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
    sprintf(buf + at, "?api_key=%s", apiKey.c_str());
    return buf;
}

void APIURL::makeBaseURL(Region region, char* store) const {
    sprintf(store, "https://%s.api.pvp.net", regionStrings[region]);
}

RiotAPI::RiotAPI(const std::string & apiKey, Region region) : _apiKey(apiKey), _region(region) {
}

std::string RiotAPI::getURL(EndPoint endPoint, const std::unordered_map<std::string, std::string> & params) const {
    return endPointTable.at(endPoint).getURL(_region, _apiKey, params);
}

void RiotAPI::setAPIKey(const std::string &key) {
    _apiKey = key;
}

//Riot APIs

std::vector<ChampionStatus> RiotAPI::getAllChampionStatus() {
    CurlRequest request;
    request.request(getURL(CHAMPION_ALL));
    return {};
}

ChampionStatus RiotAPI::getChampionStatus(long championId) {
    CurlRequest request;
    CurlResponse res = request.request(getURL(CHAMPION_BY_ID, {{"id", std::to_string(championId)}}));
    return ChampionStatus(res.data);
}

std::unordered_map<std::string, SummonerInfo> RiotAPI::getSummonerByNames(std::unordered_set<std::string> & names) {
    const static std::size_t limit = 40;
    std::size_t n = std::min(limit, names.size());
    std::size_t count = 0;
    std::stringstream namess;
    for (auto & name : names) {
        namess << name;
        if (++count == n) {
            break;
        }
        namess << ',';
    }
    CurlRequest request;
    CurlResponse res = request.request(getURL(SUMMONER_BY_NAMES, {{"summonerNames", namess.str()}}));
    rapidjson::Document jsonObject;
    jsonObject.Parse(res.data.c_str());
    std::unordered_map<std::string, SummonerInfo> ret;
    count = 0;
    for (auto & name : names) {
        if (jsonObject[name.c_str()].IsObject()) {
            ret[name] = SummonerInfo(jsonObject[name.c_str()]);
        }
        if (++count == n) {
            break;
        }
    }
    return ret;
}

NS_RIOT_END