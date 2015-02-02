#include "RiotAssetManager.h"
#include "XPFService/Utils/Downloader.h"
using XPF::CurlRequest;
using XPF::CurlResponse;
using json11::Json;
NS_RIOT_BEGIN

const char * RiotAssetManager::baseUrl = "http://ddragon.leagueoflegends.com";
const URLPattern RiotAssetManager::versionUrl = {"/realms/{region}.json"};
const char * RiotAssetManager::profileImageUrlPattern = "/cdn/{version}/img/profileicon/{icon_id}.png";


void RiotAssetManager::updateVersionInfo(Region region) {
    CurlRequest request;
    std::string url = baseUrl + versionUrl.getEndPointURL(region);
    auto res = request.request(url);
    std::string err;
    _versionInfo[region] = Json::parse(res.data, err);
}

std::string RiotAssetManager::getProfileIconVersion(Region region) const {
    if (_versionInfo.find(region) == _versionInfo.end()) {
        throw std::runtime_error("riot version info not updated yet");
    }
    printf("versions %s\n", _versionInfo.at(region).dump().c_str());
    return _versionInfo.at(region)["n"]["profileicon"].string_value();
}

std::string RiotAssetManager::getProfileIconUrl(long id, Region region) {
    URLPattern urlPattern(profileImageUrlPattern, getProfileIconVersion(region).c_str());
    return baseUrl + urlPattern.getEndPointURL(region, "", {{"icon_id", std::to_string(id)}});
}

std::string RiotAssetManager::getProfileIconPath(long id, Region region) {
    auto downloader = XPF::Downloader::getInstance();
    std::string url = getProfileIconUrl(id, region);
    if (downloader->isDownloaded(url)) {
        return downloader->makeDownloadPath(url);
    }
    return downloader->downloadUrl(url, std::string(regionStrings[region]) + "#profileIcon" + std::to_string(id));
}

NS_RIOT_END