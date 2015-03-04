#import "Constants.h"
#import "XMPPJID+Riot.h"
#import "XMPPResourceCoreDataStorageObject.h"
#import "XMPPUserCoreDataStorageObject+Riot.h"
#import "UIImage+PathCache.h"
#import <objc/runtime.h>

static void* lastActivePropertyKey = &lastActivePropertyKey;

@implementation XMPPUserCoreDataStorageObject (Riot)

// sectionNum modifier based on Riot game status
- (NSNumber*) sectionNum {
    NSString* gameStatus = self.gameStatus;
    if (gameStatus != nil) {
        if ([gameStatus isEqualToString:kRiotGameStatusInGame]) {
            return @(kRiotFriendSectionInGame);
        } else if ([gameStatus isEqualToString:kRiotGameStatusInQueue]) {
            return @(kRiotFriendSectionInQueue);
        } else if([gameStatus isEqualToString:kRiotGameStatusSpectating]) {
            return @(kRiotFriendSectionSpectating);
        } else if([gameStatus isEqualToString:kRiotGameStatusChampionSelect]) {
            return @(kRiotFriendSectionChampionSelect);
        } else if([gameStatus isEqualToString:kRiotGameStatusHostingPracticeGame]) {
            return @(kRiotFriendSectionHostingPracticeGame);
        } else if ([gameStatus isEqualToString:kRiotGameStatusOutOfGame]) {
            if(self.primaryResource.intShow >= 3) {
                return @(kRiotFriendSectionOnline);
            } else {
                return @(kRiotFriendSectionAway);
            }
        }
    }
    return @(kRiotFriendSectionOffline);
}

- (long long)lastActive {
    NSNumber* result = objc_getAssociatedObject(self, lastActivePropertyKey);
    return [result longLongValue];
}

- (void)setLastActive:(long long)timestamp {
    objc_setAssociatedObject(self, lastActivePropertyKey, [NSNumber numberWithLongLong:timestamp], OBJC_ASSOCIATION_RETAIN);
}

- (NSString*)lastActiveDate {
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm"];
    NSDate* date = [NSDate dateWithTimeIntervalSince1970:self.lastActive];
    return [dateFormatter stringFromDate:date];
}

- (NSString*)gameStatus {
    if (self.primaryResource.status) {
        NSXMLElement* element = [[NSXMLElement alloc] initWithXMLString:self.primaryResource.status error:nil];
        return [[element elementForName:@"gameStatus"] stringValue];
    } else {
        return nil;
    }
}

- (NSString*)summonerId {
    return self.jid.summonerId;
}

- (NSString*)playingAs {
    if (self.primaryResource.status) {
        NSXMLElement* element = [[NSXMLElement alloc] initWithXMLString:self.primaryResource.status error:nil];
        return [[element elementForName:@"skinname"] stringValue];
    }
    return nil;
}

- (long long)timestamp {
    if (self.primaryResource.status) {
        NSXMLElement* element = [[NSXMLElement alloc] initWithXMLString:self.primaryResource.status error:nil];
        return [[[element elementForName:@"timeStamp"] stringValue] longLongValue] / 1000L;
    }
    return time(0);
}

- (NSString*)statusMsg {
    if (self.primaryResource.status) {
        NSXMLElement* element = [[NSXMLElement alloc] initWithXMLString:self.primaryResource.status error:nil];
        return [[element elementForName:@"statusMsg"] stringValue];
    }
    return nil;
}

- (NSString*)rank {
    if (self.primaryResource.status) {
        NSXMLElement* element = [[NSXMLElement alloc] initWithXMLString:self.primaryResource.status error:nil];
        NSString* tier = [[element elementForName:@"rankedLeagueTier"] stringValue];
        NSString* division = [[element elementForName:@"rankedLeagueDivision"] stringValue];
        if ([tier length] == 0 || [division length] == 0) {
            return @"Unranked";
        }
        return [NSString stringWithFormat:@"%@ %@", tier, division];
    }
    return @"";
}

@end

