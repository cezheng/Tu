#ifndef __Tu__ChatMessage__
#define __Tu__ChatMessage__
#include <string>
class ChatMessage {
public:
    ChatMessage(const std::string & message, const std::string & from, const std::string & to, time_t timestamp, bool read, const std::string & id = "");
    ChatMessage(const std::string & json, bool decode = false);
    ChatMessage(const char * json, bool decode = false);
    std::string getJson();
    std::string getMessage();
    std::string getSender();
    std::string getReceiver();
    time_t getTimestamp();
    bool isRead();
    bool setRead(bool b);
    std::string getId();
    void setId(const std::string& id);
protected:
    void makeJsonString();
    void decodeJson();
private:
    std::string _message;
    std::string _from;
    std::string _to;
    time_t _timestamp;
    bool _read;
    std::string _json;
    bool _decoded;
    bool _encoded;
    std::string _id;
};

#endif /* defined(__Tu__ChatMessage__) */
