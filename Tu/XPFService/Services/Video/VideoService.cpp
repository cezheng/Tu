#include "VideoService.h"
#include "XPFService/Utils/VideoUtil.h"

NS_XPF_BEGIN

Json VideoService::GetInfo::internalCall() {
    return VideoUtil::getInstance()->getYoutubeVideoInfo(_params["id"].string_value());
}

NS_XPF_END