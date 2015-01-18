#import <Foundation/Foundation.h>
#import "../XPFData.h"

@interface XPFService : NSObject
- (XPFData*) callWithEndPoint:(NSString*)endPoint params:(XPFData*) params;
@end
