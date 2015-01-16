#ifndef __XPFSerivce_Base_Singleton_h__
#define __XPFSerivce_Base_Singleton_h__
#include "../XPFDefine.h"
#include <mutex>

NS_XPF_BEGIN

template <typename T>
class Singleton {
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    static T* getInstance(){
        std::call_once(s_onceFlag,
                       [] {
                           s_instance.reset(constructInstance());
                       });
        return s_instance.get();
    }
    static T* constructInstance() {
        return T::constructInstance();
    }
    //TODO add destroyInstance
protected:
    static std::unique_ptr<T> s_instance;
    static std::once_flag s_onceFlag;

    Singleton() {};
    virtual ~Singleton() {};
};

template <typename T> std::unique_ptr<T> Singleton<T>::s_instance;
template <typename T> std::once_flag Singleton<T>::s_onceFlag;


NS_XPF_END
#endif // __XPFSerivce_Base_Singleton_h__
