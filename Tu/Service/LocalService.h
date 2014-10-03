//
//  LocalService.h
//  Tu
//
//  Created by Ce Zheng on 10/2/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef __Tu__LocalService__
#define __Tu__LocalService__

#include "../Backend/Base/Singleton.h"
#include "LocalAPI.h"
#include <unordered_map>

class LocalService : public Singleton<LocalService> {
public:
    bool call(LocalAPI* api, const std::string& params, std::string& result);
    bool call(const LocalAPI::EndPoint& endPoint, const std::string& params, std::string& result);
    bool call(const std::string& endPointName, const std::string& params, std::string& result);
    static LocalService* constructInstance();
    static const std::unordered_map<std::string, LocalAPI::EndPoint> s_apiMap;

};

#endif /* defined(__Tu__LocalService__) */
