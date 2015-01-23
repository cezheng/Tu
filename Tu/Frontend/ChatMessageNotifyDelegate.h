#import <Foundation/Foundation.h>
#import "XPFService/XPFData.h"

@protocol ChatMessageNotifyDelegate
@required
- (void) didReceivedChatMessage:(NSDictionary*)message;
@end
