<?php
/* * * *
 * * modules/login/localizableStrings.php
 * *
 * * @version 20160115_130851 (CC) Luke JZ aka SwissalpS
 * * * */

use SwissalpS\PHOCOA\Localization\Localization as WFLocalization;
use SwissalpS\PHOCOA\Localization\WFBundle;

// add our localization
WFLocalization::sharedInstance()->addKeys(array(
    'ModuleLoginNotAuthorizedPageTitle' => array(
        'en' => 'Not authorized.',
        'de' => 'Nicht authorisiert.',
    ),
    'ModuleLoginShowLoginUserIs' => array(
        'en' => 'User is logged in (%s)',
        'de' => 'Benutzer ist angemolden als %s',
    ),
    'ModuleLoginShowLoginUserNotLoggedIn' => array(
        'en' => 'No user logged in.',
        'de' => 'Kein Benutzer angemolden.',
    ),
    'ModuleLoginPromptLoginPageTitle' => array(
        'en' => 'Please log in.',
        'de' => 'Bitte anmelden.',
    ),
    'ModuleLoginDoForgotResetDone' => array(
        'en' => 'The password for %1$s %2$s has been reset. Your new password information has been emailed to the email address on file for your account.',
        'de' => 'Das Passwort für %1$s %2$s wurde zurückgesetzt. Das neue Passwort wurde an die registrierte E-Mail-Adresse gesandt.',
    ),
    'ModuleLoginSuccessPageTitle' => array(
        'en' => 'Login Successful.',
        'de' => 'Erfolgreich angemolden.',
    ),

));

// load localization functions, if not yet
WFBundle::mainBundle();

/* * * *\ modules/login/localizableStrings.php (CC) Luke JZ aka SwissalpS /* * * */
