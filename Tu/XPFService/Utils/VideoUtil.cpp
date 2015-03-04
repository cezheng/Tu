#include "VideoUtil.h"
#include "UrlUtil.h"
#include "CurlRequest.h"
#include <regex>

using json11::Json;

static const std::regex youtubeVideoUrlPattern("\"url_encoded_fmt_stream_map\":.*?url=(.*?)\\\\u0026");
static const std::regex youtubeVideoTitlePattern("\"title\":\"(.*?)\".*");
static const std::regex youtubeVideoAuthorPattern("\"author\":\"(.*?)\".*");
static const std::string youtubeUrlPrefix("https://www.youtube.com/watch?v=");
static const std::string youtubeThumbnailPrefix("http://img.youtube.com/vi/");

NS_XPF_BEGIN

Json VideoUtil::getYoutubeVideoInfo(const std::string & videoId) {
    CurlRequest request;
    auto res = request.request(youtubeUrlPrefix + videoId);
    printf("youtube : %s", res.data.c_str());
    Json::object ret;
    std::cmatch search;
    std::regex_search(res.data.c_str(), search, youtubeVideoUrlPattern);
    if (!search.empty()) {
        ret["video"] = url_decode(search[1].str());
    }
    std::regex_search(res.data.c_str(), search, youtubeVideoTitlePattern);
    if (!search.empty()) {
        ret["title"] = search[1].str();
    }
    std::regex_search(res.data.c_str(), search, youtubeVideoAuthorPattern);
    if (!search.empty()) {
        ret["author"] = search[1].str();
    }
    ret["thumbnail"] = youtubeThumbnailPrefix + videoId + "/0.jpg";
    return ret;
}

NS_XPF_END