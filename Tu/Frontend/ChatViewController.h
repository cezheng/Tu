#import <UIKit/UIKit.h>
#import "../Bridge/ChatHistoryObjc.h"
#import "ChatMessageNotifyDelegate.h"

@interface ChatViewController : UIViewController <UITableViewDataSource, UITableViewDelegate, ChatMessageNotifyDelegate>
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet UITextField *messageField;
@property (weak, nonatomic) IBOutlet UIButton *sendButton;
@property (nonatomic, strong) NSMutableArray* messages;
@property (nonatomic, copy) NSString* friendName;
@property (nonatomic, copy) NSString* friendJID;
@property (nonatomic, strong) ChatHistoryObjc* history;
- (IBAction)sendMessage:(id)sender;
@end
