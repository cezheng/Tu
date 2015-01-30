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
    protected:
        virtual Json internalCall() = 0;
        const Json::shape _requirements;
        Json _params;
    };
    class StreamAPI : public API {
    public:
        typedef std::function<void(Json)> OnReadCallBack;
        StreamAPI(Json && params, OnReadCallBack && callback) : API(std::move(params)), _onRead(std::move(callback)) {}
    protected:
        OnReadCallBack _onRead;
    };
};

NS_XPF_END
#endif // __XPFSerivce_Service_h__
