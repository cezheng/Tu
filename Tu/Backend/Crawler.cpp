//
//  Crawler.cpp
//  Tu
//
//  Created by Ce Zheng on 8/11/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#include "Crawler.h"
#include <iostream>
#include "Utils/RegexManager.h"

const std::string Crawler::s_urlBase = "http://loldb.gameguyz.com";

PlayerInfo Crawler::queryByPlayerName(const std::string& name, const std::string servers) {
    auto regexManager = RegexManager::getInstance();
    
    auto ret = PlayerInfo();
    ret.name = name;
    
    std::string url = s_urlBase + "/analyze/search?search_text=" + name + "&c_server=" + servers;
        
    auto searchRes = this->request(url);
    
    auto pageUrlRegex = regexManager->getRegexByName("#SearchResult");

    std::cmatch resultSearch;
    std::regex_search(searchRes.data.c_str(), resultSearch, pageUrlRegex);
    
    if (resultSearch.empty()) {
        return ret;
    }
    
    std::string playerPageUrl = resultSearch[1].str();
    
    auto pageRes = this->request(s_urlBase + playerPageUrl);
    
    auto classicDataRegex = regexManager->getRegexByName("#ClassicModeData");
    std::cmatch resultClassic;
    std::regex_search(pageRes.data.c_str(), resultClassic, classicDataRegex);
    
    if (resultClassic.empty()) {
        return ret;
    }
    ret.modeDataMap["classic"].winrate = std::stof(resultClassic[1].str());
    
    return ret;
}
