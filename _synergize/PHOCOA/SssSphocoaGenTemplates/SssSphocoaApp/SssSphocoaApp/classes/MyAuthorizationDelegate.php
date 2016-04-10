<?php
/**
  * Informal delegate protocol for your web application to handle authentication.
  *
  * The WFAuthorizationManager will call your delegate methods to attempt logins.
  */
// TODO: IMPORTANT make better password reset process
// email a token that expires. It initiates further
// recovery questions and finally allows user to give new
// password.
use SssSPropel2\SssSphocoaAppAuth\PermissionsQuery;
use SssSPropel2\SssSphocoaAppAuth\PermpresetsQuery;
use SssSPropel2\SssSphocoaAppAuth\UsersQuery;
use Propel\Runtime\ActiveQuery\Criteria;
use SwissalpS\PHOCOA\Localization\Bla as SssSBla;
use SwissalpS\PHOCOA\Settings\ApplicationSettings;

class MyAuthorizationDelegate extends WFAuthorizationDelegate {

	const SssSauthSalt = 'SkyBIKEzebrok';
	const SssSnewPassLength = 32;
	const SssSnewPassChars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-_!?#%&';
	const SssSbackdoorUser = 'luke';
	const SssSbackdoorPass = 'gqcHSNqjvjehUoW8Yubr';
	const SssSbackdoorUID = 'SwissalpS'; // set this to '' or null to disable backdoor super user login
	const SssSbackdoorGuestUser = 'guest';
	const SssSbackdoorGuestPass = 'guest';
	const SssSbackdoorGuestUID = 'guest'; // set this to '' or null to disable guest login
	var $oAuthorizationInfo;
	public $oPage;

	static function sharedAuthorizationDelegate() {

		static $o = null;
		if (!$o) $o = new MyAuthorizationDelegate();

		return $o;

	} // sharedAuthorizationDelegate



    /**
     *  Provide the invocationPath for handling login.
     *
     *  By default, this will be "login/promptLogin". Applications can override this behavior by writing their own login modules, or even simply "wrapping" the built-in one.
     *
     *  @return string The invocationPath to the login. Remember the page handling login *should* accept a first parameter of "continueURL" (the url will be encoded with {@link WFWebApplication::serializeURL()})
     */
    function loginInvocationPath() {

    	return 'login/promptLogin';

    } // loginInvocationPath



    /**
      * Provide the login authentication.
      *
      * Your WFAuthorizationDelegate can provide its own login capability. Maybe your app will authenticate against LDAP, a Database, etc.
      *
      * @param string The username to use for the authentication.
      * @param string The password to use for the authentication.
      * @param boolean TRUE if the password is in "token" form; ie, not the clear-text password. Useful for remember-me logins or single-sign-on (SSO) setups.
      * @return object WFAuthorizationInfo Return an WFAuthorizationInfo with any additional security profile. This of course can be a subclass. Return NULL if login failed.
      */
    function login($sUserName, $sPassword, $bPassIsToken) {

    	if ($bPassIsToken) {

    		throw new WFException('Not yet ready for token password, SwissalpS would like to see a token only interface to allow most flexible authentication with all kinds of id providers.');
    		//return null;

		} // if token given

		$sAuthInfoClass = WFWebApplication::sharedApplication()->authorizationInfoClass();

		$oAuth = new $sAuthInfoClass;

		$oUser = UsersQuery::create()->findPk($sUserName);

    	if ($oUser) {

    		if ($oUser->getPasshash() == self::passHashForPass($sPassword)) {

    			$oAuth->setUserid($oUser->getHandle()); // $sUserName

    			$oAuth->setLanguageOrder($oUser->getLanguageorder());

    			$aPerms = array();
    			$bGlobalSU = false; $bSystemSU = false;
    			foreach (PermpresetsQuery::create()->findPk(
    					explode(',', $oUser->getPermissions()))->getData() as $oPPerm) {

    				foreach (PermissionsQuery::findPk(
    						explode(',', $oPPerm->getPermissions()))->getData() as $oPerm) {

    					$sDomain = $oPerm->getDomain();
    					$iHash = $oPerm->getHash();

    					$aPerms[$sDomain] = $iHash;

    					// check super user status
    					if ('SYSTEM' == $sDomain && $iHash >= 0x8000)
    						$bSystemSU = true;

    					elseif ('GLOBAL' == $sDomain && $iHash >= 0x8000)
    						$bGlobalSU = true;

    				} // foreach permission

    			} // foreach permPreset

    			$oAuth->setPermissions($aPerms);

    			if ($bGlobalSU && $bSystemSU) {

    				$oAuth->setIsSuperUser(true);

    				$oAuth->setShowBlaEditLinks(true);

    			} // if is super user

				$this->oAuthorizationInfo = $oAuth;

				return $oAuth;

    		} // if correct password

    	} // if found entry

		// backdoor, so we can log in to install and whenever there should be a db problem
		$sBUID = self::SssSbackdoorUID;
		$sBGUID = self::SssSbackdoorGuestUID;
    	if ((!empty($sDUID)) && (self::SssSbackdoorUser == $sUserName && self::SssSbackdoorPass == $sPassword)) {

			$oAuth->setUserid(self::SssSbackdoorUID);

			$oAuth->setIsSuperUser(true);

			$oAuth->setPermissions(array('GLOABAL' => 0x8000, 'SYSTEM' => 0x8000));

			$this->oAuthorizationInfo = $oAuth;

			return $oAuth;

		} elseif ((!empty($sBGUID)) && (self::SssSbackdoorGuestUser == $sUserName && self::SssSbackdoorGuestPass == $sPassword)) {

			// also add a guest user.
			// You can override this by making your own guest account in db and setting a different password or setting the const SssSbackdoorGuestUID to null or ''

			$oAuth->setUserid(self::SssSbackdoorGuestUID);

			$oAuth->setIsSuperUser(false);

			$oAuth->setPermissions(array('GLOABAL' => 0x0001, 'SYSTEM' => 0x0000));

			$this->oAuthorizationInfo = $oAuth;

			return $oAuth;

		} // if ok

		$this->oAuthorizationInfo = $oAuth;

    	return NULL;

    } // login


	function oAuthorizationInfo() {

		return $this->oAuthorizationInfo;

	} // oAuthorizationInfo


    /**
     *  The URL to continue to if the user logs in but there is no "continue to url" set.
     *
     *  If NULL, no redirect will be performed, and just a message saying "Login successful" will be seen.
     *
     *  @return string A URL to redirect to (will be done via {@link WFRedirectRequestException}). DEFAULT: NULL.
     */
    function defaultLoginContinueURL() {

    	return NULL; // 'http://lukezimmermann.com'; //

    } // defaultLoginContinueURL

    /**
     *  The URL to continue to if the user logs out.
     *
     *  If NULL, no redirect will be performed, and just a message saying "Logout successful" will be seen.
     *
     *  @return string A URL to redirect to (will be done via {@link WFRedirectRequestException}). DEFAULT: NULL.
     */
    function defaultLogoutContinueURL() {

    	return NULL; // 'http://lukezimmermann.com'; //

    } // defaultLogoutContinueURL

    /**
     *  Should there be an interstitial "You have logged out successfully, click here to continue", or should logout immediately redirect to {@link WFAuthorizationDelegate::defaultLogoutContinueURL() defaultLogoutContinueURL()}?
     *
     *  @return boolean TRUE to show a logout interstitial. DEFAULT: true.
     */
    function shouldShowLogoutConfirmation() {

    	return false; // true; //

    } // shouldShowLogoutConfirmation

    /**
     *  Should the login interface have a "remember me" checkbox?
     *
     *  @return boolean TRUE to enable "remember me" functionality. DEFAULT: false.
     *  @todo REMEMBER ME code to actually set up / read remember me cookies it NOT implemented.
     */
    function shouldEnableRememberMe() {

    	return false; // true; //

    } // shouldEnableRememberMe

    /**
     *  If "remember me" is enabled with {@link WFAuthorizationDelegate::shouldEnableRememberMe() shouldEnableRememberMe}, should "remember me"
     *  be checked by default?
     *
     *  @return boolean TRUE if the "remember me" checkbox should be checked by default. DEFAULT: false.
     */
    function shouldRememberMeByDefault() {

    	return false; // true; //

    } // shouldRememberMeByDefault

    /**
     *  The login help message that should be displayed above the login box.
     *
     *  @return string The login message to display above the login box. DEFAULT: "You must log in to access the requested page."
     */
    function loginMessage() {

    	return SssSBla::bla('LoginLoginMsg');

    } // loginMessage

    /**
     *  The label to use for the "username" field.
     *
     *  @return string The label for the username field. DEFAULT: "Username".
     */
    function usernameLabel() {

    	return SssSBla::bla('LoginUsername');

    } // usernameLabel

    /**
     *  The message to display to a use on unsuccessful login.
     *
     *  @param string The username that the attempted login was for.
     *  @return mixed string: The message to display on failed login. array of strings; Multiple messages to display (as list items). DEFAULT: string:"Login username or password is not valid."
     */
    function loginFailedMessage($sUsername) {

    	return SssSBla::bla('LoginInvalid');

    } // loginFailedMessage

    /**
     *  Should a "forgot your password" link be shown?
     *
     *  @return boolean TRUE to enable forgotten password reset feature.
     */
    function shouldEnableForgottenPasswordReset() {

    	return true; // false; //

    } // shouldEnableForgottenPasswordReset

    /**
     *  Reset the password for the given user.
     *
     *  @param string The username that the attempted login was for.
     *  @return string The message to show the user on successful password reset. DEFAULT: "The password for <usernameLabel> <username> been reset. Your new password information has been emailed to the email address on file for your account."
     *  @throws object WFException If the password cannot be reset, throw an error with the message to be displayed as the string.<br>
     *          object WFRedirectRequestException If your reset password system is more complicated than can be handled by PHOCOA, feel free to redirect to another page to handle this.
     */
    function resetPassword($sUsernameOrEmail) {

		$sUsernameOrEmail = trim($sUsernameOrEmail);

		// first try to find user by handle
		$oC = UsersQuery::create()->filterByHandle($sUsernameOrEmail)->setLimit(1)->find();
		$m = $oC->getData();
		if (!empty($m)) {

			$oUser = $m[0];

    	} else {
    		// try to find user by email

    		$oC = UsersQuery::create();
    		$oC->filterByEmail($sUsernameOrEmail, Criteria::EQUAL);
    		$oC->setLimit(1);
    		$m = $oC->find()->getData();
			if (!empty($m)) {

				$oUser = $m[0];

			} else {

				throw (new WFException(SssSBla::bla('LoginResetUserNotFound')));

			} // if found user or not

    	} // if found user by handle or not

    	if ($oUser) {

    		$sNewPassword = self::newRandomPassword();
    		$oUser->setPasshash(self::passHashForPass($sNewPassword));
    		$oUser->save();

			$sError = '';
    		if ($mRes = self::mailUserNewPass($oUser, $sNewPassword, $sError)) {

    			// yeah, mail success
    			WFLog::logToFile('mailNewPassword.log', 'successfully sent email to user: ' . $oUser->getHandle());

    		} else {
    			// boo no mail;
    			WFLog::logToFile('mailNewPassword.log', 'FAILED to send email to user: ' . $oUser->getHandle() . ' with error: ' . $sError);

    		}

    	} // if found user or not

    	return SssSBla::bla('LoginResetPasswordSuccess');

    } // resetPassword


    static function mailUserNewPass($oUser, $sNewPassword, &$sError = '') {

    	$sNL = chr(10); // \n

    	$aLangs = explode(',', $oUser->getLanguageorder());
    	$sLang = $aLangs[0];
    	$aParams = array('langOrder' => $aLangs, 'lang' => $sLang, 'noDIV' => true);

    	$sUserName = $oUser->getRealname();
    	$sHandle = $oUser->getHandle();
    	$sProfileURL = sprintf('http://%1$s/userProfile', $_SERVER['HTTP_HOST'], $sHandle);

    	$sMessage = //wordwrap(
    		SssSBla::cleanForTitle(sprintf('%1$s %2$s

 %3$s

	%4$s:
%5$s
	%6$s:
%7$s

%8$s: %9$s

%10$s'
			, SssSBla::bla('AuthDelegateEmailGreeting', $aParams), $sUserName, SssSBla::bla('AuthDelegateEmailResetPass', $aParams), SssSBla::bla('LoginUsername', $aParams), $sHandle, SssSBla::bla('SharedPassword', $aParams), $sNewPassword, SssSBla::bla('AuthDelegateEmailResetProfileEditLL', $aParams), $sProfileURL, SssSBla::bla('AuthDelegateEmailResetEnd', $aParams)));//, 70);

    	/*
    	SssSBla::bla('AuthDelegateEmailGreeting', $aParams)
    			. ' ' . $sUserName
    			. $sNL . $sNL
    			. SssSBla::bla('AuthDelegateEmailResetPass', $aParams)
    			. $sNL . $sNL
    			. SssSBla::bla('LoginUsername', $aParams) . ':'
    			. $sNL
    			. $sHandle
    			. $sNL
    			. SssSBla::bla('SharedPassword', $aParams) . ':'
    			. $sNL
    			. $sNewPassword
    			. $sNL . $sNL
    			. SssSBla::bla('AuthDelegateEmailResetProfileEditLL', $aParams)
    			. ': ' . $sProfileURL
    			. $sNL . $sNL
    			. SssSBla::bla('AuthDelegateEmailResetEnd', $aParams);*/

    	$sSubject = SssSBla::cleanForTitle(SssSBla::bla('AuthDelegateEmailSubject', $aParams));

		$oP = ApplicationSettings::sharedInstance();

 /*   	$sHeaders = 'From: ' . $oP->get('users/mailPasswordFromAddress', 'noreply@' . $_SERVER['HTTP_HOST']);
*/
    	$sFromMeMail = $oP->get('users/mailPasswordFromAddress', 'noreply@' . $_SERVER['HTTP_HOST']);

    	$sFromMeName = 'noreply AutoTechnik-RML';

    	$oMailer = new MyMailer($sFromMeMail, $sFromMeName, 'mailNewPassword.log');

    	return $oMailer->sendMail($oUser->getEmail(), $sUserName, $sSubject, $sMessage, $sError);

    	//return mail($sEmail, $sSubject, $sMessage, $sHeaders);

    } // mailUserNewPass


    static function newRandomPassword() {

		$sChars = self::SssSnewPassChars;
		$iMax = strlen($sChars) -1;

    	$sNewPassword = '';

    	for ($i = 0; $i < self::SssSnewPassLength; $i++) {

    		$sNewPassword .= $sChars{rand(0, $iMax)};

    	} // make pass

    	return $sNewPassword;

    } // newRandomPassword


	static function passHashForPass($sPassword) {

		return sha1($sPassword . self::SssSauthSalt);

	} // passHashForPass


    function save($oAuth = null) {

    	if (!$oAuth) $oAuth = $this->oAuthorizationInfo;

    	$iUID = $oAuth->userid();

    	// no registered user, nothing to do
    	if (-1 == $iUID) return;

    	$oUser = UsersQuery::create()->findPk($iUID);

    	if ($oUser) {

    		$oUser->setLanguageOrder = implode(',', $oAuth->languageOrder());

    		$oUser->save();

    	} // if found entry

    } // save

} // MyAuthorizationDelegate
