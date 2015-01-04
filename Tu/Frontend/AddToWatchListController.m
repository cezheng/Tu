//
//  AddToWatchListController.m
//  Tu
//
//  Created by Ce Zheng on 10/1/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#import "AddToWatchListController.h"

@interface AddToWatchListController ()
@property (weak, nonatomic) IBOutlet UITextField *textField;
@property (weak, nonatomic) IBOutlet UIButton *addButton;

@end

@implementation AddToWatchListController
- (void) prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if (sender == self.addButton) {
        if (self.textField.text.length > 0) {
            self.summonerName = self.textField.text;
        }
    }
}

- (void) viewDidLoad {
    [super viewDidLoad];
    UIGestureRecognizer *tapper;
    tapper = [[UITapGestureRecognizer alloc]
              initWithTarget:self action:@selector(handleSingleTap:)];
    tapper.cancelsTouchesInView = NO;
    [self.view addGestureRecognizer:tapper];
}

- (void)handleSingleTap:(UITapGestureRecognizer *) sender {
    [self.view endEditing:YES];
}

@end
