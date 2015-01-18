#include "Data.h"
#include <stdexcept>
NS_XPF_BEGIN
Data::Data(const char * json, Mode mode) {
    if (json == NULL || *json == '\0') {
        if (mode == OBJECT) {
            _document.SetObject();
        } else if(mode == ARRAY) {
            _document.SetArray();
        }
        _document.SetObject();
    } else {
        _document.Parse(json);
        if (_document.IsObject()) {
            _mode = OBJECT;
        } else if(_document.IsArray()) {
            _mode = ARRAY;
        }
    }
}

Data::Data(Data && data) noexcept : _document(std::move(data._document)) {
}

Data::Data(std::vector<Data> && dataArray) {
    _document.SetArray();
    for (auto && data : dataArray) {
        _document.PushBack(data._document, _document.GetAllocator());
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

int Data::getInt(const char * key) const {
    if (_document[key].IsInt()) {
        return _document[key].GetInt();
    }
    throw std::logic_error("XPF::Data::getInt : value type error");
}

bool Data::getBool(const char * key) const {
    if (_document[key].IsBool()) {
        return _document[key].GetBool();
    }
    throw std::logic_error("XPF::Data::getBool : value type error");
}
std::size_t Data::getSizeT(const char * key) const {
    if (_document[key].IsInt64()) {
        return _document[key].GetInt64();
    }
    throw std::logic_error("XPF::Data::getSizeT : value type error");
}

const char * Data::getString(const char * key) const {
    if (_document[key].IsString()) {
        return _document[key].GetString();
    }
    throw std::logic_error("XPF::Data::getString : value type error");
}

std::string Data::getObjectJson(const char * key) const {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    _document[key].Accept(writer);
    return buffer.GetString();
}

void Data::pushBack(Data && data) {
    _document.PushBack(_document, _document.GetAllocator());
}

void Data::popBack() {
    _document.PopBack();
}

std::string Data::getObjectJson(int index) const {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    _document[index].Accept(writer);
    return buffer.GetString();
}

bool Data::isEmpty() const {
    return _document.Empty();
}

std::size_t Data::getSize() const {
    return _document.Size();
}

Data::Mode Data::getMode() const {
    return _mode;
}

std::string Data::getJson() {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    _document.Accept(writer);
    return buffer.GetString();
}

Data & Data::operator= (Data && data) {
    _document = std::move(data._document);
    return *this;
}

NS_XPF_END