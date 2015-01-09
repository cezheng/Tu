#ifndef __Tu__RiotAPI__
#define __Tu__RiotAPI__
#include <string>
#include <vector>
#include <unordered_map>
#define NS_RIOT_BEGIN namespace Riot {
#define NS_RIOT_END }
NS_RIOT_BEGIN
enum Region { BR, EUNE, EUW, KR, LAN, LAS, NA, OCE, RU, TR};
extern const char* regionStrings[];

struct APIURL {
    struct URLElement {
        enum Type {
            STRING, REGION, PARAMETER
        } type;
        const char* value;
    };
    std::vector<URLElement> elements;
    std::string getURL(Region region, const std::string& apiKey, const std::unordered_map<std::string, std::string> & params = {}) const;
    void makeBaseURL(Region region, char* store) const;
};

class RiotAPI {
protected:
public:
    enum EndPoint : short {
        CHAMPION_ALL,
        CHAMPION_BY_ID,
        GAME_RECENT,
        LEAGUE_BY_SUMMONER_ID_SET,
        LEAGUE_ENTRY_BY_SUMMONER_ID_SET,
        LEAGUE_BY_TEAM_ID,
        LEAGUE_ENTRY_BY_TEAM_ID,
        LEAGUE_CHALLENGER,
        LOL_STATIC_DATA,
        LOL_STATUS,
        MATCH,
        MATCH_HISTORY,
        STATS,
        SUMMONER,
        TEAM
    };
    
    static const char* apiVersion[];
    static const std::unordered_map<RiotAPI::EndPoint, APIURL, std::hash<short>> endPointTable;

    RiotAPI(const std::string & apiKey, Region region = NA);
    std::string getURL(EndPoint endPoint) const;
    void setAPIKey(const std::string &key);
    
    class Champion {
    public:
        static const char* version;
    };
    std::vector<std::string> getAllChampionStatus();
    std::string getChampionStatus(long championId);
    
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
