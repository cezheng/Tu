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

ServiceEntrance* ServiceEntrance::constructInstance() {
    auto ret = new ServiceEntrance();
    return ret;
}

Json ServiceEntrance::call(const char * endpoint, Json && params) {
    try {
        return serviceEndpointTable.at(endpoint)(std::move(params));
    } catch (std::exception & e) {
        return Json(Json::object {{"ok", false}, {"message", e.what()}});
    }
}

NS_XPF_END