#include "UnreadChatEntry.h"
#include "KVS.h"
#include <future>

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
    auto & kvs = (*KVS::getInstance())[kvsNameSpacePrefix + me];
    Json unread;
    kvs.getJson(with, unread);
    std::size_t id = unread.object_items().size();
    Json::object object(unread.object_items());
    printf("ok0 %s\n", json11::Json(object).dump().c_str());

    for (auto & message : messages) {
        object[formatId(id++)] = message;
    }
    printf("ok %s\n", json11::Json(object).dump().c_str());
    kvs.set(with, Json(object).dump());
}

void UnreadChatEntry::setRead(const std::string & me, const std::string & with) {
    auto & kvs = (*KVS::getInstance())[kvsNameSpacePrefix + me];
    kvs.remove(with);
}

std::string UnreadChatEntry::formatId(std::size_t id) {
    char buf[ID_LENGTH + 1];
    sprintf(buf, "%04ld", id);
    return buf;
}
NS_XPF_END