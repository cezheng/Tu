#ifndef __Tu__RiotAssetManager__
#define __Tu__RiotAssetManager__

#include "RiotBase.h"
#include <string>
#include "json11/json11.hpp"
#include <future>

NS_RIOT_BEGIN

class RiotAssetManager {
public:
    enum Entry : unsigned char {
        ITEM = 0,
        RUNE = 1,
        MASTERY = 2,
        SUMMONER = 3,
        CHAMPION = 4,
        PROFILEICON = 5,
        MAP = 6,
        UI = 7,
        LANGUAGE = 8
    };
    
    void updateVersionInfo(Region region = NA);
    std::string getVersion(Entry entry, Region region = NA) const;
    std::string getSpriteImageUrl(const std::string & version, const std::string & spriteName, Region region = NA) const;
    std::string getSpriteImagePath(const std::string & version, const std::string & spriteName, Region region) const;
    
    std::string getProfileIconUrl(long id, Region region = NA) const;
    std::string getProfileIconPath(long id, Region region = NA) const;
    
    void updateChampionImageInfoList(Region region = NA);
    std::string getChampionImageUrl(const std::string & imageName, Region region = NA) const;
    std::string getChampionImagePath(const std::string & imageName, Region region = NA) const;
    json11::Json getChampionImageInfo(long id, Region region = NA) const;
protected:
private:
    std::unordered_map<Region, json11::Json, std::hash<unsigned char>> _versionInfo;
    std::unordered_map<Region, json11::Json, std::hash<unsigned char>> _championImageInfoList;
};

NS_RIOT_END

#endif /* defined(__Tu__RiotAssetManager__) */
