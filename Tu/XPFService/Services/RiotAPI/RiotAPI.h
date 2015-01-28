#ifndef __Tu__RiotAPI__
#define __Tu__RiotAPI__
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "RiotDefine.h"
#include "json11/json11.hpp"
using json11::Json;

NS_RIOT_BEGIN

enum Region { BR, EUNE, EUW, KR, LAN, LAS, NA, OCE, RU, TR};
extern const char* regionStrings[];

struct APIURL {
    struct URLElement {
        enum Type {
            STRING, REGION, PARAMETER, VERSION
        } type;
        std::string value;
    };
    APIURL(const char* pattern, const char* version);
    std::vector<URLElement> elements;
    std::string getURL(Region region, const std::string& apiKey, const std::unordered_map<std::string, std::string> & params = {}) const;
    void makeBaseURL(Region region, char* store) const;
};

class RiotAPI {
public:
    enum EndPoint : short {
        CHAMPION_ALL,
        CHAMPION_BY_ID,
        GAME_RECENT,
        LEAGUE_BY_SUMMONER_IDS,
        LEAGUE_ENTRY_BY_SUMMONER_IDS,
        LEAGUE_BY_TEAM_IDS,
        LEAGUE_ENTRY_BY_TEAM_IDS,
        LEAGUE_CHALLENGER,
        LOL_STATIC_DATA_CHAMPION,
        LOL_STATUS,
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
    static const std::unordered_map<RiotAPI::EndPoint, APIURL, std::hash<short>> endPointTable;
    
    RiotAPI(const std::string & apiKey, Region region = NA);
    
    std::string getURL(EndPoint endPoint, const std::unordered_map<std::string, std::string> & params = {}) const;
    void setAPIKey(const std::string &key);
    void setRegion(Region region);
    
    class Champion {
    public:
        static const char* version;
    };
    Json getAllChampionStatus();
    Json getChampionStatus(long championId);
    
    class Game {
    public:
        static const char* version;
    };
    
    class League {
    public:
        static const char* version;
    };
    
    class LOLStaticData {
    public:
        static const char* version;
    };
    
    class LOLStatus {
    public:
        static const char* version;
    };
    
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
