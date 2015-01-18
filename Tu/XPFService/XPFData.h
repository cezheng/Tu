#import <Foundation/Foundation.h>

@interface XPFData : NSObject
@property (readonly, nonatomic) void* cppObject;
@property (readonly, nonatomic, copy) NSString* json;

- (id)initWithDictionary:(NSDictionary*)dict;
- (id)initWithCppObject:(void*)obj;
@end
