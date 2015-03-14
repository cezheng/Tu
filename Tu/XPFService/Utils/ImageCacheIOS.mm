#include "ImageCacheIOS.h"
#import "ImagePathCache.h"
NS_XPF_BEGIN

ImageCache* ImageCache::constructInstance() {
    return new ImageCacheIOS();
}

std::string ImageCacheIOS::put(const std::string & path, ISize size) {
    if (size.w < 0 || size.h < 0) {
        [[ImagePathCache sharedCache] imageWithPath:@(path.c_str())];
    } else {
        struct ::ISize iosISize = {size.w, size.h};
        [[ImagePathCache sharedCache] imageWithPath:@(path.c_str()) scaleToSize:iosISize];
    }
    return path;
}

std::string ImageCacheIOS::putCropped(const std::string & path, Rect area, ISize size) {
    NSString* nsPath = @(path.c_str());
    CGRect cgRect = CGRectMake(area.x, area.y, area.w, area.h);
    if (size.w < 0 || size.h < 0) {
        [[ImagePathCache sharedCache] imageWithPath:nsPath
                                           area:cgRect];
    } else {
        struct ::ISize iosISize = {size.w, size.h};
        [[ImagePathCache sharedCache] imageWithPath:nsPath area:cgRect scaleToSize:iosISize];
    }
    return [[ImagePathCache cropCacheKeyByPath:nsPath area:cgRect] UTF8String];
}

NS_XPF_END