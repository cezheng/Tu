#ifndef __XPFSerivce_Services_ChatHistory_ChatService_h__
#define __XPFSerivce_Services_ChatHistory_ChatService_h__

#include "XPFService/Service.h"
#include "ChatHistoryEntry.h"
#include <unordered_map>
#include <vector>

NS_XPF_BEGIN

class ChatService : public Service, public Singleton<ChatService> {
public:
    virtual ~ChatService();
    //APIs
    XPF_API_DECLARE(ChatService, GetRecentN, ({
        {"withWhom", Json::STRING},
        {"amount", Json::NUMBER}
    }));
    XPF_API_DECLARE(ChatService, Add, ({
        {"withWhom", Json::STRING}
        //{"messages", Json::ARRAY}
    }));
    XPF_API_DECLARE(ChatService, Update, ({
        {"messages", Json::ARRAY}
    }));
    XPF_API_DECLARE(ChatService, Unread, ({
    }));
    //TODO switching accounts will show chat history not for this account, resolve it


private:
    bool entryExists(const std::string & me, const std::string & with);
    std::unordered_map<std::string, std::unordered_map<std::string, ChatHistoryEntry*>> _entries;
};

NS_XPF_END

#endif // __XPFSerivce_Services_ChatHistory_ChatService_h__
