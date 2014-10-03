//
//  APIRecentMatches.cpp
//  Tu
//
//  Created by Ce Zheng on 10/2/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#include "APIRecentMatches.h"
bool APIRecentMatches::exec(const rapidjson::Document& params, std::string &result) {
    result = "test api";
    return true;
}

APIRecentMatches* APIRecentMatches::constructInstance() {
    return new APIRecentMatches();
}
