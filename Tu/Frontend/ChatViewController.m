#import "ChatViewController.h"
#import "XMPPDelegate.h"

@interface ChatViewController ()

@end

@implementation ChatViewController
@synthesize messages;
@synthesize friendJID;
@synthesize friendName;
@synthesize history;

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
    
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
    [_tableView setDataSource:self];
    [_tableView setDelegate:self];
    if (messages == nil) {
        messages = [[NSMutableArray alloc] init];
    }
    if ([messages count] == 0) {
        [messages addObjectsFromArray:[history getRecentN:5]];
    }
    self.navigationItem.title = friendName;
    UIGestureRecognizer *tapper;
    tapper = [[UITapGestureRecognizer alloc]
              initWithTarget:self action:@selector(handleSingleTap:)];
    tapper.cancelsTouchesInView = NO;
    [self.view addGestureRecognizer:tapper];
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
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"MessageCell" forIndexPath:indexPath];
    ChatMessageObjc* message = [messages objectAtIndex:indexPath.row];
    cell.textLabel.text = message.message;
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
                                     @"read" : @"0",
                                     @"timestamp" : [NSString stringWithFormat:@"%ld", time(0) ]
                                     };
        ChatMessageObjc* historyRecord = [[ChatMessageObjc alloc] initWithDictionary:messageDict];
        [messages addObject:historyRecord];
        [history add:historyRecord];
        [_tableView insertRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:[messages indexOfObject:historyRecord] inSection:0]] withRowAnimation:UITableViewRowAnimationAutomatic];
        [_messageField setText:@""];
    }
}

- (void) didReceivedChatMessage:(ChatMessageObjc*)message {
    [messages addObject:message];
    [_tableView insertRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:[messages indexOfObject:message] inSection:0]] withRowAnimation:UITableViewRowAnimationAutomatic];
}

- (void)handleSingleTap:(UITapGestureRecognizer *) sender {
    [self.view endEditing:YES];
}
@end
