#ifndef __XPFSerivce_Services_ChatHistory_ChatHistoryEntry_h__
#define __XPFSerivce_Services_ChatHistory_ChatHistoryEntry_h__
#include "XPFService/XPFDefine.h"
#include "leveldb/db.h"
#include "leveldb/comparator.h"
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
    virtual leveldb::Options levelDBOptions() const;
    void fetchCount();
    std::string formatId(std::size_t intId);
    virtual std::string getLevelDBBasePath() const;
    virtual std::string getLevelDBFileName() const;
private:
    std::string _me;
    std::string _withWhom;
    std::size_t _count;
    void init();
};

class ChatHistoryKeyComparator : public leveldb::Comparator {
public:
    int Compare(const leveldb::Slice& a, const leveldb::Slice& b) const {
        std::size_t key1 = std::stoul(a.ToString(), nullptr, 16);
        std::size_t key2 = std::stoul(b.ToString(), nullptr, 16);
        if (key1 < key2) {
            return -1;
        }
        if (key1 > key2) {
            return 1;
        }
        return 0;
    }
    const char* Name() const { return "ChatHistoryKeyComparator"; }
    void FindShortestSeparator(std::string*, const leveldb::Slice&) const { }
    void FindShortSuccessor(std::string*) const { }
};

NS_XPF_END

#endif // __XPFSerivce_Services_ChatHistory_ChatHistoryEntry_h__
