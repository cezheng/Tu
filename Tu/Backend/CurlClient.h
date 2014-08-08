#ifndef __UTILS_CURLCLIENT_H__
#define __UTILS_CURLCLIENT_H__
#include "stdio.h"
#include "stdlib.h"
#include "curl/curl.h"

class CurlResponse{
public:
    bool jsonSuccess();
    std::string header;
    std::string data;
    CURLcode code;
};

class CurlClient{
public:
    CurlClient(std::string cookiePath = "");
    CurlResponse request(const std::string& url);
    CURLcode downloadFile(const std::string& url, const std::string& saveTo);
    static size_t dataToString(void *buffer, size_t size, size_t nmemb, void *userp);
    static size_t dataToFile(void *buffer, size_t size, size_t nmemb, void *userp);
protected:
    std::string _cookiePath;
};
#endif /* __UTILS_CURLCLIENT_H__ */
