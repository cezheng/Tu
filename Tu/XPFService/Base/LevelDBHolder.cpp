#include "LevelDBHolder.h"
#include "XPFService/Utils/FileUtil.h"

NS_XPF_BEGIN

LevelDBHolder::LevelDBHolder(leveldb::DB * db) : _db(db) {
    
}

LevelDBHolder::~LevelDBHolder() {
    delete _db;
}

bool LevelDBHolder::set(const std::string & key, const std::string & value) {
    leveldb::Status s = _db->Put(leveldb::WriteOptions(), key, value);
    return s.ok();
}

bool LevelDBHolder::get(const std::string & key, std::string & value) {
    leveldb::Status s = _db->Get(leveldb::ReadOptions(), key, &value);
    return s.ok();
}

void LevelDBHolder::init() {
    leveldb::Options options;
    options.create_if_missing = true;
    FileUtil::getInstance()->createDirectory(getLevelDBBasePath(), 0755);
    leveldb::Status status = leveldb::DB::Open(options, getLevelDBFullPath(), &_db);
    if (!status.ok()) {
        throw std::runtime_error(status.ToString());
    }
}

std::string LevelDBHolder::getLevelDBFullPath() const {
    return getLevelDBBasePath() + getLevelDBFileName();
}

NS_XPF_END