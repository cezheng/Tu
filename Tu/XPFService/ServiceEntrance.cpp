#include "ServiceEntrance.h"
#include "Services/Chat/ChatService.h"
#include "Services/RiotAPI/RiotService.h"
#include "Services/Download/DownloadService.h"
#include "Services/Video/VideoService.h"
#include "Services/Chat/UnreadChatEntry.h"

NS_XPF_BEGIN

const std::unordered_map<const char *, std::function<Json(Json && params)>> ServiceEntrance::serviceEndpointTable {
    {"ChatHistory/getRecentN", XPF_API_CALL(ChatService, GetRecentN)},
    {"ChatHistory/add", XPF_API_CALL(ChatService, Add)},
    {"ChatHistory/update", XPF_API_CALL(ChatService, Update)},
    {"Download/download", XPF_API_CALL(DownloadService, Download)},
    {"Download/getDownloaded", XPF_API_CALL(DownloadService, GetDownloaded)},
    {"RiotAPI/summonerByNames", XPF_API_CALL(RiotService, GetSummonerByNames)},
    {"RiotAPI/serviceStatusByRegion", XPF_API_CALL(RiotService, GetServiceStatusByRegion)},
    {"RiotAPI/initSelfData", XPF_API_CALL(RiotService, InitSelfData)},
    {"Video/GetInfo", XPF_API_CALL(VideoService, GetInfo)},
    {"Chat/Unread", XPF_API_CALL(ChatService, Unread)},
    {"Chat/Read", XPF_API_CALL(ChatService, Read)}
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