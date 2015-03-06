#import "Constants.h"
#import "XMPPRiotResourceUtil.h"
#import "XMPPJID+Riot.h"
#import "XMPPResourceCoreDataStorageObject.h"
#import "XMPPUserCoreDataStorageObject+Riot.h"
#import "UIImage+PathCache.h"
#import <objc/runtime.h>

static void* lastActivePropertyKey = &lastActivePropertyKey;

@implementation XMPPUserCoreDataStorageObject (Riot)

- (long long)lastActive {
    NSNumber* result = objc_getAssociatedObject(self, lastActivePropertyKey);
    return [result longLongValue];
}

- (void)setLastActive:(long long)timestamp {
    objc_setAssociatedObject(self, lastActivePropertyKey, [NSNumber numberWithLongLong:timestamp], OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (NSString*)lastActiveDate {
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm"];
    NSDate* date = [NSDate dateWithTimeIntervalSince1970:self.lastActive];
    return [dateFormatter stringFromDate:date];
}

- (NSString*)gameStatus {
    return [XMPPRiotResourceUtil gameStatusByStatus:self.primaryResource.status];
}

- (NSString*)summonerId {
    return self.jid.summonerId;
}

- (NSString*)playingAs {
    return [XMPPRiotResourceUtil playingAsByStatus:self.primaryResource.status];
}

- (long long)timestamp {
    return [XMPPRiotResourceUtil timestampByStatus:self.primaryResource.status];
}

- (NSString*)statusMsg {
    return [XMPPRiotResourceUtil statusMsgByStatus:self.primaryResource.status];
}

- (NSString*)rank {
    return [XMPPRiotResourceUtil rankByStatus:self.primaryResource.status];
}

@end

