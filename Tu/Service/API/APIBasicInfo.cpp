//
//  APIBasicInfo.cpp
//  Tu
//
//  Created by Ce Zheng on 10/2/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#include "APIBasicInfo.h"
#include "../../Backend/Crawler.h"
#include "../../Backend/PlayerInfo.h"
#include "../../Backend/Utils/DownloadManager.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"


bool APIBasicInfo::exec(const rapidjson::Document& params, std::string &result) {
    Crawler client;
    PlayerInfo info = client.queryByPlayerName(params["summoner_name"].GetString());
    std::string imagePath = DownloadManager::getInstance()->download(info.profileImageUrl, "#profile#" + info.name);
    rapidjson::Document resultJson;
    resultJson.SetObject();
    rapidjson::Value value;
    value.SetString(imagePath.c_str(), resultJson.GetAllocator());
    resultJson.AddMember("image", value, resultJson.GetAllocator());
    value.SetString((info.name + " " + info.modeDataMap.at("classic").winrate).c_str(), resultJson.GetAllocator());
    resultJson.AddMember("text", value, resultJson.GetAllocator());
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    resultJson.Accept(writer);
    result = buffer.GetString();
    return true;
}

APIBasicInfo* APIBasicInfo::constructInstance() {
    return new APIBasicInfo();
}