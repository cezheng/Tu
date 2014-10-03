//
//  DownloadManager.cpp
//  Tu
//
//  Created by Ce Zheng on 8/25/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#include "DownloadManager.h"
#include "FileUtil.h"
#include "CurlClient.h"

const std::string DownloadManager::s_downloadedKeyPrefix = "##";

DownloadManager* DownloadManager::constructInstance() {
    auto ret = new DownloadManager();
    ret->init();
    return ret;
}

void DownloadManager::init() {
    auto fu = FileUtil::getInstance();
    leveldb::Options options;
    options.create_if_missing = true;
    std::string basePath = fu->getWritablePath();
    leveldb::Status status = leveldb::DB::Open(options, basePath + "download.leveldb", &_db);
    //TODO throw exception on not ok
}

std::string DownloadManager::download(const std::string& url, const std::string& key) {
    CurlClient client;
    std::hash<std::string> hash;
    std::string hashed = std::to_string(hash(url));
    std::string path = FileUtil::getInstance()->getWritablePath() + hashed;
    if(!this->isDownloaded(url)) {
        client.downloadFile(url, path);
        _db->Put(leveldb::WriteOptions(), key, path);
        _db->Put(leveldb::WriteOptions(), s_downloadedKeyPrefix + hashed, "1");
    }
    return path;
    //TODO on failure
}

bool DownloadManager::isDownloaded(const std::string& url) {
    std::string value;
    std::hash<std::string> hash;
    leveldb::Status status = _db->Get(leveldb::ReadOptions(), std::to_string(hash(url)), &value);
    if(status.IsNotFound()) {
        return false;
    }
    return true;
}

std::string DownloadManager::getFilePathByKey(const std::string& key) {
    std::string value;
    _db->Get(leveldb::ReadOptions(), key, &value);
    return value;
    //TODO if not found 
}