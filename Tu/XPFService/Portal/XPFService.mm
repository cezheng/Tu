#import "XPFService.h"
#include "XPFService/ServiceEntrance.h"

static const char * requestQueueName = "xpf.network.request";

@interface XPFService () {
    dispatch_queue_t requestQueue;
}

@end

@implementation XPFService
- (id) init {
    self = [super init];
    if (self) {
        requestQueue = dispatch_queue_create(requestQueueName, DISPATCH_QUEUE_SERIAL);
    }
    return self;
}

- (XPFData*) callWithEndPoint:(NSString*)endPoint params:(id) params {
    XPF::Data response = XPF::ServiceEntrance::getInstance()->call([endPoint UTF8String], *(XPF::Data*)[[XPFData alloc]initWithObject:params].cppObject);
    return [[XPFData alloc] initWithCppObject:&response];
}

- (void) callWithEndPoint:(NSString*)endPoint
                   params:(id)params
                 callback:(void(^)(id))onResponse {
    [self callWithEndPoint:endPoint params:params callback:onResponse callbackInMainThread:NO];
}

- (void) callWithEndPoint:(NSString*)endPoint
                   params:(id)params
                 callback:(void(^)(id))onResponse
             callbackInMainThread:(BOOL)callbackInMainThread {
    dispatch_queue_t queue;
    if (callbackInMainThread) {
        queue = dispatch_get_main_queue();
    } else {
        queue = requestQueue;
    }
    dispatch_async(requestQueue, ^{
        XPF::Data response = XPF::ServiceEntrance::getInstance()->call([endPoint UTF8String], *(XPF::Data*)[[XPFData alloc]initWithObject:params].cppObject);
        if (onResponse) {
            if (callbackInMainThread) {
                void* resPtr = &response;
                dispatch_sync(dispatch_get_main_queue(), ^{
                    onResponse([[[XPFData alloc] initWithCppObject:resPtr] decodeObject]);
                });
            } else {
                onResponse([[[XPFData alloc] initWithCppObject:&response] decodeObject]);
            }
            
        }
    });
}

+ (XPFService*) sharedService {
    static dispatch_once_t onceToken = 0;
    static XPFService *instance = nil;
    
    dispatch_once(&onceToken, ^{
        instance = [[XPFService alloc] init];
    });
    return instance;
}


@end
