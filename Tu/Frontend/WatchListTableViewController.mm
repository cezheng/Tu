//
//  WatchListTableViewController.m
//  Tu
//
//  Created by Ce Zheng on 8/25/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//
#import "../Bridge/LocalServiceObjc.h"
#import "WatchListTableViewController.h"
#import "AddToWatchListController.h"
#import "../Backend/Utils/UserDefault.h"

@interface WatchListTableViewController () {
    dispatch_semaphore_t semaphore;
}

@end

@implementation WatchListTableViewController

- (IBAction)unwindToList:(UIStoryboardSegue *)segue {
    AddToWatchListController* source = [segue sourceViewController];
    if (source.summonerName.length > 0) {
        [_watchList addObject:source.summonerName];
        [self writeWatchList];
        [self.refreshControl beginRefreshing];
        [self refresh];
    }
    
}

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
    NSError *error = nil;
    NSString* listJson = [NSString stringWithUTF8String:UserDefault::getInstance()->get("watchlist").c_str()];
    if (listJson.length <= 0) {
        listJson = @"[]";
    }
    _watchList = [NSJSONSerialization JSONObjectWithData:[listJson dataUsingEncoding:NSUTF8StringEncoding]
                                                 options: NSJSONReadingMutableContainers
                                                   error: &error];

    _fetchedList = [[NSMutableDictionary alloc] init];
    semaphore = dispatch_semaphore_create(0);
    self.refreshControl = [[UIRefreshControl alloc] init];
    self.refreshControl.attributedTitle = [[NSAttributedString alloc] initWithString:@"Pull to Refresh"];
    [self.refreshControl addTarget:self action:@selector(refresh) forControlEvents:UIControlEventValueChanged];
    self.edgesForExtendedLayout = UIRectEdgeNone;
    [self.refreshControl beginRefreshing];
    [self refresh];
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
    NSDictionary* cellInfo = _fetchedList[[@(indexPath.row) stringValue]];
    if (cellInfo != nil) {
        UIImage *image = [[UIImage alloc] initWithContentsOfFile:cellInfo[@"image"]];
        cell.textLabel.text = cellInfo[@"text"];
        void (^updateUI)(void);
        updateUI = ^{
            [cell.imageView setImage:image];
            [cell setNeedsLayout];
        };
        if ([NSThread isMainThread]) {
            updateUI();
        } else {
            dispatch_sync(dispatch_get_main_queue(), updateUI);
        }
    }
    return cell;
}



// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the specified item to be editable.
    return YES;
}


// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [_watchList removeObjectAtIndex:indexPath.row];
        [_fetchedList removeObjectForKey:[@(indexPath.row) stringValue]];
        [self writeWatchList];
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
    } else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}


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

//custom methods
- (void)refresh {
    if (_watchList.count <= 0) {
        [self.refreshControl endRefreshing];
        return;
    }
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        for (int i = 0; i < _watchList.count; i++) {
            dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            [self.refreshControl endRefreshing];
        });
    });
    for (int i = 0; i < _watchList.count; i++) {
        NSIndexPath* indexPath = [NSIndexPath indexPathForRow:i inSection:0];
        [[LocalServiceObjc getInstance] callWithAPIEndPoint:@"basic_info"
                                                     params:@{
                                                              @"summoner_name" : [_watchList objectAtIndex:indexPath.row]
                                                              }
                                               successBlock:^(NSDictionary* response){
                                                   [_fetchedList setObject:response forKey:[@(i) stringValue]];
                                                   dispatch_sync(dispatch_get_main_queue(), ^{
                                                       [self.tableView reloadRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
                                                       dispatch_semaphore_signal(semaphore);
                                                   });
                                               }
                                                failedBlock:^(NSString* response) {
                                                    NSLog(@"%@", response);
                                                }
         ];
    }
}

- (void)writeWatchList {
    NSError* error = nil;
    NSString *jsonString = [[NSString alloc] initWithData:[NSJSONSerialization dataWithJSONObject:_watchList options:NSJSONWritingPrettyPrinted error:&error]
                                                 encoding:NSUTF8StringEncoding];
    UserDefault::getInstance()->set("watchlist", [jsonString UTF8String]);
}

@end
