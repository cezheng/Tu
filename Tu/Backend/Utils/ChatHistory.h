#ifndef __Tu__ChatHistory__
#define __Tu__ChatHistory__
#include "leveldb/db.h"
#include <vector>
#include "../Value/ChatMessage.h"

class ChatHistory {
public:
    ChatHistory(const std::string & with);
    ChatHistory(const char * with);
    virtual ~ChatHistory();
    std::size_t getRecentN(std::vector<ChatMessage> & result, int n);
    std::size_t getRecentN(std::vector<ChatMessage*> & result, int n);
    bool add(std::vector<ChatMessage> & messages);
    bool add(ChatMessage & message);
    bool update(ChatMessage& message);
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

#endif /* defined(__Tu__ChatHistory__) */
