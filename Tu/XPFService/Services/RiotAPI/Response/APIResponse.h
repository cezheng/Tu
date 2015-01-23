#ifndef __RiotAPI_Response_APIResponse_h__
#define __RiotAPI_Response_APIResponse_h__

#include <string>
#include "RiotAPI/RiotDefine.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

NS_RIOT_BEGIN

class APIResponse {
public:
    APIResponse(const std::string& json);
    APIResponse(const char * json = "");
    std::string getJson() const;
protected:
    virtual void encodeJson() = 0;
    virtual void encodeJson(const rapidjson::Value & jsonObject);
    virtual void decodeJson();
    virtual void decodeJson(const rapidjson::Value & jsonObject) = 0;
    std::string _json;
};

NS_RIOT_END

#endif /* defined(__RiotAPI_Response_APIResponse_h__) */
