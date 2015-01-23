#ifndef __RiotAPI_Response_SummonerInfoObjc_h__
#define __RiotAPI_Response_SummonerInfoObjc_h__

@interface SummonerInfoObjc : NSObject
@property (readonly, nonatomic) void* cppObject;
@property (readonly, nonatomic, assign) long id;
@property (readonly, nonatomic, copy) NSString* name;
@property (readonly, nonatomic, assign) int profileIconId;
@property (readonly, nonatomic, assign) time_t revisionDate;
@property (readonly, nonatomic, assign) long summonerLevel;
- (id)initWithCppObject:(void*)obj;



@end

#endif //  __RiotAPI_Response_SummonerInfoObjc_h__
