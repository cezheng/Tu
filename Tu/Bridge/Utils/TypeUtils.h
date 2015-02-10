#ifndef Tu_TypeUtils_h
#define Tu_TypeUtils_h
#include <string>

NSString* stringToNSString(const std::string& str);
std::string nsstringToString(NSString* str);
std::string jsonEncodeStd(NSDictionary* dict);
NSString* jsonEncodeNS(id object);
id jsonDecode(const std::string& str);
id jsonDecode(NSString* str);
#endif
