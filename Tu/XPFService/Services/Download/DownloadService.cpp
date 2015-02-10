#include "DownloadService.h"
#include "XPFService/Utils/FileUtil.h"
#include "XPFService/Utils/Downloader.h"

NS_XPF_BEGIN

Json DownloadService::Download::internalCall() {
    auto downloader = Downloader::getInstance();
    std::string relativePath = downloader->downloadUrl(_params["url"].string_value(), _params["key"].string_value());
    Json::object response;
    response["path"] = relativePath;
    return Json(response);
}

Json DownloadService::GetDownloaded::internalCall() {
    auto downloader = Downloader::getInstance();
    Json::object response {{"downloaded", false}};
    std::string err;
    if (_params.has_shape({{"url", Json::STRING}}, err)) {
        std::string url = _params["url"].string_value();
        if (downloader->isDownloaded(url)) {
            response["downloaded"] = true;
            response["path"] = downloader->makeDownloadPath(url);
        }
    } else if (_params.has_shape({{"key", Json::STRING}}, err)) {
        std::string key = _params["key"].string_value();
        std::string path = downloader->getDownloadedPathByKey(key);
        if (!path.empty()) {
            response["downloaded"] = true;
            response["path"] = path;
        }
    }
    return Json(response);
}
NS_XPF_END