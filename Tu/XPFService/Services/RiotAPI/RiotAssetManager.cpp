#include "RiotAssetManager.h"
#include "RiotAPI.h"
#include "XPFService/Utils/Downloader.h"
#include "XPFService/Utils/Exception.h"
#include "XPFService/Utils/KVS.h"

using XPF::CurlRequest;
using XPF::CurlResponse;
using json11::Json;
NS_RIOT_BEGIN

const char * RiotAssetManager::baseUrl = "http://ddragon.leagueoflegends.com";
const URLPattern RiotAssetManager::versionUrl = {"/realms/{region}.json", NULL, {}, {}};
const char * RiotAssetManager::profileImageUrlPattern = "/cdn/{version}/img/profileicon/{icon_id}.png";

//general

void RiotAssetManager::updateVersionInfo(Region region) {
    CurlRequest request;
    std::string url = baseUrl + versionUrl.generateURL(region);
    auto res = request.request(url);
    std::string err;
    _versionInfo[region] = Json::parse(res.data, err);
}

//profileImage

std::string RiotAssetManager::getProfileIconVersion(Region region) const {
    XPF::REQUIRE(_versionInfo.find(region) != _versionInfo.end(), "riot version info not updated yet");
    return _versionInfo.at(region)["n"]["profileicon"].string_value();
}

std::string RiotAssetManager::getProfileIconUrl(long id, Region region) const {
    URLPattern urlPattern(profileImageUrlPattern, getProfileIconVersion(region).c_str(), {}, {});
    return baseUrl + urlPattern.generateURL(region, {{"icon_id", std::to_string(id)}});
}

std::string RiotAssetManager::getProfileIconPath(long id, Region region) const {
    auto downloader = XPF::Downloader::getInstance();
    std::string url = getProfileIconUrl(id, region);
    if (downloader->isDownloaded(url)) {
        return downloader->makeDownloadPath(url);
    }
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

Json RiotAssetManager::getChampionImageInfo(long id, Region region) const {
    XPF::REQUIRE(_championImageInfoList.find(region) != _championImageInfoList.end(), "riot champion image list not fetched yet");
    return _championImageInfoList.at(region)["data"][std::to_string(id)];
}

NS_RIOT_END