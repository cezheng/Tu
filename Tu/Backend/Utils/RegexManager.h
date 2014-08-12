//
//  RegexManager.h
//  Tu
//
//  Created by Ce Zheng on 8/12/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef __Tu__RegexManager__
#define __Tu__RegexManager__
#include "../Base/Singleton.h"
#include <regex>

class RegexManager: public Singleton<RegexManager>{
public:
    std::regex getRegexByName(const std::string &name);
};

#endif /* defined(__Tu__RegexManager__) */
