#ifndef __Tu__RiotAssetManager__
#define __Tu__RiotAssetManager__

#include "RiotBase.h"
#include <string>
#include "json11/json11.hpp"

NS_RIOT_BEGIN

class RiotAssetManager {
public:
    static const char * baseUrl;
    static const URLPattern versionUrl;
    static const char* profileImageUrlPattern;
    
    void updateVersionInfo(Region region = NA);
    std::string getProfileIconVersion(Region region = NA) const;
    std::string getProfileIconUrl(long id, Region region = NA) const;
    std::string getProfileIconPath(long id, Region region = NA) const;
    
    void updateChampionImageInfoList(Region region = NA);
    json11::Json getChampionImageInfo(long id, Region region = NA) const;
protected:
private:
    std::unordered_map<Region, json11::Json, std::hash<unsigned char>> _versionInfo;
    std::unordered_map<Region, json11::Json, std::hash<unsigned char>> _championImageInfoList;
};

NS_RIOT_END

#endif /* defined(__Tu__RiotAssetManager__) */
