//
//  UserDefault.h
//  Tu
//
//  Created by Ce Zheng on 10/1/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef __Tu__UserDefault__
#define __Tu__UserDefault__

#include "../Base/Singleton.h"
#include "leveldb/db.h"

class UserDefault : public Singleton<UserDefault> {
public:
    std::string get(const std::string& key);
    void set(const std::string& key, const std::string& value);
    static UserDefault* constructInstance();
private:
    leveldb::DB* _db;
    UserDefault(): _db(nullptr){}
    void init();
};

#endif /* defined(__Tu__UserDefault__) */
