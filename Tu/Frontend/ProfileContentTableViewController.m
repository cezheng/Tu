#import "ProfileContentTableViewController.h"
#import "ImagePathCache.h"
#import "XPFService.h"


@implementation ProfileContentTableViewController
@synthesize leagueEntry;
@synthesize summonerInfo;
@synthesize summonerIcon;

- (void) viewDidLoad {
}

- (void) viewWillAppear:(BOOL)animated {
    [self addObserver:self forKeyPath:@"leagueEntry" options:0 context:nil];
    [self addObserver:self forKeyPath:@"summonerInfo" options:0 context:nil];
    [[XPFService sharedService] callWithEndPoint:@"RiotAPI/initSelfData"
                                          params:@{}
                                        callback:^(id response) {
                                            NSLog(@"profile res : %@", response);
                                            self.leagueEntry = [response objectForKey:@"leagueEntry"];
                                            self.summonerIcon = [[ImagePathCache sharedCache] imageWithPath:[response objectForKey:@"summonerIconPath"]];
                                            self.summonerInfo = [response objectForKey:@"summonerInfo"];
                                        }
                            callbackInMainThread:YES];

}

- (void) viewWillDisappear:(BOOL)animated {
    [self removeObserver:self forKeyPath:@"leagueEntry"];
    [self removeObserver:self forKeyPath:@"summonerInfo"];
}

#pragma mark - KVO

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
    if ([keyPath isEqualToString:@"leagueEntry"]) {
        [self refreshLeagueEntryUI];
    } else if ([keyPath isEqualToString:@"summonerInfo"]) {
        [self refreshSummonerInfoUI];
    }
}

#pragma mark - UI Refresh
- (void) refreshLeagueEntryUI {
    NSLog(@"league entry %@", leagueEntry);
    if (leagueEntry == nil || leagueEntry == [NSNull null]) {
        _leaguePointLabel.text = @"League Points:0";
        _tierLabel.text = @"Unranked";
        _winLoseLabel.text = @"W:0 L:0";
        _winRateLabel.text = @"Win Rate: 0.00%";
        _tierIconImage.image = [UIImage imageNamed:@"unranked"];
        return;
    }
    for (NSDictionary* league in (NSArray*)leagueEntry) {
        NSLog(@"league %@", league);
        if([[league objectForKey:@"queue"] isEqualToString:@"RANKED_SOLO_5x5"]) {
            NSDictionary* entry = league[@"entries"][0];
            int wins = [entry[@"wins"] intValue];
            int losses = [entry[@"losses"] intValue];
            _leaguePointLabel.text = [NSString stringWithFormat:@"League Points:%@", entry[@"leaguePoints"]];
            _tierLabel.text = [NSString stringWithFormat:@"%@ %@", league[@"tier"], entry[@"division"]];
            _winLoseLabel.text = [NSString stringWithFormat:@"W:%d L:%d", wins, losses];
            _winRateLabel.text = [NSString stringWithFormat:@"Win Rate: %0.2f%%", 100.f * (float)wins / (float)(wins + losses)];
            _tierIconImage.image = [UIImage imageNamed:[NSString stringWithFormat:@"%@_%@", league[@"tier"], entry[@"division"]]];
        }
    }
    //[[self.tableView cellForRowAtIndexPath:[NSIndexPath indexPathForRow:0 inSection:1]] setNeedsDisplay];
    /*
    _levelLabel.text = summonerStats[@"level"];
    _summonerIconImage.image = [UIImage imageWithPathCache:summonerStats[@"profileIcon"]];
    _xpProgress.progress = 0.5f;
     */
}

- (void) refreshSummonerInfoUI {
    if (summonerInfo == nil) {
        return;
    }
    _summonerNameLabel.text = summonerInfo[@"name"];
    _levelLabel.text = [NSString stringWithFormat:@"Level %@", summonerInfo[@"summonerLevel"]];
    _summonerIconImage.image = summonerIcon;
}
@end
