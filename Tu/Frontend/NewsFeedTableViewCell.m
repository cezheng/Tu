#import "NewsFeedTableViewCell.h"
#import "InGameItemCollectionViewCell.h"
#import "UIImage+RiotCrop.h"

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
    if (indexPath.row < _itemsData.count) {
        id info = _itemsData[indexPath.row];
        NSLog(@"info dump %@", info);
        if (info != [NSNull null]) {
            cell.itemImageView.image = [UIImage imageWithPathCache:info[@"sprite_path"] cropInfo:info[@"image"]];
        }

    }
    return cell;
}

@end
