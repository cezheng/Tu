//
//  MyHomeUICollectionViewController.m
//  Tu
//
//  Created by Ce Zheng on 8/22/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#import "MyHomeUICollectionViewController.h"
#import "../Backend/Crawler.h"
#import "../Backend/Utils/FileUtil.h"

@interface MyHomeUICollectionViewController ()

@end

@implementation MyHomeUICollectionViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    auto fu = FileUtil::getInstance();
    Crawler crawler;
    std::string path = fu->getWritablePath() + "images_sqlite_images_champions_avatar_104_104_2.png";
    crawler.downloadFile("http://loldb.gameguyz.com/images/sqlite_images/champions_avatar_104_104/2.png", path);
    _images = [[NSMutableArray alloc] init];
    [_images addObject:[NSString stringWithUTF8String:path.c_str()]];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

#pragma mark =
#pragma mark UICollectionViewDataSource
-(NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView
{
    return 1;
}

-(NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    return _images.count;
}

-(UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    MyHomeCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"MyHomeCollectionViewCell" forIndexPath:indexPath];
    UIImage *image;
    long row = [indexPath row];
    image = [[UIImage alloc] initWithContentsOfFile:_images[row]];
    cell.imageView.image = image;
    return cell;
}
@end
