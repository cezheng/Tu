#import "ProfileViewController.h"
#import "ProfileContentTableViewController.h"

@interface ProfileViewController () <ViewPagerDataSource, ViewPagerDelegate> {
    NSMutableArray* tabLabelTexts;
}
@end


@implementation ProfileViewController

- (void)viewDidLoad {
    
    [super viewDidLoad];
    
    self.dataSource = self;
    self.delegate = self;
    tabLabelTexts = [[NSMutableArray alloc] initWithArray:@[@"Profile", @"League", @"Match History"]];
    
}

#pragma mark - ViewPagerDataSource

- (NSUInteger) numberOfTabsForViewPager:(ViewPagerController *)viewPager {
    return tabLabelTexts.count;
}

- (UIView *)viewPager:(ViewPagerController *)viewPager viewForTabAtIndex:(NSUInteger)index {
    UILabel* label = [[UILabel alloc] init];
    label.backgroundColor = [UIColor clearColor];
    label.font = [UIFont systemFontOfSize:12.0];
    label.text = tabLabelTexts[index];
    label.textAlignment = NSTextAlignmentCenter;
    label.textColor = [UIColor blackColor];
    [label sizeToFit];
    return label;
}

- (UIViewController *)viewPager:(ViewPagerController *)viewPager contentViewControllerForTabAtIndex:(NSUInteger)index {
    ProfileViewController* controller = [self.storyboard instantiateViewControllerWithIdentifier:@"ProfileContentViewController"];
    return controller;
}

#pragma mark - ViewPagerDelegate
- (CGFloat)viewPager:(ViewPagerController *)viewPager valueForOption:(ViewPagerOption)option withDefault:(CGFloat)value {
    
    switch (option) {
        case ViewPagerOptionTabWidth:
            return [[UIScreen mainScreen] bounds].size.width / tabLabelTexts.count;
        default:
            return value;
    }
}

@end