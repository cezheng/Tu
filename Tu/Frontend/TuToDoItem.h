#import <Foundation/Foundation.h>

@interface TuToDoItem : NSObject
@property NSString *itemName;
@property BOOL completed;
@property (readonly) NSDate *creationDate;
- (void) getInfo: (NSString*) name;
@end
