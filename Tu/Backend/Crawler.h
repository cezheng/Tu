//
//  Crawler.h
//  Tu
//
//  Created by Ce Zheng on 8/11/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef __Tu__Crawler__
#define __Tu__Crawler__
#include <iostream>
#include "PlayerInfo.h"
#include <regex>
#include "Utils/CurlClient.h"

class Crawler : public CurlClient {
public:
    PlayerInfo queryByPlayerName(const std::string& name, const std::string servers = "1");
    std::string queryRecentMatches(const std::string name, const std::string servers = "1");
    static const std::string s_urlBase;
protected:
    std::cmatch match(const std::string& str, const std::regex& regex);
};

#endif /* defined(__Tu__Crawler__) */
