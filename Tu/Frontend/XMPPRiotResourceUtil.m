#import "XMPP.h"
#import "Constants.h"
#import "XMPPRiotResourceUtil.h"

@implementation XMPPRiotResourceUtil

+ (NSInteger) riotSectionByStatus:(NSString*)status show:(NSString*)show {
    NSString* gameStatus = [self gameStatusByStatus:status];
    if (show == nil) {
        return kRiotFriendSectionOffline;
    }
    if (gameStatus != nil) {
        if ([gameStatus isEqualToString:kRiotGameStatusInGame]) {
            return kRiotFriendSectionInGame;
        } else if ([gameStatus isEqualToString:kRiotGameStatusInQueue]) {
            return kRiotFriendSectionInQueue;
        } else if([gameStatus isEqualToString:kRiotGameStatusSpectating]) {
            return kRiotFriendSectionSpectating;
        } else if([gameStatus isEqualToString:kRiotGameStatusChampionSelect]) {
            return kRiotFriendSectionChampionSelect;
        } else if([gameStatus isEqualToString:kRiotGameStatusHostingPracticeGame]) {
            return kRiotFriendSectionHostingPracticeGame;
        } else if ([gameStatus isEqualToString:kRiotGameStatusOutOfGame]) {
            if([show isEqualToString:@"chat"]) {
                return kRiotFriendSectionOnline;
            } else {
                return kRiotFriendSectionAway;
            }
        }
    }
    if ([show isEqualToString:@"away"]) {
        return kRiotFriendSectionAway;
    }
    return kRiotFriendSectionOffline;
}

+ (NSString*)gameStatusByStatus:(NSString*)status {
    if (status) {
        NSXMLElement* element = [[NSXMLElement alloc] initWithXMLString:status error:nil];
        return [[element elementForName:@"gameStatus"] stringValue];
    } else {
        return nil;
    }
}

+ (NSString*)playingAsByStatus:(NSString*)status {
    if (status) {
        NSXMLElement* element = [[NSXMLElement alloc] initWithXMLString:status error:nil];
        return [[element elementForName:@"skinname"] stringValue];
    }
    return nil;
}

+ (long long)timestampByStatus:(NSString*)status {
    if (status) {
        NSXMLElement* element = [[NSXMLElement alloc] initWithXMLString:status error:nil];
        return [[[element elementForName:@"timeStamp"] stringValue] longLongValue] / 1000L;
    }
    return time(0);
}

+ (NSString*)statusMsgByStatus:(NSString*)status {
    if (status) {
        NSXMLElement* element = [[NSXMLElement alloc] initWithXMLString:status error:nil];
        return [[element elementForName:@"statusMsg"] stringValue];
    }
    return nil;
}

+ (NSString*)rankByStatus:(NSString*)status {
    if (status) {
        NSXMLElement* element = [[NSXMLElement alloc] initWithXMLString:status error:nil];
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
