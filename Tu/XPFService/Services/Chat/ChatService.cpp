#include "ChatService.h"

NS_XPF_BEGIN

ChatService::~ChatService() {
    for (auto & kv1 : _entries) {
        for (auto & kv2 : kv1.second) {
            delete kv2.second;
        }
    }
}

Json ChatService::GetRecentN::internalCall() {
    std::string withWhom = _params["withWhom"].string_value();
    std::string me = _params["me"].string_value();
    int n = _params["amount"].int_value();
    if (!_service->entryExists(me, withWhom)) {
        _service->_entries[me][withWhom] = new ChatHistoryEntry(me, withWhom);
    }
    return _service->_entries[me][withWhom]->getRecentN(n);
}

Json ChatService::Add::internalCall() {
    std::string withWhom = _params["withWhom"].string_value();
    std::string me = _params["me"].string_value();
    if (!_service->entryExists(me, withWhom)) {
        _service->_entries[me][withWhom] = new ChatHistoryEntry(me, withWhom);
    }
    if(_service->_entries[me][withWhom]->add(_params["messages"])) {
        return Json(Json::object {{"ok", true}});
    }
    return Json(Json::object {{"ok", true}});
}

Json ChatService::Update::internalCall() {
    return Json(Json::object {{"ok", true}});
}

Json ChatService::Unread::internalCall() {
    return Json(Json::object {{"ok", true}});
}

bool ChatService::entryExists(const std::string & me, const std::string & with) {
    return _entries.find(me) != _entries.end() && _entries[me].find(with) != _entries[me].end();
}

NS_XPF_END