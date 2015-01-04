#import <Foundation/Foundation.h>
#import "ChatHistoryObjc.h"
#include "../Backend/Utils/ChatHistory.h"
@interface ChatHistoryObjc () {
    ChatHistory* cppObject;
}

@end

@implementation ChatHistoryObjc

- (id) initWithFriendJID:(NSString*)friendJID {
    self = [super init];
    if (self) {
        cppObject = new ChatHistory([friendJID UTF8String]);
        if (!cppObject) self = nil;
    }
    return self;
}

- (void)dealloc {
    delete cppObject;
    //[super dealloc]; // uncomment if not using ARC
}

- (NSArray*) getRecentN:(int)n {
    std::vector<ChatMessage*> messages;
    cppObject->getRecentN(messages, n);
    NSMutableArray* ret = [[NSMutableArray alloc] init];
    for(auto & m : messages) {
        [ret addObject:[[ChatMessageObjc alloc] initWithCppObject:m]];
    }
    return ret;
}

- (BOOL) add:(ChatMessageObjc*) message {
    return cppObject->add(*(ChatMessage*)message.cppObject);
}

- (BOOL) updateReadStatusWithMessageId:(NSString*)messageId status:(BOOL)status {
    return cppObject->updateReadStatus([messageId UTF8String], status);
}

@end