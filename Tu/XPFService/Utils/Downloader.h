#ifndef __Tu__Downloader__
#define __Tu__Downloader__

#include "XPFDefine.h"
#include "CurlRequest.h"
#include "XPFService/Base/Singleton.h"
#include <mutex>
#include <unordered_map>

NS_XPF_BEGIN

class Downloader : public Singleton<Downloader> {
public:
    virtual ~Downloader() {};
    
    std::string downloadUrl(const std::string& url, const std::string& key);
    bool isDownloaded(const std::string& url);
    std::string getDownloadedPathByKey(const std::string& key);
    
    std::string makeDownloadPath(const std::string & url);
    
    static const std::string s_downloadedKeyPrefix;
    static const char * kvsNameSpace;
    //static Downloader* constructInstance() {return new Downloader();};

private:
    std::hash<std::string> _stringHasher;
    std::unordered_map<std::string, std::mutex> _urlMutex;
};

NS_XPF_END

#endif /* defined(__Tu__Downloader__) */
