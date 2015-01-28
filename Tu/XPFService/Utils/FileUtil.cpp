#include "FileUtil.h"
#include <libgen.h>

#define B

NS_XPF_BEGIN

bool FileUtil::createDirectory(const std::string & path, mode_t mode) {
    char * baseName = basename((char*)path.c_str());
    if (!fileExists(baseName)) {
        createDirectory(baseName, mode);
    }
    return mkdir(path.c_str(), mode) == 0;
}

bool FileUtil::fileExists(const std::string &path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

NS_XPF_END