#ifndef __Tu__FileUtils__
#define __Tu__FileUtils__

#include "../Base/Singleton.h"
#include <string>
#include "sys/stat.h"

NS_XPF_BEGIN

class FileUtil : public Singleton<FileUtil> {
public:
    virtual std::string getWritablePath() = 0;
    virtual std::string getResourcePath(const std::string& fileName) = 0;
    virtual bool createDirectory(const std::string & path, mode_t mode);
    virtual bool fileExists(const std::string & path);
    static FileUtil* constructInstance();
protected:
    FileUtil(){};
};

NS_XPF_END

#endif /* defined(__Tu__FileUtils__) */
