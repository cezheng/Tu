#include "ServiceEntrance.h"
#include "Services/ChatHistory/ChatHistoryService.h"
#include "Services/RiotAPI/RiotService.h"
#include "Services/Download/DownloadService.h"

NS_XPF_BEGIN

const std::unordered_map<const char *, std::function<Json(Json && params)>> ServiceEntrance::serviceEndpointTable = {
    {"ChatHistory/getRecentN", XPF_API_VALUE(ChatHistoryService, GetRecentN)},
    {"ChatHistory/add", XPF_API_VALUE(ChatHistoryService, Add)},
    {"ChatHistory/update", XPF_API_VALUE(ChatHistoryService, Update)},
    {"Download/download", XPF_API_VALUE(DownloadService, Download)},
    {"Download/getDownloaded", XPF_API_VALUE(DownloadService, GetDownloaded)},
    {"RiotAPI/summonerByNames", XPF_API_VALUE(RiotService, GetSummonerByNames)},
    {"RiotAPI/serviceStatusByRegion", XPF_API_VALUE(RiotService, GetServiceStatusByRegion)}
};

const std::unordered_map<const char *, std::function<Json(Json&&, std::function<void(Json)>)>> ServiceEntrance::streamEndpointTable = {
    {"RiotService/matchFeedByIds", XPF_STREAM_API_VALUE(RiotService, GetMatchFeedByIds)}
};

Json ServiceEntrance::call(const char * endpoint, Json && params) {
    try {
        return serviceEndpointTable.at(endpoint)(std::move(params));
    } catch (std::exception & e) {
        return Json(Json::object {{"ok", false}, {"message", e.what()}});
    }
}

Json ServiceEntrance::readStream(const char * endpoint, Json && params, std::function<void(Json)> && onRead) {
    printf("b4 fuk! %s\n", params.dump().c_str());
    return streamEndpointTable.at(endpoint)(std::move(params), std::move(onRead));
}

NS_XPF_END