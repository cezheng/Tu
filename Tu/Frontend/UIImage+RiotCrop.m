#import "UIImage+RiotCrop.h"

static NSCache* croppedCache;

@implementation UIImage (RiotCrop)
+ (id) imageWithPathCache:(NSString *)path
                 cropInfo:(NSDictionary*)info {
    id ret;
    NSString* key = [NSString stringWithFormat:@"%@-%@-%@", path, info[@"x"], info[@"y"]];
    if (!croppedCache) {
        croppedCache = [[NSCache alloc] init];
    } else {
        ret = [croppedCache objectForKey:key];
        if (ret) {
            return ret;
        }
    }
    UIImage* sprite = [UIImage imageWithPathCache:path];
    if (!sprite) {
        NSLog(@"UIImage RiotCrop : load path cache from relative path %@ failed", path);
        return nil;
    }
    CGRect area = CGRectMake([info[@"x"] integerValue], [info[@"y"] integerValue], [info[@"w"] integerValue], [info[@"h"] integerValue]);
    CGImageRef imageRef = CGImageCreateWithImageInRect([sprite CGImage], area);
    ret = [UIImage imageWithCGImage:imageRef];
    if (ret) {
        [croppedCache setObject:ret forKey:key];
    } else {
        NSLog(@"UIImage RiotCrop : crop image from file %@ failed", path);
    }
    return ret;
}

@end
