#import <Foundation/Foundation.h>
#import "Utils/TypeUtils.h"
#import "ChatMessageObjc.h"
#include "../Backend/Value/ChatMessage.h"

@interface ChatMessageObjc () {
    ChatMessage* cppObject;
}

@end

@implementation ChatMessageObjc
- (id)initWithDictionary:(NSDictionary*)dict {
    self = [super init];
    if (self) {
        NSError* error;
        NSString *jsonString = [[NSString alloc] initWithData:[NSJSONSerialization dataWithJSONObject:dict options:NSJSONWritingPrettyPrinted error:&error] encoding:NSUTF8StringEncoding];
        cppObject = new ChatMessage([jsonString UTF8String]);
        if (!cppObject) self = nil;
    }
    return self;
}

- (id)initWithCppObject:(void*)obj {
    self = [super init];
    if (self) {
        cppObject = (ChatMessage*)obj;
        if (!cppObject) self = nil;
    }
    return self;
}

- (void)dealloc {
    delete cppObject;
    //[super dealloc]; // uncomment if not using ARC
}
- (NSString*) messageId {
    return @(cppObject->getId().c_str());
}

- (void) setMessageId:(NSString *)messageId {
    cppObject->setId([messageId UTF8String]);
}

- (NSString*) message {
    return @(cppObject->getMessage().c_str());
}

- (NSString*) from {
    return @(cppObject->getSender().c_str());
}

- (NSString*) to {
    return @(cppObject->getReceiver().c_str());
}

- (time_t) timestamp {
    return cppObject->getTimestamp();
}

- (BOOL) read {
    return cppObject->isRead();
}

- (void) setRead:(BOOL)read {
    cppObject->setRead(read);
}

- (void*) cppObject {
    return cppObject;
}

@end