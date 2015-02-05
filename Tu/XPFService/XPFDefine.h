#ifndef __XPFService_XPFDefine_h__
#define __XPFService_XPFDefine_h__

#define NS_XPF_BEGIN namespace XPF {
#define NS_XPF_END }

#define XPF_ENDPOINT(CLASS, METHOD) std::bind(&CLASS::METHOD, CLASS::getInstance(), std::placeholders::_1)
#define XPF_ENDPOINT_INSTANCE(CLASS, METHOD, INSTANCE) std::bind(&CLASS::METHOD, INSTANCE, std::placeholders::_1)

#define XPF_API_CALL(SERVICE, CLASS) [](Json && params) -> Json {return SERVICE::CLASS(SERVICE::getInstance(), std::move(params)).call();}

#define XPF_STREAM_API_CALL(SERVICE, CLASS) [](Json && params, Service::StreamAPI::OnReadCallBack && onRead) -> Json {return SERVICE::CLASS(SERVICE::getInstance(), std::move(params), std::move(onRead)).call();}

#define XPF_API_DECLARE(SERVICE, CLASS, REQUIREMENTS) \
class CLASS : public Service::API { \
public: \
    CLASS(SERVICE* service, Json && params) : API(std::move(params)), _service(service) {}\
protected: \
    virtual Json internalCall(); \
    virtual void requireParams() const { \
        std::string err; \
        if (!_params.has_shape(Json::shape REQUIREMENTS, err)) { \
            throw std::invalid_argument(err); \
        }\
    }\
private: \
    SERVICE* _service;\
}

#define XPF_STREAM_API_DECLARE(SERVICE, CLASS, REQUIREMENTS) \
class CLASS : public Service::StreamAPI { \
public: \
    CLASS(SERVICE* service, Json && params, OnReadCallBack && callback) : StreamAPI(std::move(params), std::move(callback)), _service(service) {}\
protected: \
    virtual Json internalCall(); \
    virtual void requireParams() const { \
        std::string err; \
        if (!_params.has_shape(Json::shape REQUIREMENTS, err)) { \
            throw std::invalid_argument(err); \
        }\
    }\
private: \
    SERVICE* _service;\
}

#endif  // __XPFService_XPFDefine_h__
