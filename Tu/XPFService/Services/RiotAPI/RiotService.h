#ifndef __Tu__RiotService__
#define __Tu__RiotService__

#include "RiotAPI.h"
#include "XPFService/Service.h"
#include "XPFService/Base/LevelDBHolder.h"

NS_XPF_BEGIN

class RiotService : public Service, public Singleton<RiotService> {
public:
    RiotService(std::string apiKey, Riot::Region region = Riot::Region::NA);
    XPF_SERVICE_API_DECLARE(RiotService, GetSummonerByNames, ({
        {"names", Json::ARRAY}
    }));
    XPF_SERVICE_API_DECLARE(RiotService, GetMatchFeedByNames, ({
        {"names", Json::ARRAY}
    }));
    XPF_SERVICE_API_DECLARE(RiotService, GetServiceStatusByRegion, ({
        {"region", Json::STRING}
    }));
    void setRegion(Riot::Region region);
    void setApiKey(std::string apiKey);
    static RiotService* constructInstance();
protected:
    std::string _apiKey;
    Riot::Region _region;
    Riot::RiotAPI _api;
};

NS_XPF_END

#endif /* defined(__Tu__RiotService__) */
