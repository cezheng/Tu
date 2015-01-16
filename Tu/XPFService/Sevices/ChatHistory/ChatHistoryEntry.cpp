#include "ChatHistoryEntry.h"
#include "Backend/Utils/FileUtil.h"
#include "sys/stat.h"
#include "leveldb/write_batch.h"

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
    std::string basePath = fu->getWritablePath() + "ChatHistoryEntry/";
    _dbPath = basePath + _withWhom + ".leveldb";
    mkdir(basePath.c_str(), 0755);
    leveldb::Status status = leveldb::DB::Open(options, _dbPath, &_db);
    fetchCount();
    //TODO throw exception on not ok
}

std::size_t ChatHistoryEntry::getRecentN(Data & result, int n) {
    return 0;
}

bool ChatHistoryEntry::add(const Data & messages) {
    return true;
}

bool ChatHistoryEntry::update(const Data & messages) {
    return true;
}

bool ChatHistoryEntry::updateReadStatus(const std::string & id, bool status) {
    std::string value;
    leveldb::Status s = _db->Get(leveldb::ReadOptions(), id, &value);
    if (s.ok()) {
        Data message(value);
        if(message.set("read", true)) {
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
}

std::string ChatHistoryEntry::formatId(std::size_t intId) {
    char buf[22];
    sprintf(buf, "%020lu", intId);
    return buf;
}

NS_XPF_END