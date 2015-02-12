#include "Downloader.h"
#include "FileUtil.h"
#include "KVS.h"
#include "Exception.h"

NS_XPF_BEGIN

#define BUF_LEN 20

const char * Downloader::kvsNameSpace = "downloader";
const std::string Downloader::s_downloadedKeyPrefix = "##";

std::string Downloader::downloadUrl(const std::string& url, const std::string& key) {
    CurlRequest request;
    std::string relativePath = makeDownloadPath(url);
    std::string path = FileUtil::getInstance()->getWritablePath() + relativePath;
    auto & kvs = (*KVS::getInstance())[kvsNameSpace];
    _downloadMutex.lock();
    if (_urlMutex.find(url) == _urlMutex.end()) {
        _urlMutex[url];
    }
    _downloadMutex.unlock();
    std::lock_guard<std::mutex> lock(_urlMutex[url]);
    if(!FileUtil::getInstance()->fileExists(path)) {
        CURLcode status = request.downloadFile(url, path);
        if (status != CURLE_OK) {
            std::string response = FileUtil::getInstance()->readFileContent(path);
            std::stringstream ss;
            ss << "Downloader failed : status code :" << status << " , response : " << response;
            FileUtil::getInstance()->deleteFile(path);
            throw Exception(ss.str());
        }
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