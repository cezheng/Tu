//
//  LocalAPI.h
//  Tu
//
//  Created by Ce Zheng on 10/2/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef __Tu__LocalAPI__
#define __Tu__LocalAPI__
#include <string>
#include "rapidjson/document.h"

class LocalAPI {
public:
    enum class EndPoint {
        BASIC_INFO,
        RECENT_MATCHES
    };
    virtual bool exec(const std::string& paramsString, std::string& result);
    virtual bool exec(const rapidjson::Document& params, std::string& result) = 0;
    static LocalAPI* getInstanceByEndPoint(const EndPoint& endPoint);
};

#endif /* defined(__Tu__LocalAPI__) */
