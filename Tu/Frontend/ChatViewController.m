#import "ChatViewController.h"
#import "BubbleTableViewCell.h"
#import "BubbleTableView.h"
#import "XMPPDelegate.h"

@interface ChatViewController ()


@end

@implementation ChatViewController
@synthesize messages;
@synthesize friendJID;
@synthesize friendName;
@synthesize bubbleTable;

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
    
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;

    if (messages == nil) {
        messages = [[NSMutableArray alloc] init];
    }
    if ([messages count] == 0) {
        NSArray* fetchMessages = [[[XPFService sharedService] callWithEndPoint:@"ChatHistory/getRecentN" params:@{@"withWhom" : friendJID, @"amount" : @10}] decodeObject];
        for (int i = 0; i < fetchMessages.count; i++) {
            BubbleType bubbleType;
            NSDictionary *messageCurrent = [fetchMessages objectAtIndex:i];
            if (messageCurrent[@"from"] == friendJID) {
                bubbleType = BubbleTypeSomeoneElse;
            } else {
                bubbleType = BubbleTypeMine;
            }
            BubbleData *historyBubble = [BubbleData dataWithText:[fetchMessages objectAtIndex:i][@"message"] date:[NSDate dateWithTimeIntervalSinceNow:-i] type:bubbleType];
            [messages addObject:historyBubble];
        }
    }

    bubbleTable.bubbleDataSource = self;
    // The line below sets the snap interval in seconds. This defines how the bubbles will be grouped in time.
    // Interval of 120 means that if the next messages comes in 2 minutes since the last message, it will be added into the same group.
    // Groups are delimited with header which contains date and time for the first message in the group.
    
    bubbleTable.snapInterval = 120;
    
    // The line below enables avatar support. Avatar can be specified for each bubble with .avatar property of NSBubbleData.
    // Avatars are enabled for the whole table at once. If particular NSBubbleData misses the avatar, a default placeholder will be set (missingAvatar.png)
    
    bubbleTable.showAvatars = YES;
    
    [bubbleTable reloadData];
    
    self.navigationItem.title = friendName;
    UIGestureRecognizer *tapper;
    tapper = [[UITapGestureRecognizer alloc]
              initWithTarget:self action:@selector(handleSingleTap:)];
    tapper.cancelsTouchesInView = NO;
    [self.view addGestureRecognizer:tapper];
    [self registerForKeyboardNotifications];
    self.navigationController.navigationBar.translucent = NO;
    
    // Keyboard events
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWasShown:) name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillBeHidden:) name:UIKeyboardWillHideNotification object:nil];
}

- (void) viewWillAppear:(BOOL)animated {
    [[XMPPDelegate sharedDelegate] addMessageNotifyDelegate:self forJID:friendJID];
}

- (void) viewWillDisappear:(BOOL)animated {
    [[XMPPDelegate sharedDelegate] removeMessageNotifyDelegateForJID:friendJID];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
#pragma mark - UIBubbleTableViewDataSource implementation

- (NSInteger)rowsForBubbleTable:(BubbleTableView *)tableView
{
    return [messages count];
}

- (BubbleData *)bubbleTableView:(BubbleTableView *)tableView dataForRow:(NSInteger)row
{
    return [messages objectAtIndex:row];
}

/*#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [messages count];
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    BubbleTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"MessageCell" forIndexPath:indexPath];
    NSDictionary* message = [messages objectAtIndex:indexPath.row];
    cell.messageLabel.text = message[@"message"];
    return cell;
}
*/

/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/

/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
    } else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/

/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}
*/

/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (IBAction)sendMessage:(id)sender {
    NSString* messageText = _messageField.text;
    if ([messageText length]) {
        XMPPDelegate* xmppDelegate = [XMPPDelegate sharedDelegate];
        NSXMLElement *body = [NSXMLElement elementWithName:@"body"];
        [body setStringValue:messageText];
        XMPPMessage* message = [XMPPMessage messageWithType:@"chat" to:[XMPPJID jidWithString:friendJID] elementID:nil child:body];
        [xmppDelegate.xmppStream sendElement:message];
        NSDictionary* messageDict = @{
                                     @"id" : @"",
                                     @"message" : messageText,
                                     @"from" : [[NSUserDefaults standardUserDefaults] stringForKey:@"kRiotJID"],
                                     @"to" : friendJID,
                                     @"read" : @"0",
                                     @"timestamp" : [NSString stringWithFormat:@"%ld", time(0) ]
                                     };
        NSDictionary* params = \
        @{
          @"withWhom" : friendJID,
          @"messages" : messageDict
          };
        [[XPFService sharedService] callWithEndPoint:@"ChatHistory/add" params:params];
        BubbleData *sendData = [BubbleData dataWithText:messageDict[@"message"] date:[NSDate dateWithTimeIntervalSinceNow:0] type:BubbleTypeMine];
        [messages addObject:sendData];
        //[bubbleTable insertRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:messages.count - 1 inSection:0]] withRowAnimation:UITableViewRowAnimationAutomatic];
        [bubbleTable reloadData];
        [_messageField setText:@""];
        [self.view endEditing:YES];
    }
}

- (void) didReceivedChatMessage:(NSDictionary*)message {
    [messages addObject:message];
    [bubbleTable insertRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:[messages indexOfObject:message] inSection:0]] withRowAnimation:UITableViewRowAnimationAutomatic];
}

- (void)handleSingleTap:(UITapGestureRecognizer *) sender {
    if (CGRectContainsPoint(bubbleTable.frame, [sender locationInView:self.view])) {
        [self.view endEditing:YES];
    }
}

// Call this method somewhere in your view controller setup code.
- (void)registerForKeyboardNotifications {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardWasShown:)
                                                 name:UIKeyboardDidShowNotification object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardWillBeHidden:)
                                                 name:UIKeyboardWillHideNotification object:nil];
}



#pragma mark - Keyboard events

- (void)keyboardWasShown:(NSNotification*)aNotification
{
    NSDictionary* info = [aNotification userInfo];
    CGSize kbSize = [[info objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;
    
    [UIView animateWithDuration:0.2f animations:^{
        
        CGRect frame = _inputView.frame;
        frame.origin.y -= kbSize.height;
        _inputView.frame = frame;
        
        frame = bubbleTable.frame;
        frame.size.height -= kbSize.height;
        bubbleTable.frame = frame;
    }];
}

- (void)keyboardWillBeHidden:(NSNotification*)aNotification
{
    NSDictionary* info = [aNotification userInfo];
    CGSize kbSize = [[info objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;
    
    [UIView animateWithDuration:0.2f animations:^{
        
        CGRect frame = _inputView.frame;
        frame.origin.y += kbSize.height;
        _inputView.frame = frame;
        
        frame = bubbleTable.frame;
        frame.size.height += kbSize.height;
        bubbleTable.frame = frame;
    }];
}
@end
