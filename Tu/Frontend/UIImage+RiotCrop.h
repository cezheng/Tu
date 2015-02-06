#import <UIKit/UIKit.h>
#import "UIImage+PathCache.h"

@interface UIImage (RiotCrop)
+ (id)imageWithPathCache:(NSString *)path
                cropInfo:(NSDictionary*)info;
@end
