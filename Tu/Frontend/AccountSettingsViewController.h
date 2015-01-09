#import <UIKit/UIKit.h>

@interface AccountSettingsViewController : UIViewController

@property (weak, nonatomic) IBOutlet UITextField *loginNameField;
@property (weak, nonatomic) IBOutlet UITextField *passwordField;

- (IBAction)done:(id)sender;
- (IBAction)hideKeyboard:(id)sender;

@end
