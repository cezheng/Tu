#import <UIKit/UIKit.h>

@interface ProfileContentTableViewController : UITableViewController
@property (strong, nonatomic) NSDictionary* leagueEntry;
@property (strong, nonatomic) NSDictionary* summonerInfo;
@property (strong, nonatomic) UIImage* summonerIcon;

@property (weak, nonatomic) IBOutlet UILabel *summonerNameLabel;
@property (weak, nonatomic) IBOutlet UILabel *levelLabel;
@property (weak, nonatomic) IBOutlet UIImageView *summonerIconImage;
@property (weak, nonatomic) IBOutlet UIProgressView *xpProgress;
@property (weak, nonatomic) IBOutlet UILabel *tierLabel;
@property (weak, nonatomic) IBOutlet UILabel *leaguePointLabel;
@property (weak, nonatomic) IBOutlet UILabel *winLoseLabel;
@property (weak, nonatomic) IBOutlet UILabel *winRateLabel;
@property (weak, nonatomic) IBOutlet UIImageView *tierIconImage;

@end
