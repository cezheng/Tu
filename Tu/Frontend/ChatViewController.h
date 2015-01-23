#import <UIKit/UIKit.h>
#import "ChatMessageNotifyDelegate.h"

@interface ChatViewController : UIViewController <UITableViewDataSource, UITableViewDelegate, ChatMessageNotifyDelegate>
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet UITextField *messageField;
@property (weak, nonatomic) IBOutlet UIButton *sendButton;
@property (weak, nonatomic) IBOutlet UIView *inputView;
@property (nonatomic, strong) NSMutableArray* messages;
@property (nonatomic, copy) NSString* friendName;
@property (nonatomic, copy) NSString* friendJID;
- (IBAction)sendMessage:(id)sender;
@end
