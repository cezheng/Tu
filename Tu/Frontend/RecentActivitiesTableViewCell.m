#import "RecentActivitiesTableViewCell.h"
#import "InGameItemCollectionViewCell.h"
#import "UIImage+RiotCrop.h"

@implementation RecentActivitiesTableViewCell

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
        if (info != [NSNull null]) {
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                UIImage* image = [UIImage imageWithPathCache:info[@"sprite_path"] cropInfo:info[@"image"]];
                dispatch_sync(dispatch_get_main_queue(), ^{
                    cell.itemImageView.image = image;
                    cell.itemImageView.alpha = 1.f;
                });
            });
        } else {
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                UIImage* image = [UIImage imageNamed:@"item_slot.png"];
                dispatch_sync(dispatch_get_main_queue(), ^{
                    cell.itemImageView.image = image;
                    cell.itemImageView.alpha = 0.5f;
                });
            });
        }
    }
    return cell;
}

@end
