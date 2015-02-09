#import "UIImage+RiotCrop.h"

static NSMutableDictionary* croppedCache;

@implementation UIImage (RiotCrop)
+ (id) imageWithPathCache:(NSString *)path
                 cropInfo:(NSDictionary*)info {
    id ret;
    NSString* key = [NSString stringWithFormat:@"%@-%@-%@-%@-%@", path, info[@"x"], info[@"y"], info[@"w"], info[@"h"]];
    if (!croppedCache) {
        croppedCache = [[NSMutableDictionary alloc] init];
    } else {
        ret = [croppedCache objectForKey:key];
        if (ret) {
            return ret;
        }
    }
    UIImage* sprite = [UIImage imageWithPathCache:path];
    CGRect area = CGRectMake([info[@"x"] integerValue], [info[@"y"] integerValue], [info[@"w"] integerValue], [info[@"h"] integerValue]);
    CGImageRef imageRef = CGImageCreateWithImageInRect([sprite CGImage], area);
    ret = [UIImage imageWithCGImage:imageRef];
    [croppedCache setObject:ret forKey:key];
    return ret;
}

@end
