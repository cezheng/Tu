#ifndef __XPFService_XPFDefine_h__
#define __XPFService_XPFDefine_h__

#define NS_XPF_BEGIN namespace XPF {
#define NS_XPF_END }

#define XPF_ENDPOINT(CLASS, METHOD) std::bind(&CLASS::METHOD, CLASS::getInstance(), std::placeholders::_1)
#define XPF_ENDPOINT_INSTANCE(CLASS, METHOD, INSTANCE) std::bind(&CLASS::METHOD, INSTANCE, std::placeholders::_1)

#define XPF_API_VALUE(SERVICE, CLASS) [](Json && params) -> Json {return SERVICE::CLASS(SERVICE::getInstance(), std::move(params)).call();}

#define XPF_SERVICE_API_DECLARE(SERVICE, CLASS, REQUIREMENTS) \
class CLASS : public Service::API { \
public: \
    CLASS(SERVICE* service, Json && params) : API(std::move(params)), _service(service) {}\
    virtual Json::shape getRequirements() const { return Json::shape REQUIREMENTS;} \
protected: \
    virtual Json internalCall(); \
private: \
    SERVICE* _service;\
}

#endif  // __XPFService_XPFDefine_h__
