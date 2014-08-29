//
//  Singleton.h
//  Tu
//
//  Created by Ce Zheng on 8/12/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef Tu_Singleton_h
#define Tu_Singleton_h
#include <iostream>

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

#endif
