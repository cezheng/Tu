//
//  BubbleData.m
//  Tu
//
//  Created by Yuan Lin on 2/27/15.
//  Copyright (c) 2015 Ce Zheng. All rights reserved.
//

#import "BubbleData.h"

#import <QuartzCore/QuartzCore.h>

@implementation BubbleData

#pragma mark - Properties

@synthesize date = _date;
@synthesize type = _type;
@synthesize view = _view;
@synthesize insets = _insets;
@synthesize avatar = _avatar;

#pragma mark - Lifecycle

#if !__has_feature(objc_arc)
- (void)dealloc
{
    [_date release];
    _date = nil;
    [_view release];
    _view = nil;
    
    self.avatar = nil;
    
    [super dealloc];
}
#endif

#pragma mark - Text bubble

const UIEdgeInsets textInsetsMine = {5, 10, 11, 17};
const UIEdgeInsets textInsetsSomeone = {5, 15, 11, 10};

+ (id)dataWithText:(NSString *)text date:(NSDate *)date type:(BubbleType)type
{
#if !__has_feature(objc_arc)
    return [[[BubbleData alloc] initWithText:text date:date type:type] autorelease];
#else
    return [[BubbleData alloc] initWithText:text date:date type:type];
#endif
}

- (id)initWithText:(NSString *)text date:(NSDate *)date type:(BubbleType)type
{
    UIFont *font = [UIFont systemFontOfSize:[UIFont systemFontSize]];
    CGSize size = [(text ? text : @"") sizeWithFont:font constrainedToSize:CGSizeMake(220, 9999) lineBreakMode:NSLineBreakByWordWrapping];
    
    UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, size.width, size.height)];
    label.numberOfLines = 0;
    label.lineBreakMode = NSLineBreakByWordWrapping;
    label.text = (text ? text : @"");
    label.font = font;
    label.backgroundColor = [UIColor clearColor];
    
#if !__has_feature(objc_arc)
    [label autorelease];
#endif
    
    UIEdgeInsets insets = (type == BubbleTypeMine ? textInsetsMine : textInsetsSomeone);
    return [self initWithView:label date:date type:type insets:insets];
}

#pragma mark - Custom view bubble

+ (id)dataWithView:(UIView *)view date:(NSDate *)date type:(BubbleType)type insets:(UIEdgeInsets)insets
{
#if !__has_feature(objc_arc)
    return [[[BubbleData alloc] initWithView:view date:date type:type insets:insets] autorelease];
#else
    return [[BubbleData alloc] initWithView:view date:date type:type insets:insets];
#endif
}

- (id)initWithView:(UIView *)view date:(NSDate *)date type:(BubbleType)type insets:(UIEdgeInsets)insets
{
    self = [super init];
    if (self)
    {
#if !__has_feature(objc_arc)
        _view = [view retain];
        _date = [date retain];
#else
        _view = view;
        _date = date;
#endif
        _type = type;
        _insets = insets;
    }
    return self;
}

@end
