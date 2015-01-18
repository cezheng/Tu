#include "Service.h"
#include "Services/ChatHistory/ChatHistoryService.h"

NS_XPF_BEGIN

const std::unordered_map<const char *, std::function<Data(const Data & params)>> Service::serviceEndpointTable = {
    {"ChatHistory/getRecentN", XPF_ENDPOINT(ChatHistoryService, getRecentN)},
    {"ChatHistory/add", XPF_ENDPOINT(ChatHistoryService, add)},
    {"ChatHistory/update", XPF_ENDPOINT(ChatHistoryService, update)}
};

NS_XPF_END