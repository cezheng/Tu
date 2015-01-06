#ifndef __Tu__RiotAPI__
#define __Tu__RiotAPI__
#include <string>
#include <unordered_map>
#define NS_RIOT_BEGIN namespace Riot {
#define NS_RIOT_END }
NS_RIOT_BEGIN
class RiotAPI {
protected:
public:
    void setAPIKey(const std::string &key);
    enum EndPoint {CHAMPION, GAME, LEAGUE, LOL_STATIC_DATA, LOL_STATUS, MATCH, MATCH_HISTORY, STATS, SUMMONER, TEAM};
    enum Region { BR, EUNE, EUW, KR, LAN, LAS, NA, OCE, RU, TR};
    
    class Champion {
        static const char* version;
    };
    
    class Game {
        static const char* version;
    };
    
    class League {
        static const char* version;
    };
    
    class LOLStaticData {
        static const char* version;
    };
    
    class LOLStatus {
        static const char* version;
    };
    
    class Match {
        static const char* version;
    };
    
    class MatchHistoryAPI {
        static const char* version;
    };
    
    class StatsAPI {
        static const char* version;
    };
    
    class SummonerAPI {
        static const char* version;
    };
    
    class TeamAPI {
        static const char* version;
    };
protected:
    std::string makeUrl(const char* fmt...);
private:
    std::string _apiKey;
    Region _region;
};
NS_RIOT_END

#endif /* defined(__Tu__RiotAPI__) */
