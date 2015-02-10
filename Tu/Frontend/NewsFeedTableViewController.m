#import "NewsFeedTableViewController.h"
#import "NewsFeedTableViewCell.h"
#import "../Bridge/LocalServiceObjc.h"
#import "XPFService.h"
#import "UIImage+RiotCrop.h"

@interface NewsFeedTableViewController ()
@property (strong, nonatomic) NSArray* recentMatches;
@end

@implementation NewsFeedTableViewController

@synthesize recentMatches;


-(void)viewDidLoad {
    self.tableView.estimatedRowHeight = 135.0;
    self.tableView.rowHeight = UITableViewAutomaticDimension;
    [self setupRefreshControl];
}

- (void) setRecentMatches:(NSArray *)matches {
    if (![matches isEqualToArray:recentMatches]) {
        recentMatches = matches;
        [self.tableView reloadData];
    }
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return recentMatches.count;
}

- (NSInteger) tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [recentMatches[section] count] + 2;
}

- (UITableViewCell*) tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    NSArray* matchGroup = recentMatches[indexPath.section];
    NSDictionary* gameStats = matchGroup[0][@"stats"];
    NewsFeedTableViewCell* cell = nil;
    if (indexPath.row == 0) {
        NSString *cellIdentifier = @"matchSummaryCell";
        cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier forIndexPath:indexPath];
        NSMutableString* summary = [[NSMutableString alloc] init];
        NSUInteger index = 0;
        for (NSDictionary* match in matchGroup) {
            if (index++ != 0) {
                [summary appendString:@", "];
            }
            [summary appendString:match[@"name"]];
        }
        [summary appendString:[NSString stringWithFormat:@" %@ a %@ game", [gameStats[@"win"] integerValue] ? @"won" : @"lost", matchGroup[0][@"subType"]]];
        if (index > 1) {
            [summary appendString:@" together"];
        }
        [summary appendString:@"."];
        cell.summaryLabel.text = summary;
    } else if(indexPath.row <= matchGroup.count) {
        NSString* cellIdentifier = @"playerMatchDataCell";
        NSDictionary* match = matchGroup[indexPath.row - 1];
        cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier forIndexPath:indexPath];
        NSDictionary* stats = match[@"stats"];
        NSDictionary* champData = match[@"champData"];
        NSNumber* kills = stats[@"championsKilled"] ? stats[@"championsKilled"] : @0;
        NSNumber* deaths = stats[@"numDeaths"] ? stats[@"numDeaths"] : @0;
        NSNumber* assists = stats[@"assists"] ? stats[@"assists"] : @0;
        NSString* content = [NSString stringWithFormat:@"%@ %@/%@/%@", match[@"name"], kills, deaths, assists];
        cell.contentLabel.text = content;
        
        cell.itemsData = match[@"itemsData"];
        [cell.itemCollectionView reloadData];
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            UIImage* image = [UIImage imageWithPathCache:champData[@"sprite_path"] cropInfo:champData[@"image"]];
            if (image) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    NewsFeedTableViewCell* cellToUpdate = (NewsFeedTableViewCell*)[self.tableView cellForRowAtIndexPath:indexPath];
                    [cellToUpdate.championImage setImage:image];
                    [cellToUpdate setNeedsLayout];
                });
            }
        });
    } else {
        NSString* cellIdentifier = @"matchDatetimeCell";
        cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier forIndexPath:indexPath];
        NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
        [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm"];
        NSDate* date = [NSDate dateWithTimeIntervalSince1970:[matchGroup[0][@"createDate"] longLongValue] / 1000];
        cell.datetimeLabel.text = [dateFormatter stringFromDate:date];
    }
    [cell sizeToFit];
    return cell;
}

#pragma mark - refresh control
- (void)setupRefreshControl {
    UIRefreshControl *refreshControl = [[UIRefreshControl alloc] init];
    [refreshControl addTarget:self action:@selector(refresh:) forControlEvents:UIControlEventValueChanged];
    refreshControl.attributedTitle = [[NSAttributedString alloc] initWithString:@"Pull Down To Refresh"];
    [self setRefreshControl:refreshControl];
}

- (void)refresh:(id)sender {
    NSLog(@"Refreshing");
    self.refreshControl.attributedTitle = [[NSAttributedString alloc] initWithString:@"Refreshing"];
    [[XPFService sharedService] readStreamWithEndPoint:@"RiotService/matchFeedByIds"
                                                params:@{
                                                         @"ids" : @[@48245995, @52375717, @52335849]
                                                         }
                                              callback:^(id data) {
                                                  self.recentMatches = data;
                                              }
                                         finalCallback: ^(id finalResponse) {
                                             [(UIRefreshControl *)sender endRefreshing];
                                             self.refreshControl.attributedTitle = [[NSAttributedString alloc] initWithString:@"Pull Down To Refresh"];
                                         }
                                  callbackInMainThread:YES];
}

@end
