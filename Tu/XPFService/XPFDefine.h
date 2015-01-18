#ifndef __XPFService_XPFDefine_h__
#define __XPFService_XPFDefine_h__

#define NS_XPF_BEGIN namespace XPF {
#define NS_XPF_END }

#define XPF_ENDPOINT(CLASS, METHOD) std::bind(&CLASS::METHOD, CLASS::getInstance(), std::placeholders::_1)

#endif  // __XPFService_XPFDefine_h__
