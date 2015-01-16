#include "RiotAPI/Response/SummonerInfo.h"

NS_RIOT_BEGIN

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
    JSON_ADD_int(jsonObject, "profileIconId", _profileIconId);;
    JSON_ADD_time_t(jsonObject, "revisionDate", _revisionDate);;
    JSON_ADD_INT64(jsonObject, "summonerLevel", _summonerLevel);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jsonObject.Accept(writer);
    _json = buffer.GetString();
}
void SummonerInfo::decodeJson() {
    rapidjson::Document document;
    document.Parse(_json.c_str());
    _id = document["id"].GetInt64();
    _name = document["name"].GetString();
    _profileIconId = document["profileIconId"].Getint();
    _revisionDate = document["revisionDate"].Gettime_t();
    _summonerLevel = document["summonerLevel"].GetInt64();
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

