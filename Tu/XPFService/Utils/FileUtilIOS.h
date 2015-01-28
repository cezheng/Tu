#ifndef __Tu__FileUtilIOS__
#define __Tu__FileUtilIOS__

#include "FileUtil.h"

NS_XPF_BEGIN

class FileUtilIOS : public FileUtil {
    //make parent static method able to create an instance
    friend class FileUtil;
public:
    virtual std::string getWritablePath();
    virtual std::string getResourcePath(const std::string& fileName);
protected:
    FileUtilIOS() {};
};

NS_XPF_END
#endif /* defined(__Tu__FileUtilIOS__) */
