#ifndef __XPFSerivce_Service_h__
#define __XPFSerivce_Service_h__
#include "Base/Singleton.h"
#include "json11/json11.hpp"
#include <unordered_map>
#include <functional>
#include <stdexcept>

NS_XPF_BEGIN

using json11::Json;

class Service {
public:
    class API {
    public:
        API(Json && params) : _params(std::move(params)) {}
        virtual Json call();
        virtual void requireParams() const;
        virtual Json::shape getRequirements() const = 0;
    protected:
        virtual Json internalCall() = 0;
        Json _params;
    };

};

NS_XPF_END
#endif // __XPFSerivce_Service_h__
