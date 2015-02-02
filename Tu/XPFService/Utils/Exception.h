#ifndef __Tu__Exception__
#define __Tu__Exception__

#include "XPFService/XPFDefine.h"
#include <stdexcept>
#include <string>

#define REQUIRE(expr, msg) ExceptionUtil::require(expr, #expr, __FILE__, __LINE__, msg)

#define EXPECT(expr, msg) ExceptionUtil::expect(expr, #expr, __FILE__, __LINE__, msg)

NS_XPF_BEGIN

class Exception : public std::runtime_error {
public:
    Exception(const std::string & what) : std::runtime_error(what) {}
};

namespace ExceptionUtil {
    void require(bool result, const char* expr, const char* file, int line, const std::string & customMessage = "");
    void expect(bool result, const char* expr, const char* file, int line, const std::string & customMessage = "");
}

NS_XPF_END

#endif /* defined(__Tu__Exception__) */
