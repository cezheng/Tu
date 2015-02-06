#import "UIImage+RiotCrop.h"

@implementation UIImage (RiotCrop)
+ (id) imageWithPathCache:(NSString *)path
                cropInfo:(NSDictionary*)info {
    UIImage* sprite = [UIImage imageWithPathCache:path];
    CGRect area = CGRectMake([info[@"x"] integerValue], [info[@"y"] integerValue], [info[@"w"] integerValue], [info[@"h"] integerValue]);
    CGImageRef imageRef = CGImageCreateWithImageInRect([sprite CGImage], area);
    return [UIImage imageWithCGImage:imageRef];
}
@end
