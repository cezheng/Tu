#ifndef __UTILS_CURLCLIENT_H__
#define __UTILS_CURLCLIENT_H__
#include "XPFDefine.h"
#include "stdio.h"
#include "stdlib.h"
#include "curl/curl.h"
#include <string>
#include <sstream>
#include <functional>

NS_XPF_BEGIN

class CurlResponse{
public:
    std::string header;
    std::string data;
    CURLcode code;
};

class CurlRequest{
public:
    CurlRequest(std::string cookiePath = "");
    CurlResponse request(const std::string& url);
    void request(const std::string& url, const std::function<void(CurlResponse)> & callback);
    CURLcode downloadFile(const std::string& url, const std::string& saveTo);
protected:
    static size_t dataToString(void *buffer, size_t size, size_t nmemb, void *userp);
    static size_t dataToFile(void *buffer, size_t size, size_t nmemb, void *userp);
    CURL* makeRequestHandle(const std::string& url, std::stringstream* headerStream, std::stringstream* dataStream);
    std::string _cookiePath;
};

NS_XPF_END
#endif /* __UTILS_CURLCLIENT_H__ */
