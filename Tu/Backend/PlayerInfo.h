//
//  PlayerInfo.h
//  Tu
//
//  Created by Ce Zheng on 8/11/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef __Tu__PlayerInfo__
#define __Tu__PlayerInfo__
#include<string>
#include<unordered_map>

struct ModeData {
    int total;
    int win;
    std::string winrate;
};

struct PlayerInfo {
    std::string name;
    std::string profileImageUrl;
    std::unordered_map<std::string, ModeData> modeDataMap;
};

#endif /* defined(__Tu__PlayerInfo__) */
