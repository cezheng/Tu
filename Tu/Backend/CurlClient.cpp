#include "CurlClient.h"
#include "rapidjson/document.h"
using namespace rapidjson;

bool CurlResponse::jsonSuccess(){
    Document json;
    json.Parse<0>(data.c_str());
    return json.IsObject() && json.HasMember("success") && json["success"].GetBool();
}

CurlClient::CurlClient(std::string cookiePath):_cookiePath(cookiePath){
}

CurlResponse CurlClient::request(const std::string& url){
    CurlResponse response;
    CURL *handle = curl_easy_init();
    if (handle) {
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(handle, CURLOPT_WRITEHEADER, (void*)&response.header);
        curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION,  CurlClient::dataToString);
        curl_easy_setopt(handle, CURLOPT_COOKIEJAR,_cookiePath.c_str());
        curl_easy_setopt(handle, CURLOPT_COOKIEFILE,_cookiePath.c_str());
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION,  CurlClient::dataToString);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&response.data);
    }
    response.code = curl_easy_perform(handle);
    curl_easy_cleanup(handle);
    return response;
}

CURLcode CurlClient::downloadFile(const std::string& url, const std::string& saveTo){
    CURL *handle = curl_easy_init();
    FILE* fp = fopen(saveTo.c_str(),"wb");
    if (handle) {
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION,  CurlClient::dataToFile);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)fp);
    }
    CURLcode code = curl_easy_perform(handle);
    curl_easy_cleanup(handle);
    fclose(fp);
    return code;
}

size_t CurlClient::dataToString(void *buffer, size_t size, size_t nmemb, void *userp){
    std::string* str= (std::string*)userp;
    for(int i = 0; i < size * nmemb; i++){
        *str += ((char*)buffer)[i];
    }
    return size * nmemb;
}

size_t CurlClient::dataToFile(void *buffer, size_t size, size_t nmemb, void *userp){
    size_t written = fwrite(buffer, size, nmemb, (FILE*)userp);
    return written;
}