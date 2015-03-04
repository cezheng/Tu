#import "Constants.h"
#import "ChatViewController.h"
#import "FriendsTableViewController.h"
#import "FriendsTableViewCell.h"
#import "AccountSettingsViewController.h"
#import "XPFService/Portal/XPFService.h"
#import "XMPPUserCoreDataStorageObject+Riot.h"
#import "UIImage+PathCache.h"

#import "XMPPFramework.h"
#import "DDLog.h"

// Log levels: off, error, warn, info, verbose
#if DEBUG
static const int ddLogLevel = LOG_LEVEL_VERBOSE;
#else
static const int ddLogLevel = LOG_LEVEL_INFO;
#endif

@interface FriendsTableViewController () {
    BOOL needRefresh;
    NSDictionary* summonerInfo;
}

@end

@implementation FriendsTableViewController

#pragma mark - UIViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    needRefresh = YES;
    [[XPFService sharedService] callWithEndPoint:@"RiotAPI/serviceStatusByRegion"
                                          params:@{@"region" : @"na"}
                                        callback:^(id response) {
    }];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    XMPPDelegate* xmppDelegate = [XMPPDelegate sharedDelegate];
    [xmppDelegate setMainMessageNotifyDelegate:self];
    [xmppDelegate addObserver:self
                   forKeyPath:@"myDisplayName"
                      options:0
                      context:nil];
    if ([xmppDelegate connect]) {
    } else {
        self.navigationItem.title = @"Logged out";
    }
}

- (void)viewWillDisappear:(BOOL)animated {
    XMPPDelegate* xmppDelegate = [XMPPDelegate sharedDelegate];
    [xmppDelegate setMainMessageNotifyDelegate:nil];
    [xmppDelegate removeObserver:self forKeyPath:@"myDisplayName"];
    [super viewWillDisappear:animated];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
    if ([segue.identifier isEqualToString:@"ChatSegue"]) {
        ChatViewController* destinationViewController = [segue destinationViewController];
        UITableViewCell* cell = (UITableViewCell*)sender;
        XMPPUserCoreDataStorageObject *user = [[self fetchedResultsController] objectAtIndexPath:[self.tableView indexPathForCell:cell]];
        if (![destinationViewController.friendJID isEqualToString:user.jidStr]) {
            destinationViewController.friendJID = user.jidStr;
            destinationViewController.friendName = user.displayName;
            [destinationViewController.messages removeAllObjects];
        }
    }
}

#pragma mark - NSObject

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
    if ([keyPath isEqualToString:@"myDisplayName"]) {
        self.navigationItem.title = [object valueForKeyPath:keyPath];
    }
}

#pragma mark - NSFetchedResultsController

- (NSFetchedResultsController *)fetchedResultsController {
    if (fetchedResultsController == nil) {
        NSManagedObjectContext *moc = [[XMPPDelegate sharedDelegate] managedObjectContext_roster];
        
        NSEntityDescription *entity = [NSEntityDescription entityForName:@"XMPPUserCoreDataStorageObject"
                                                  inManagedObjectContext:moc];
        
        NSSortDescriptor *sd1 = [[NSSortDescriptor alloc] initWithKey:@"sectionNum" ascending:YES];
        NSSortDescriptor *sd2 = [[NSSortDescriptor alloc] initWithKey:@"displayName" ascending:YES];
        
        NSArray *sortDescriptors = [NSArray arrayWithObjects:sd1, sd2, nil];
        
        NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
        [fetchRequest setEntity:entity];
        [fetchRequest setSortDescriptors:sortDescriptors];
        [fetchRequest setFetchBatchSize:10];
        
        fetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest
                                                                       managedObjectContext:moc
                                                                         sectionNameKeyPath:@"sectionNum"
                                                                                  cacheName:nil];
        [fetchedResultsController setDelegate:self];
        
        NSError *error = nil;
        if (![fetchedResultsController performFetch:&error]) {
            DDLogError(@"Error performing fetch: %@", error);
        }
        
    }
    
    return fetchedResultsController;
}

- (void)controllerWillChangeContent:(NSFetchedResultsController *)controller {
    [self.tableView beginUpdates];
}

- (void)controllerDidChangeContent:(NSFetchedResultsController *)controller {
    [self.tableView endUpdates];
}

- (void) controller:(NSFetchedResultsController *)controller
   didChangeSection:(id<NSFetchedResultsSectionInfo>)sectionInfo
            atIndex:(NSUInteger)sectionIndex
      forChangeType:(NSFetchedResultsChangeType)type {
    UITableView *tableView = self.tableView;
    switch(type) {
        case NSFetchedResultsChangeInsert:
            [tableView insertSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationAutomatic];
            break;
        case NSFetchedResultsChangeDelete:
            [tableView deleteSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationAutomatic];
            break;
        default:
            break;
    }
}

- (void) controller:(NSFetchedResultsController *)controller
    didChangeObject:(id)anObject
        atIndexPath:(NSIndexPath *)indexPath
      forChangeType:(NSFetchedResultsChangeType)type
       newIndexPath:(NSIndexPath *)newIndexPath {
    UITableView *tableView = self.tableView;
    switch(type) {
        case NSFetchedResultsChangeInsert:
            [tableView insertRowsAtIndexPaths:@[newIndexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
            break;
            
        case NSFetchedResultsChangeDelete:
            [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
            break;
            
        case NSFetchedResultsChangeUpdate:
            [tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
            break;
            
        case NSFetchedResultsChangeMove:
            [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
            [tableView insertRowsAtIndexPaths:@[newIndexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
            break;
        default:
            break;
    }
}

#pragma mark - UITableViewCell helpers

- (void)configurePhotoForCell:(FriendsTableViewCell *)cell user:(XMPPUserCoreDataStorageObject *)user {
    if (user.photo) {
        cell.profileImage.image = user.photo;
    }
}

#pragma mark - UITableViewDataSource

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return [[[self fetchedResultsController] sections] count];
}

- (NSString *)tableView:(UITableView *)sender titleForHeaderInSection:(NSInteger)sectionIndex {
    NSArray *sections = [[self fetchedResultsController] sections];
    
    if (sectionIndex < [sections count]) {
        id <NSFetchedResultsSectionInfo> sectionInfo = [sections objectAtIndex:sectionIndex];
        
        int section = [sectionInfo.name intValue];
        if (section <= kRiotFriendSectionOffline) {
            return kRiotFriendSectionTitles[section];
        } else {
            return kRiotFriendSectionTitles[kRiotFriendSectionOffline];
        }
    }
    return @"";
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)sectionIndex {
    NSArray *sections = [[self fetchedResultsController] sections];
    
    if (sectionIndex < [sections count]) {
        id <NSFetchedResultsSectionInfo> sectionInfo = [sections objectAtIndex:sectionIndex];
        return sectionInfo.numberOfObjects;
    }
    
    return 0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"FriendCell";
    
    FriendsTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[FriendsTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault
                                      reuseIdentifier:CellIdentifier];
    }
    
    XMPPUserCoreDataStorageObject *user = [[self fetchedResultsController] objectAtIndexPath:indexPath];
    
    id <NSFetchedResultsSectionInfo> sectionInfo = [[[self fetchedResultsController] sections] objectAtIndex:indexPath.section];
    int section = [sectionInfo.name intValue];
    NSString* additionalInfo;
    switch (section) {
        case kRiotFriendSectionInGame:
            additionalInfo = [NSString stringWithFormat:@"Playing as %@ for %lld minutes",  user.playingAs, (time(0) - user.timestamp ) / 60];
            break;
        case kRiotFriendSectionInQueue:
            additionalInfo = [NSString stringWithFormat:@"For %lld minutes", (user.timestamp - time(0)) / 60];
            break;
        case kRiotFriendSectionOffline:
            additionalInfo = [NSString stringWithFormat:@"Last Active : %@", user.lastActiveDate];
            break;
        default:
            if ([user.statusMsg length]) {
                additionalInfo = user.statusMsg;
            } else {
                additionalInfo = user.rank;
            }
            break;
    }
    cell.additionalInfoLabel.text = additionalInfo;
    cell.displayNameLabel.text = user.displayName;
    
    [self configurePhotoForCell:cell user:user];
    
    return cell;
}

#pragma mark - IBAction

- (IBAction)login:(id)sender{
    [[XMPPDelegate sharedDelegate] connect];
}

#pragma mark - ChatMessageNotifyDelegate

- (void) didReceivedChatMessage:(NSDictionary*)message {
    
}
@end