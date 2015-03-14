#import "ProfileContentTableViewController.h"
#import "UIImage+PathCache.h"
#import "XPFService.h"


@implementation ProfileContentTableViewController
@synthesize summonerStats;
@synthesize summonerName;
- (void) viewDidLoad {
}

- (void) viewWillAppear:(BOOL)animated {
    /*
    [self addObserver:self forKeyPath:@"summonerStats" options:0 context:nil];
    [self addObserver:self forKeyPath:@"summonerName" options:0 context:nil];
    void (^getSummonerProfile)(NSNumber* summonerId) = ^(NSNumber* summonerId) {
        [[XPFService sharedService] readStreamWithEndPoint:@"RiotService/profileByIds"
                                                    params:@[summonerId]
                                                  callback:^(id response) {
                                                      summonerStats = response;
                                                  }
                                             finalCallback:nil
                                      callbackInMainThread:YES];
    };
    [[XPFService sharedService] callWithEndPoint:@"RiotAPI/summonerByNames"
                                          params:@[summonerName]
                                        callback:^(id response) {
                                            
                                        }
                            callbackInMainThread:YES];
     */

}

- (void) viewWillDisappear:(BOOL)animated {
    //[self removeObserver:self forKeyPath:@"summonerStats"];
}

#pragma mark - KVO

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
    if ([keyPath isEqualToString:@"summonerStats"]) {
        [self refreshSummonerStatsUI];
    }
}

#pragma mark - UI Refresh
- (void) refreshSummonerStatsUI {
    /*
    _summonerNameLabel.text = summonerStats[@"name"];
    _levelLabel.text = summonerStats[@"level"];
    _summonerIconImage.image = [UIImage imageWithPathCache:summonerStats[@"profileIcon"]];
    _xpProgress.progress = 0.5f;
    _tierLabel;
    _leaguePointLabel;
    int win = [summonerStats[@"win"] intValue];
    int lose = [summonerStats[@"lose"] intValue];
    _winLoseLabel;
    _winRateLabel;
    _tierIconImage;
     */
}
@end
