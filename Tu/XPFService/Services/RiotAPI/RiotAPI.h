#ifndef __Tu__RiotAPI__
#define __Tu__RiotAPI__

#include <unordered_set>
#include "RiotBase.h"
#include "json11/json11.hpp"
using json11::Json;

NS_RIOT_BEGIN

class RiotAPI {
public:
    enum EndPoint : unsigned char {
        CHAMPION_ALL,
        CHAMPION_BY_ID,
        GAME_RECENT,
        LEAGUE_BY_SUMMONER_IDS,
        LEAGUE_ENTRY_BY_SUMMONER_IDS,
        LEAGUE_BY_TEAM_IDS,
        LEAGUE_ENTRY_BY_TEAM_IDS,
        LEAGUE_CHALLENGER,
        LOL_STATIC_DATA_CHAMPION_LIST,
        LOL_STATIC_DATA_CHAMPION_BY_ID,
        LOL_STATUS_SHARD_LIST,
        LOL_STATUS_SHARD_BY_REGION,
        MATCH,
        MATCH_HISTORY,
        STATS,
        SUMMONER_BY_NAMES,
        SUMMONER_BY_IDS,
        SUMMONER_MASTERIES_BY_IDS,
        SUMMONER_NAME_BY_IDS,
        SUMMONER_RUNES_BY_IDS,
        TEAM
    };
    
    static const char* apiVersion[];
    static const std::unordered_map<RiotAPI::EndPoint, URLPattern, std::hash<short>> endPointTable;
    
    RiotAPI(const std::string & apiKey, Region region = NA);
    std::string makeBaseUrl(EndPoint endPoint) const;
    std::string getURL(EndPoint endPoint, const std::unordered_map<std::string, std::string> & params = {}, bool appendApiKey = true) const;
    void setAPIKey(const std::string &key);
    void setRegion(Region region);
    
    class Champion {
    public:
        static const char* version;
    };
    Json getAllChampionStatus();
    Json getChampionStatusById(long championId);
    
    class Game {
    public:
        static const char* version;
    };
    
    Json getRecentGamesBySummonerId(const std::string & id);
    
    class League {
    public:
        static const char* version;
    };
    
    class LOLStaticData {
    public:
        static const char* version;
    };
    Json getChampionList();
    Json getChampionById(long championId);
    
    class LOLStatus {
    public:
        static const char* version;
    };
    Json getShardByRegion(std::string region);
    
    class Match {
    public:
        static const char* version;
    };
    
    class MatchHistory {
    public:
        static const char* version;
    };
    
    class Stats {
    public:
        static const char* version;
    };
    
    class Summoner {
    public:
        static const char* version;
    };
    Json getSummonerByIds(const Json & ids);
    Json getSummonerByNames(const Json & names);
    
    class Team {
    public:
        static const char* version;
    };
private:
    std::string _apiKey;
    Region _region;
};

NS_RIOT_END

#endif /* defined(__Tu__RiotAPI__) */
