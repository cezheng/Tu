#import <UIKit/UIKit.h>

@interface NewsFeedTableViewCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *datetimeLabel;
@property (weak, nonatomic) IBOutlet UILabel *contentLabel;
@property (weak, nonatomic) IBOutlet UILabel *summaryLabel;

@end
