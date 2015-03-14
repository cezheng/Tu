#import "UIImage+PathCache.h"

static NSCache* cache;

@implementation UIImage (PathCache)
+ (id)imageWithPathCache:(NSString *)path {
    id ret;
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    path = [documentsDirectory stringByAppendingPathComponent:path];
    if (!cache) {
        cache = [[NSCache alloc] init];
    } else {
        ret = [cache objectForKey:path];
        if (ret) {
            return ret;
        }
    }
    ret = [UIImage imageWithContentsOfFile:path];
    if (ret) {
        [cache setObject:ret forKey:path];
    }
    return ret;
}

@end
