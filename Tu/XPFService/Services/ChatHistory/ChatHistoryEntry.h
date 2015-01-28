#ifndef __XPFSerivce_Services_ChatHistory_ChatHistoryEntry_h__
#define __XPFSerivce_Services_ChatHistory_ChatHistoryEntry_h__
#include "XPFService/XPFDefine.h"
#include "leveldb/db.h"
#include <vector>
#include "json11/json11.hpp"

using json11::Json;

NS_XPF_BEGIN

class ChatHistoryEntry {
public:
    ChatHistoryEntry(const std::string & with);
    ChatHistoryEntry(const char * with);
    virtual ~ChatHistoryEntry();
    Json getRecentN(int n);
    bool add(const Json & messages);
    bool updateReadStatus(const std::string & id, bool status);
protected:
    void fetchCount();
    std::string formatId(std::size_t intId);
private:
    leveldb::DB* _db;
    std::string _dbPath;
    std::string _withWhom;
    std::size_t _count;
    void init();
};

NS_XPF_END

#endif // __XPFSerivce_Services_ChatHistory_ChatHistoryEntry_h__
