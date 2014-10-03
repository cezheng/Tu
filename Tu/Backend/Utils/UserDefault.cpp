//
//  UserDefault.cpp
//  Tu
//
//  Created by Ce Zheng on 10/1/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#include "UserDefault.h"
#include "FileUtil.h"

UserDefault* UserDefault::constructInstance() {
    auto ret = new UserDefault();
    ret->init();
    return ret;
}

void UserDefault::init() {
    auto fu = FileUtil::getInstance();
    leveldb::Options options;
    options.create_if_missing = true;
    std::string basePath = fu->getWritablePath();
    leveldb::Status status = leveldb::DB::Open(options, basePath + "userdefault.leveldb", &_db);
    //TODO throw exception on not ok
}

std::string UserDefault::get(const std::string& key) {
    std::string value;
    _db->Get(leveldb::ReadOptions(), key, &value);
    return value;
    //TODO if not found
}

void UserDefault::set(const std::string& key, const std::string& value) {
    _db->Put(leveldb::WriteOptions(), key, value);
}