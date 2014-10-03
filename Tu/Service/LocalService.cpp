//
//  LocalService.cpp
//  Tu
//
//  Created by Ce Zheng on 10/2/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#include "LocalService.h"
const std::unordered_map<std::string, LocalAPI::EndPoint> LocalService::s_apiMap = {
    {"basic_info", LocalAPI::EndPoint::BASIC_INFO},
    {"recent_matches", LocalAPI::EndPoint::RECENT_MATCHES}
};

bool LocalService::call(LocalAPI* api, const std::string& params, std::string& result) {
    api->exec(params, result);
    return true;
}

bool LocalService::call(const LocalAPI::EndPoint& endPoint, const std::string& params, std::string& result) {
    LocalAPI* api = LocalAPI::getInstanceByEndPoint(endPoint);
    if (api != nullptr) {
        return api->exec(params, result);
    }
    return false;
}

bool LocalService::call(const std::string& endPointName, const std::string& params, std::string& result) {
    if (s_apiMap.find(endPointName) != s_apiMap.end()) {
        return call(s_apiMap.at(endPointName), params, result);
    }
    return false;
}

LocalService* LocalService::constructInstance() {
    return new LocalService();
}
