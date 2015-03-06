#import "Constants.h"
#import "XMPPJID+Riot.h"
#import "XMPPPresence+Riot.h"
#import "XMPPRiotResourceUtil.h"
#import "XMPP.h"

@implementation XMPPPresence (Riot)

// sectionNum modifier based on Riot game status
- (NSInteger) riotSection {
    return [XMPPRiotResourceUtil riotSectionByStatus:self.status show:self.show];
}

- (NSString*)gameStatus {
    return [XMPPRiotResourceUtil gameStatusByStatus:self.status];
}

- (NSString*)summonerId {
    return self.from.summonerId;
}

- (NSString*)playingAs {
    return [XMPPRiotResourceUtil playingAsByStatus:self.status];
}

- (long long)timestamp {
    return [XMPPRiotResourceUtil timestampByStatus:self.status];
}

- (NSString*)statusMsg {
    return [XMPPRiotResourceUtil statusMsgByStatus:self.status];
}

- (NSString*)rank {
    return [XMPPRiotResourceUtil rankByStatus:self.status];
}
@end
