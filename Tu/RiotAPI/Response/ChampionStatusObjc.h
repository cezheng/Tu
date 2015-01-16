
#ifndef __RiotAPI_Response_ChampionStatusObjc_h__
#define __RiotAPI_Response_ChampionStatusObjc_h__

@interface ChampionStatusObjc : NSObject
@property (readonly, nonatomic) void* cppObject;
@property (readonly, nonatomic, assign) BOOL active;
@property (readonly, nonatomic, assign) BOOL botEnabled;
@property (readonly, nonatomic, assign) BOOL botMmEnabled;
@property (readonly, nonatomic, assign) BOOL freeToPlay;
@property (readonly, nonatomic, assign) long id;
@property (readonly, nonatomic, assign) BOOL rankedPlayEnabled;
- (id)initWithCppObject:(void*)obj;



@end

#endif //  __RiotAPI_Response_ChampionStatusObjc_h__
