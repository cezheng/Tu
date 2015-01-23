//
//  TypeUtils.h
//  Tu
//
//  Created by Ce Zheng on 10/3/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef Tu_TypeUtils_h
#define Tu_TypeUtils_h
#include <string>

NSString* stringToNSString(const std::string& str);
std::string nsstringToString(NSString* str);
std::string jsonEncodeStd(NSDictionary* dict);
NSString* jsonEncodeNS(NSDictionary* dict);
id jsonDecode(const std::string& str);
id jsonDecode(NSString* str);
#endif
