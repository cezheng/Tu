#import "XPFData.h"
#import "Data.h"
#import "../Bridge/Utils/TypeUtils.h"

@interface XPFData () {
    XPF::Data* __cppObject__;;
}

@end

@implementation XPFData

- (id)initWithObject:(id)object {
    self = [super init];
    if (self) {
        NSError* error;
        NSString *jsonString = [[NSString alloc] initWithData:[NSJSONSerialization dataWithJSONObject:object options:NSJSONWritingPrettyPrinted error:&error] encoding:NSUTF8StringEncoding];
        __cppObject__ = new XPF::Data([jsonString UTF8String]);
        if (!__cppObject__) self = nil;
    }
    return self;
}

- (id)initWithCppObject:(void*)obj {
    if (obj == NULL) {
        self = nil;
        return self;
    }
    self = [super init];
    if (self) {
        __cppObject__ = new XPF::Data(std::move(*(XPF::Data*)obj));
        if (!__cppObject__) self = nil;
    }
    return self;
}

- (void)dealloc {
    delete __cppObject__;
    //[super dealloc]; // uncomment if not using ARC
}

- (id) decodeObject {
    return jsonDecode(__cppObject__->getJson());
}

- (void*) cppObject {
    return __cppObject__;
}

- (NSString*) json {
    return @(__cppObject__->getJson().c_str());
}

@end
