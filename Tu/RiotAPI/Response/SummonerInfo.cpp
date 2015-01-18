#include "RiotAPI/Response/SummonerInfo.h"

NS_RIOT_BEGIN

SummonerInfo::SummonerInfo(const rapidjson::Value & jsonObject) {
    decodeJson(jsonObject);
    encodeJson(jsonObject);
}

SummonerInfo::SummonerInfo(const std::string & json) : APIResponse(json) {
    decodeJson();
}

SummonerInfo::SummonerInfo(const char * json) : APIResponse(json) {
    decodeJson();
}

SummonerInfo::SummonerInfo(long id, const std::string & name, int profileIconId, time_t revisionDate, long summonerLevel) : _id(id), _name(name), _profileIconId(profileIconId), _revisionDate(revisionDate), _summonerLevel(summonerLevel) {
    encodeJson();
}

void SummonerInfo::encodeJson() {
    rapidjson::Document jsonObject;
    jsonObject.SetObject();
    JSON_ADD_INT64(jsonObject, "id", _id);
    JSON_ADD_STRING(jsonObject, "name", _name);
    JSON_ADD_INT(jsonObject, "profileIconId", _profileIconId);
    JSON_ADD_INT64(jsonObject, "revisionDate", _revisionDate);
    JSON_ADD_INT64(jsonObject, "summonerLevel", _summonerLevel);
    encodeJson(jsonObject);
}

void SummonerInfo::encodeJson(const rapidjson::Value & jsonObject) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jsonObject.Accept(writer);
    _json = buffer.GetString();
}

void SummonerInfo::decodeJson() {
    rapidjson::Document document;
    document.Parse(_json.c_str());
    if (document.IsObject()) {
        decodeJson(document);
    }
}

void SummonerInfo::decodeJson(const rapidjson::Value & jsonObject) {
    _id = jsonObject["id"].GetInt64();
    _name = jsonObject["name"].GetString();
    _profileIconId = jsonObject["profileIconId"].GetInt();
    _revisionDate = jsonObject["revisionDate"].GetInt64();
    _summonerLevel = jsonObject["summonerLevel"].GetInt64();
}
long SummonerInfo::getId() {
    return _id;
}

std::string SummonerInfo::getName() {
    return _name;
}

int SummonerInfo::getProfileIconId() {
    return _profileIconId;
}

time_t SummonerInfo::getRevisionDate() {
    return _revisionDate;
}

long SummonerInfo::getSummonerLevel() {
    return _summonerLevel;
}

NS_RIOT_END

