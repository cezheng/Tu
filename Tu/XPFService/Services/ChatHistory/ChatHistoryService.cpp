#include "ChatHistoryService.h"

NS_XPF_BEGIN

ChatHistoryService* ChatHistoryService::constructInstance() {
    auto ret = new ChatHistoryService();
    return ret;
}


Data ChatHistoryService::getRecentN(const Data & params) {
    std::string withWhom = params.getString("withWhom");
    int n = params.getInt("amount");
    if (_index.find(withWhom) != _index.end()) {
        _entries.push_back(ChatHistoryEntry(withWhom));
        _index[withWhom] = _entries.size() - 1;
    }
    return _entries[_index[withWhom]].getRecentN(n);
}

Data ChatHistoryService::add(const Data & params) {
    return Data("");
}

Data ChatHistoryService::update(const Data & params) {
    return Data("");
}

NS_XPF_END