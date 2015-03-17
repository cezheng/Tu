#ifndef __Tu__StringUtil__
#define __Tu__StringUtil__

#include "XPFService/XPFDefine.h"
#include "json11/json11.hpp"

NS_XPF_BEGIN

#define NO_LIMIT -1

namespace StringUtil {
    
std::string join(const json11::Json::array & strings, const std::string & delim, int limit = NO_LIMIT);
std::string toLower(const std::string & str);
    
}

NS_XPF_END

#endif /* defined(__Tu__StringUtil__) */
