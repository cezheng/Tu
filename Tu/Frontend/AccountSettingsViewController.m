#import "AccountSettingsViewController.h"


NSString *const kRiotJID = @"kRiotJID";
NSString *const kRiotJIDDomain = @"pvp.net";
NSString *const kRiotLoginName = @"kRiotLoginName";
NSString *const kRiotPassword = @"kRiotmyPassword";


@implementation AccountSettingsViewController

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark Init/dealloc methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

- (void)awakeFromNib {
    self.modalTransitionStyle = UIModalTransitionStyleFlipHorizontal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark View lifecycle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    loginNameField.text = [[NSUserDefaults standardUserDefaults] stringForKey:kRiotLoginName];
    passwordField.text = [[NSUserDefaults standardUserDefaults] stringForKey:kRiotPassword];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark Actions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
- (IBAction)login:(id)sender {
    NSString* jid = loginNameField.text != nil ? [NSString stringWithFormat:@"%@@%@", loginNameField.text, kRiotJIDDomain] : nil;
    [[NSUserDefaults standardUserDefaults] setObject:loginNameField.text forKey:kRiotLoginName];
    [[NSUserDefaults standardUserDefaults] setObject:jid forKey:kRiotJID];
    [[NSUserDefaults standardUserDefaults] setObject:passwordField.text forKey:kRiotPassword];
    [self dismissViewControllerAnimated:YES completion:NULL];
}

- (IBAction)back:(id)sender {

}

- (IBAction)hideKeyboard:(id)sender {
    [sender resignFirstResponder];
    [self login:sender];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark Getter/setter methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

@synthesize loginNameField;
@synthesize passwordField;

@end
