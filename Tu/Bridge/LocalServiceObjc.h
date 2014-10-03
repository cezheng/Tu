//
//  LocalServiceObjc.h
//  Tu
//
//  Created by Ce Zheng on 10/2/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#ifndef Tu_LocalServiceObjc_h
#define Tu_LocalServiceObjc_h

@interface LocalServiceObjc : NSObject
+(LocalServiceObjc*) getInstance;
-(void)callWithAPIEndPoint:(NSString*)endPoint
                    params:(NSDictionary*)params
                 successBlock:(void (^)(NSDictionary* response))onSuccess
               failedBlock:(void (^)(NSString* message))onFailure;
@end

#endif
