//
//  LocalAPI.cpp
//  Tu
//
//  Created by Ce Zheng on 10/2/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#include "LocalAPI.h"
#include "API/APIRecentMatches.h"
#include "API/APIBasicInfo.h"


LocalAPI* LocalAPI::getInstanceByEndPoint(const EndPoint& endPoint) {
    switch (endPoint) {
        case EndPoint::BASIC_INFO:
            return APIBasicInfo::getInstance();
        case EndPoint::RECENT_MATCHES:
            return APIRecentMatches::getInstance();
        default:
            return nullptr;
    }
}

bool LocalAPI::exec(const std::string& paramsString, std::string &result) {
    rapidjson::Document params;
    params.Parse<0>(paramsString.c_str());
    return exec(params, result);
}