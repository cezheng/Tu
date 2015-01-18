#include "RiotAPI/Response/ChampionStatus.h"

NS_RIOT_BEGIN

ChampionStatus::ChampionStatus(const rapidjson::Value & jsonObject) {
    decodeJson(jsonObject);
    encodeJson(jsonObject);
}

ChampionStatus::ChampionStatus(const std::string & json) : APIResponse(json) {
    decodeJson();
}

ChampionStatus::ChampionStatus(const char * json) : APIResponse(json) {
    decodeJson();
}

ChampionStatus::ChampionStatus(bool active, bool botEnabled, bool botMmEnabled, bool freeToPlay, long id, bool rankedPlayEnabled) : _active(active), _botEnabled(botEnabled), _botMmEnabled(botMmEnabled), _freeToPlay(freeToPlay), _id(id), _rankedPlayEnabled(rankedPlayEnabled) {
    encodeJson();
}

void ChampionStatus::encodeJson() {
    rapidjson::Document jsonObject;
    jsonObject.SetObject();
    JSON_ADD_BOOL(jsonObject, "active", _active);
    JSON_ADD_BOOL(jsonObject, "botEnabled", _botEnabled);
    JSON_ADD_BOOL(jsonObject, "botMmEnabled", _botMmEnabled);
    JSON_ADD_BOOL(jsonObject, "freeToPlay", _freeToPlay);
    JSON_ADD_INT64(jsonObject, "id", _id);
    JSON_ADD_BOOL(jsonObject, "rankedPlayEnabled", _rankedPlayEnabled);
    encodeJson(jsonObject);
}

void ChampionStatus::encodeJson(const rapidjson::Value & jsonObject) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jsonObject.Accept(writer);
    _json = buffer.GetString();
}

void ChampionStatus::decodeJson() {
    rapidjson::Document document;
    document.Parse(_json.c_str());
    if (document.IsObject()) {
        decodeJson(document);
    }
}

void ChampionStatus::decodeJson(const rapidjson::Value & jsonObject) {
    _active = jsonObject["active"].GetBool();
    _botEnabled = jsonObject["botEnabled"].GetBool();
    _botMmEnabled = jsonObject["botMmEnabled"].GetBool();
    _freeToPlay = jsonObject["freeToPlay"].GetBool();
    _id = jsonObject["id"].GetInt64();
    _rankedPlayEnabled = jsonObject["rankedPlayEnabled"].GetBool();
}
bool ChampionStatus::isActive() {
    return _active;
}

bool ChampionStatus::isBotEnabled() {
    return _botEnabled;
}

bool ChampionStatus::isBotMmEnabled() {
    return _botMmEnabled;
}

bool ChampionStatus::isFreeToPlay() {
    return _freeToPlay;
}

long ChampionStatus::getId() {
    return _id;
}

bool ChampionStatus::isRankedPlayEnabled() {
    return _rankedPlayEnabled;
}

NS_RIOT_END

