#import "XMPPJID+Riot.h"

@implementation XMPPJID (Riot)
- (NSString*)summonerId {
    NSMutableString* jid = [NSMutableString stringWithString:self.bare];
    NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:@"[0-9]+" options:0 error:nil];
    NSArray* matches = [regex matchesInString:jid options:0 range:NSMakeRange(0, [jid length])];
    NSRange range = [(NSTextCheckingResult*)[matches objectAtIndex:0] rangeAtIndex:0];
    return [jid substringWithRange:range];
}
@end
