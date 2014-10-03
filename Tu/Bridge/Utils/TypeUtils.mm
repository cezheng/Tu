//
//  TypeUtils.m
//  Tu
//
//  Created by Ce Zheng on 10/3/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#import "TypeUtils.h"

NSString* stringToNSString(const std::string& str) {
    return [NSString stringWithUTF8String:str.c_str()];
}

std::string nsstringToString(NSString* str) {
    return [str UTF8String];
}

std::string jsonEncodeStd(NSDictionary* dict) {
    return [jsonEncodeNS(dict) UTF8String];
}

NSString* jsonEncodeNS(NSDictionary* dict) {
    NSError *error;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dict
                                                       options:0
                                                         error:&error];
    return [[NSString alloc] initWithBytes:[jsonData bytes] length:[jsonData length] encoding:NSUTF8StringEncoding];
}

NSDictionary* jsonDecode(const std::string& str) {
    return jsonDecode(stringToNSString(str));
}

NSDictionary* jsonDecode(NSString* str) {
    NSError *error;
    return [NSJSONSerialization JSONObjectWithData:[str dataUsingEncoding:NSUTF8StringEncoding] options:0 error:&error];
}