//
//  LocalServiceObjc.m
//  Tu
//
//  Created by Ce Zheng on 10/2/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LocalServiceObjc.h"
#import "../Service/LocalService.h"
#import "Utils/TypeUtils.h"


@interface LocalServiceObjc ()

@end

@implementation LocalServiceObjc

+(LocalServiceObjc*)getInstance {
    static dispatch_once_t onceToken = 0;
    static LocalServiceObjc *instance = nil;
    
    dispatch_once(&onceToken, ^{
        instance = [[LocalServiceObjc alloc] init];
    });
    return instance;
}

-(void)callWithAPIEndPoint:(NSString*)endPoint
                    params:(NSDictionary*)params
              successBlock:(void (^)(NSDictionary* response))onSuccess
               failedBlock:(void (^)(NSString* message))onFailure
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        std::string result;
        bool ret = LocalService::getInstance()->call([endPoint UTF8String], jsonEncodeStd(params), result);
        if (onSuccess != nil && ret) {
            onSuccess(jsonDecode(result));
        } else if (onFailure != nil && !ret) {
            onFailure(@"Failed");
        }
    });
}
@end