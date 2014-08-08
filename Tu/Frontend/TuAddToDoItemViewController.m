//
//  TuAddToDoItemViewController.m
//  Tu
//
//  Created by Ce Zheng on 7/18/14.
//  Copyright (c) 2014 Ce Zheng. All rights reserved.
//

#import "TuAddToDoItemViewController.h"

@interface TuAddToDoItemViewController ()
@property (weak, nonatomic) IBOutlet UITextField *textField;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *doneButton;

@end

@implementation TuAddToDoItemViewController
- (void) prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if(sender != self.doneButton) return;
    if(self.textField.text.length > 0) {
        self.toDoItem = [[TuToDoItem alloc] init];
        self.toDoItem.itemName = self.textField.text;
        [self.toDoItem setRandom];
        self.toDoItem.completed = NO;
    }
}

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

@end
