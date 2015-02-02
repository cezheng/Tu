#ifndef __XPFSerivce_Services_ChatHistory_ChatHistoryService_h__
#define __XPFSerivce_Services_ChatHistory_ChatHistoryService_h__

#include "XPFService/Service.h"
#include "ChatHistoryEntry.h"
#include <unordered_map>
#include <vector>

NS_XPF_BEGIN

class ChatHistoryService : public Service, public Singleton<ChatHistoryService> {
public:
    virtual ~ChatHistoryService();
    //APIs
    XPF_SERVICE_API_DECLARE(ChatHistoryService, GetRecentN, ({
        {"withWhom", Json::STRING},
        {"amount", Json::NUMBER}
    }));
    XPF_SERVICE_API_DECLARE(ChatHistoryService, Add, ({
        {"withWhom", Json::STRING}
        //{"messages", Json::ARRAY}
    }));
    XPF_SERVICE_API_DECLARE(ChatHistoryService, Update, ({
        {"messages", Json::ARRAY}
    }));
    //TODO switching accounts will show chat history not for this account, resolve it


private:
    std::unordered_map<std::string, ChatHistoryEntry*> _entries;
};

NS_XPF_END

#endif // __XPFSerivce_Services_ChatHistory_ChatHistoryService_h__
