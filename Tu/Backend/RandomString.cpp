//
//  RandomString.cpp
//  Tu
//
//  Created by Ce Zheng on 8/7/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#include "RandomString.h"
#include "stdlib.h"

std::string RandomString::get(){
    return std::string("") + char(rand() % 256);
}
