#ifndef __Tu__VideoService__
#define __Tu__VideoService__

#include "XPFService/Service.h"

NS_XPF_BEGIN

class VideoService : public Service, public Singleton<VideoService> {
public:
    virtual ~VideoService() {}
    //Service API
    XPF_API_DECLARE(VideoService, GetInfo, ({
        {"id", Json::STRING}
    }));
};

NS_XPF_END

#endif /* defined(__Tu__VideoService__) */
