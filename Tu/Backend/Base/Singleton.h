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
        if (s_instance == nullptr) {
            s_instance = T::constructInstance();
        }
        return s_instance;
    }
    static T* constructInstance();
protected:
    static T* s_instance;

    Singleton() {};
    ~Singleton() {};
};

template <typename T> T* Singleton<T>::s_instance = nullptr;

#endif
