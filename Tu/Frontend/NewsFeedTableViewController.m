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
    self.tableView.estimatedRowHeight = 100.0;
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
;
    }
    NSDictionary* match = recentMatches[indexPath.row];
    NSDictionary* stats = match[@"stats"];
    cell.contentLabel.text = [NSString stringWithFormat:@"%@ went %@/%@/%@ in a %@ match.", match[@"name"], stats[@"championsKilled"], stats[@"numDeaths"] ? stats[@"numDeaths"] : @0, stats[@"assists"], match[@"subType"]];
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm"];
    NSDate* date = [NSDate dateWithTimeIntervalSince1970:[recentMatches[indexPath.row][@"createDate"] integerValue] / 1000];
    cell.datetimeLabel.text = [dateFormatter stringFromDate:date];
    NSLog(@"%@", recentMatches[indexPath.row]);
    return cell;
}

@end
