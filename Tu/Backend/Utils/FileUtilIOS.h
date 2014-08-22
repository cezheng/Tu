//
//  FileUtilIOS.h
//  Tu
//
//  Created by Ce Zheng on 8/21/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef __Tu__FileUtilIOS__
#define __Tu__FileUtilIOS__

#include "FileUtil.h"

class FileUtilIOS : public FileUtil {
    //make template static method able to create an instance
    friend class Singleton<FileUtil>;
public:
    virtual std::string getWritablePath();
    virtual std::string getResourcePath(const std::string& fileName);
protected:
    FileUtilIOS() {};
};

#endif /* defined(__Tu__FileUtilIOS__) */
