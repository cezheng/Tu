#ifndef __Tu__KVS__
#define __Tu__KVS__

#include "XPFService/XPFDefine.h"
#include "XPFService/Base/Singleton.h"
#include "LevelDBHolder.h"
#include <unordered_map>

NS_XPF_BEGIN

class KVS : public Singleton<KVS> {
public:
    LevelDBHolder& operator[](const std::string & nameSpace);
protected:
    class KVSHolder : public LevelDBHolder {
    public:
        KVSHolder(const std::string & nameSpace = "global");
    protected:
        virtual std::string getLevelDBBasePath() const;
        virtual std::string getLevelDBFileName() const;
    private:
        std::string _nameSpace;
    };
private:
    std::unordered_map<std::string, LevelDBHolder*> _nameSpaces;
};

NS_XPF_END
#endif /* defined(__Tu__KVS__) */
