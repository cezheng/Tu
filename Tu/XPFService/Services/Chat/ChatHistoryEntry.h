#ifndef __XPFSerivce_Services_ChatHistory_ChatHistoryEntry_h__
#define __XPFSerivce_Services_ChatHistory_ChatHistoryEntry_h__
#include "XPFService/XPFDefine.h"
#include "leveldb/db.h"
#include <vector>
#include "json11/json11.hpp"
#include "XPFService/Base/LevelDBHolder.h"

using json11::Json;

NS_XPF_BEGIN

class ChatHistoryEntry : public LevelDBHolder {
public:
    ChatHistoryEntry(const std::string me, const std::string & with);
    ChatHistoryEntry(const char* const me, const char * const with);
    virtual ~ChatHistoryEntry() {}
    Json getRecentN(int n);
    bool add(const Json & messages);
protected:
    void fetchCount();
    std::string formatId(std::size_t intId);
    virtual std::string getLevelDBBasePath() const;
    virtual std::string getLevelDBFileName() const;
private:
    //leveldb::DB* _db;
    //std::string _dbPath;
    std::string _me;
    std::string _withWhom;
    std::size_t _count;
    void init();
};

NS_XPF_END

#endif // __XPFSerivce_Services_ChatHistory_ChatHistoryEntry_h__
