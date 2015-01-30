#ifndef __XPFSerivce_ServiceEntrance_h__
#define __XPFSerivce_ServiceEntrance_h__

#include "Base/Singleton.h"
//#include "XPFService/Data.h"
#include "json11/json11.hpp"
#include <unordered_map>
#include <functional>

using json11::Json;

NS_XPF_BEGIN
class ServiceEntrance : public Singleton<ServiceEntrance> {
public:
    static const std::unordered_map<const char *, std::function<Json(Json&&)>> serviceEndpointTable;
    static const std::unordered_map<const char *, std::function<Json(Json&&, std::function<void(Json)>)>> streamEndpointTable;
    Json call(const char * endpoint, Json && params);
    Json readStream(const char * endpoint, Json && params, std::function<void(Json)> && onRead);
};
NS_XPF_END

#endif // __XPFSerivce_ServiceEntrance_h__
