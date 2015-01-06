//
//  APIRecentMatches.h
//  Tu
//
//  Created by Ce Zheng on 10/2/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef __Tu__APIRecentMatches__
#define __Tu__APIRecentMatches__
#include "../../Backend/Base/Singleton.h"
#include "../LocalAPI.h"

class APIRecentMatches : public LocalAPI, public Singleton<APIRecentMatches> {
public:
    virtual bool exec(const rapidjson::Document& params, std::string &result);
    static APIRecentMatches* constructInstance();
};

#endif /* defined(__Tu__APIRecentMatches__) */
