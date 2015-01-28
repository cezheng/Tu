#ifndef __Tu__LevelDBHolder__
#define __Tu__LevelDBHolder__

#include "XPFService/XPFDefine.h"
#include "leveldb/db.h"

NS_XPF_BEGIN

class LevelDBHolder {
public:
    virtual ~LevelDBHolder();
protected:
    virtual void init();
    leveldb::DB* _db;
    virtual std::string getLevelDBBasePath() const = 0;
    virtual std::string getLevelDBFileName() const = 0;
    virtual std::string getLevelDBFullPath() const;
};

NS_XPF_END

#endif /* defined(__Tu__LevelDBHolder__) */
