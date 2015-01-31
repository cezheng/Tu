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

- (NSString*)summonerId {
    NSMutableString* jid = [NSMutableString stringWithString:self.jidStr];
    NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:@"[0-9]+" options:0 error:nil];
    NSArray* matches = [regex matchesInString:self.jidStr options:0 range:NSMakeRange(0, [jid length])];
    NSRange range = [(NSTextCheckingResult*)[matches objectAtIndex:0] rangeAtIndex:0];
    return [jid substringWithRange:range];
}


@end

