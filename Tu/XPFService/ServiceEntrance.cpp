#include "ServiceEntrance.h"
#include "Services/ChatHistory/ChatHistoryService.h"
#include "Services/RiotAPI/RiotService.h"
#include "Services/Download/DownloadService.h"
#include "Services/Video/VideoService.h"

NS_XPF_BEGIN

const std::unordered_map<const char *, std::function<Json(Json && params)>> ServiceEntrance::serviceEndpointTable {
    {"ChatHistory/getRecentN", XPF_API_CALL(ChatHistoryService, GetRecentN)},
    {"ChatHistory/add", XPF_API_CALL(ChatHistoryService, Add)},
    {"ChatHistory/update", XPF_API_CALL(ChatHistoryService, Update)},
    {"Download/download", XPF_API_CALL(DownloadService, Download)},
    {"Download/getDownloaded", XPF_API_CALL(DownloadService, GetDownloaded)},
    {"RiotAPI/summonerByNames", XPF_API_CALL(RiotService, GetSummonerByNames)},
    {"RiotAPI/serviceStatusByRegion", XPF_API_CALL(RiotService, GetServiceStatusByRegion)},
    {"Video/GetInfo", XPF_API_CALL(VideoService, GetInfo)}
};

const std::unordered_map<const char *, std::function<Json(Json&&, std::function<void(Json)>)>> ServiceEntrance::streamEndpointTable {
    {"RiotService/matchFeedByIds", XPF_STREAM_API_CALL(RiotService, GetMatchFeedByIds)},
    {"RiotService/profileByIds", XPF_STREAM_API_CALL(RiotService, GetProfileByIds)}
};

Json ServiceEntrance::call(const char * endpoint, Json && params) {
    try {
        return serviceEndpointTable.at(endpoint)(std::move(params));
    } catch (std::exception & e) {
        return Json(Json::object {{"ok", false}, {"message", e.what()}});
    }
}

Json ServiceEntrance::readStream(const char * endpoint, Json && params, std::function<void(Json)> && onRead) {
    try {
        return streamEndpointTable.at(endpoint)(std::move(params), std::move(onRead));
    } catch (std::exception & e) {
        return Json(Json::object {{"ok", false}, {"message", e.what()}});
    }
}

NS_XPF_END