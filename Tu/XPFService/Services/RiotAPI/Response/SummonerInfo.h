#ifndef __RiotAPI_Response_SummonerInfo_h__
#define __RiotAPI_Response_SummonerInfo_h__
#include "RiotAPI/Response/APIResponse.h"

NS_RIOT_BEGIN

class SummonerInfo : public APIResponse {
public:
    SummonerInfo(const rapidjson::Value & jsonObject);
    SummonerInfo(const std::string & json);
    SummonerInfo(const char * json = "");
    SummonerInfo(long id, const std::string & name, int profileIconId, time_t revisionDate, long summonerLevel);
    long getId();
    std::string getName();
    int getProfileIconId();
    time_t getRevisionDate();
    long getSummonerLevel();
protected:
    virtual void encodeJson();
    virtual void encodeJson(const rapidjson::Value & jsonObject);
    virtual void decodeJson();
    virtual void decodeJson(const rapidjson::Value & jsonObject);
private:
    long _id;
    std::string _name;
    int _profileIconId;
    time_t _revisionDate;
    long _summonerLevel;
};

NS_RIOT_END

#endif // __RiotAPI_Response_SummonerInfo_h__