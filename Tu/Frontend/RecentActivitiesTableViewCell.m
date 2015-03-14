#import "RecentActivitiesTableViewCell.h"
#import "InGameItemCollectionViewCell.h"
#import "ImagePathCache.h"

static const NSInteger itemCount = 7;
static const ISize itemImageSize = {24, 24};

@implementation RecentActivitiesTableViewCell


- (void)awakeFromNib {
    // Initialization code
    self.summaryLabel.preferredMaxLayoutWidth = self.summaryLabel.bounds.size.width;
    self.contentLabel.preferredMaxLayoutWidth = self.contentLabel.bounds.size.width;
}

- (void)setItemsData:(NSArray *)itemsData {
    _itemsData = itemsData;
    void (^updateImages)() = ^{
        ImagePathCache* cache = [ImagePathCache sharedCache];
        for (int i = 0; i < itemCount; i++) {
            UIImage* image = nil;
            if (i < itemsData.count) {
                id info = _itemsData[i];
                if (info != [NSNull null] && [info objectForKey:@"image"] != [NSNull null]) {
                    image = [cache imageWithPath:info[@"sprite_path"] cropInfo:info[@"image"] scaleToSize:itemImageSize];
                } else {
                    image = [cache imageWithName:@"item_slot.png" scaleToSize:itemImageSize];
                }
            } else {
                image = [cache imageWithName:@"item_slot.png" scaleToSize:itemImageSize];
            }
            UIImageView* imageView = [self valueForKey:[NSString stringWithFormat:@"itemImage%d", i]];
            dispatch_async(dispatch_get_main_queue(), ^{
                imageView.image = image;
            });
        }
    };
    if ([NSThread isMainThread]) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), updateImages);
    } else {
        updateImages();
    }
}

@end
