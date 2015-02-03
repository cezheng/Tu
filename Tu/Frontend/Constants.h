#ifndef Tu_Constants_h
#define Tu_Constants_h

extern NSString *const kUDKRiotJID;
extern NSString *const kRiotJIDDomain;
extern NSString *const kUDKRiotLoginName;
extern NSString *const kRiotPasswordPrefix;
extern NSString *const kUDKRiotPassword;

extern NSString* kRiotFriendSectionTitles[];

extern NSString * kRiotGameStatusOutOfGame;
extern NSString * kRiotGameStatusInGame;
extern NSString * kRiotGameStatusInQueue;
extern NSString * kRiotGameStatusSpectating;
extern NSString * kRiotGameStatusChampionSelect;
extern NSString * kRiotGameStatusHostingPracticeGame;

enum SectionByStatus {
    kRiotFriendSectionOnline = 0,
    kRiotFriendSectionInGame = 1,
    kRiotFriendSectionInQueue = 2,
    kRiotFriendSectionSpectating = 3,
    kRiotFriendSectionChampionSelect = 4,
    kRiotFriendSectionHostingPracticeGame = 5,
    kRiotFriendSectionAway = 6,
    kRiotFriendSectionOffline = 7
};

#endif
