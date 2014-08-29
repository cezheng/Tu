//
//  DownloadManager.h
//  Tu
//
//  Created by Ce Zheng on 8/25/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef __Tu__DownloadManager__
#define __Tu__DownloadManager__
#include "../Base/Singleton.h"
#include "leveldb/db.h"

class DownloadManager : public Singleton<DownloadManager> {
public:
    std::string getFilePathByKey(const std::string& key);
    std::string download(const std::string& url, const std::string& key);
    bool isDownloaded(const std::string& url);
    static DownloadManager* constructInstance();

    static std::string s_downloadedKeyPrefix;
private:
    leveldb::DB* _db;
    DownloadManager(): _db(nullptr){}
    void init();
};

#endif /* defined(__Tu__DownloadManager__) */
