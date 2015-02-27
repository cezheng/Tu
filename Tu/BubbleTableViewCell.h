//
//  BubbleTableViewCell.h
//  Tu
//
//  Created by Yuan Lin on 2/27/15.
//  Copyright (c) 2015 Ce Zheng. All rights reserved.
//

#ifndef Tu_BubbleTableViewCell_h
#define Tu_BubbleTableViewCell_h

#import "BubbleData.h"

@interface BubbleTableViewCell : UITableViewCell

@property (nonatomic, strong) BubbleData *data;
@property (nonatomic) BOOL showAvatar;

@end


#endif
