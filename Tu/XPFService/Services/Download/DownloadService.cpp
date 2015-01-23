#include "DownloadService.h"
#include "Backend/Utils/FileUtil.h"
#include "Backend/Utils/CurlRequest.h"

NS_XPF_BEGIN

const std::string DownloadService::s_downloadedKeyPrefix = "##";

DownloadService* DownloadService::constructInstance() {
    auto ret = new DownloadService();
    ret->init();
    return ret;
}

void DownloadService::init() {
    auto fu = FileUtil::getInstance();
    leveldb::Options options;
    options.create_if_missing = true;
    std::string basePath = fu->getWritablePath();
    leveldb::Status status = leveldb::DB::Open(options, basePath + "download.leveldb", &_db);
    //TODO throw exception on not ok
}

std::string DownloadService::download(const std::string& url, const std::string& key) {
    CurlRequest request;
    std::string path = FileUtil::getInstance()->getWritablePath() + hashed;
    if(!this->isDownloaded(url)) {
        request.downloadFile(url, path);
        _db->Put(leveldb::WriteOptions(), key, path);
        _db->Put(leveldb::WriteOptions(), makeDownloadedKey(url), "1");
    }
    return path;
    //TODO on failure
}

bool DownloadService::isDownloaded(const std::string& url) {
    std::string value;
    leveldb::Status status = _db->Get(leveldb::ReadOptions(), makeDownloadedKey(url), &value);
    if(status.IsNotFound()) {
        return false;
    }
    return true;
}

std::string DownloadService::makeDownloadedKey(const std::string & url) {
    return s_downloadedKeyPrefix + std::to_string(std::hash<std::string>()(url));
}
NS_XPF_END