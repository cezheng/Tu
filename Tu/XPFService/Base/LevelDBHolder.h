#ifndef __Tu__LevelDBHolder__
#define __Tu__LevelDBHolder__

#include "XPFService/XPFDefine.h"
#include "leveldb/db.h"

NS_XPF_BEGIN

class LevelDBHolder {
public:
    LevelDBHolder(leveldb::DB * db = nullptr);
    virtual ~LevelDBHolder();
    bool set(const std::string & key, const std::string & value);
    bool get(const std::string & key, std::string & value);
protected:
    virtual void init();
    leveldb::DB* _db;
    virtual std::string getLevelDBBasePath() const = 0;
    virtual std::string getLevelDBFileName() const = 0;
    virtual std::string getLevelDBFullPath() const;
};

NS_XPF_END

#endif /* defined(__Tu__LevelDBHolder__) */
