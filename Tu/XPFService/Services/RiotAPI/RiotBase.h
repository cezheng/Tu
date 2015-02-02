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
    struct URLElement {
        enum Type {
            STRING, REGION, PARAMETER, VERSION
        } type;
        std::string value;
    };
    URLPattern(const char* pattern, const char* version = NULL);
    std::vector<URLElement> elements;
    std::string generateURL(Region region, const std::string& apiKey = "", const std::unordered_map<std::string, std::string> & params = {}) const;
};

NS_RIOT_END

#endif /* defined(__Tu__RiotBase__) */
