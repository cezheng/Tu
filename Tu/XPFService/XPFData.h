#import <Foundation/Foundation.h>

@interface XPFData : NSObject
@property (readonly, nonatomic) void* cppObject;
@property (readonly, nonatomic, copy) NSString* json;

- (id) initWithObject:(NSDictionary*)dict;
- (id) initWithCppObject:(void*)obj;
- (id) decodeObject;
@end
