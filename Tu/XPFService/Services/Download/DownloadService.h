#ifndef __Tu__DownloadService__
#define __Tu__DownloadService__
#include "XPFService/Service.h"
#include "leveldb/db.h"
NS_XPF_BEGIN

class DownloadService : public Service, public Singleton<DownloadService> {
public:
    virtual ~DownloadService();
    //Service API
    XPF_SERVICE_API_DECLARE(DownloadService, Download, ({
        {"url", Json::STRING},
        {"key", Json::STRING}
    }));
    XPF_SERVICE_API_DECLARE(DownloadService, GetDownloaded, ({}));

    std::string downloadUrl(const std::string& url, const std::string& key);
    bool isDownloaded(const std::string& url);
    std::string getDownloadedPathByKey(const std::string& key);
    
    static const std::string s_downloadedKeyPrefix;
    static DownloadService* constructInstance();
protected:
    std::string makeDownloadedKey(const std::string & url);
    std::string makeDownloadPath(const std::string & url);
private:
    std::hash<std::string> _stringHasher;
    leveldb::DB* _db;
    DownloadService(): _db(nullptr){}
    void init();
};

NS_XPF_END

#endif /* defined(__Tu__DownloadService__) */
