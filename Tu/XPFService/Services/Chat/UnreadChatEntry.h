#ifndef __Tu__UnreadChatEntry__
#define __Tu__UnreadChatEntry__

#include "json11/json11.hpp"
#include "XPFService/Utils/KVS.h"
#include <unordered_map>
#include <thread>

NS_XPF_BEGIN

class UnreadChatEntry : public Singleton<UnreadChatEntry> {
public:
    typedef std::function<void(void*)> NotificationCallback;
    
    json11::Json::object getUnreads(const std::string & me);
    void addUnread(const std::string & me, const std::string & with, const json11::Json::array & messages);
    void setRead(const std::string & me, const std::string & with);
    void addSubscriber(const std::string & me, const std::string & identifier, NotificationCallback callback);
    void removeSubscriber(const std::string & me, const std::string & identifier);
    static void eventLoop();
protected:
    std::string formatId(std::size_t id);
    std::unordered_map<std::string, std::unordered_map<std::string, NotificationCallback>> _subscriberCallbacks;
    std::thread _eventThread;
};

NS_XPF_END

#endif /* defined(__Tu__UnreadChatEntry__) */
