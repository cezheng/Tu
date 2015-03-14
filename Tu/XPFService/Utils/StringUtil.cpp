#include "StringUtil.h"
#include <sstream>
NS_XPF_BEGIN
namespace StringUtil {
    
std::string join(const json11::Json::array & strings, const std::string & delim, int limit) {
    std::stringstream ss;
    if (limit == NO_LIMIT || limit > strings.size()) {
        limit = (int)strings.size();
    }
    int count = 0;
    for (int i = 0; i < limit; i++) {
        ss << strings.at(i).dump();
        if (++count == limit) {
            break;
        }
        ss << ',';
    }
    return ss.str();
}
    
}
NS_XPF_END