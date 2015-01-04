#import <UIKit/UIKit.h>
#import "XMPPDelegate.h"
#import <CoreData/CoreData.h>

@interface FriendsTableViewController : UITableViewController <NSFetchedResultsControllerDelegate> {
    NSFetchedResultsController *fetchedResultsController;
}
- (IBAction)login:(id)sender;
@end
