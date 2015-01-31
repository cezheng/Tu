#ifndef __Tu__Downloader__
#define __Tu__Downloader__

#include "XPFDefine.h"
#include "CurlRequest.h"
#include "XPFService/Base/Singleton.h"

NS_XPF_BEGIN

class Downloader : public Singleton<Downloader> {
public:
    virtual ~Downloader() {};
    
    std::string downloadUrl(const std::string& url, const std::string& key);
    bool isDownloaded(const std::string& url);
    std::string getDownloadedPathByKey(const std::string& key);
    
    std::string makeDownloadedKey(const std::string & url);
    std::string makeDownloadPath(const std::string & url);
    
    static const std::string s_downloadedKeyPrefix;
    static const char * kvsNameSpace;
    //static Downloader* constructInstance() {return new Downloader();};

private:
    std::hash<std::string> _stringHasher;
};

NS_XPF_END

#endif /* defined(__Tu__Downloader__) */
