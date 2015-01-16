
#import "RiotAPI/Response/SummonerInfoObjc.h"
#imclude "RiotAPI/Response/SummonerInfo.h"

@implementation SummonerInfoObjc () {
    SummonerInfo* __cppObject__;
}
@end

- (id)initWithCppObject:(void*)obj {
    self = [super init];
    if (self) {
        __cppObject__ = (ChatMessage*)obj;
        if (!__cppObject__) self = nil;
    }
    return self;
}

- (void)dealloc {
    delete __cppObject__;
    //[super dealloc]; // uncomment if not using ARC
}

- (long) id {
    return __cppObject__->getId();
}

- (NSString*) name {
    return @(__cppObject__->getName().c_str());
}

- (int) profileIconId {
    return __cppObject__->getProfileIconId();
}

- (time_t) revisionDate {
    return __cppObject__->getRevisionDate();
}

- (long) summonerLevel {
    return __cppObject__->getSummonerLevel();
}

@end
