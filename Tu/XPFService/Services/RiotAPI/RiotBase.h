#ifndef __Tu__RiotBase__
#define __Tu__RiotBase__

#include "RiotDefine.h"
#include <string>
#include <vector>
#include <unordered_map>

NS_RIOT_BEGIN

enum Region : unsigned char { BR, EUNE, EUW, KR, LAN, LAS, NA, OCE, RU, TR};
extern const char* regionStrings[];
extern const std::unordered_map<const char*, Region> regionEnums;

struct URLPattern {
    typedef std::unordered_map<std::string, std::string> Param;
    struct URLElement {
        enum Type {
            STRING, REGION, PARAMETER
        } type;
        std::string value;
    };
    URLPattern(const char* pattern, const char* version = NULL, const std::vector<std::string> & optionalParamKeys = {}, const std::vector<std::string> & requiredParamKeys = {"api_key"});
    std::vector<URLElement> elements;
    std::vector<std::string> optionalParamKeys;
    std::vector<std::string> requiredParamKeys;
    std::string generateURL(Region region, const Param & params = {}, const Param & queryParams = {}) const;
    bool requires(const std::string & key) const ;
};

NS_RIOT_END

#endif /* defined(__Tu__RiotBase__) */
