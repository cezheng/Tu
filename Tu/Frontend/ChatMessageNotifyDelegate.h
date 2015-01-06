#import <Foundation/Foundation.h>
#import "../Bridge/ChatMessageObjc.h"

@protocol ChatMessageNotifyDelegate
@required
- (void) didReceivedChatMessage:(ChatMessageObjc*)message;
@end
