#ifndef __XPFSerivce_Services_ChatHistory_ChatHistoryEntry_h__
#define __XPFSerivce_Services_ChatHistory_ChatHistoryEntry_h__
#include "leveldb/db.h"
#include <vector>
#include "XPFService/Data.h"

NS_XPF_BEGIN

class ChatHistoryEntry {
public:
    ChatHistoryEntry(const std::string & with);
    ChatHistoryEntry(const char * with);
    virtual ~ChatHistoryEntry();
    std::size_t getRecentN(const Data & result, int n);
    bool add(const Data & messages);
    bool update(const Data & messages);
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
