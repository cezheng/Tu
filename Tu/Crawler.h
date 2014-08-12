//
//  Crawler.h
//  Tu
//
//  Created by Ce Zheng on 8/11/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef __Tu__Crawler__
#define __Tu__Crawler__

#include "PlayerInfo.h"
#include "CurlClient.h"

class Crawler : public CurlClient {
public:
    PlayerInfo queryByPlayerName(const std::string& name, const std::string servers = "1");
    
    static const std::string s_urlBase;
};

#endif /* defined(__Tu__Crawler__) */
