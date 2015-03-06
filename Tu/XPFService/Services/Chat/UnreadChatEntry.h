#ifndef __Tu__UnreadChatEntry__
#define __Tu__UnreadChatEntry__

#include "json11/json11.hpp"
#include "XPFService/Utils/KVS.h"
#include <unordered_map>
#include <thread>

NS_XPF_BEGIN

class UnreadChatEntry : public Singleton<UnreadChatEntry> {
public:
    
    json11::Json::object getUnreads(const std::string & me);
    void addUnread(const std::string & me, const std::string & with, const json11::Json::array & messages);
    void setRead(const std::string & me, const std::string & with);

protected:
    std::string formatId(std::size_t id);
};

NS_XPF_END

#endif /* defined(__Tu__UnreadChatEntry__) */
