#import <UIKit/UIKit.h>

@interface AccountSettingsViewController : UIViewController

@property (weak, nonatomic) IBOutlet UITextField *loginNameField;
@property (weak, nonatomic) IBOutlet UITextField *passwordField;

- (IBAction)login:(id)sender;
- (IBAction)back:(id)sender;
- (IBAction)hideKeyboard:(id)sender;

@end
