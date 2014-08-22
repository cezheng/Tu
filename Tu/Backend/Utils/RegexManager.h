//
//  RegexManager.h
//  Tu
//
//  Created by Ce Zheng on 8/12/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef __Tu__RegexManager__
#define __Tu__RegexManager__
#include "leveldb/db.h"
#include "../Base/Singleton.h"
#include <regex>

class RegexManager: public Singleton<RegexManager> {
    //make template static method able to create an instance
    friend Singleton<RegexManager>;
public:
    std::regex getRegexByName(const std::string &name);
private:
    leveldb::DB* _db;
    void init();
    RegexManager(): _db(nullptr){}
};

#endif /* defined(__Tu__RegexManager__) */
