#include "Downloader.h"
#include "FileUtil.h"
#include "KVS.h"

NS_XPF_BEGIN

#define BUF_LEN 20

const char * Downloader::kvsNameSpace = "downloader";
const std::string Downloader::s_downloadedKeyPrefix = "##";

std::string Downloader::downloadUrl(const std::string& url, const std::string& key) {
    CurlRequest request;
    std::string relativePath = makeDownloadPath(url);
    std::string path = FileUtil::getInstance()->getWritablePath() + relativePath;
    auto & kvs = (*KVS::getInstance())[kvsNameSpace];
    std::mutex & mutex = _urlMutex[url];
    std::lock_guard<std::mutex> lock(mutex);
    if(!FileUtil::getInstance()->fileExists(path)) {
        request.downloadFile(url, path);
        kvs.set(key, relativePath);
    }
    return relativePath;
}

bool Downloader::isDownloaded(const std::string& url) {
    std::string path = FileUtil::getInstance()->getWritablePath() + makeDownloadPath(url);
    return FileUtil::getInstance()->fileExists(path);
}

std::string Downloader::getDownloadedPathByKey(const std::string& key) {
    auto & kvs = (*KVS::getInstance())[kvsNameSpace];
    std::string value;
    if(kvs.get(key, value)) {
        return value;
    }
    return "";
}

std::string Downloader::makeDownloadPath(const std::string & url) {
    char buf[BUF_LEN];
    sprintf(buf, "%017zx", _stringHasher(url));
    return buf;
}

NS_XPF_END