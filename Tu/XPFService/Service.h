#ifndef __XPFSerivce_Service_h__
#define __XPFSerivce_Service_h__
#include "Base/Singleton.h"
#include "XPFService/Data.h"
#include <unordered_map>
#include <functional>

NS_XPF_BEGIN

class Service {
public:
    static const std::unordered_map<const char *, std::function<Data(const Data & params)>> serviceEndpointTable;
};

NS_XPF_END
#endif // __XPFSerivce_Service_h__
