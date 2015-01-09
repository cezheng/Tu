#include "CurlRequest.h"
#include "rapidjson/document.h"
using namespace rapidjson;

CurlRequest::CurlRequest(std::string cookiePath):_cookiePath(cookiePath){
}

CurlResponse CurlRequest::request(const std::string& url){
    CurlResponse response;
    std::stringstream headerStream, dataStream;
    CURL *handle = makeRequestHandle(url, &headerStream, &dataStream);
    response.code = curl_easy_perform(handle);
    curl_easy_cleanup(handle);
    response.data = dataStream.str();
    response.header = headerStream.str();
    return response;
}

void CurlRequest::request(const std::string& url, const std::function<void(CurlResponse)> & callback) {
    CurlResponse response;
    std::stringstream headerStream, dataStream;
    CURL *handle = makeRequestHandle(url, &headerStream, &dataStream);
    response.code = curl_easy_perform(handle);
    curl_easy_cleanup(handle);
    response.data = dataStream.str();
    response.header = headerStream.str();
    callback(response);
}

CURL* CurlRequest::makeRequestHandle(const std::string& url, std::stringstream* headerStream, std::stringstream* dataStream) {
    CURL *handle = curl_easy_init();
    if (handle) {
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(handle, CURLOPT_WRITEHEADER, (void*)headerStream);
        curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION,  CurlRequest::dataToString);
        if (!_cookiePath.empty()) {
            curl_easy_setopt(handle, CURLOPT_COOKIEJAR,_cookiePath.c_str());
            curl_easy_setopt(handle, CURLOPT_COOKIEFILE,_cookiePath.c_str());
        }
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION,  CurlRequest::dataToString);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)dataStream);
    }
    return handle;
}

CURLcode CurlRequest::downloadFile(const std::string& url, const std::string& saveTo){
    CURL *handle = curl_easy_init();
    FILE* fp = fopen(saveTo.c_str(),"wb");
    if (handle) {
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, CurlRequest::dataToFile);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)fp);
    }
    CURLcode code = curl_easy_perform(handle);
    curl_easy_cleanup(handle);
    fclose(fp);
    return code;
}

size_t CurlRequest::dataToString(void *buffer, size_t size, size_t nmemb, void *userp){
    std::stringstream* ss= (std::stringstream*)userp;
    ss->write((char*)buffer, size * nmemb);
    return size * nmemb;
}

size_t CurlRequest::dataToFile(void *buffer, size_t size, size_t nmemb, void *userp){
    size_t written = fwrite(buffer, size, nmemb, (FILE*)userp);
    return written;
}