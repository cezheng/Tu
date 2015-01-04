#include "ChatHistory.h"
#include "FileUtil.h"
#include "sys/stat.h"
#include "leveldb/write_batch.h"

ChatHistory::ChatHistory(const std::string & with): _db(nullptr), _withWhom(with), _count(0) {
    this->init();
}

ChatHistory::ChatHistory(const char * with): _db(nullptr), _withWhom(with), _count(0) {
    this->init();
}

ChatHistory::~ChatHistory() {
    delete _db;
}

void ChatHistory::init() {
    auto fu = FileUtil::getInstance();
    leveldb::Options options;
    options.create_if_missing = true;
    std::string basePath = fu->getWritablePath() + "chathistory/";
    _dbPath = basePath + _withWhom + ".leveldb";
    mkdir(basePath.c_str(), 0755);
    leveldb::Status status = leveldb::DB::Open(options, _dbPath, &_db);
    fetchCount();
    //TODO throw exception on not ok
}

std::size_t ChatHistory::getRecentN(std::vector<ChatMessage> & result, int n) {
    leveldb::Iterator* it = _db->NewIterator(leveldb::ReadOptions());
    std::size_t fetched = 0;
    std::size_t originalSize = result.size();
    for (it->SeekToLast(); it->Valid() && fetched < n; it->Prev(), fetched++) {
        result.push_back(it->value().ToString());
    }
    std::reverse(result.begin() + originalSize, result.end());
    return fetched;
}

std::size_t ChatHistory::getRecentN(std::vector<ChatMessage*> & result, int n) {
    leveldb::Iterator* it = _db->NewIterator(leveldb::ReadOptions());
    std::size_t fetched = 0;
    std::size_t originalSize = result.size();
    for (it->SeekToLast(); it->Valid() && fetched < n; it->Prev(), fetched++) {
        result.push_back(new ChatMessage(it->value().ToString()));
    }
    std::reverse(result.begin() + originalSize, result.end());
    return fetched;
}

bool ChatHistory::add(std::vector<ChatMessage> & messages) {
    if (messages.empty()) {
        return true;
    } else if(messages.size() == 1) {
        return add(messages.front());
    } else {
        std::size_t oldCount = _count;
        std::size_t id = _count;
        leveldb::WriteBatch writeBatch;
        for (auto it = messages.begin(); it != messages.end(); it++) {
            writeBatch.Put(formatId(++id), it->getJson());
        }
        leveldb::Status s = _db->Write(leveldb::WriteOptions(), &writeBatch);
        bool success = s.ok();
        if(success) {
            _count = id;
        } else {
            fetchCount();
        }
        if (oldCount < _count) {
            for (std::size_t i = 1; i <= _count - oldCount; i++) {
                messages[i].setId(formatId(oldCount + i));
            }
        }
        return success;
    }
}

bool ChatHistory::add(ChatMessage & message) {
    std::string formattedId = formatId(_count + 1);
    leveldb::Status s = _db->Put(leveldb::WriteOptions(), formattedId, message.getJson());
    if(s.ok()) {
        ++_count;
        message.setId(formattedId);
        return true;
    }
    return false;
}

bool ChatHistory::update(ChatMessage& message) {
    std::string value;
    leveldb::Status s = _db->Get(leveldb::ReadOptions(), message.getId(), &value);
    if (s.ok()) {
        if(message.getJson() != value) {
            s = _db->Put(leveldb::WriteOptions(), message.getId(), message.getJson());
        }
    }
    return s.ok();
}

bool ChatHistory::updateReadStatus(const std::string & id, bool status) {
    std::string value;
    leveldb::Status s = _db->Get(leveldb::ReadOptions(), id, &value);
    if (s.ok()) {
        ChatMessage message(value);
        if(message.setRead(status)) {
            s = _db->Put(leveldb::WriteOptions(), id, message.getJson());
        }
    }
    return s.ok();
}

void ChatHistory::fetchCount() {
    leveldb::Iterator* it = _db->NewIterator(leveldb::ReadOptions());
    it->SeekToLast();
    if (it->Valid()) {
        _count = std::stoll(it->key().ToString());
    } else {
        _count = 0;
    }
}

std::string ChatHistory::formatId(std::size_t intId) {
    char buf[22];
    sprintf(buf, "%020lu", intId);
    return buf;
}