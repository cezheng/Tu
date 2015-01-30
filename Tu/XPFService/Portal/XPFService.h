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

- (void) readStreamWithEndPoint:(NSString*)endPoint
                         params:(id)params
                       callback:(void(^)(id))onRead
           callbackInMainThread:(BOOL)callbackInMainThread;

+ (XPFService*) sharedService;
@end
