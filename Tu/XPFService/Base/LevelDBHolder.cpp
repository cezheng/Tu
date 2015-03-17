#include "LevelDBHolder.h"
#include "XPFService/Utils/FileUtil.h"
#include "XPFService/Utils/Exception.h"
#include "leveldb/write_batch.h"

using json11::Json;

NS_XPF_BEGIN

LevelDBHolder::LevelDBHolder(leveldb::DB * db) : _db(db) {
    pthread_rwlock_init(&_rwlock, NULL);
}

LevelDBHolder::~LevelDBHolder() {
    pthread_rwlock_destroy(&_rwlock);
    delete _db;
}

bool LevelDBHolder::set(const std::string & key, const std::string & value) {
    pthread_rwlock_wrlock(&_rwlock);
    leveldb::Status s = _db->Put(leveldb::WriteOptions(), key, value);
    pthread_rwlock_unlock(&_rwlock);
    return s.ok();
}

bool LevelDBHolder::get(const std::string & key, std::string & value) {
    pthread_rwlock_rdlock(&_rwlock);
    leveldb::Status s = _db->Get(leveldb::ReadOptions(), key, &value);
    pthread_rwlock_unlock(&_rwlock);
    return s.ok();
}

bool LevelDBHolder::getJson(const std::string & key, Json & json) {
    pthread_rwlock_rdlock(&_rwlock);
    std::string value;
    leveldb::Status s = _db->Get(leveldb::ReadOptions(), key, &value);
    pthread_rwlock_unlock(&_rwlock);
    if(!s.ok()) {
        return false;
    }
    std::string err;
    json = Json::parse(value, err);
    return err.empty();
}

bool LevelDBHolder::getMutiple(json11::Json::object & object, std::size_t begin, std::size_t n) {
    pthread_rwlock_rdlock(&_rwlock);
    leveldb::Iterator* it = _db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid() && n; it->Next()) {
        if (begin > 0) {
            --begin;
            continue;
        }
        std::string err;
        auto item = Json::parse(it->value().ToString(), err);
        if (err.empty()) {
            object[it->key().ToString()] = item;
            --n;
        }
    }
    delete it;
    pthread_rwlock_unlock(&_rwlock);
    return true;
}

bool LevelDBHolder::setMutiple(const json11::Json::object &object) {
    leveldb::WriteBatch writeBatch;
    for (auto & kv : object) {
        writeBatch.Put(kv.first, kv.second.dump());
    }
    pthread_rwlock_wrlock(&_rwlock);
    leveldb::Status s = _db->Write(leveldb::WriteOptions(), &writeBatch);
    pthread_rwlock_unlock(&_rwlock);
    return s.ok();
}

bool LevelDBHolder::clear() {
    pthread_rwlock_wrlock(&_rwlock);
    leveldb::WriteBatch writeBatch;
    leveldb::Iterator* it = _db->NewIterator(leveldb::ReadOptions());
    it->SeekToFirst();
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        writeBatch.Delete(it->key());
    }
    delete it;
    leveldb::Status s = _db->Write(leveldb::WriteOptions(), &writeBatch);
    pthread_rwlock_unlock(&_rwlock);
    return s.ok();
}

bool LevelDBHolder::remove(const std::string &key) {
    pthread_rwlock_wrlock(&_rwlock);
    leveldb::Status s = _db->Delete(leveldb::WriteOptions(), key);
    pthread_rwlock_unlock(&_rwlock);
    return s.ok();
}

void LevelDBHolder::init() {
    pthread_rwlock_wrlock(&_rwlock);
    FileUtil::getInstance()->createDirectory(getLevelDBBasePath(), 0755);
    leveldb::Status status = leveldb::DB::Open(this->levelDBOptions(), getLevelDBFullPath(), &_db);
    REQUIRE(status.ok(), status.ToString());
    pthread_rwlock_unlock(&_rwlock);
}

leveldb::Options LevelDBHolder::levelDBOptions() const {
    leveldb::Options options;
    options.create_if_missing = true;
    return options;
}

std::string LevelDBHolder::getLevelDBFullPath() const {
    return getLevelDBBasePath() + getLevelDBFileName();
}

NS_XPF_END