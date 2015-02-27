//
//  BubbleTableViewDataSource.h
//  Tu
//
//  Created by Yuan Lin on 2/27/15.
//  Copyright (c) 2015 Ce Zheng. All rights reserved.
//

#ifndef Tu_BubbleTableViewDataSource_h
#define Tu_BubbleTableViewDataSource_h
#import <Foundation/Foundation.h>

@class BubbleData;
@class BubbleTableView;
@protocol BubbleTableViewDataSource <NSObject>

@optional

@required

- (NSInteger)rowsForBubbleTable:(BubbleTableView *)tableView;
- (BubbleData *)bubbleTableView:(BubbleTableView *)tableView dataForRow:(NSInteger)row;

@end

#endif
