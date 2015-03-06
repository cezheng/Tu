#import "ChatViewController.h"
#import "ChatMessageTableViewCell.h"
#import "XMPPDelegate.h"

@interface ChatViewController ()

@end

@implementation ChatViewController
@synthesize messages;
@synthesize friendJID;
@synthesize friendName;
@synthesize initialLoadCount;

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
        NSDictionary* param = @{
                                @"me" : [[XMPPDelegate sharedDelegate] xmppStream].myJID.bare,
                                @"withWhom" : friendJID,
                                @"amount" : @(initialLoadCount)
                                };
        NSArray* fetchMessages = [[[XPFService sharedService] callWithEndPoint:@"ChatHistory/getRecentN" params:param] decodeObject];
        [messages addObjectsFromArray:fetchMessages];
    }

    [_tableView setDataSource:self];
    [_tableView setDelegate:self];
    _tableView.estimatedRowHeight = 60.0;
    _tableView.rowHeight = UITableViewAutomaticDimension;
    
    self.navigationItem.title = friendName;
    UIGestureRecognizer *tapper;
    tapper = [[UITapGestureRecognizer alloc]
              initWithTarget:self action:@selector(handleSingleTap:)];
    tapper.cancelsTouchesInView = NO;
    [self.view addGestureRecognizer:tapper];
    [self registerForKeyboardNotifications];
    self.navigationController.navigationBar.translucent = NO;
    [[XPFService sharedService] callWithEndPoint:@"Chat/Read"
                                          params:@{
                                                   @"me" : [[XMPPDelegate sharedDelegate] xmppStream].myJID.bare,
                                                   @"withWhom" : friendJID
                                                   }];
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

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [messages count];
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    ChatMessageTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"MessageCell" forIndexPath:indexPath];
    NSDictionary* message = [messages objectAtIndex:indexPath.row];
    cell.messageLabel.text = message[@"message"];
    return cell;
}


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
                                     @"timestamp" : [NSString stringWithFormat:@"%ld", time(0) ]
                                     };
        [messages addObject:messageDict];
        NSDictionary* params = \
        @{
          @"me" : xmppDelegate.xmppStream.myJID.bare,
          @"withWhom" : friendJID,
          @"messages" : messageDict
          };
        [[XPFService sharedService] callWithEndPoint:@"ChatHistory/add" params:params];
        [_tableView insertRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:[messages indexOfObject:messageDict] inSection:0]] withRowAnimation:UITableViewRowAnimationAutomatic];
        [_messageField setText:@""];
        [self.view endEditing:YES];
    }
}

- (void) didReceivedChatMessage:(NSDictionary*)message {
    [messages addObject:message];
    [_tableView insertRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:[messages indexOfObject:message] inSection:0]] withRowAnimation:UITableViewRowAnimationAutomatic];
    [[XPFService sharedService] callWithEndPoint:@"Chat/Read"
                                          params:@{
                                                   @"me" : [[XMPPDelegate sharedDelegate] xmppStream].myJID.bare,
                                                   @"withWhom" : message[@"from"]
                                                   }];
}

- (void)handleSingleTap:(UITapGestureRecognizer *) sender {
    if (CGRectContainsPoint(_tableView.frame, [sender locationInView:self.view])) {
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



// Called when the UIKeyboardDidShowNotification is sent.
- (void)keyboardWasShown:(NSNotification*)aNotification {
    NSDictionary* info = [aNotification userInfo];
    CGSize kbSize = [[info objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;
    [UIView beginAnimations:nil context:NULL];
    [UIView setAnimationDelegate:self];
    [UIView setAnimationDuration:0];
    [UIView setAnimationBeginsFromCurrentState:YES];
    _tableView.frame = CGRectMake(_tableView.frame.origin.x, _tableView.frame.origin.y, _tableView.frame.size.width, _tableView.frame.size.height - kbSize.height);
    _inputView.frame = CGRectMake(_inputView.frame.origin.x, (_inputView.frame.origin.y - kbSize.height), _inputView.frame.size.width, _inputView.frame.size.height);
    [UIView commitAnimations];
}

// Called when the UIKeyboardWillHideNotification is sent
- (void)keyboardWillBeHidden:(NSNotification*)aNotification {
    NSDictionary* info = [aNotification userInfo];
    CGSize kbSize = [[info objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;
    [UIView beginAnimations:nil context:NULL];
    [UIView setAnimationDelegate:self];
    [UIView setAnimationDuration:0];
    [UIView setAnimationBeginsFromCurrentState:YES];
    _tableView.frame = CGRectMake(_tableView.frame.origin.x, (_tableView.frame.origin.y), _tableView.frame.size.width, _tableView.frame.size.height + kbSize.height);
    _inputView.frame = CGRectMake(_inputView.frame.origin.x, (_inputView.frame.origin.y + kbSize.height), _inputView.frame.size.width, _inputView.frame.size.height);
    [UIView commitAnimations];
}
@end
