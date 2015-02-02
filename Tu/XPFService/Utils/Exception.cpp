#include "Exception.h"
#include <iostream>
#include <sstream>

NS_XPF_BEGIN

namespace ExceptionUtil {
    void require(bool result, const char *expr, const char *file, int line, const std::string & customMessage) {
        if(result) return;
        std::stringstream msg;
        msg << "Requirement not met: " << expr << " in " << file << ":" << line;
        if (!customMessage.empty()) {
            msg << " : " << customMessage;
        }
        throw Exception(msg.str());
    };
    void expect(bool result, const char *expr, const char *file, int line, const std::string & customMessage) {
        if(result) return;
        std::stringstream msg;
        msg << "Expectation not met: " << expr << " in " << file << ":" << line;
        if (!customMessage.empty()) {
            msg << " : " << customMessage;
        }
        std::cerr << msg.str() << std::endl;
    };
}

NS_XPF_END