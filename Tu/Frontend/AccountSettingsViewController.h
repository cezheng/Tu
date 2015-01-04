#import <UIKit/UIKit.h>

extern NSString *const kRiotJID;
extern NSString *const kRiotJIDDomain;
extern NSString *const kRiotLoginName;
extern NSString *const kRiotPassword;


@interface AccountSettingsViewController : UIViewController

@property (weak, nonatomic) IBOutlet UITextField *loginNameField;
@property (weak, nonatomic) IBOutlet UITextField *passwordField;

- (IBAction)login:(id)sender;
- (IBAction)back:(id)sender;
- (IBAction)hideKeyboard:(id)sender;

@end
