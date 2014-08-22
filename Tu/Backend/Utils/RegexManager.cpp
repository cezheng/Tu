//
//  RegexManager.cpp
//  Tu
//
//  Created by Ce Zheng on 8/12/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//
#include "RegexManager.h"
#include "leveldb/write_batch.h"
#include "FileUtil.h"
#include <fstream>

template<> RegexManager* Singleton<RegexManager>::constructInstance() {
    auto ret = new RegexManager();
    ret->init();
    return ret;
}

void RegexManager::init() {
    auto fu = FileUtil::getInstance();
    leveldb::Options options;
    options.create_if_missing = true;
    std::string basePath = fu->getWritablePath();
    leveldb::Status status = leveldb::DB::Open(options, basePath + "regex.leveldb", &_db);
    if(status.ok()){
        std::ifstream file(fu->getResourcePath("regex"));
        std::string key, fileValue;
        leveldb::WriteBatch batch;
        while(std::getline(file, key)) {
            std::getline(file, fileValue);
            std::string value;
            leveldb::Status s = _db->Get(leveldb::ReadOptions(), key, &value);
            if (s.IsNotFound()) {
                batch.Put(key, fileValue);
            }
        }
        _db->Write(leveldb::WriteOptions(), &batch);
    }
}

std::regex RegexManager::getRegexByName(const std::string& name) {
    std::string value;
    _db->Get(leveldb::ReadOptions(), name, &value);
    return std::regex(value);
}