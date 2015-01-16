#ifndef __XPFSerivce_Data_h__
#define __XPFSerivce_Data_h__

#include "XPFDefine.h"
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

NS_XPF_BEGIN

class Data {
public:
    Data(const char * json);
    void set(const char * key, int value);
    void set(const char * key, std::size_t value);
    void set(const char * key, time_t value);
    void set(const char * key, bool value);
    void set(const char * key, const char * value);
    int getInt(const char * key);
    bool getBool(const char * key);
    std::size_t getSizeT(const char * key);
    const char * getString(const char * key);
    std::string getJson();
    
protected:
    rapidjson::Document _document;
};

NS_XPF_END

#endif
