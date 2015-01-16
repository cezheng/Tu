
#import "RiotAPI/Response/ChampionStatusObjc.h"
#imclude "RiotAPI/Response/ChampionStatus.h"

@implementation ChampionStatusObjc () {
    ChampionStatus* __cppObject__;
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

- (BOOL) active {
    return __cppObject__->getActive();
}

- (BOOL) botEnabled {
    return __cppObject__->getBotEnabled();
}

- (BOOL) botMmEnabled {
    return __cppObject__->getBotMmEnabled();
}

- (BOOL) freeToPlay {
    return __cppObject__->getFreeToPlay();
}

- (long) id {
    return __cppObject__->getId();
}

- (BOOL) rankedPlayEnabled {
    return __cppObject__->getRankedPlayEnabled();
}

@end
