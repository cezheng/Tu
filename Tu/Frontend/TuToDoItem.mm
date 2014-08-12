#import "TuToDoItem.h"
#import "../Backend/RandomString.h"

@interface TuToDoItem () {
    RandomString wrapped;
}
@end

@implementation TuToDoItem
- (void) getInfo : (NSString*) name
{
    self.itemName =  [[NSString alloc] initWithCString:self->wrapped.get([name UTF8String]).c_str() encoding:NSStringEncodingConversionAllowLossy];
}
@end
