#import "TypeUtils.h"

NSString* stringToNSString(const std::string& str) {
    return @(str.c_str());
}

std::string nsstringToString(NSString* str) {
    return [str UTF8String];
}

std::string jsonEncodeStd(NSDictionary* dict) {
    return [jsonEncodeNS(dict) UTF8String];
}

NSString* jsonEncodeNS(id object) {
    NSError *error;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:object
                                                       options:0
                                                         error:&error];
    return [[NSString alloc] initWithBytes:[jsonData bytes] length:[jsonData length] encoding:NSUTF8StringEncoding];
}

id jsonDecode(const std::string& str) {
    return jsonDecode(stringToNSString(str));
}

id jsonDecode(NSString* str) {
    NSError *error;
    return [NSJSONSerialization JSONObjectWithData:[str dataUsingEncoding:NSUTF8StringEncoding] options:0 error:&error];
}