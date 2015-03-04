#ifndef __Tu__UrlUtil__
#define __Tu__UrlUtil__

#include "XPFService/XPFDefine.h"
#include <string>

NS_XPF_BEGIN

/* Converts a hex character to its integer value */
char from_hex(char ch);

/* Converts an integer value to its hex character*/
char to_hex(char code);

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
std::string url_encode(const std::string & url);

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
std::string url_decode(const std::string & url);

NS_XPF_END

#endif /* defined(__Tu__UrlUtil__) */
