#ifndef __Tu__ImageCacheIOS__
#define __Tu__ImageCacheIOS__

#include "ImageCache.h"

NS_XPF_BEGIN

class ImageCacheIOS : public ImageCache {
public:
    virtual std::string put(const std::string & path, ISize size = ISize::INVALID_SIZE);
    virtual std::string putCropped(const std::string & path, Rect area, ISize size = ISize::INVALID_SIZE);
};

NS_XPF_END

#endif /* defined(__Tu__ImageCacheIOS__) */
