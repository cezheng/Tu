//
//  WatchListTableViewController.h
//  Tu
//
//  Created by Ce Zheng on 8/25/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface WatchListTableViewController : UITableViewController
- (IBAction)unwindToList:(UIStoryboardSegue *)segue;
@property NSMutableArray* watchList;
@property NSMutableDictionary* fetchedList;
@end
