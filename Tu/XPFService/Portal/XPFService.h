#import <Foundation/Foundation.h>
#import "../XPFData.h"

@interface XPFService : NSObject
- (XPFData*) callWithEndPoint:(NSString*)endPoint params:(id) params;

- (void) callWithEndPoint:(NSString*)endPoint
                   params:(id)params
                 callback:(void(^)(id))onResponse
     callbackInMainThread:(BOOL)callbackInMainThread;

- (void) callWithEndPoint:(NSString*)endPoint
                   params:(id)params
                 callback:(void(^)(id))onResponse;
+ (XPFService*) sharedService;
@end
