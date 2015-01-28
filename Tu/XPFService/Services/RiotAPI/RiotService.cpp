#include "RiotService.h"
#include <sstream>
#include "Backend/Utils/CurlRequest.h"

NS_XPF_BEGIN

Json RiotService::GetSummonerByNames::internalCall() {
    return _service->_api.getSummonerByNames(_params["names"]);
}

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
