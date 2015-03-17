#include "StringUtil.h"
#include <sstream>
#include <algorithm>
NS_XPF_BEGIN
namespace StringUtil {
    
std::string join(const json11::Json::array & strings, const std::string & delim, int limit) {
    std::stringstream ss;
    if (limit == NO_LIMIT || limit > strings.size()) {
        limit = (int)strings.size();
    }
    int count = 0;
    for (int i = 0; i < limit; i++) {
        if (strings.at(i).is_string()) {
            ss << strings.at(i).string_value();
        } else {
            ss << strings.at(i).dump();
        }
        if (++count == limit) {
            break;
        }
        ss << ',';
    }
    return ss.str();
}
    
std::string toLower(const std::string & str) {
    std::string ret(str);
    std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
    return ret;
}
    
}
NS_XPF_END