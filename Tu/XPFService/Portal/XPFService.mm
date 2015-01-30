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
    json11::Json response = XPF::ServiceEntrance::getInstance()->call([endPoint UTF8String], std::move(*(json11::Json*)[[XPFData alloc]initWithObject:params].cppObject));
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
    dispatch_async(requestQueue, ^{
        Json response = XPF::ServiceEntrance::getInstance()->call([endPoint UTF8String], std::move(*(Json*)[[XPFData alloc]initWithObject:params].cppObject));
        if (onResponse) {
            if (callbackInMainThread && ![NSThread isMainThread]) {
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

- (void) readStreamWithEndPoint:(NSString*)endPoint
                         params:(id)params
                       callback:(void(^)(id))onRead
           callbackInMainThread:(BOOL)callbackInMainThread {
    dispatch_async(requestQueue, ^{
        Json paramsCpp = std::move(*(Json*)[[XPFData alloc]initWithObject:params].cppObject);
        auto onReadCpp = [=](Json data) {
            void* dataPtr = &data;
            if (onRead) {
                if (callbackInMainThread && ![NSThread isMainThread]) {
                    dispatch_sync(dispatch_get_main_queue(), ^{
                        onRead([[[XPFData alloc] initWithCppObject:dataPtr] decodeObject]);
                    });
                } else {
                    onRead([[[XPFData alloc] initWithCppObject:dataPtr] decodeObject]);
                }
            }
        };
        XPF::ServiceEntrance::getInstance()->readStream([endPoint UTF8String], std::move(paramsCpp), std::move(onReadCpp));
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
