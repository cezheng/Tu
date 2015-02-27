
//
//  BubbleTableView.h
//  Tu
//
//  Created by Yuan Lin on 2/27/15.
//  Copyright (c) 2015 Ce Zheng. All rights reserved.
//

#ifndef Tu_BubbleTableView_h
#define Tu_BubbleTableView_h
#import <UIKit/UIKit.h>

#import "BubbleTableViewDataSource.h"
#import "BubbleTableViewCell.h"

@interface BubbleTableView : UITableView <UITableViewDelegate, UITableViewDataSource>

@property (nonatomic, assign) IBOutlet id<BubbleTableViewDataSource> bubbleDataSource;
@property (nonatomic) NSTimeInterval snapInterval;
@property (nonatomic) BOOL showAvatars;

- (void) scrollBubbleViewToBottomAnimated:(BOOL)animated;

@end

#endif
