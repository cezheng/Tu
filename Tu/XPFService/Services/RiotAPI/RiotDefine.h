#ifndef __RiotAPI_RiotDefine_h__
#define __RiotAPI_RiotDefine_h__

#define NS_RIOT_BEGIN namespace Riot {
#define NS_RIOT_END }
#define JSON_ADD_STRING(OBJ, KEY, VALUE) {rapidjson::Value value; value.SetString(VALUE.c_str(), OBJ.GetAllocator()); OBJ.AddMember(KEY, value, OBJ.GetAllocator());}
#define JSON_ADD_BOOL(OBJ, KEY, VALUE) {rapidjson::Value value; value.SetBool(VALUE); OBJ.AddMember(KEY, value, OBJ.GetAllocator());}
#define JSON_ADD_INT(OBJ, KEY, VALUE) {rapidjson::Value value; value.SetInt(VALUE); OBJ.AddMember(KEY, value, OBJ.GetAllocator());}
#define JSON_ADD_INT64(OBJ, KEY, VALUE) {rapidjson::Value value; value.SetInt64(VALUE); OBJ.AddMember(KEY, value, OBJ.GetAllocator());}

#endif // __RiotAPI_RiotDefine_h__
