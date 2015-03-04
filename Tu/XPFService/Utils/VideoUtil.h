#ifndef __Tu__VideoUtil__
#define __Tu__VideoUtil__
#include "XPFService/XPFDefine.h"
#include "XPFService/Base/Singleton.h"
#include "json11/json11.hpp"

NS_XPF_BEGIN

class VideoUtil: public Singleton<VideoUtil> {
public:
    json11::Json getYoutubeVideoInfo(const std::string & videoId);
};

NS_XPF_END
#endif /* defined(__Tu__VideoUtil__) */
