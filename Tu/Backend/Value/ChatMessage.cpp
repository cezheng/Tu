#include "ChatMessage.h"
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#define JSON_ADD_STRING(OBJ, KEY, VALUE) {rapidjson::Value value; value.SetString(VALUE.c_str(), OBJ.GetAllocator()); OBJ.AddMember(KEY, value, OBJ.GetAllocator());}
#define JSON_ADD_BOOL(OBJ, KEY, VALUE) {rapidjson::Value value; value.SetBool(VALUE); OBJ.AddMember(KEY, value, OBJ.GetAllocator());}

ChatMessage::ChatMessage(const std::string & message, const std::string & from, const std::string & to, time_t timestamp, bool read, const std::string & id) : _message(message), _from(from), _to(to), _timestamp(timestamp), _read(read), _id(id), _decoded(true), _encoded(false) {
}

ChatMessage::ChatMessage(const std::string & json, bool decode) : _json(json), _read(false), _decoded(decode), _encoded(true) {
    if (decode) {
        decodeJson();
    }
}

ChatMessage::ChatMessage(const char* json, bool decode) : _json(json), _read(false), _decoded(decode), _encoded(true){
    if (decode) {
        decodeJson();
    }
}

std::string ChatMessage::getJson() {
    if (!_encoded) {
        makeJsonString();
    }
    return _json;
}

std::string ChatMessage::getMessage() {
    if (!_decoded) {
        decodeJson();
    }
    return _message;
}

std::string ChatMessage::getSender() {
    if (!_decoded) {
        decodeJson();
    }
    return _from;
}

std::string ChatMessage::getReceiver() {
    if (!_decoded) {
        decodeJson();
    }
    return _to;
}

time_t ChatMessage::getTimestamp() {
    if (!_decoded) {
        decodeJson();
    }
    return _timestamp;
}

bool ChatMessage::isRead() {
    if (!_decoded) {
        decodeJson();
    }
    return _read;
}

bool ChatMessage::setRead(bool b) {
    if (!_decoded) {
        decodeJson();
    }
    if (_read == b) {
        return false;
    }
    _read = b;
    _encoded = false;
    return true;
}

std::string ChatMessage::getId() {
    if (!_decoded) {
        decodeJson();
    }
    return _id;
}

void ChatMessage::setId(const std::string& id) {
    if (_decoded) {
        decodeJson();
    }
    _id = id;
    _encoded = false;
}

void ChatMessage::makeJsonString() {
    rapidjson::Document jsonObject;
    jsonObject.SetObject();
    JSON_ADD_STRING(jsonObject, "message", _message);
    JSON_ADD_STRING(jsonObject, "from", _from);
    JSON_ADD_STRING(jsonObject, "to", _to);
    JSON_ADD_STRING(jsonObject, "timestamp", std::to_string(_timestamp));
    JSON_ADD_STRING(jsonObject, "read", std::to_string(_read));
    JSON_ADD_STRING(jsonObject, "id", _id);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jsonObject.Accept(writer);
    _json = buffer.GetString();
}

void ChatMessage::decodeJson() {
    rapidjson::Document document;
    document.Parse(_json.c_str());
    _timestamp = std::stol(document["timestamp"].GetString());
    _message = document["message"].GetString();
    _from = document["from"].GetString();
    _to = document["to"].GetString();
    _read = std::stoi(document["read"].GetString());
    _id = document["id"].GetString();
    _decoded = true;
}