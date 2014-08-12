//
//  Singleton.h
//  Tu
//
//  Created by Ce Zheng on 8/12/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef Tu_Singleton_h
#define Tu_Singleton_h

template <typename T>
class Singleton {
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    static T& getInstance(){
        static T instance;
        return instance;
    }
};

#endif
