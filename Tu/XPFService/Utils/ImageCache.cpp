#include "ImageCache.h"

NS_XPF_BEGIN

const ISize ISize::INVALID_SIZE(-1, -1);

std::string ImageCache::putRiotSprite(const std::string & path, const json11::Json & imageInfo, ISize size) {
    if (path.empty() || imageInfo.object_items().empty()) {
        return "";
    }
    Rect area = Rect((float)imageInfo["x"].int_value(),
                     (float)imageInfo["y"].int_value(),
                     (float)imageInfo["w"].int_value(),
                     (float)imageInfo["h"].int_value());
    return this->putCropped(path, area, size);
}

NS_XPF_END
