#import "RiotAPI/Response/ChampionStatusObjc.h"
#include "RiotAPI/Response/ChampionStatus.h"

@interface ChampionStatusObjc () {
    Riot::ChampionStatus* __cppObject__;
}
@end

@implementation ChampionStatusObjc

- (id)initWithCppObject:(void*)obj {
    self = [super init];
    if (self) {
        __cppObject__ = (Riot::ChampionStatus*)obj;
        if (!__cppObject__) self = nil;
    }
    return self;
}

- (void)dealloc {
    delete __cppObject__;
    //[super dealloc]; // uncomment if not using ARC
}

- (BOOL) active {
    return __cppObject__->isActive();
}

- (BOOL) botEnabled {
    return __cppObject__->isBotEnabled();
}

- (BOOL) botMmEnabled {
    return __cppObject__->isBotMmEnabled();
}

- (BOOL) freeToPlay {
    return __cppObject__->isFreeToPlay();
}

- (long) id {
    return __cppObject__->getId();
}

- (BOOL) rankedPlayEnabled {
    return __cppObject__->isRankedPlayEnabled();
}

@end
