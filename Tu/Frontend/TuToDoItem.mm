#import "TuToDoItem.h"
#import "../Backend/RandomString.h"

@interface TuToDoItem () {
    RandomString wrapped;
}
@end

@implementation TuToDoItem
- (void) setRandom
{
    self.itemName =  [[NSString alloc] initWithCString:self->wrapped.get().c_str() encoding:NSStringEncodingConversionAllowLossy];
}
@end
