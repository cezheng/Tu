#import "NewsFeedTableViewCell.h"
#import "InGameItemCollectionViewCell.h"
#import "UIImage+PathCache.h"

@implementation NewsFeedTableViewCell

- (void)awakeFromNib {
    // Initialization code
    self.summaryLabel.preferredMaxLayoutWidth = self.summaryLabel.bounds.size.width;
    self.contentLabel.preferredMaxLayoutWidth = self.contentLabel.bounds.size.width;
}

#pragma mark - UICollectionViewDataSource

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return 1;
}
- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return 7;
}

- (UICollectionViewCell*)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    InGameItemCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"inGameItemCell" forIndexPath:indexPath];
    
    cell.itemImageView.image = [UIImage imageWithPathCache:@"5585028973319979110"];
    return cell;
}

@end
