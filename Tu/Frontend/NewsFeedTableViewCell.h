#import <UIKit/UIKit.h>

@interface NewsFeedTableViewCell : UITableViewCell<UICollectionViewDataSource, UICollectionViewDelegate>
@property (weak, nonatomic) IBOutlet UILabel *contentLabel;
@property (weak, nonatomic) IBOutlet UILabel *datetimeLabel;
@property (weak, nonatomic) IBOutlet UILabel *summaryLabel;
@property (weak, nonatomic) IBOutlet UIImageView *championImage;
@property (weak, nonatomic) IBOutlet UICollectionView *itemCollectionView;
@property (weak, nonatomic) NSArray* itemsData;
@end
