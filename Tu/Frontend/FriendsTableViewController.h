#import <UIKit/UIKit.h>
#import "XMPPDelegate.h"
#import <CoreData/CoreData.h>
#import "ChatMessageNotifyDelegate.h"

@interface FriendsTableViewController : UITableViewController <NSFetchedResultsControllerDelegate, ChatMessageNotifyDelegate> {
    NSFetchedResultsController *fetchedResultsController;
}
- (IBAction)login:(id)sender;
@end
