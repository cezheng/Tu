#include "RiotBase.h"

NS_RIOT_BEGIN

const char* regionStrings[] = {"br", "eune", "euw", "kr", "lan", "las", "na", "oce", "ru", "tr"};
const std::unordered_map<const char*, Region> regionEnums {
    {"br", BR}, {"eune", EUNE}, {"euw", EUW},
    {"kr", KR}, {"lan", LAN}, {"las", LAS},
    {"na", NA}, {"oce", OCE}, {"ru", RU},
    {"tr", TR}
};

APIURL::APIURL(const char* pattern, const char* version) {
    char buf[TOKEN_BUFF_LEN];
    char * start = (char*)pattern;
    char * end = start;
    while (true) {
        if (*end == '{' || *end == '\0') {
            if (start < end) {
                memcpy(buf, start, end - start);
                buf[end - start] = '\0';
                elements.push_back({URLElement::STRING, buf});
            }
            start = end + 1;
        } else if(*end == '}') {
            memcpy(buf, start, end - start);
            buf[end - start] = '\0';
            if (version && strcmp(buf, "version") == 0) {
                elements.push_back({URLElement::STRING, version});
            } else if (strcmp(buf, "region") == 0) {
                elements.push_back({URLElement::REGION, ""});
            } else {
                elements.push_back({URLElement::PARAMETER, buf});
            }
            start = end + 1;
        }
        if (*end == '\0') {
            break;
        }
        end++;
    }
}

std::string APIURL::getEndPointURL(Region region, const std::string& apiKey, const std::unordered_map<std::string, std::string> & params) const {
    char buf[MAX_URL_LEN];
    std::size_t at = 0;
    int written = 0;
    for (auto & e : elements) {
        switch (e.type) {
            case URLElement::STRING:
                written = sprintf(buf + at, "%s", e.value.c_str());
                break;
            case URLElement::REGION:
                written = sprintf(buf + at, "%s%s", regionStrings[region], e.value.c_str());
                break;
            case URLElement::PARAMETER:
                written = sprintf(buf + at, "%s", params.at(e.value).c_str());
                break;
            default:
                break;
        }
        if (written < 0) {
            std::runtime_error("sprintf failed in APIURL::getURL");
        } else {
            at += written;
        }
    }
    if(!apiKey.empty()) {
        sprintf(buf + at, "?api_key=%s", apiKey.c_str());
    }
    return buf;
}

NS_RIOT_END