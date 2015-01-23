#import "XMPPResourceCoreDataStorageObject.h"
#import "XMPPUserCoreDataStorageObject+Riot.h"

@implementation XMPPUserCoreDataStorageObject (Riot)
- (NSString*)gameStatus {
    if (self.primaryResource.status) {
        NSXMLElement* element = [[NSXMLElement alloc] initWithXMLString:self.primaryResource.status error:nil];
        return [[element elementForName:@"gameStatus"] stringValue];
    } else {
        return nil;
    }
    
}
@end

