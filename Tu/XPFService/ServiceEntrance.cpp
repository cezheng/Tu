#include "ServiceEntrance.h"
#include "Services/ChatHistory/ChatHistoryService.h"
#include "Services/RiotAPI/RiotAPI.h"
#include "Services/Download/DownloadService.h"


NS_XPF_BEGIN

const std::unordered_map<const char *, std::function<Data(const Data & params)>> ServiceEntrance::serviceEndpointTable = {
    {"ChatHistory/getRecentN", XPF_ENDPOINT(ChatHistoryService, getRecentN)},
    {"ChatHistory/add", XPF_ENDPOINT(ChatHistoryService, add)},
    {"ChatHistory/update", XPF_ENDPOINT(ChatHistoryService, update)},
    {"RiotAPI/summonerByNames", XPF_ENDPOINT_INSTANCE(Riot::RiotAPI, getSummonerByNames, Riot::RiotAPI("a6ef5db9-1e5f-4bc1-aad0-0cdeb42821e7"))},
    {"Download/download", XPF_ENDPOINT(DownloadService, download)},
    {"Download/getDownloaded", XPF_ENDPOINT(DownloadService, getDownloaded)}
};

ServiceEntrance* ServiceEntrance::constructInstance() {
    auto ret = new ServiceEntrance();
    return ret;
}

Data ServiceEntrance::call(const char * endpoint, const Data & params) {
    return std::move(serviceEndpointTable.at(endpoint)(params));
}

NS_XPF_END