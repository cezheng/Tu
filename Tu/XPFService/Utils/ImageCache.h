#ifndef __Tu__ImageCache__
#define __Tu__ImageCache__

#include "XPFService/XPFDefine.h"
#include "XPFService/Base/Singleton.h"
#include <unordered_map>
#include <string>
#include "json11/json11.hpp"
NS_XPF_BEGIN

struct Rect {
    Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
    float x,y,w,h;
};

struct ISize {
    ISize(int w, int h) : w(w), h(h) {}
    int w, h;
    static const ISize INVALID_SIZE;
};

class ImageCache : public Singleton<ImageCache> {
public:
    virtual std::string put(const std::string & path, ISize size = ISize::INVALID_SIZE) = 0;
    virtual std::string putCropped(const std::string & path, Rect area, ISize size = ISize::INVALID_SIZE) = 0;
    virtual std::string putRiotSprite(const std::string & path, const json11::Json & imageInfo, ISize size = ISize::INVALID_SIZE);
    static ImageCache* constructInstance();
};

NS_XPF_END

#endif /* defined(__Tu__ImageCache__) */
