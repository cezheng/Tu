#import "ChatViewController.h"
#import "FriendsTableViewController.h"
#import "FriendsTableViewCell.h"
#import "AccountSettingsViewController.h"

#import "XMPPFramework.h"
#import "DDLog.h"

// Log levels: off, error, warn, info, verbose
#if DEBUG
static const int ddLogLevel = LOG_LEVEL_VERBOSE;
#else
static const int ddLogLevel = LOG_LEVEL_INFO;
#endif

@interface FriendsTableViewController ()

@end

@implementation FriendsTableViewController

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark View lifecycle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    XMPPDelegate* xmppDelegate = [XMPPDelegate sharedDelegate];
    [xmppDelegate setMainMessageNotifyDelegate:self];
    if ([xmppDelegate connect]) {
        void (^updateTitle)(void) = ^{
            self.navigationItem.title = xmppDelegate.myDisplayName;
        };
        void (^waitForUpdateTitle)(void) =  ^{
            while (xmppDelegate.myDisplayName == nil) {
                sleep(1);
            }
            if ([NSThread isMainThread]) {
                updateTitle();
            } else {
                dispatch_async(dispatch_get_main_queue(), updateTitle);
            }
        };
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), waitForUpdateTitle);
    } else {
        self.navigationItem.title = @"Logged out";
    }
}

- (void)viewWillDisappear:(BOOL)animated {
    [[XMPPDelegate sharedDelegate] setMainMessageNotifyDelegate:nil];
    [super viewWillDisappear:animated];
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark NSFetchedResultsController
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

- (void)controllerDidChangeContent:(NSFetchedResultsController *)controller {
    [[self tableView] reloadData];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark UITableViewCell helpers
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

- (void)configurePhotoForCell:(FriendsTableViewCell *)cell user:(XMPPUserCoreDataStorageObject *)user {
    // Our xmppRosterStorage will cache photos as they arrive from the xmppvCardAvatarModule.
    // We only need to ask the avatar module for a photo, if the roster doesn't have it.
    
    if (user.photo != nil) {
        cell.profileImage.image = user.photo;
    }
    else {
        NSData *photoData = [[[XMPPDelegate sharedDelegate] xmppvCardAvatarModule] photoDataForJID:user.jid];
        
        if (photoData != nil)
            cell.profileImage.image = [UIImage imageWithData:photoData];
        else
            cell.profileImage.image = [UIImage imageNamed:@"defaultPerson"];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark UITableView
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return [[[self fetchedResultsController] sections] count];
}

- (NSString *)tableView:(UITableView *)sender titleForHeaderInSection:(NSInteger)sectionIndex
{
    NSArray *sections = [[self fetchedResultsController] sections];
    
    if (sectionIndex < [sections count])
    {
        id <NSFetchedResultsSectionInfo> sectionInfo = [sections objectAtIndex:sectionIndex];
        
        int section = [sectionInfo.name intValue];
        switch (section) {
            case 0  : return @"Available";
            case 1  : return @"Away";
            default : return @"Offline";
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
    
    cell.displayNameLabel.text = user.displayName;
    [self configurePhotoForCell:cell user:user];
    NSSet *resources = [user resources];
    NSLog(@"resources size for %@: %ld", [user jidStr], [resources count]);
    return cell;
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
            destinationViewController.history = [[XMPPDelegate sharedDelegate] chatHistoryWithJID:user.jidStr];
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark Actions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

- (IBAction)login:(id)sender{
    [[XMPPDelegate sharedDelegate] connect];
}

- (void) didReceivedChatMessage:(ChatMessageObjc*)message {
    
}
@end