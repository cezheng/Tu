#include "DownloadService.h"
#include "Backend/Utils/FileUtil.h"
#include "Backend/Utils/CurlRequest.h"

NS_XPF_BEGIN

const std::string DownloadService::s_downloadedKeyPrefix = "##";

DownloadService::~DownloadService() {
    delete _db;
}

Data DownloadService::download(const Data & params) {
    std::string path = downloadUrl(params.getString("url"), params.getString("key"));
    Data response;
    response.set("path", path.c_str());
    return response;
}

Data DownloadService::getDownloaded(const Data &params) {
    Data response;
    if (params.hasKey("url")) {
        std::string url = params.getString("url");
        if (isDownloaded(url)) {
            response.set("downloaded", true);
            response.set("path", makeDownloadPath(url).c_str());
        } else {
            response.set("downloaded", false);
        }
        
    } else if (params.hasKey("key")) {
        std::string key = params.getString("key");
        std::string path = getDownloadedPathByKey(key);
        if (path.empty()) {
            response.set("downloaded", false);
        } else {
            response.set("downloaded", true);
            response.set("path", path.c_str());
        }
    } else {
        response.set("downloaded", false);
    }
    return response;
}

DownloadService* DownloadService::constructInstance() {
    auto ret = new DownloadService();
    ret->init();
    return ret;
}

void DownloadService::init() {
    auto fu = FileUtil::getInstance();
    leveldb::Options options;
    options.create_if_missing = true;
    std::string basePath = fu->getWritablePath() + "XPF/";
    mkdir(basePath.c_str(), 0755);
    basePath += "Download/";
    mkdir(basePath.c_str(), 0755);
    leveldb::Status status = leveldb::DB::Open(options, basePath + "download.leveldb", &_db);
    //TODO throw exception on not ok
}

std::string DownloadService::downloadUrl(const std::string& url, const std::string& key) {
    CurlRequest request;
    std::string path = makeDownloadPath(url);
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

std::string DownloadService::getDownloadedPathByKey(const std::string& key) {
    std::string value;
    leveldb::Status status = _db->Get(leveldb::ReadOptions(), key, &value);
    if(status.IsNotFound()) {
        return "";
    }
    return value;
}

std::string DownloadService::makeDownloadedKey(const std::string & url) {
    return s_downloadedKeyPrefix + std::to_string(_stringHasher(url));
}

std::string DownloadService::makeDownloadPath(const std::string & url) {
    return FileUtil::getInstance()->getWritablePath() + std::to_string(_stringHasher(url));
}
NS_XPF_END