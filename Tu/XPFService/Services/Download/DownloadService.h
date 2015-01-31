#ifndef __Tu__DownloadService__
#define __Tu__DownloadService__
#include "XPFService/Service.h"
#include "leveldb/db.h"
NS_XPF_BEGIN

class DownloadService : public Service, public Singleton<DownloadService> {
public:
    virtual ~DownloadService() {}
    //Service API
    XPF_SERVICE_API_DECLARE(DownloadService, Download, ({
        {"url", Json::STRING},
        {"key", Json::STRING}
    }));
    XPF_SERVICE_API_DECLARE(DownloadService, GetDownloaded, ({}));
};

NS_XPF_END

#endif /* defined(__Tu__DownloadService__) */
