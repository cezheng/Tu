#import "FeedCollectionViewController.h"
#import "FeedCollectionViewCell.h"
#import "XPFService.h"
#import <MediaPlayer/MediaPlayer.h>

@interface FeedCollectionViewController() {
    MPMoviePlayerController *moviePlayer;
}
@end

@implementation FeedCollectionViewController

-(void)viewDidLoad {
    feeds = [[NSMutableArray alloc] init];
    [[XPFService sharedService] callWithEndPoint:@"Video/GetInfo"
                                          params:@{@"id" : @"MAiOAert8us"}
                                        callback:^(id info) {
                                            [feeds addObject:info];
                                            [self.collectionView reloadData];
                                        }
                            callbackInMainThread:YES];
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return [feeds count];
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *identifier = @"FeedCell";
    
    FeedCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:identifier forIndexPath:indexPath];
    cell.preview.image = [UIImage imageNamed:@"top_charts.png"];
    return cell;
}

- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath {
    NSURL* url = [NSURL URLWithString:[feeds[indexPath.row] objectForKey:@"video"]];
    moviePlayer =  [[MPMoviePlayerController alloc]
                    initWithContentURL:url];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(moviePlayBackDidFinish:)
                                                 name:MPMoviePlayerPlaybackDidFinishNotification
                                               object:moviePlayer];
    
    moviePlayer.controlStyle = MPMovieControlStyleDefault;
    moviePlayer.shouldAutoplay = YES;
    [self.view addSubview:moviePlayer.view];
    [moviePlayer setFullscreen:YES animated:YES];
}

- (void) moviePlayBackDidFinish:(NSNotification*)notification {
    MPMoviePlayerController *player = [notification object];
    [[NSNotificationCenter defaultCenter]
     removeObserver:self
     name:MPMoviePlayerPlaybackDidFinishNotification
     object:player];
    
    if ([player
         respondsToSelector:@selector(setFullscreen:animated:)]) {
        [player.view removeFromSuperview];
    }
}
@end
