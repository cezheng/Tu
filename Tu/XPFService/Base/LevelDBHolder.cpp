#include "LevelDBHolder.h"
#include "XPFService/Utils/FileUtil.h"

NS_XPF_BEGIN

LevelDBHolder::~LevelDBHolder() {
    delete _db;
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