#import "XPFData.h"
#import "Data.h"

@interface XPFData () {
    XPF::Data* __cppObject__;;
}

@end

@implementation XPFData

- (id)initWithDictionary:(NSDictionary*)dict {
    self = [super init];
    if (self) {
        NSError* error;
        NSString *jsonString = [[NSString alloc] initWithData:[NSJSONSerialization dataWithJSONObject:dict options:NSJSONWritingPrettyPrinted error:&error] encoding:NSUTF8StringEncoding];
        __cppObject__ = new XPF::Data([jsonString UTF8String]);
        if (!__cppObject__) self = nil;
    }
    return self;
}

- (id)initWithCppObject:(void*)obj {
    self = [super init];
    if (self) {
        __cppObject__ = (XPF::Data*)obj;
        if (!__cppObject__) self = nil;
    }
    return self;
}

- (void)dealloc {
    delete __cppObject__;
    //[super dealloc]; // uncomment if not using ARC
}

@end
