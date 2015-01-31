#include "Downloader.h"
#include "FileUtil.h"
#include "KVS.h"

NS_XPF_BEGIN
const char * Downloader::kvsNameSpace = "downloader";
const std::string Downloader::s_downloadedKeyPrefix = "##";

std::string Downloader::downloadUrl(const std::string& url, const std::string& key) {
    CurlRequest request;
    std::string path = makeDownloadPath(url);
    auto & kvs = (*KVS::getInstance())[kvsNameSpace];
    if(!this->isDownloaded(url)) {
        request.downloadFile(url, path);
        kvs.set(key, path);
        kvs.set(makeDownloadedKey(url), "1");
    }
    return path;
}

bool Downloader::isDownloaded(const std::string& url) {
    auto & kvs = (*KVS::getInstance())[kvsNameSpace];
    std::string value;
    return kvs.get(makeDownloadedKey(url), value);
}

std::string Downloader::getDownloadedPathByKey(const std::string& key) {
    auto & kvs = (*KVS::getInstance())[kvsNameSpace];
    std::string value;
    if(kvs.get(key, value)) {
        return value;
    }
    return "";
}

std::string Downloader::makeDownloadedKey(const std::string & url) {
    return s_downloadedKeyPrefix + std::to_string(_stringHasher(url));
}

std::string Downloader::makeDownloadPath(const std::string & url) {
    return FileUtil::getInstance()->getWritablePath() + std::to_string(_stringHasher(url));
}

NS_XPF_END