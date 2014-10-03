//
//  APIBasicInfo.h
//  Tu
//
//  Created by Ce Zheng on 10/2/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef __Tu__APIBasicInfo__
#define __Tu__APIBasicInfo__
#include "../../Backend/Base/Singleton.h"
#include "../LocalAPI.h"

class APIBasicInfo : public LocalAPI, public Singleton<APIBasicInfo> {
public:
    virtual bool exec(const rapidjson::Document& params, std::string &result);
    static APIBasicInfo* constructInstance();
};

#endif /* defined(__Tu__APIBasicInfo__) */
