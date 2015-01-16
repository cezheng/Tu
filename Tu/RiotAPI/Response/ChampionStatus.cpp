#include "RiotAPI/Response/ChampionStatus.h"

ChampionStatus::ChampionStatus(const std::string & json) : _json(json) {
}

ChampionStatus::ChampionStatus(const char * json) : _json(json) {
}

ChampionStatus::ChampionStatus(bool active, bool botEnabled, bool botMmEnabled, bool freeToPlay, long id, bool rankedPlayEnabled) : _active(active), _botEnabled(botEnabled), _botMmEnabled(botMmEnabled), _freeToPlay(freeToPlay), _id(id), _rankedPlayEnabled(rankedPlayEnabled) {
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

