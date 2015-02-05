#import "NewsFeedTableViewCell.h"

@implementation NewsFeedTableViewCell
- (void)awakeFromNib {
    // Initialization code
    self.contentLabel.preferredMaxLayoutWidth = self.contentLabel.bounds.size.width;
    self.summaryLabel.preferredMaxLayoutWidth = self.summaryLabel.bounds.size.width;
}
@end
