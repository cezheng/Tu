#include "ChatHistoryService.h"

NS_XPF_BEGIN

ChatHistoryService::~ChatHistoryService() {
    for (auto & kv : _entries) {
        delete kv.second;
    }
}

Json ChatHistoryService::GetRecentN::internalCall() {
    std::string withWhom = _params["withWhom"].string_value();
    int n = _params["amount"].int_value();
    if (_service->_entries.find(withWhom) == _service->_entries.end()) {
        _service->_entries[withWhom] = new ChatHistoryEntry(withWhom);
    }
    return _service->_entries[withWhom]->getRecentN(n);
}

Json ChatHistoryService::Add::internalCall() {
    std::string withWhom = _params["withWhom"].string_value();
    if (_service->_entries.find(withWhom) == _service->_entries.end()) {
        _service->_entries[withWhom] = new ChatHistoryEntry(withWhom);
    }
    if(_service->_entries[withWhom]->add(_params["messages"])) {
        return Json(Json::object {{"ok", true}});
    }
    return Json(Json::object {{"ok", true}});
}

Json ChatHistoryService::Update::internalCall() {
    return Json(Json::object {{"ok", true}});
}

NS_XPF_END