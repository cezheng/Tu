//
//  FileUtils.h
//  Tu
//
//  Created by Ce Zheng on 8/12/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef __Tu__FileUtils__
#define __Tu__FileUtils__

#include "../Base/Singleton.h"

class FileUtil : public Singleton<FileUtil> {
public:
    virtual std::string getWritablePath() = 0;
    virtual std::string getResourcePath(const std::string& fileName) = 0;
protected:
    FileUtil(){};
};

#endif /* defined(__Tu__FileUtils__) */
