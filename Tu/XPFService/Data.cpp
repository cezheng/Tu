#include "Data.h"
#include <stdexcept>
NS_XPF_BEGIN
Data::Data(const char * json) {
    if (json == NULL || *json == '\0') {
        _document.SetObject();
    } else {
        _document.Parse(json);
    }
}

void Data::set(const char * key, int value) {
    rapidjson::Value k(key, _document.GetAllocator());
    rapidjson::Value v(value);
    _document.AddMember(k, v, _document.GetAllocator());
}

void Data::set(const char * key, std::size_t value) {
    rapidjson::Value k(key, _document.GetAllocator());
    rapidjson::Value v;
    v.SetInt64(value);
    _document.AddMember(k, v, _document.GetAllocator());
}

void Data::set(const char * key, bool value) {
    rapidjson::Value k(key, _document.GetAllocator());
    rapidjson::Value v(value);
    _document.AddMember(k, v, _document.GetAllocator());
}

void Data::set(const char * key, const char * value) {
    rapidjson::Value k(key, _document.GetAllocator());
    rapidjson::Value v(value, _document.GetAllocator());
    _document.AddMember(k, v, _document.GetAllocator());
}

int Data::getInt(const char * key) {
    if (_document[key].IsInt()) {
        return _document[key].GetInt();
    }
    throw std::logic_error("XPF::Data::getInt : value type error");
}

bool Data::getBool(const char * key) {
    if (_document[key].IsBool()) {
        return _document[key].GetBool();
    }
    throw std::logic_error("XPF::Data::getBool : value type error");
}
std::size_t Data::getSizeT(const char * key) {
    if (_document[key].IsInt64()) {
        return _document[key].GetInt64();
    }
    throw std::logic_error("XPF::Data::getSizeT : value type error");
}

const char * Data::getString(const char * key) {
    if (_document[key].IsString()) {
        return _document[key].GetString();
    }
    throw std::logic_error("XPF::Data::getString : value type error");
}

std::string Data::getJson() {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    _document.Accept(writer);
    return buffer.GetString();
}


NS_XPF_END