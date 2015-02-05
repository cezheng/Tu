#import "NewsFeedTableViewController.h"
#import "NewsFeedTableViewCell.h"
#import "../Bridge/LocalServiceObjc.h"
#import "XPFService.h"

@interface NewsFeedTableViewController ()
@property (strong, nonatomic) NSArray* recentMatches;

@end

@implementation NewsFeedTableViewController

@synthesize recentMatches;


-(void)viewDidLoad {
    self.tableView.estimatedRowHeight = 135.0;
    self.tableView.rowHeight = UITableViewAutomaticDimension;
    [[XPFService sharedService] readStreamWithEndPoint:@"RiotService/matchFeedByIds"
                                                params:@{
                                                         @"ids" : @[@48245995, @52375717, @52335849]
                                                         }
                                              callback:^(id data) {
                                                  self.recentMatches = data;
                                              }
                                  callbackInMainThread:YES];
}

- (void) setRecentMatches:(NSArray *)matches {
    if (![matches isEqualToArray:recentMatches]) {
        recentMatches = matches;
        [self.tableView reloadData];
    }
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


- (NSInteger) tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    if (section == 0) {
        return recentMatches.count;
    }
    return 0;
}

- (UITableViewCell*) tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    NSString* CellIdentifier = @"recentMatchCell";
    NewsFeedTableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:@"recentMatchCell"];
    if (cell == nil) {
        cell = [[NewsFeedTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault
                                            reuseIdentifier:CellIdentifier];
    }
    
    NSArray* matchGroup = recentMatches[indexPath.row];
    NSDictionary* firstItemInGroup = matchGroup[0];
    NSDictionary* gameStats = firstItemInGroup[@"stats"];
    
    NSMutableString* summary = [[NSMutableString alloc] init];
    NSUInteger index = 0;
    for (NSDictionary* match in matchGroup) {
        if (index++ != 0) {
            [summary appendString:@", "];
        }
        [summary appendString:match[@"name"]];
    }
    [summary appendString:[NSString stringWithFormat:@" %@ a %@ game", [gameStats[@"win"] integerValue] ? @"won" : @"lost", firstItemInGroup[@"subType"]]];
    if (index > 1) {
        [summary appendString:@" together"];
    }
    [summary appendString:@"."];
    cell.summaryLabel.text = summary;
    
    NSMutableString* content = [[NSMutableString alloc] init];
    for (NSDictionary* match in matchGroup) {
        NSDictionary* stats = match[@"stats"];
        NSDictionary* champData = match[@"champData"];
        NSNumber* kills = stats[@"championsKilled"] ? stats[@"championsKilled"] : @0;
        NSNumber* deaths = stats[@"numDeaths"] ? stats[@"numDeaths"] : @0;
        NSNumber* assists = stats[@"assists"] ? stats[@"assists"] : @0;
        NSString* line = [NSString stringWithFormat:@"%@ went %@/%@/%@ as %@\n", match[@"name"], kills, deaths, assists, champData[@"name"]];
        [content appendString:line];
    }
    cell.contentLabel.text = content;
    
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm"];
    NSDate* date = [NSDate dateWithTimeIntervalSince1970:[firstItemInGroup[@"createDate"] longLongValue] / 1000];
    cell.datetimeLabel.text = [dateFormatter stringFromDate:date];
    NSLog(@"well %@", recentMatches[indexPath.row]);
    
    return cell;
}

@end
