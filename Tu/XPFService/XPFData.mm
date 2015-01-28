#import "XPFData.h"
#include "json11/json11.hpp"
#import "../Bridge/Utils/TypeUtils.h"

@interface XPFData () {
    json11::Json* __cppObject__;;
}

@end

@implementation XPFData

- (id)initWithObject:(id)object {
    self = [super init];
    if (self) {
        NSError* error;
        NSString *jsonString = [[NSString alloc] initWithData:[NSJSONSerialization dataWithJSONObject:object options:NSJSONWritingPrettyPrinted error:&error] encoding:NSUTF8StringEncoding];
        std::string parseError;
        __cppObject__ = new json11::Json(std::move(json11::Json::parse([jsonString UTF8String], parseError)));
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
        __cppObject__ = new json11::Json(std::move(*(json11::Json*)obj));
        if (!__cppObject__) self = nil;
    }
    return self;
}

- (void)dealloc {
    delete __cppObject__;
    //[super dealloc]; // uncomment if not using ARC
}

- (id) decodeObject {
    return jsonDecode(__cppObject__->dump());
}

- (void*) cppObject {
    return __cppObject__;
}

- (NSString*) json {
    return @(__cppObject__->dump().c_str());
}

@end
