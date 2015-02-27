//
//  BubbleTableView.m
//  Tu
//
//  Created by Yuan Lin on 2/27/15.
//  Copyright (c) 2015 Ce Zheng. All rights reserved.
//

#import "BubbleTableView.h"
#import "BubbleData.h"

@interface BubbleTableView ()

@property (nonatomic, retain) NSMutableArray *bubbleSection;

@end

@implementation BubbleTableView

@synthesize bubbleDataSource = _bubbleDataSource;
@synthesize snapInterval = _snapInterval;
@synthesize bubbleSection = _bubbleSection;
@synthesize showAvatars = _showAvatars;

#pragma mark - Initializators

- (void)initializator
{
    // UITableView properties
    
    self.backgroundColor = [UIColor clearColor];
    self.separatorStyle = UITableViewCellSeparatorStyleNone;
    assert(self.style == UITableViewStylePlain);
    
    self.delegate = self;
    self.dataSource = self;
    
    // BubbleTableView default properties
    
    self.snapInterval = 120;
}

- (id)init
{
    self = [super init];
    if (self) [self initializator];
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) [self initializator];
    return self;
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) [self initializator];
    return self;
}

- (id)initWithFrame:(CGRect)frame style:(UITableViewStyle)style
{
    self = [super initWithFrame:frame style:UITableViewStylePlain];
    if (self) [self initializator];
    return self;
}

#if !__has_feature(objc_arc)
- (void)dealloc
{
    [_bubbleSection release];
    _bubbleSection = nil;
    [super dealloc];
}
#endif

#pragma mark - Override

- (void)reloadData
{
    self.showsVerticalScrollIndicator = NO;
    self.showsHorizontalScrollIndicator = NO;
    
    // Cleaning up
    self.bubbleSection = nil;
    
    // Loading new data
    int count = 0;
#if !__has_feature(objc_arc)
    self.bubbleSection = [[[NSMutableArray alloc] init] autorelease];
#else
    self.bubbleSection = [[NSMutableArray alloc] init];
#endif
    
    if (self.bubbleDataSource && (count = [self.bubbleDataSource rowsForBubbleTable:self]) > 0)
    {
#if !__has_feature(objc_arc)
        NSMutableArray *bubbleData = [[[NSMutableArray alloc] initWithCapacity:count] autorelease];
#else
        NSMutableArray *bubbleData = [[NSMutableArray alloc] initWithCapacity:count];
#endif
        
        for (int i = 0; i < count; i++)
        {
            NSObject *object = [self.bubbleDataSource bubbleTableView:self dataForRow:i];
            assert([object isKindOfClass:[BubbleData class]]);
            [bubbleData addObject:object];
        }
        
        [bubbleData sortUsingComparator:^NSComparisonResult(id obj1, id obj2)
         {
             BubbleData *bubbleData1 = (BubbleData *)obj1;
             BubbleData *bubbleData2 = (BubbleData *)obj2;
             
             return [bubbleData1.date compare:bubbleData2.date];
         }];
        
        NSDate *last = [NSDate dateWithTimeIntervalSince1970:0];
        NSMutableArray *currentSection = nil;
        
        for (int i = 0; i < count; i++)
        {
            BubbleData *data = (BubbleData *)[bubbleData objectAtIndex:i];
            
            if ([data.date timeIntervalSinceDate:last] > self.snapInterval)
            {
#if !__has_feature(objc_arc)
                currentSection = [[[NSMutableArray alloc] init] autorelease];
#else
                currentSection = [[NSMutableArray alloc] init];
#endif
                [self.bubbleSection addObject:currentSection];
            }
            
            [currentSection addObject:data];
            last = data.date;
        }
    }
    
    [super reloadData];
}

#pragma mark - UITableViewDelegate implementation

#pragma mark - UITableViewDataSource implementation

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    int result = [self.bubbleSection count];
    return result;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    
    return [[self.bubbleSection objectAtIndex:section] count];
}

- (float)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    BubbleData *data = [[self.bubbleSection objectAtIndex:indexPath.section] objectAtIndex:indexPath.row];
    return MAX(data.insets.top + data.view.frame.size.height + data.insets.bottom, self.showAvatars ? 52 : 0);
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Standard bubble
    static NSString *cellId = @"tblBubbleCell";
    BubbleTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellId];
    BubbleData *data = [[self.bubbleSection objectAtIndex:indexPath.section] objectAtIndex:indexPath.row];
    
    if (cell == nil) cell = [[BubbleTableViewCell alloc] init];
    
    cell.data = data;
    cell.showAvatar = self.showAvatars;
    
    return cell;
}

#pragma mark - Public interface

- (void) scrollBubbleViewToBottomAnimated:(BOOL)animated
{
    NSInteger lastSectionIdx = [self numberOfSections];
    
    if (lastSectionIdx >= 0)
    {
        [self scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:([self numberOfRowsInSection:lastSectionIdx] - 1) inSection:lastSectionIdx] atScrollPosition:UITableViewScrollPositionBottom animated:animated];
    }
}


@end