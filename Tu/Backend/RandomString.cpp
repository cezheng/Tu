//
//  RandomString.cpp
//  Tu
//
//  Created by Ce Zheng on 8/7/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#include "RandomString.h"
#include "stdlib.h"
#include "Crawler.h"
#include <sstream>

std::string RandomString::get(const std::string & name){
    Crawler client;
    PlayerInfo info = client.queryByPlayerName(name);
    if(info.modeDataMap.find("classic") != info.modeDataMap.end()){
        char buf[16];
        sprintf(buf, "%0.2lf", info.modeDataMap["classic"].winrate);
        return name + " " + buf;
    }
    return name + " not found";
}
