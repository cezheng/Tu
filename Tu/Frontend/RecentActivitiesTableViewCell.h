#import <UIKit/UIKit.h>

@interface RecentActivitiesTableViewCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *contentLabel;
@property (weak, nonatomic) IBOutlet UILabel *datetimeLabel;
@property (weak, nonatomic) IBOutlet UILabel *summaryLabel;
@property (weak, nonatomic) IBOutlet UIImageView *championImage;

@property (weak, nonatomic) IBOutlet UIImageView *itemImage0;
@property (weak, nonatomic) IBOutlet UIImageView *itemImage1;
@property (weak, nonatomic) IBOutlet UIImageView *itemImage2;
@property (weak, nonatomic) IBOutlet UIImageView *itemImage3;
@property (weak, nonatomic) IBOutlet UIImageView *itemImage4;
@property (weak, nonatomic) IBOutlet UIImageView *itemImage5;
@property (weak, nonatomic) IBOutlet UIImageView *itemImage6;

@property (weak, nonatomic) IBOutlet UIImageView *summonerSpell0;
@property (weak, nonatomic) IBOutlet UIImageView *summonerSpell1;

@property (strong, nonatomic) NSArray* itemsData;
@end
