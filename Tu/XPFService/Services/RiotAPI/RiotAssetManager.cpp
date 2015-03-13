#include "RiotAssetManager.h"
#include "RiotAPI.h"
#include "XPFService/Utils/Downloader.h"
#include "XPFService/Utils/Exception.h"
#include "XPFService/Utils/KVS.h"

using XPF::CurlRequest;
using XPF::CurlResponse;
using json11::Json;
NS_RIOT_BEGIN

static const char * const entryKeys[] {
    "item", "rune", "mastery",
    "summoner", "champion", "profileicon",
    "map", "ui", "language"
};

static const char * const baseUrl = "http://ddragon.leagueoflegends.com";
static const URLPattern versionUrl {"/realms/{region}.json", NULL, {}, {}};
static const char * const profileImageUrlPattern = "/cdn/{version}/img/profileicon/{icon_id}.png";
static const char * const spriteImageUrlPattern = "/cdn/{version}/img/sprite/{sprite_name}";
static const char * const championImageUrlPattern = "/cdn/{version}/img/champion/{image_name}";

//general

void RiotAssetManager::updateVersionInfo(Region region) {
    CurlRequest request;
    std::string url = baseUrl + versionUrl.generateURL(region);
    auto res = request.request(url);
    std::string err;
    _versionInfo[region] = Json::parse(res.data, err);
}

std::string RiotAssetManager::getVersion(Entry entry, Region region) const {
    XPF::REQUIRE(_versionInfo.find(region) != _versionInfo.end(), "riot version info not updated yet");
    return _versionInfo.at(region)["n"][entryKeys[entry]].string_value();
}

std::string RiotAssetManager::getSpriteImageUrl(const std::string & version, const std::string & spriteName, Region region) const {
    URLPattern urlPattern(spriteImageUrlPattern, version.c_str(), {}, {});
    return baseUrl + urlPattern.generateURL(region, {
        {"sprite_name", spriteName}
    });
}

std::string RiotAssetManager::getSpriteImagePath(const std::string & version, const std::string & spriteName, Region region) const {
    auto downloader = XPF::Downloader::getInstance();
    std::string url = getSpriteImageUrl(version, spriteName, region);
    return downloader->downloadUrl(url, std::string(regionStrings[region]) + "#sprite#" + spriteName);
}

//profileImage

std::string RiotAssetManager::getProfileIconUrl(long id, Region region) const {
    URLPattern urlPattern(profileImageUrlPattern, getVersion(PROFILEICON, region).c_str(), {}, {});
    return baseUrl + urlPattern.generateURL(region, {{"icon_id", std::to_string(id)}});
}

std::string RiotAssetManager::getProfileIconPath(long id, Region region) const {
    auto downloader = XPF::Downloader::getInstance();
    std::string url = getProfileIconUrl(id, region);
    return downloader->downloadUrl(url, std::string(regionStrings[region]) + "#profileIcon" + std::to_string(id));
}

//champion image

void RiotAssetManager::updateChampionImageInfoList(Region region) {
    XPF::CurlRequest request;
    RiotAPI* api = RiotAPIHolder::getInstance()->getAPIByRegion(region);
    Json res = api->getChampionList({
        {"champData", "image"},
        {"dataById" , "true"}
    });
    printf("dump champion image list %s\n", res.dump().c_str());
    _championImageInfoList[region] = res;
}

std::string RiotAssetManager::getChampionImageUrl(const std::string & imageName, Region region) const {
    URLPattern urlPattern(championImageUrlPattern, getVersion(CHAMPION, region).c_str(), {}, {});
    return baseUrl + urlPattern.generateURL(region, {
        {"image_name", imageName}
    });
}

std::string RiotAssetManager::getChampionImagePath(const std::string & imageName, Region region) const {
    auto downloader = XPF::Downloader::getInstance();
    std::string url = getChampionImageUrl(imageName, region);
    return downloader->downloadUrl(url, std::string(regionStrings[region]) + "#championImage#" + imageName);
}

Json RiotAssetManager::getChampionImageInfo(long id, Region region) const {
    XPF::REQUIRE(_championImageInfoList.find(region) != _championImageInfoList.end(), "riot champion image list not fetched yet");
    Json::object info(_championImageInfoList.at(region)["data"][std::to_string(id)].object_items());
    std::string spriteName = info["image"]["sprite"].string_value();
    std::string imageName = info["image"]["full"].string_value();
    auto spriteFuture = std::async(std::launch::async, [this, spriteName, region](){
        return getSpriteImagePath(getVersion(CHAMPION), spriteName, region);
    });
    auto imageFuture = std::async(std::launch::async, [this, imageName, region](){
        return getChampionImagePath(imageName, region);
    });
    info["sprite_path"] = spriteFuture.get();
    info["image_path"] = imageFuture.get();
    return info;
}

//item
void RiotAssetManager::updateItemImageInfoList(Region region) {
    XPF::CurlRequest request;
    RiotAPI* api = RiotAPIHolder::getInstance()->getAPIByRegion(region);
    Json res = api->getItemList({
        {"itemListData", "image"}
    });
    printf("dump item image list %s\n", res.dump().c_str());
    _itemImageInfoList[region] = res;
}

Json RiotAssetManager::getItemImageInfo(const std::vector<int> ids, Region region) const {
    XPF::REQUIRE(_itemImageInfoList.find(region) != _itemImageInfoList.end(), "riot item image list not fetched yet");
    Json::array infoList;
    for (auto & id : ids) {
        if (id == 0) {
            infoList.push_back({});
            continue;
        }
        Json::object info(_itemImageInfoList.at(region)["data"][std::to_string(id)].object_items());
        std::string spriteName = info["image"]["sprite"].string_value();
        if (spriteName.empty()) {
            printf("omfg %d : %s\n", id, json11::Json(info).dump().c_str());
        }
        auto spriteFuture = std::async(std::launch::async, [this, spriteName, region](){
            return getSpriteImagePath(getVersion(ITEM), spriteName, region);
        });
        info["sprite_path"] = spriteFuture.get();
        infoList.push_back(info);
    }
    return infoList;
}


NS_RIOT_END