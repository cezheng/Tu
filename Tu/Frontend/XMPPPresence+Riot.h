#import "XMPPPresence.h"

@interface XMPPPresence (Riot)
- (NSInteger) riotSection;
- (NSString*)gameStatus;
- (NSString*)summonerId;
- (NSString*)playingAs;
- (long long)timestamp;
- (NSString*)statusMsg;
- (NSString*)rank;
@end
