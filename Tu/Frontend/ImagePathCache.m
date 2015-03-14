#import "ImagePathCache.h"

static const CGFloat epsilon = 0.00001;

@interface ImagePathCache () {
    NSCache* _pathCache;
    NSCache* _cropCache;
}
@property (atomic, strong) NSCache* pathCache;
@property (atomic, strong) NSCache* cropCache;
@end

@implementation ImagePathCache

+ (NSString*)cropCacheKeyByPath:(NSString *)path area:(CGRect)area {
    return [NSString stringWithFormat:@"%@-%d-%d-%d-%d", path, (int)area.origin.x, (int)area.origin.y, (int)area.size.width, (int)area.size.height];
}

+ (id) sharedCache {
    static dispatch_once_t onceToken = 0;
    static ImagePathCache *instance = nil;
    
    dispatch_once(&onceToken, ^{
        instance = [[ImagePathCache alloc] init];
    });
    return instance;
}

- (id) init {
    self = [super init];
    if (self) {
        _pathCache = [[NSCache alloc] init];
        _cropCache = [[NSCache alloc] init];
    }
    return self;
}

- (id)imageWithPath:(NSString *)path {
    id ret;
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    path = [documentsDirectory stringByAppendingPathComponent:path];
    ret = [self.pathCache objectForKey:path];
    if (ret) {
        return ret;
    }
    ret = [UIImage imageWithContentsOfFile:path];
    if (ret) {
        [self.pathCache setObject:ret forKey:path];
    }
    return ret;
}

- (id) imageWithPath:(NSString *)path
                area:(CGRect)area {
    id ret;
    NSString* key = [ImagePathCache cropCacheKeyByPath:path area:area];
    ret = [self.cropCache objectForKey:key];
    if (ret) {
        return ret;
    }
    UIImage* sprite = [self imageWithPath:path];
    if (!sprite) {
        NSLog(@"ImagePathCache : load path cache from relative path %@ failed", path);
        return nil;
    }
    CGImageRef imageRef = CGImageCreateWithImageInRect([sprite CGImage], area);
    ret = [UIImage imageWithCGImage:imageRef];
    if (ret) {
        [self.cropCache setObject:ret forKey:key];
    } else {
        NSLog(@"ImagePathCache : crop image from file %@ failed", path);
    }
    return ret;
}

- (id) imageWithPath:(NSString *)path
            cropInfo:(NSDictionary*)info {
    CGRect area = CGRectMake([info[@"x"] integerValue], [info[@"y"] integerValue], [info[@"w"] integerValue], [info[@"h"] integerValue]);
    return [self imageWithPath:path area:area];
}

- (id) imageWithPath:(NSString *)path
            scaleToSize:(ISize)size {
    NSString* key = [NSString stringWithFormat:@"%@-%d-%d-", path, size.w, size.h];
    id ret = [_pathCache objectForKey:key];
    if (ret) {
        return ret;
    }
    UIImage* unscaled = [self imageWithPath:path];
    if (unscaled) {
        ret = [self scaleImage:unscaled toSize:CGSizeMake(size.w, size.h)];
        if (ret) {
            [_pathCache setObject:ret forKey:key];
        }
    }
    return ret;
}

- (id)imageWithPath:(NSString *)path
           area:(CGRect)area
        scaleToSize:(ISize)size {
    NSString* key = [NSString stringWithFormat:@"%@-%d-%d", [ImagePathCache cropCacheKeyByPath:path area:area], size.w, size.h];
    id ret = [_cropCache objectForKey:key];
    if (ret) {
        return ret;
    }
    UIImage* unscaled = [self imageWithPath:path area:area];
    if (unscaled) {
        ret = [self scaleImage:unscaled toSize:CGSizeMake(size.w, size.h)];
        if (ret) {
            [_cropCache setObject:ret forKey:key];
        }
    }
    return ret;
}

- (id)imageWithPath:(NSString *)path
           cropInfo:(NSDictionary*)info
              scaleToSize:(ISize)size {
    CGRect area = CGRectMake([info[@"x"] integerValue], [info[@"y"] integerValue], [info[@"w"] integerValue], [info[@"h"] integerValue]);
    return [self imageWithPath:path area:area scaleToSize:size];
}

- (id)imageWithName:(NSString *)name
              scaleToSize:(ISize)size {
    NSString* key = [NSString stringWithFormat:@"%@-%d-%d-", name, size.w, size.h];
    id ret = [_pathCache objectForKey:key];
    if (ret) {
        return ret;
    }
    UIImage* unscaled = [UIImage imageNamed:name];
    if (unscaled) {
        ret = [self scaleImage:unscaled toSize:CGSizeMake(size.w, size.h)];
        if (ret) {
            [_pathCache setObject:ret forKey:key];
        }
    }
    return ret;
}

- (UIImage*)scaleImage:(UIImage*)image toSize:(CGSize)size {
    if (fabsf(image.size.width - size.width) < epsilon && fabsf(image.size.height - size.height) < epsilon) {
        return image;
    } else {
        UIGraphicsBeginImageContextWithOptions(size, YES, 0.0);
        [image drawInRect:CGRectMake(0.0, 0.0, size.width, size.height)];
        UIImage* newImage = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
        return newImage;
    }
}

@end
