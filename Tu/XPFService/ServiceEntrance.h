#ifndef __XPFSerivce_ServiceEntrance_h__
#define __XPFSerivce_ServiceEntrance_h__

#include "Base/Singleton.h"
#include "XPFService/Data.h"
#include <unordered_map>
#include <functional>

NS_XPF_BEGIN
class ServiceEntrance : public Singleton<ServiceEntrance> {
public:
    static const std::unordered_map<const char *, std::function<Data(const Data & params)>> serviceEndpointTable;
    static ServiceEntrance* constructInstance();
    Data call(const char * endpoint, const Data & params);
};
NS_XPF_END

#endif // __XPFSerivce_ServiceEntrance_h__
