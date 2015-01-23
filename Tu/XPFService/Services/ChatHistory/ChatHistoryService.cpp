#include "ChatHistoryService.h"

NS_XPF_BEGIN

ChatHistoryService::~ChatHistoryService() {
    for (auto & kv : _entries) {
        delete kv.second;
    }
}

ChatHistoryService* ChatHistoryService::constructInstance() {
    return new ChatHistoryService();
}

Data ChatHistoryService::getRecentN(const Data & params) {
    std::string withWhom = params.getString("withWhom");
    int n = params.getInt("amount");
    if (_entries.find(withWhom) == _entries.end()) {
        _entries[withWhom] = new ChatHistoryEntry(withWhom);
    }
    return _entries[withWhom]->getRecentN(n);
}

Data ChatHistoryService::add(const Data & params) {
    std::string withWhom = params.getString("withWhom");
    if (_entries.find(withWhom) == _entries.end()) {
        _entries[withWhom] = new ChatHistoryEntry(withWhom);
    }
    //TODO avoid data copy here
    if(_entries[withWhom]->add(params.getObjectJson("messages").c_str())) {
        return Data("{\"ok\":true}");
    }
    return Data("{\"ok\":false}");
}

Data ChatHistoryService::update(const Data & params) {
    return Data("");
}

NS_XPF_END