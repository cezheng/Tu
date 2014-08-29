//
//  FileUtilIOS.cpp
//  Tu
//
//  Created by Ce Zheng on 8/21/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#include "FileUtilIOS.h"

FileUtil* FileUtil::constructInstance() {
    return new FileUtilIOS();
}

std::string FileUtilIOS::getWritablePath() {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    std::string strRet = [documentsDirectory UTF8String];
    strRet.append("/");
    return strRet;
}

std::string FileUtilIOS::getResourcePath(const std::string& fileName) {
    NSString *pathInfo = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:fileName.c_str()] ofType:@""];
    return [pathInfo UTF8String];
}