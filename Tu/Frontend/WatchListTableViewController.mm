//
//  WatchListTableViewController.m
//  Tu
//
//  Created by Ce Zheng on 8/25/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#import "WatchListTableViewController.h"
#import "../Backend/Crawler.h"
#import "../Backend/Utils/DownloadManager.h"

@interface WatchListTableViewController ()

@end

@implementation WatchListTableViewController

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
    
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
    _watchList = [[NSMutableArray alloc] init];
    [_watchList addObject:@"AdamCe"];
    [_watchList addObject:@"JokerRin"];
    [_watchList addObject:@"kanatatsuki"];
    self.refreshControl = [[UIRefreshControl alloc] init];

    NSArray *countryCodes = [NSLocale ISOCountryCodes];
    NSArray *currencyCodes = [NSLocale ISOCurrencyCodes];
    NSLog(@"%@", countryCodes);
    NSLog(@"%@", currencyCodes);

}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [_watchList count];
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"WatchListCell" forIndexPath:indexPath];
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0ul);
    dispatch_async(queue, ^{
        Crawler client;
        PlayerInfo info = client.queryByPlayerName([[_watchList objectAtIndex:indexPath.row] UTF8String]);
        NSString *imagePath = [NSString stringWithUTF8String:DownloadManager::getInstance()->download(info.profileImageUrl, "#profile#" + info.name).c_str()];
        UIImage *image = [[UIImage alloc] initWithContentsOfFile:imagePath];
        void (^updateUI)(void);
        updateUI = ^{
            cell.textLabel.text = [NSString stringWithUTF8String:(info.name + " " + info.modeDataMap.at("classic").winrate).c_str()];
            [cell.imageView setImage:image];
            [cell setNeedsLayout];
        };
        if ([NSThread isMainThread]) {
            updateUI();
        } else {
            dispatch_sync(dispatch_get_main_queue(), updateUI);
        }
    });

    return cell;
}


/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/

/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
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
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath
{
}
*/

/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
