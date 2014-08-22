//
//  DBUtil.h
//  Tu
//
//  Created by Ce Zheng on 8/21/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef __Tu__DBUtil__
#define __Tu__DBUtil__

#include "sqlite3.h"
#include <string>

class DBSqlite {
public:
    DBSqlite():_pDB(nullptr) {}
    DBSqlite(const std::string& dbFilePath);
    bool open(const std::string& dbFilePath);
    
    void close();
protected:
    sqlite3 *_pDB;
};

#endif /* defined(__Tu__DBUtil__) */
