#include "RiotAPI.h"
#include "../Backend/Utils/CurlRequest.h"
NS_RIOT_BEGIN
#define MAX_URL_LEN 4096

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
    {RiotAPI::CHAMPION_ALL, {
        { // /api/lol/{region}/v1.2/champion
            {APIURL::URLElement::STRING, "/api/lol/"},
            {APIURL::URLElement::REGION, "/"},
            {APIURL::URLElement::STRING, Champion::version},
            {APIURL::URLElement::STRING, "/champion"}
        }
    }},
    {RiotAPI::CHAMPION_BY_ID, {
        { // /api/lol/{region}/v1.2/champion/{id}
            {APIURL::URLElement::STRING, "/api/lol/"},
            {APIURL::URLElement::REGION, "/"},
            {APIURL::URLElement::STRING, Champion::version},
            {APIURL::URLElement::STRING, "/champion/"},
            {APIURL::URLElement::PARAMETER, "id"}
        }
    }},
    {RiotAPI::GAME_RECENT, {
        { // /api/lol/{region}/v1.3/game/by-summoner/{summonerId}/recent
            {APIURL::URLElement::STRING, "/api/lol/"},
            {APIURL::URLElement::REGION, "/"},
            {APIURL::URLElement::STRING, Game::version},
            {APIURL::URLElement::STRING, "/game/by-summoner/"},
            {APIURL::URLElement::PARAMETER, "summonerId"},
            {APIURL::URLElement::STRING, "/recent"},
        }
    }},
    {RiotAPI::LEAGUE_BY_SUMMONER_ID_SET, {
        {
            
        }
    }},
    {RiotAPI::LEAGUE_ENTRY_BY_SUMMONER_ID_SET, {
        {
            
        }
    }},
    {RiotAPI::LEAGUE_BY_TEAM_ID, {
        {
            
        }
    }},
    {RiotAPI::LEAGUE_ENTRY_BY_TEAM_ID, {
        {
            
        }
    }},
    {RiotAPI::LEAGUE_CHALLENGER, {
        {
            
        }
    }},
    {RiotAPI::LOL_STATIC_DATA, {
        {
            
        }
    }},
    {RiotAPI::LOL_STATUS, {
        {
            
        }
    }}
};

std::string APIURL::getURL(Region region, const std::string& apiKey, const std::unordered_map<std::string, std::string> & params) const {
    char buf[MAX_URL_LEN];
    makeBaseURL(region, buf);
    std::size_t at = strlen(buf);
    int written = 0;
    for (auto & e : elements) {
        switch (e.type) {
            case URLElement::STRING:
                written = sprintf(buf + at, "%s", e.value);
                break;
            case URLElement::REGION:
                written = sprintf(buf + at, "%s%s", regionStrings[region], e.value);
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

std::string RiotAPI::getURL(EndPoint endPoint) const {
    return endPointTable.at(endPoint).getURL(_region, _apiKey);
}

void RiotAPI::setAPIKey(const std::string &key) {
    _apiKey = key;
}

std::vector<std::string> RiotAPI::getAllChampionStatus() {
    CurlRequest request;
    request.request(getURL(CHAMPION_ALL));
    return {};
}

std::string RiotAPI::getChampionStatus(long championId) {
    CurlRequest request;
    request.request(getURL(CHAMPION_BY_ID));
    return "";
}

NS_RIOT_END