#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

#import "XMPPFramework.h"
#import "../Bridge/ChatHistoryObjc.h"
#import "ChatMessageNotifyDelegate.h"

#define DEBUG 1
@class AccountSettingsViewController;


@interface XMPPDelegate : NSObject <XMPPRosterDelegate> {
	XMPPStream *xmppStream;
	XMPPReconnect *xmppReconnect;
    XMPPRoster *xmppRoster;
	XMPPRosterCoreDataStorage *xmppRosterStorage;
    XMPPvCardCoreDataStorage *xmppvCardStorage;
	XMPPvCardTempModule *xmppvCardTempModule;
	XMPPvCardAvatarModule *xmppvCardAvatarModule;
	XMPPCapabilities *xmppCapabilities;
	XMPPCapabilitiesCoreDataStorage *xmppCapabilitiesStorage;
	
	NSString *password;
	
	BOOL customCertEvaluation;
	
	BOOL isXmppConnected;
    
    NSMutableDictionary* chatHistory;
    NSMutableDictionary* messageNotifyDelegates;
    id<ChatMessageNotifyDelegate> mainMessageNotifyDelegate;
}

@property (nonatomic, strong, readonly) XMPPStream *xmppStream;
@property (nonatomic, strong, readonly) XMPPReconnect *xmppReconnect;
@property (nonatomic, strong, readonly) XMPPRoster *xmppRoster;
@property (nonatomic, strong, readonly) XMPPRosterCoreDataStorage *xmppRosterStorage;
@property (nonatomic, strong, readonly) XMPPvCardTempModule *xmppvCardTempModule;
@property (nonatomic, strong, readonly) XMPPvCardAvatarModule *xmppvCardAvatarModule;
@property (nonatomic, strong, readonly) XMPPCapabilities *xmppCapabilities;
@property (nonatomic, strong, readonly) XMPPCapabilitiesCoreDataStorage *xmppCapabilitiesStorage;
@property (nonatomic, strong, readonly) NSString* myDisplayName;

- (NSManagedObjectContext *)managedObjectContext_roster;
- (NSManagedObjectContext *)managedObjectContext_capabilities;
- (ChatHistoryObjc*)chatHistoryWithJID:(NSString*)jid;
- (void)addMessageNotifyDelegate:(id<ChatMessageNotifyDelegate>)delegate forJID:(NSString*)jid;
- (void)removeMessageNotifyDelegateForJID:(NSString*)jid;
- (void)setMainMessageNotifyDelegate:(id<ChatMessageNotifyDelegate>)delegate;

+ (id)sharedDelegate;
- (BOOL)connect;
- (void)disconnect;

@end
