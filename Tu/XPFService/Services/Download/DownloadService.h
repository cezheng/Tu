#ifndef __Tu__DownloadService__
#define __Tu__DownloadService__
#include "XPFService/Service.h"
#include "leveldb/db.h"
NS_XPF_BEGIN

class DownloadService : public Singleton<DownloadService> {
public:
    //Service API
    Data download(const Data & params);

    std::string download(const std::string& url, const std::string& key);
    bool isDownloaded(const std::string& url);
    
    static const std::string s_downloadedKeyPrefix;
    static DownloadService* constructInstance();
protected:
    std::string makeDownloadedKey(const std::string & url);
private:
    leveldb::DB* _db;
    DownloadService(): _db(nullptr){}
    void init();
};

NS_XPF_END

#endif /* defined(__Tu__DownloadService__) */
