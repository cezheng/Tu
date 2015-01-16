#ifndef __XPFSerivce_Services_ChatHistory_ChatHistoryService_h__
#define __XPFSerivce_Services_ChatHistory_ChatHistoryService_h__

#include "XPFService/Service.h"
#include "ChatHistoryEntry.h"
#include <unordered_map>
#include <vector>

NS_XPF_BEGIN

class ChatHistoryService : public Service, public Singleton<ChatHistoryService>  {
public:
    Data getRecentN(const Data & params);
    bool add(const Data & params);
    bool update(const Data & params);
private:
    std::unordered_map<std::string, std::size_t> _index;
    std::vector<ChatHistoryEntry> _entries;
};

NS_XPF_END

#endif // __XPFSerivce_Services_ChatHistory_ChatHistoryService_h__
