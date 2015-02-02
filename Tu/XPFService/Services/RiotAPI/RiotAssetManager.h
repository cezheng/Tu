#ifndef __Tu__RiotAssetManager__
#define __Tu__RiotAssetManager__

#include "RiotBase.h"
#include <string>
#include "json11/json11.hpp"

NS_RIOT_BEGIN

class RiotAssetManager {
public:
    typedef json11::Json VersionInfo;
    static const char * baseUrl;
    static const URLPattern versionUrl;
    static const char* profileImageUrlPattern;
    
    void updateVersionInfo(Region region = NA);
    std::string getProfileIconVersion(Region region = NA) const;
    std::string getProfileIconUrl(long id, Region region = NA);
    std::string getProfileIconPath(long id, Region region = NA);
protected:
private:
    std::unordered_map<Region, VersionInfo, std::hash<unsigned char>> _versionInfo;
};

NS_RIOT_END

#endif /* defined(__Tu__RiotAssetManager__) */
