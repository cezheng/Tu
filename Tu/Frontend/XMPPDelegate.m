#import "Constants.h"
#import "XMPPDelegate.h"
#import "AccountSettingsViewController.h"
#import "GCDAsyncSocket.h"
#import "XMPP.h"
#import "XMPPLogging.h"
#import "XMPPReconnect.h"
#import "XMPPCapabilitiesCoreDataStorage.h"
#import "XMPPRosterCoreDataStorage.h"
#import "XMPPvCardAvatarModule.h"
#import "XMPPvCardCoreDataStorage.h"
#import "UIImage+PathCache.h"
#import "XMPPUserCoreDataStorageObject+Riot.h"
#import "XMPPJID+Riot.h"
#import "XMPPPresence+Riot.h"

#import "DDLog.h"
#import "DDTTYLogger.h"

#import <CFNetwork/CFNetwork.h>

// Log levels: off, error, warn, info, verbose
#if DEBUG
  static const int ddLogLevel = LOG_LEVEL_VERBOSE;
#else
  static const int ddLogLevel = LOG_LEVEL_INFO;
#endif

@interface XMPPDelegate() {
    NSMutableDictionary* summonerInfo;
}
@property (nonatomic, strong) NSString* myDisplayName;
- (void)setupStream;
- (void)teardownStream;

- (void)goOnline;
- (void)goOffline;

@end

#pragma mark -

@implementation XMPPDelegate

@synthesize xmppStream;
@synthesize xmppReconnect;
@synthesize xmppRoster;
@synthesize xmppRosterStorage;
@synthesize xmppvCardTempModule;
@synthesize xmppvCardAvatarModule;
@synthesize xmppCapabilities;
@synthesize xmppCapabilitiesStorage;
@synthesize myDisplayName;

- (void)dealloc {
	[self teardownStream];
}

+ (id)sharedDelegate {
    static XMPPDelegate *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[self alloc] init];
        [instance setupStream];
    });
    return instance;
}

#pragma mark Core Data

- (NSManagedObjectContext *)managedObjectContext_roster {
	return [xmppRosterStorage mainThreadManagedObjectContext];
}

- (NSManagedObjectContext *)managedObjectContext_capabilities {
	return [xmppCapabilitiesStorage mainThreadManagedObjectContext];
}

- (void)setupStream {
	NSAssert(xmppStream == nil, @"Method setupStream invoked multiple times");
	xmppStream = [[XMPPStream alloc] init];
	
	#if !TARGET_IPHONE_SIMULATOR
	{
		xmppStream.enableBackgroundingOnSocket = YES;
	}
	#endif
	
	xmppReconnect = [[XMPPReconnect alloc] init];
    xmppReconnect.reconnectTimerInterval = 5.0;
    xmppReconnect.usesOldSchoolSecureConnect = YES;
	
	xmppRosterStorage = [[XMPPRosterCoreDataStorage alloc] init];
//	xmppRosterStorage = [[XMPPRosterCoreDataStorage alloc] initWithInMemoryStore];
	
	xmppRoster = [[XMPPRoster alloc] initWithRosterStorage:xmppRosterStorage];
	
	xmppRoster.autoFetchRoster = YES;
	xmppRoster.autoAcceptKnownPresenceSubscriptionRequests = YES;
    xmppRoster.autoClearAllUsersAndResources = NO;
	
	xmppvCardStorage = [XMPPvCardCoreDataStorage sharedInstance];
	xmppvCardTempModule = [[XMPPvCardTempModule alloc] initWithvCardStorage:xmppvCardStorage];
	
	xmppvCardAvatarModule = [[XMPPvCardAvatarModule alloc] initWithvCardTempModule:xmppvCardTempModule];
	
	xmppCapabilitiesStorage = [XMPPCapabilitiesCoreDataStorage sharedInstance];
    xmppCapabilities = [[XMPPCapabilities alloc] initWithCapabilitiesStorage:xmppCapabilitiesStorage];

    xmppCapabilities.autoFetchHashedCapabilities = YES;
    xmppCapabilities.autoFetchNonHashedCapabilities = NO;

	[xmppReconnect         activate:xmppStream];
	[xmppRoster            activate:xmppStream];
	[xmppvCardTempModule   activate:xmppStream];
	[xmppvCardAvatarModule activate:xmppStream];
	[xmppCapabilities      activate:xmppStream];

	[xmppStream addDelegate:self delegateQueue:dispatch_get_main_queue()];
	[xmppRoster addDelegate:self delegateQueue:dispatch_get_main_queue()];
	
	[xmppStream setHostName:@"chat.na2.lol.riotgames.com"];
	[xmppStream setHostPort:5223];
	
	customCertEvaluation = YES;
    
    messageNotifyDelegates = [[NSMutableDictionary alloc] init];
    summonerInfo = [[NSMutableDictionary alloc] init];
}

- (void)addMessageNotifyDelegate:(id<ChatMessageNotifyDelegate>)delegate forJID:(NSString*)jid {
    [messageNotifyDelegates setObject:delegate forKey:jid];
}

- (void)removeMessageNotifyDelegateForJID:(NSString*)jid {
    [messageNotifyDelegates removeObjectForKey:jid];
}

- (void)setMainMessageNotifyDelegate:(id<ChatMessageNotifyDelegate>)delegate {
    mainMessageNotifyDelegate = delegate;
}

#pragma mark Private

- (void)teardownStream {
	[xmppStream removeDelegate:self];
	[xmppRoster removeDelegate:self];
	
	[xmppReconnect         deactivate];
	[xmppRoster            deactivate];
	[xmppvCardTempModule   deactivate];
	[xmppvCardAvatarModule deactivate];
	[xmppCapabilities      deactivate];
	
	[xmppStream disconnect];
	
	xmppStream = nil;
	xmppReconnect = nil;
    xmppRoster = nil;
	xmppRosterStorage = nil;
	xmppvCardStorage = nil;
    xmppvCardTempModule = nil;
	xmppvCardAvatarModule = nil;
	xmppCapabilities = nil;
	xmppCapabilitiesStorage = nil;
}

- (void)goOnline {
	XMPPPresence *presence = [XMPPPresence presence]; // type="available" is implicit
	[[self xmppStream] sendElement:presence];
}

- (void)goOffline {
    XMPPPresence *presence = [XMPPPresence presenceWithType:@"unavailable"];
	[[self xmppStream] sendElement:presence];
}

#pragma mark Connect/disconnect

- (BOOL)connect {
	if (![xmppStream isDisconnected]) {
		return YES;
	}

	NSString *myJID = [[NSUserDefaults standardUserDefaults] stringForKey:kUDKRiotJID];
	NSString *myPassword = [NSString stringWithFormat:@"%@%@", kRiotPasswordPrefix, [[NSUserDefaults standardUserDefaults] stringForKey:kUDKRiotPassword]];
	
	if (myJID == nil || myPassword == nil) {
		return NO;
	}

	[xmppStream setMyJID:[XMPPJID jidWithString:myJID]];
	password = myPassword;

	NSError *error = nil;
	if (![xmppStream oldSchoolSecureConnectWithTimeout:10 error:&error]) {
		UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Error connecting" 
		                                                    message:@"See console for error details." 
		                                                   delegate:nil 
		                                          cancelButtonTitle:@"Ok" 
		                                          otherButtonTitles:nil];
		[alertView show];

		DDLogError(@"Error connecting: %@", error);

		return NO;
	}
    
	return YES;
}

- (void)disconnect {
	[self goOffline];
	[xmppStream disconnect];
}

#pragma mark UIApplicationDelegate

- (void)applicationDidEnterBackground:(UIApplication *)application {
	DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);

	#if TARGET_IPHONE_SIMULATOR
	DDLogError(@"The iPhone simulator does not process background network traffic. "
			   @"Inbound traffic is queued until the keepAliveTimeout:handler: fires.");
	#endif

	if ([application respondsToSelector:@selector(setKeepAliveTimeout:handler:)]) {
		[application setKeepAliveTimeout:600 handler:^{
			
			DDLogVerbose(@"KeepAliveHandler");
			
			// Do other keep alive stuff here.
		}];
	}
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
	DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);
}

- (void)applicationWillTerminate:(UIApplication *)application {
    DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);

    [self teardownStream];
}

#pragma mark XMPPStream Delegate

- (void)xmppStream:(XMPPStream *)sender socketDidConnect:(GCDAsyncSocket *)socket {
	DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);
}

- (void)xmppStream:(XMPPStream *)sender willSecureWithSettings:(NSMutableDictionary *)settings {
	DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);
	if (customCertEvaluation) {
		[settings setObject:@(YES) forKey:GCDAsyncSocketManuallyEvaluateTrust];
	}
}

- (void)xmppStream:(XMPPStream *)sender didReceiveTrust:(SecTrustRef)trust
                                      completionHandler:(void (^)(BOOL shouldTrustPeer))completionHandler {
	DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);
	
	// The delegate method should likely have code similar to this,
	// but will presumably perform some extra security code stuff.
	// For example, allowing a specific self-signed certificate that is known to the app.
	
	dispatch_queue_t bgQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
	dispatch_async(bgQueue, ^{
		
		SecTrustResultType result = kSecTrustResultDeny;
		OSStatus status = SecTrustEvaluate(trust, &result);
		
		if (status == noErr && (result == kSecTrustResultProceed || result == kSecTrustResultUnspecified)) {
			completionHandler(YES);
		}
		else {
			completionHandler(NO);
		}
	});
}

- (void)xmppStreamDidSecure:(XMPPStream *)sender {
	DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);
}

- (void)xmppStreamDidConnect:(XMPPStream *)sender {
	DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);
	
	isXmppConnected = YES;
	
	NSError *error = nil;
	
	if (![[self xmppStream] authenticateWithPassword:password error:&error]) {
		DDLogError(@"Error authenticating: %@", error);
	}
}

- (void)xmppStreamDidAuthenticate:(XMPPStream *)sender {
	DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);
    self.myDisplayName = [[[[sender sessionStartIQ] childElement] elementForName:@"summoner_name"] stringValue];
	[self goOnline];
}

- (void)xmppStream:(XMPPStream *)sender didNotAuthenticate:(NSXMLElement *)error {
	DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);
}

- (BOOL)xmppStream:(XMPPStream *)sender didReceiveIQ:(XMPPIQ *)iq {
	DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);
    //NSLog(@"didReceiveIQ: %@", [iq XMLString]);
	return NO;
}

- (void)xmppStream:(XMPPStream *)sender didReceiveMessage:(XMPPMessage *)message {
	DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);

	// A simple example of inbound message handling.
	if ([message isChatMessageWithBody]) {
		XMPPUserCoreDataStorageObject *user = [xmppRosterStorage userForJID:[message from]
		                                                         xmppStream:xmppStream
		                                               managedObjectContext:[self managedObjectContext_roster]];
		NSString *body = [[message elementForName:@"body"] stringValue];
		NSString *displayName = [user displayName];
        //NSLog(@"from %@, to %@", [message fromStr], [message to]);
        NSDictionary* messageDict = \
        @{
          @"id" : @"",
          @"message" : body,
          @"from" : [user jidStr],
          @"to" : [[NSUserDefaults standardUserDefaults] stringForKey:kUDKRiotJID],
          @"read" : @"0",
          @"timestamp" : [NSString stringWithFormat:@"%ld", time(0) ]
          };
        NSDictionary* params = \
        @{
          @"me" : xmppStream.myJID.bare,
          @"withWhom" : [user jidStr],
          @"messages" : messageDict
          };
        [[XPFService sharedService] callWithEndPoint:@"ChatHistory/add" params:params];

		if ([[UIApplication sharedApplication] applicationState] == UIApplicationStateActive) {
            [mainMessageNotifyDelegate didReceivedChatMessage:messageDict];
            [[messageNotifyDelegates objectForKey:[user jidStr]] didReceivedChatMessage:messageDict];
		}
		else {
			// We are not active, so use a local notification instead
			UILocalNotification *localNotification = [[UILocalNotification alloc] init];
			localNotification.alertAction = @"Ok";
			localNotification.alertBody = [NSString stringWithFormat:@"From: %@\n\n%@",displayName,body];

			[[UIApplication sharedApplication] presentLocalNotificationNow:localNotification];
		}
	}
}

- (void)xmppStream:(XMPPStream *)sender didReceivePresence:(XMPPPresence *)presence {
	DDLogVerbose(@"%@: %@ - %@", THIS_FILE, THIS_METHOD, [presence fromStr]);
    XMPPUserCoreDataStorageObject *user = [xmppRosterStorage userForJID:[presence from]
                                                             xmppStream:xmppStream
                                                   managedObjectContext:[self managedObjectContext_roster]];
    NSInteger riotSection = [presence riotSection];
    if ([user.sectionNum integerValue] != riotSection) {
        user.section = riotSection;
    }
}

- (void)xmppStream:(XMPPStream *)sender didReceiveError:(id)error {
	DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);
}

- (void)xmppStreamDidDisconnect:(XMPPStream *)sender withError:(NSError *)error {
	DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);
	
	if (!isXmppConnected) {
		DDLogError(@"Unable to connect to server. Check xmppStream.hostName");
	}
}

#pragma mark XMPPRosterDelegate

- (void)xmppRoster:(XMPPRoster *)sender didReceiveBuddyRequest:(XMPPPresence *)presence {
	DDLogVerbose(@"%@: %@", THIS_FILE, THIS_METHOD);
	
	XMPPUserCoreDataStorageObject *user = [xmppRosterStorage userForJID:[presence from]
	                                                         xmppStream:xmppStream
	                                               managedObjectContext:[self managedObjectContext_roster]];
	
	NSString *displayName = [user displayName];
	NSString *jidStrBare = [presence fromStr];
	NSString *body = nil;
	if (![displayName isEqualToString:jidStrBare]) {
		body = [NSString stringWithFormat:@"Buddy request from %@ <%@>", displayName, jidStrBare];
	}
	else {
		body = [NSString stringWithFormat:@"Buddy request from %@", displayName];
	}
	
	
	if ([[UIApplication sharedApplication] applicationState] == UIApplicationStateActive) {
		UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:displayName
		                                                    message:body 
		                                                   delegate:nil 
		                                          cancelButtonTitle:@"Not implemented"
		                                          otherButtonTitles:nil];
		[alertView show];
	} 
	else  {
		// We are not active, so use a local notification instead
		UILocalNotification *localNotification = [[UILocalNotification alloc] init];
		localNotification.alertAction = @"Not implemented";
		localNotification.alertBody = body;
		
		[[UIApplication sharedApplication] presentLocalNotificationNow:localNotification];
	}
}

- (void)xmppRosterDidEndPopulating:(XMPPRoster *)sender {
    void (^onReadData)(id) = ^(id data) {
        NSString* jidStr = [NSString stringWithFormat:@"sum%@@pvp.net", [data objectForKey:@"id"]];
        XMPPUserCoreDataStorageObject *user = [xmppRosterStorage userForJID:[XMPPJID jidWithString:jidStr]
                                                                 xmppStream:xmppStream
                                                       managedObjectContext:[self managedObjectContext_roster]];
        NSString* path = [data objectForKey:@"profileImagePath"];
        user.photo = [UIImage imageWithPathCache:path];
        user.lastActive = [(NSNumber*)[data objectForKey:@"revisionDate"] longLongValue] / 1000;
    };
    NSMutableArray* ids = [[NSMutableArray alloc] init];
    for (XMPPJID* jid in [xmppRosterStorage jidsForXMPPStream:self.xmppStream]) {
        [ids addObject:@([jid.summonerId integerValue])];
        XMPPUserCoreDataStorageObject *user = [xmppRosterStorage userForJID:jid
                                                                 xmppStream:xmppStream
                                                       managedObjectContext:[self managedObjectContext_roster]];
        if (user.primaryResource == nil) {
            user.section = kRiotFriendSectionOffline;
        }
    }
    [[XPFService sharedService] readStreamWithEndPoint:@"RiotService/profileByIds"
                                                params:@{@"ids" : ids}
                                              callback:onReadData
                                         finalCallback:^(id finalResponse) {
                                             //do something
                                         }
                                  callbackInMainThread:YES];
}

@end
