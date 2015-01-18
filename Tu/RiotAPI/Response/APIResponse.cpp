#include "RiotAPI/Response/APIResponse.h"

NS_RIOT_BEGIN

APIResponse::APIResponse(const std::string& json) : _json(json) {
    
}

APIResponse::APIResponse(const char * json) : _json(json) {
    
}
std::string APIResponse::getJson() const {
    return _json;
}

void APIResponse::encodeJson(const rapidjson::Value & jsonObject) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jsonObject.Accept(writer);
    _json = buffer.GetString();
}

void APIResponse::decodeJson() {
    rapidjson::Document document;
    document.Parse(_json.c_str());
    if (document.IsObject()) {
        decodeJson(document);
    }
}

NS_RIOT_END