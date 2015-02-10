#include "FileUtilIOS.h"

NS_XPF_BEGIN

static std::string getDocumentsDirectory() {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    std::string strRet = [documentsDirectory UTF8String];
    strRet.append("/");
    return strRet;
}

FileUtil* FileUtil::constructInstance() {
    return new FileUtilIOS();
}

std::string FileUtilIOS::getWritablePath() {
    static std::string path = getDocumentsDirectory();
    return path;
}

std::string FileUtilIOS::getResourcePath(const std::string& fileName) {
    NSString *pathInfo = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:fileName.c_str()] ofType:@""];
    return [pathInfo UTF8String];
}

NS_XPF_END