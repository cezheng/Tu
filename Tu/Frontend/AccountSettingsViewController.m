#import "Constants.h"
#import "AccountSettingsViewController.h"

@implementation AccountSettingsViewController

#pragma mark Init/dealloc methods

- (void)awakeFromNib {
    self.modalTransitionStyle = UIModalTransitionStyleFlipHorizontal;
}

#pragma mark View lifecycle

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    loginNameField.text = [[NSUserDefaults standardUserDefaults] stringForKey:kUDKRiotLoginName];
    passwordField.text = [[NSUserDefaults standardUserDefaults] stringForKey:kUDKRiotPassword];
}

#pragma mark Actions

- (IBAction)done:(id)sender {
    NSString* jid = loginNameField.text != nil ? [NSString stringWithFormat:@"%@@%@", loginNameField.text, kRiotJIDDomain] : nil;
    [[NSUserDefaults standardUserDefaults] setObject:loginNameField.text forKey:kUDKRiotLoginName];
    [[NSUserDefaults standardUserDefaults] setObject:jid forKey:kUDKRiotJID];
    [[NSUserDefaults standardUserDefaults] setObject:passwordField.text forKey:kUDKRiotPassword];
    [self dismissViewControllerAnimated:YES completion:NULL];
}

- (IBAction)hideKeyboard:(id)sender {
    [sender resignFirstResponder];
}

#pragma mark Getter/setter methods

@synthesize loginNameField;
@synthesize passwordField;

@end
