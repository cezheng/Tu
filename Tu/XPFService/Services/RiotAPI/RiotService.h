#ifndef __Tu__RiotService__
#define __Tu__RiotService__

#include "RiotAPI.h"
#include "RiotAssetManager.h"
#include "XPFService/Service.h"

NS_XPF_BEGIN

class RiotService : public Service, public Singleton<RiotService> {
public:
    RiotService(Riot::Region region = Riot::Region::NA);
    XPF_API_DECLARE(RiotService, GetSummonerByNames, ({
        {"names", Json::ARRAY}
    }));
    XPF_API_DECLARE(RiotService, GetServiceStatusByRegion, ({
        {"region", Json::STRING}
    }));

    XPF_STREAM_API_DECLARE(RiotService, GetMatchFeedByIds, ({
        {"ids", Json::ARRAY}
    }));
    
    XPF_STREAM_API_DECLARE(RiotService, GetProfileByIds, ({
        {"ids", Json::ARRAY}
    }));
    
    void setRegion(Riot::Region region);
protected:
    Riot::Region _region;
    Riot::RiotAssetManager _assetManager;
    bool saveSummonerInfo(const std::string & summonerId, const Json & res);
    Json getSummonerInfoCache(const std::string & summonerId);
};

NS_XPF_END

#endif /* defined(__Tu__RiotService__) */
