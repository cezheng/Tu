#ifndef Tu_ChatHistoryObjc_h
#define Tu_ChatHistoryObjc_h
#import "ChatMessageObjc.h"

@interface ChatHistoryObjc : NSObject
- (id) initWithFriendJID:(NSString*)friendJID;
- (NSArray*) getRecentN:(int)n;
- (BOOL) add:(ChatMessageObjc*) message;
- (BOOL) updateReadStatusWithMessageId:(NSString*)messageId status:(BOOL)status;
@end

#endif
