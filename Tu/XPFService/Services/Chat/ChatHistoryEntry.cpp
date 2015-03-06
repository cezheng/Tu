#include "ChatHistoryEntry.h"
#include "XPFService/Utils/FileUtil.h"
#include "leveldb/write_batch.h"
#include <sstream>
#include "UnreadChatEntry.h"

#define BUF_LEN 20

NS_XPF_BEGIN

ChatHistoryEntry::ChatHistoryEntry(const std::string me, const std::string & with): _me(me), _withWhom(with), _count(0) {
    this->init();
}

ChatHistoryEntry::ChatHistoryEntry(const char * const me, const char * const with): _me(me), _withWhom(with), _count(0) {
    this->init();
}

std::string ChatHistoryEntry::getLevelDBBasePath() const {
    return FileUtil::getInstance()->getWritablePath() + "XPF/ChatHistoryEntry/";
}

std::string ChatHistoryEntry::getLevelDBFileName() const {
    return _me + '-' + _withWhom + ".leveldb";
}

void ChatHistoryEntry::init() {
    LevelDBHolder::init();
    fetchCount();
}

Json ChatHistoryEntry::getRecentN(int n) {
    leveldb::Iterator* it = _db->NewIterator(leveldb::ReadOptions());
    std::size_t fetched = 0;
    Json::array result;
    std::string err;
    for (it->SeekToLast(); it->Valid() && fetched < n; it->Prev(), fetched++) {
        result.push_back(Json::parse(it->value().ToString(), err));
    }
    std::reverse(result.begin(), result.end());
    delete it;
    return result;
}

bool ChatHistoryEntry::add(const Json & messages) {
    printf("total %ld", _count);
    if(messages.is_object()) {
        std::string formattedId = formatId(_count + 1);
        auto message = messages.object_items();
        message["id"] = formattedId;
        leveldb::Status s = _db->Put(leveldb::WriteOptions(), formattedId, Json(message).dump());
        if(s.ok()) {
            ++_count;
            if (messages["from"] == _withWhom) {
                UnreadChatEntry::getInstance()->addUnread(_me, _withWhom, Json::array {message});
            }
            return true;
        }
        return false;
    } else if (messages.array_items().empty()) {
        return true;
    } else {
        std::size_t id = _count;
        leveldb::WriteBatch writeBatch;
        for (int i = 0; i < messages.array_items().size(); i++) {
            writeBatch.Put(formatId(++id), messages.array_items()[i].dump());
        }
        leveldb::Status s = _db->Write(leveldb::WriteOptions(), &writeBatch);
        bool success = s.ok();
        if(success) {
            _count = id;
        } else {
            fetchCount();
        }
        Json::array unread;
        for (auto & message : messages.array_items()) {
            if (messages["from"] == _withWhom) {
                unread.push_back(message);
            }
        }
        UnreadChatEntry::getInstance()->addUnread(_me, _withWhom, unread);
        return success;
    }
}

void ChatHistoryEntry::fetchCount() {
    leveldb::Iterator* it = _db->NewIterator(leveldb::ReadOptions());
    it->SeekToLast();
    if (it->Valid()) {
        _count = std::stoull(it->key().ToString());
    } else {
        _count = 0;
    }
    delete it;
}

std::string ChatHistoryEntry::formatId(std::size_t intId) {
    char buf[BUF_LEN];
    sprintf(buf, "%017zx", intId);
    return buf;
}

NS_XPF_END