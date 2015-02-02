#include "KVS.h"
#include "XPFService/Utils/FileUtil.h"
#include "XPFService/Utils/Exception.h"

NS_XPF_BEGIN

LevelDBHolder& KVS::operator[](const std::string & nameSpace) {
    if (_nameSpaces.find(nameSpace) == _nameSpaces.end() || _nameSpaces[nameSpace] == nullptr) {
        _nameSpaces[nameSpace] = new KVSHolder(nameSpace);
        REQUIRE(_nameSpaces[nameSpace] != nullptr, "create KVSHolder instance failed.");
    }
    return *(_nameSpaces[nameSpace]);
}

KVS::KVSHolder::KVSHolder(const std::string & nameSpace) : _nameSpace(nameSpace) {
    this->init();
}

std::string KVS::KVSHolder::getLevelDBBasePath() const {
    return FileUtil::getInstance()->getWritablePath() + "XPF/KVS/";
}

std::string KVS::KVSHolder::getLevelDBFileName() const {
    return _nameSpace + ".leveldb"; 
}


NS_XPF_END