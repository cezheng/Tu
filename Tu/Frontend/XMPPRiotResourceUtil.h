#import <Foundation/Foundation.h>

@interface XMPPRiotResourceUtil : NSObject 
+ (NSInteger)riotSectionByStatus:(NSString*)status show:(NSString*)show;
+ (NSString*)gameStatusByStatus:(NSString*)status;
+ (NSString*)playingAsByStatus:(NSString*)status;
+ (long long)timestampByStatus:(NSString*)status;
+ (NSString*)statusMsgByStatus:(NSString*)status;
+ (NSString*)rankByStatus:(NSString*)status;
@end
