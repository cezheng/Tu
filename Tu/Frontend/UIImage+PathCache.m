#import "UIImage+PathCache.h"
static NSMutableDictionary* cache;

@implementation UIImage (PathCache)
+ (id)imageWithPathCache:(NSString *)path {
    id ret;
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    path = [documentsDirectory stringByAppendingPathComponent:path];
    if (!cache) {
        cache = [[NSMutableDictionary alloc] init];
    } else {
        ret = [cache objectForKey:path];
        if (ret) {
            return ret;
        }
    }
    ret = [UIImage imageWithContentsOfFile:path];
    [cache setObject:ret forKey:path];
    return ret;
}

@end
