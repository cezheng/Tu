//
//  BubbleData.h
//  Tu
//
//  Created by Yuan Lin on 2/27/15.
//  Copyright (c) 2015 Ce Zheng. All rights reserved.
//

#ifndef Tu_BubbleData_h
#define Tu_BubbleData_h
#import <Foundation/Foundation.h>

typedef enum _BubbleType
{
    BubbleTypeMine = 0,
    BubbleTypeSomeoneElse = 1
} BubbleType;

@interface BubbleData : NSObject

@property (readonly, nonatomic, strong) NSDate *date;
@property (readonly, nonatomic) BubbleType type;
@property (readonly, nonatomic, strong) UIView *view;
@property (readonly, nonatomic) UIEdgeInsets insets;
@property (nonatomic, strong) UIImage *avatar;

- (id)initWithText:(NSString *)text date:(NSDate *)date type:(BubbleType)type;
+ (id)dataWithText:(NSString *)text date:(NSDate *)date type:(BubbleType)type;
- (id)initWithView:(UIView *)view date:(NSDate *)date type:(BubbleType)type insets:(UIEdgeInsets)insets;
+ (id)dataWithView:(UIView *)view date:(NSDate *)date type:(BubbleType)type insets:(UIEdgeInsets)insets;

@end


#endif
