#include "FileUtil.h"
#include <libgen.h>
#include <fstream>

NS_XPF_BEGIN

bool FileUtil::createDirectory(const std::string & path, mode_t mode) {
    char * dirName = dirname((char*)path.c_str());
    if (!fileExists(dirName)) {
        createDirectory(dirName, mode);
    }
    return mkdir(path.c_str(), mode) == 0;
}

bool FileUtil::fileExists(const std::string &path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

std::string FileUtil::readFileContent(const std::string & path) {
    std::ifstream fin(path);
    if (!fin) {
        return "";
    }
    std::string str;
    fin.seekg(0, std::ios::end);
    str.reserve(fin.tellg());
    fin.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(fin)),
               std::istreambuf_iterator<char>());
    return str;
}

void FileUtil::deleteFile(const std::string &path) {
    remove(path.c_str());
}

NS_XPF_END