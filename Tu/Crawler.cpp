//
//  Crawler.cpp
//  Tu
//
//  Created by Ce Zheng on 8/11/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#include "Crawler.h"
#include <regex>
#include <iostream>
const std::string Crawler::s_urlBase = "http://loldb.gameguyz.com";

PlayerInfo Crawler::queryByPlayerName(const std::string& name, const std::string servers) {
    
    auto ret = PlayerInfo();
    ret.name = name;
    
    std::string url = s_urlBase + "/analyze/search?search_text=" + name + "&c_server=" + servers;
        
    auto searchRes = this->request(url);
    
    std::regex pageUrlpattern("<div class=\"search_result_content radius shadow clearfix\">\\s*<div class=\"result_list radius\"\\s*onclick=\"window.location='(/analyze/default/index/\\d+/" + servers + "/\\d+)';\">");
    
    std::cmatch resultSearch;
    std::regex_search(searchRes.data.c_str(), resultSearch, pageUrlpattern);
    
    if (resultSearch.empty()) {
        return ret;
    }
    
    std::string playerPageUrl = resultSearch[1].str();
    
    auto pageRes = this->request(s_urlBase + playerPageUrl);
    
    std::regex classicDataRegex("<div class=\"fl mBoxCon boxsdow mb\\w{3,4} \">\\s*<p>Classic</p>\\s*<div class=\"mbcWin clearfix\">Wins:<span>\\d+</span></div>\\s*<div class=\"mbcWin clearfix hide\">Total:<span>\\d+</span></div>\\s*<div class=\"mbcWinRate clearfix \">Win Rate:<span>(\\d+\\.\\d+)%</span></div>\\s*</div>");
    std::cmatch resultClassic;
    std::regex_search(pageRes.data.c_str(), resultClassic, classicDataRegex);
    
    if (resultClassic.empty()) {
        return ret;
    }
    ret.modeDataMap["classic"].winrate = std::stof(resultClassic[1].str());
    
    return ret;
}
