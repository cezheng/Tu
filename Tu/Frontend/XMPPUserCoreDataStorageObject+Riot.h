#import "XMPPUserCoreDataStorageObject.h"

@interface XMPPUserCoreDataStorageObject (Riot)
@property (nonatomic, assign) long long lastActive;
- (NSString*)lastActiveDate;
- (NSString*)gameStatus;
- (NSString*)summonerId;
- (NSString*)playingAs;
- (long long)timestamp;
- (NSString*)statusMsg;
- (NSString*)rank;
@end
