#include "RiotService.h"
#include <sstream>
#include "Backend/Utils/CurlRequest.h"

NS_XPF_BEGIN

Json RiotService::GetSummonerByNames::internalCall() {
    return _service->_api.getSummonerByNames(_params["names"]);
}

Json RiotService::GetMatchFeedByNames::internalCall() {
    Json summoners = _service->_api.getSummonerByNames(_params["names"]);
    return Json();
}

Json RiotService::GetServiceStatusByRegion::internalCall() {
    Json status = _service->_api.getShardByRegion(_params["region"].string_value());
    return Json(status);
}

//----

RiotService::RiotService(std::string apiKey, Riot::Region region) : _apiKey(apiKey), _region(region), _api(apiKey, region) {
}

RiotService* RiotService::constructInstance() {
    return new RiotService("a6ef5db9-1e5f-4bc1-aad0-0cdeb42821e7");
}

void RiotService::setRegion(Riot::Region region) {
    _region = region;
}

void RiotService::setApiKey(std::string apiKey) {
    _apiKey = apiKey;
}

NS_XPF_END
