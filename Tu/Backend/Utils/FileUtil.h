#ifndef __Tu__FileUtils__
#define __Tu__FileUtils__

#include "../Base/Singleton.h"
#include <string>
#include "sys/stat.h"

class FileUtil : public Singleton<FileUtil> {
public:
    virtual std::string getWritablePath() = 0;
    virtual std::string getResourcePath(const std::string& fileName) = 0;
    static FileUtil* constructInstance();
protected:
    FileUtil(){};
};

#endif /* defined(__Tu__FileUtils__) */
