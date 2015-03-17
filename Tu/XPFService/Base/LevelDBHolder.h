#ifndef __Tu__LevelDBHolder__
#define __Tu__LevelDBHolder__

#include "XPFService/XPFDefine.h"
#include "json11/json11.hpp"
#include "leveldb/db.h"
#include "pthread.h"

NS_XPF_BEGIN

class LevelDBHolder {
public:
    LevelDBHolder(leveldb::DB * db = nullptr);
    virtual ~LevelDBHolder();
    bool set(const std::string & key, const std::string & value);
    bool get(const std::string & key, std::string & value);
    bool getJson(const std::string & key, json11::Json & json);
    bool getMutiple(json11::Json::object & object, std::size_t begin = 0, std::size_t n = UINT_MAX);
    bool setMutiple(const json11::Json::object & object);
    bool clear();
    bool remove(const std::string & key);
protected:
    virtual void init();
    virtual leveldb::Options levelDBOptions() const;
    leveldb::DB* _db;
    virtual std::string getLevelDBBasePath() const = 0;
    virtual std::string getLevelDBFileName() const = 0;
    virtual std::string getLevelDBFullPath() const;
    pthread_rwlock_t _rwlock;
};

NS_XPF_END

#endif /* defined(__Tu__LevelDBHolder__) */
