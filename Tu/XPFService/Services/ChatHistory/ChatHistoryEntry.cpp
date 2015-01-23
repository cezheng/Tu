#include "ChatHistoryEntry.h"
#include "Backend/Utils/FileUtil.h"
#include "leveldb/write_batch.h"
#include <sstream>

NS_XPF_BEGIN

ChatHistoryEntry::ChatHistoryEntry(const std::string & with): _db(nullptr), _withWhom(with), _count(0) {
    this->init();
}

ChatHistoryEntry::ChatHistoryEntry(const char * with): _db(nullptr), _withWhom(with), _count(0) {
    this->init();
}

ChatHistoryEntry::~ChatHistoryEntry() {
    delete _db;
}

void ChatHistoryEntry::init() {
    auto fu = FileUtil::getInstance();
    leveldb::Options options;
    options.create_if_missing = true;
    std::string basePath = fu->getWritablePath() + "XPF/ChatHistoryEntry/";
    _dbPath = basePath + _withWhom + ".leveldb";
    mkdir(basePath.c_str(), 0755);
    leveldb::Status status = leveldb::DB::Open(options, _dbPath, &_db);
    fetchCount();
    //TODO throw exception on not ok
}

Data ChatHistoryEntry::getRecentN(int n) {
    leveldb::Iterator* it = _db->NewIterator(leveldb::ReadOptions());
    std::size_t fetched = 0;
    std::vector<std::string> result;
    for (it->SeekToLast(); it->Valid() && fetched < n; it->Prev(), fetched++) {
        result.push_back(it->value().ToString());
    }
    delete it;
    std::stringstream ss;
    ss << '[';
    for (auto rit = result.rbegin(); rit != result.rend(); rit++) {
        if (rit != result.rbegin()) {
            ss << ',';
        }
        ss << *rit;
    }
    ss << ']';
    return Data(ss.str().c_str());
}

bool ChatHistoryEntry::add(const Data & messages) {
    if(messages.getMode() == Data::OBJECT) {
        std::string formattedId = formatId(_count + 1);
        Data message(messages.getJson().c_str());
        message.set("id", formattedId.c_str());
        leveldb::Status s = _db->Put(leveldb::WriteOptions(), formattedId, message.getJson());
        if(s.ok()) {
            ++_count;
            return true;
        }
        return false;
    } else if (messages.isEmpty()) {
        return true;
    } else {
        std::size_t id = _count;
        leveldb::WriteBatch writeBatch;
        for (int i = 0; i < messages.getSize(); i++) {
            writeBatch.Put(formatId(++id), messages.getObjectJson(i));
        }
        leveldb::Status s = _db->Write(leveldb::WriteOptions(), &writeBatch);
        bool success = s.ok();
        if(success) {
            _count = id;
        } else {
            fetchCount();
        }
        return success;
    }
}

bool ChatHistoryEntry::updateReadStatus(const std::string & id, bool status) {
    std::string value;
    leveldb::Status s = _db->Get(leveldb::ReadOptions(), id, &value);
    if (s.ok()) {
        Data message(value.c_str());
        if(message.getBool("read") == false) {
            message.set("read", true);
            s = _db->Put(leveldb::WriteOptions(), id, message.getJson());
        }
    }
    return s.ok();
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
    char buf[22];
    sprintf(buf, "%020lu", intId);
    return buf;
}

NS_XPF_END