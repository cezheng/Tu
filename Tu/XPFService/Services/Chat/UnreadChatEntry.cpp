#include "UnreadChatEntry.h"
#include "KVS.h"

using json11::Json;

static const std::string kvsNameSpacePrefix("unread_chat_");
static const std::string kvsKeySummary("summary");
#define ID_LENGTH 4

NS_XPF_BEGIN
Json::object UnreadChatEntry::getUnreads(const std::string & me) {
    auto & kvs = (*KVS::getInstance())[kvsNameSpacePrefix + me];
    Json::object json;
    kvs.getMutiple(json);
    return json;
}

void UnreadChatEntry::addUnread(const std::string & me, const std::string & with, const Json::array & messages) {
    auto unread = this->getUnreads(me);
    std::size_t id = unread.size();
    Json::object object;
    for (auto & message : messages) {
        object[formatId(id++)] = message;
    }
    auto & kvs = (*KVS::getInstance())[kvsNameSpacePrefix + me];
    kvs.setMutiple(object);
}

void UnreadChatEntry::setRead(const std::string & me, const std::string & with) {
    auto & kvs = (*KVS::getInstance())[kvsNameSpacePrefix + me];
    kvs.remove(with);
}

void UnreadChatEntry::addSubscriber(const std::string & me, const std::string & identifier, NotificationCallback callback) {
    
}

void UnreadChatEntry::removeSubscriber(const std::string & me, const std::string & identifier) {
    
}

void UnreadChatEntry::eventLoop() {
    
}

std::string UnreadChatEntry::formatId(std::size_t id) {
    char buf[ID_LENGTH + 1];
    sprintf(buf, "%04lud", id);
    return buf;
}
NS_XPF_END