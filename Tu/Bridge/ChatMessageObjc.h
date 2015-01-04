#ifndef Tu_ChatMessageObjc_h
#define Tu_ChatMessageObjc_h
@interface ChatMessageObjc : NSObject
@property (copy, nonatomic) NSString* messageId;
@property (readonly, copy, nonatomic) NSString* message;
@property (readonly, copy, nonatomic) NSString* from;
@property (readonly, copy, nonatomic) NSString* to;
@property (readonly, assign, nonatomic) time_t timestamp;
@property (readonly, nonatomic) void* cppObject;
@property (assign, nonatomic) BOOL read;
- (id)initWithDictionary:(NSDictionary*)dict;
- (id)initWithCppObject:(void*)obj;
@end

#endif
