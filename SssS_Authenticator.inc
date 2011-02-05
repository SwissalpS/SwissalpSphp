<?php
/* * * *
 * * SssS_Authenticator.inc
 * *
 * * note: this file should be encoded as UTF-8 without BOM
 * *	make sure this file doesn't send anything as this would
 * *	send the headers which might not yet be ready
 * *
 * * @version 20091024_010245 + file_put_contents_safely
 * * @version 20090716_171917 (CC) Luke JZ aka SwissalpS
 * * * */
//require_once('WG7/errorLevel.liz');
//error_reporting(E_ALL); // 0); //
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_Plist.inc');
	require_once('SssS_Xtend.inc');
} // if not autoincluding but on php's include path

// check authenticity of caller

class SssS_Authenticator {
	/**
	 * @var string path to the db.plist holding settings such as path to user credentials and sessions
	 */
	var $sPath;

	/**
	 * @var object plist settings
	 */
	var $oDB;

	/**
	 * @var array array representation of plist contents
	 */
	var $aDB;

	/**
	 * @var object plist object of user
	 */
	var $oUsersPlist;

	/**
	 * @var array array representation of user plist contents
	 */
	var $aUsersDB;

	/**
	 * @var array holds user info if user is authenticated
	 */
	var $aUserInfo;

	/**
	 * @var object plist object for sessions
	 */
	var $oSessionPlist;

	/**
	 * @var array all 'active' sessions
	 */
	var $aSessionDB;

	/**
	 * @var bool true when authenticated
	 */
	var $bAuthenticated;

	/**
	 * @var bool true when authenticated by username & password post(standard login)
	 */
	var $bAuthenticated1;

	/**
	 * @var bool true when authenticated by valid session ID by post only (watch for scripting attack possibility)
	 */
	var $bAuthenticatedS;

	/**
	 * @var bool true when authenticated by session cookie and session post (remember me)
	 */
	var $bAuthenticatedC;

	/**
	 * @var bool true when authenticated by 4 factor logon
	 */
	var $bAuthenticated4;

	/**
	 * @var int replaces the above with binnary flag. this is the future direction and will someday replace bAuthenticated series entirely
	 */
	var $iAuthFlag;

	/**
	 * @var int 0 = no permissions ie nobody. 9 = all perms ie admin
	 */
	var $iAuthLevel;

	/**
	 * @var string i.e. username
	 */
	var $sAuthNoun;

	/**
	 * @var array cookies
	 */
	var $aCookies;

	/**
	 * @var array $_POST equivalant
	 */
	var $aPost;

	/**
	 * @var object SssS_Xtend XHTML object for additional functions
	 */
	var $oXtend;

	/**
	 * Authenticator($sPath)
	 * constructor
	 *
	 * @param string $sPath parameter_description_here
	 *
	 * @return void return_description_here
	 *
	 *
	 * @uses something - uses_desciption_here
	 *
	 * @version 20090716_201043 (CC)2007 Luke JZ aka SwissalpS
	 */
	function SssS_Authenticator($sPath) {
		$this->fReset();
		if (!$this->fReadSettings($sPath)) return null;
		if (!$this->fReadUserDB()) return null;
		if (!$this->fCheckSSL()) return null;
		if (!$this->fReadSessionDB()) return null;
		$this->fGetCookies(); // $this->aCookies
		$this->fGetPost(); // $this->aPost
		$this->bInitiated = true;
	} // Authenticator


	/**
	 * fAuthenticate()
	 * Function_description_here
	 *
	 * @return bool true if authenticated
	 *
	 * @version 20090716_200214 (CC)2007 Luke JZ aka SwissalpS
	 */
	function fAuthenticate() {
		if (!$this->bInitiated) return false;
		$sSID = null;
		 $this->iAuthFlag = 0;
		// first, check required cookies against required posts
		$aP = $this->aPost;
		$aC = $this->aCookies;

		// Authentication session POST name
		$sSssS_s2	 = $this->fGetDefaultString('SssS_s2');
		// Authentication session Cookie name
		$sSssS_s	 = $this->fGetDefaultString('SssS_s');
		// logon name, noun, alias etc variable names
		$sSssS_nom	 = $this->fGetDefaultString('SssS_nom');
		$sSssS_pass	 = $this->fGetDefaultString('SssS_pass');
		$sSssS_noun  = $this->fGetDefaultString('SssS_noun');
		$sSssS_id	 = $this->fGetDefaultString('SssS_id');
		$sSssS_p	 = $this->fGetDefaultString('SssS_p');
		$sSssS_t	 = $this->fGetDefaultString('SssS_t');
		$sSssS_noun2 = $this->fGetDefaultString('SssS_noun2');
		$sSssS_id2	 = $this->fGetDefaultString('SssS_id2');
		$sSssS_p2	 = $this->fGetDefaultString('SssS_p2');
		$sSssS_t2	 = $this->fGetDefaultString('SssS_t2');

		// is 1x2 factor allowed? POST only
		if ($this->aDB['bAllowSimpleLogon']) {
			if (isset($aP[$sSssS_nom], $aP[$sSssS_pass])) {
				if (isset($this->aUsersDB[$aP[$sSssS_nom]]) && (sha1($aP[$sSssS_pass]) == $this->aUsersDB[$aP[$sSssS_nom]]['piss'])) {
					$this->sAuthNoun = $aP[$sSssS_nom];
					$this->bAuthenticated1 = true;
					$this->iAuthFlag += 1;
				} // if correct values given
			} // if 2 factors given
		} // if 1x2 allowed

		// 1x1factor allowed? POST/cookie only SessionID
		if ($this->aDB['bAllowSimpleSession']) {
			$sSID = isset($aC[$sSssS_s]) ? $aC[$sSssS_s] : $sSID;
			$sSID = isset($aP[$sSssS_s2]) ? $aP[$sSssS_s2] : $sSID;
			if (isset($this->aSessionDB[$sSID])) {
				// still valid
				$aS = $this->aSessionDB[$sSID];
				if (isset($this->aUsersDB[$aS['noun']]) && $sSID == $this->aUsersDB[$aS['noun']]['session']) {
					// check the ip against db
					if ($this->aUsersDB[$aS['noun']]['lastIP'] == $this->oXtend->getRemIP()) {
						$this->sAuthNoun = $aS['noun'];
						$this->bAuthenticatedS = true;
						$this->iAuthFlag += 2;
					} // if ip is still same
				} // noun is in db and this session is registered therein
			} // if have session id
		} // if session logon post/cookie only


		// session already logged? 2x2factor time limited logon
		// post and cookie session id
		if (isset($aC[$sSssS_s], $aP[$sSssS_s2]) && $aC[$sSssS_s] == $aP[$sSssS_s2]) {
			if (isset($this->aSessionDB[$aC[$sSssS_s]])) {
				// still valid
				$aS = $this->aSessionDB[$aC[$sSssS_s]];
				if (isset($this->aUsersDB[$aS['noun']]) && $aC[$sSssS_s] == $this->aUsersDB[$aS['noun']]['session']) {
					if ($this->aUsersDB[$aS['noun']]['lastIP'] == $this->oXtend->getRemIP()) {
						$this->sAuthNoun = $aS['noun'];
						$this->bAuthenticatedC = true;
						$this->iAuthFlag += 4;
					} // if ip is still same
				} // noun is in db and this session is registered therein
			} // if have session id
		} // if session logon

		// 2x4-factor logon
		if (isset($aC[$sSssS_noun], $aC[$sSssS_id], $aC[$sSssS_p], $aC[$sSssS_t],
				$aP[$sSssS_noun2], $aP[$sSssS_id2], $aP[$sSssS_p2], $aP[$sSssS_t2])
				&& $aC[$sSssS_noun] == $aP[$sSssS_noun2]
				&& $aC[$sSssS_id] == $aP[$sSssS_id2]
				&& $aC[$sSssS_p] == $aP[$sSssS_p2]
				&& $aC[$sSssS_t] == $aP[$sSssS_t2]) {
			// ok, all required tokens are given and are same as their counterparts
			// now let's validate
			if (isset($this->aUsersDB[$aC[$sSssS_noun]])
					&& $this->fGetCurrentToken() == strtolower($aC[$sSssS_t])) {

				// token and username is valid -> would actually do
				$aU = $this->aUsersDB[$aC[$sSssS_noun]];
				if ($aU['uid'] == $aC[$sSssS_id]
						&& $aU['piss'] == sha1($aC[$sSssS_p])) {

					$this->sAuthNoun = $aC[$sSssS_noun];
					$this->bAuthenticated4 = true;
					$this->iAuthFlag += 8;
				} // if uid and pass ok
			} // if noun ok
			// compare with post/get to make sure there is no CSSAttack
		} // if 2x4-factor logon

     $this->iMinAuthFlag = 1;
		$this->bAuthenticated = ($this->iMinAuthFlag <= $this->iAuthFlag); //$this->bAuthenticated1 || $this->bAuthenticatedC || $this->bAuthenticated4);
		if ($this->bAuthenticated) {
			// (re)new session, save the dbs
			$this->fNewSession($sSID);

			$this->aUserInfo = $this->aUsersDB[$this->sAuthNoun];
		} // if authenticated
		return $this->bAuthenticated;
	} // fAuthenticate


	/**
	 * fAuthenticateOrDie()
	 * as the name says, only let authorized thru
	 *
	 * @return void
	 *
	 * @uses fAuthenticate
	 *
	 * @version 20090716_002005 (CC)2007 Luke JZ aka SwissalpS
	 */
	function fAuthenticateOrDie() {
		if (!$this->fAuthenticate()) {
			$s = @file_get_contents_safely($this->fGetDiePath());
			$s = ($s) ? $s . chr(10) . '0' : '0';
			exit($s);
		}
	} // fAuthenticateOrDie

	/**
	 * fAuthenticateOrLogonForm()
	 * as the name says, only let authorized thru or show login page
	 *
	 * @return bool true or int if authenticated then true else the tag id of the login form
	 *
	 * @uses fAuthenticate
	 * @uses addLoginPage
	 *
	 * @version 20091012_161653 (CC)2007 Luke JZ aka SwissalpS
	 */
	function fAuthenticateOrLogonForm() {
		if (!$this->fAuthenticate()) {
			return $this->addLoginPage();
		} // if not authenticated
		return true;
	} // fAuthenticateOrLogonForm

	function addAuth2form($iForm = null) {
		// must be authenticated
		if (!$this->sAuthNoun) return false;
		 $sSssS_s2	 = $this->fGetDefaultString('SssS_s2');
		 $sSID		 = $this->aUsersDB[$this->sAuthNoun]['session'];
		// add hidden tag with session id
		return $this->oXtend->addTag('input', $iForm,
				'type="hidden" name="' . $sSssS_s2 . '" value="'
				. $sSID . '"');
	} // addAuth2form

	function addLoginPage() {
		// prefix title
		$this->oXtend->setTitle('Login: ' . $this->oXtend->getTitle());
		$this->oXtend->addLabel('Login');
		$iC = $this->oXtend->addRectangleWithContentDIV();
		$iF = $this->oXtend->addForm($iC);
		$this->oXtend->addHeader('Alias ', $iF);
		$this->oXtend->addTag('input', $iF, 'type="text" name="'
				. $this->fGetDefaultString('SssS_nom') . '"');
		$this->oXtend->addHeader('Password ', $iF);
		$this->oXtend->addTag('input', $iF, 'type="password" name="'
				. $this->fGetDefaultString('SssS_pass') . '"');
		$this->oXtend->addTag('input', $iF, 'type="submit" class="submit"');
		return $iF; // return form id so tokens can be added.
	} // addLoginPage

	/**
	 * fCheckSSL()
	 * returns false if db isn't initialized or setting is missing
	 * returns true if
	 *		settings don't require ssl
	 *		settings require ssl and connection is ssl
	 * returns nothing but dies sending contents/result of included file set in aDB[errorIncludePathSSL] which defaults to ErrorSSL.html
	 *
	 *
	 * @return void return_description_here
	 *
	 * @version 20090725_140454 (CC)2007 Luke JZ aka SwissalpS
	 */
	function fCheckSSL() {
		if (!isset($this->aDB, $this->aDB['onlySSL'], $this->oXtend)) return false;
		// make sure only an explicit setting of false allows not to use ssl
		$bSSLonly = !(false == $this->aDB['onlySSL']);
		if ($bSSLonly) {
			if ($this->oXtend->isSSL()) {
				return true;
			} else { // redirect, exit including file
				$this->fRedirect2errorSSL();
			}
		}
		return true;
	} // fCheckSSL


	/**
	 * fGetCookies()
	 * Function_description_here
	 *
	 * @return array cookies if any
	 *
	 *
	 * @uses something - uses_desciption_here
	 *
	 * @version 20090717_235914 (CC)2007 Luke JZ aka SwissalpS
	 */
	function fGetCookies() {
		$this->aCookies = $this->oXtend->getCookies();
		return $this->aCookies;
	} // fGetCookies


	/**
	 * fGetCurrentToken()
	 * generate a token
	 *
	 * @return string token
	 *
	 * @version 20090718_081408 (CC)2007 Luke JZ aka SwissalpS
	 */
	function fGetCurrentToken() {
		return $this->fGetTokenFor(gmdate('U'));
	} // fGetCurrentToken


	/**
	 * fGetTokenFor($iTS)
	 * generate the token for given time stamp
	 *
	 * @param int $iTS the time stamp
	 *
	 * @return string return_description_here
	 *
	 * @version 20090819_222906 (CC)2007 Luke JZ aka SwissalpS
	 */
	function fGetTokenFor($iTS) {
		$sDate = round($iTS / 60, -1); // date rounded to 10 mins
		$sToken = sha1(gmdate('w', $iTS) . $sDate . gmdate('z', $iTS));
		return $sToken;
	} // fGetTokenFor


	/**
	 * fGetDiePath()
	 * Function_description_here
	 *
	 * @return string return_description_here
	 *
	 * @version 20090725_142614 (CC)2007 Luke JZ aka SwissalpS
	 */
	function fGetDiePath() {
		$sDiePath = 'Cash.SwissalpS.biz.html';
		if (isset($this->aDB, $this->aDB['diePath'])) {
			$sDP = $this->aDB['diePath'];
			if (is_file($sDP) && is_readable($sDP)) $sDiePath = $sDP;
		} // get path from db
		return $sDiePath;
	} // fGetDiePath


	/**
	 * fGetPost()
	 * Function_description_here
	 *
	 * @return array $_POST php version independant
	 *
	 * @version 20090719_184919 (CC)2007 Luke JZ aka SwissalpS
	 */
	private function fGetPost() {
		if (isset($_POST)) {
			$aP 	= $_POST;
		} elseif (isset($HTTP_POST_VARS)) { // depricated as of 4.1.0
			$aP 	= $HTTP_POST_VARS;
		} elseif (isset($GLOBALS['HTTP_POST_VARS'])) {
			$aP 	= $GLOBALS['HTTP_POST_VARS'];
		} else {
			$aP 	= array();
		}
		return $this->aPost = $aP;
	} // fGetPost

	/**
	 * fGetDefaultString($sKey, $sDefaultValue = '')
	 * attempt to get a value from the auth.plist db
	 * if not available return $sDefaultValue or $sKey
	 *
	 * @param string $sKey the keys name, whose value you seek
	 * @param string $sDefaultValue value to use if not found (defaults to: $sKey)
	 *
	 * @return string value of the key's value
	 *
	 * @uses $this->aDB - auth.plist db
	 *
	 * @version 20091016_052316 (CC)2007 Luke JZ aka SwissalpS
	 */
	private function fGetDefaultString($sKey, $sDefaultValue = '') {
		$sDefaultValue = ($sDefaultValue && ('' != $sDefaultValue)) ? $sDefaultValue : $sKey;
		return (string) ((isset($this->aDB[$sKey]))
				? $this->aDB[$sKey]
				: $sDefaultValue);
	} // fGetDefaultString

	/**
	 * fLogOut($sSID = null)
	 * log-out the current user or the specified session
	 *
	 * @param string $sSID session id to log out (defaults to: null)
	 *
	 * @return void
	 *
	 * @uses $this->bAuthenticated - to determine if current user is logged in
	 * @uses $this->aUserInfo - to get the current SID
	 * @uses $this->aSessionDB - to remove the session
	 * @uses $this->fSaveSessionDB - to save the modified session db
	 *
	 * @version 20091013_071702 (CC)2007 Luke JZ aka SwissalpS
	 */
	public function fLogOut($sSID = null) {
		if (!$sSID || '' == $sSID) {
			if ($this->bAuthenticated) {
				$sSID = $this->aUserInfo['session'];
			} else {
				$sSID = '';
			} // if authenticated or not
		} // no session id given

		$this->sAuthNoun = null; // fReset();
		$this->bAuthenticated	= false;
		$this->bAuthenticated1	= false;
		$this->bAuthenticatedS	= false;
		$this->bAuthenticatedC	= false;
		$this->bAuthenticated4	= false;
		unset($this->aSessionDB[$sSID]);

		$this->fSaveSessionDB();
	} // fLogOut


	/**
	 * fNewSession()
	 * Function_description_here
	 *
	 * @return string return_description_here
	 *
	 * @version 20090720_064225 (CC)2007 Luke JZ aka SwissalpS
	 */
	private function fNewSession($sExistingSession = null) {
		if (!$this->sAuthNoun) return '0'; // must be authenticated!!!
		$sIP = $this->oXtend->getRemIP();
		$iValidForMinutes = 10; // in ten minutes default
		if (isset($this->aDB['sessionCookieValid4minutes'])) {
			$iV4mDB = $this->aDB['sessionCookieValid4minutes'];
			// is it smaller than max (40) and greater than min (1)
			if ($iV4mDB <= 40 && $iV4mDB >= 1) $iValidForMinutes = $iV4mDB;
		} // if got setting in aDB
		$iExpires = time() + 60 * $iValidForMinutes;

		if ($sExistingSession) {
			$sSID = $sExistingSession;
		} // if renew session
		else {
			$sSessionCookieIDseed = 'ha2Ohh!';
			if (isset($this->aDB['sessionCookieIDseed'])) $sSessionCookieIDseed = $this->aDB['sessionCookieIDseed'];
			$sSID = 's' . sha1($sIP . $this->sAuthNoun . date('U') . $sSessionCookieIDseed);
			// make sure it's unique
			while (isset($this->aSessionDB[$sSID])) { $sSID = 's' . sha1($this->oXtend->getRemIP() . $this->sAuthNoun . date('U') . $sSessionCookieIDseed); } // while duplicate
		} // if new session

		// update dbs
		$this->aSessionDB[$sSID]['expire'] = $iExpires;
		$this->aSessionDB[$sSID]['noun'] = $this->sAuthNoun;
		$this->aSessionDB[$sSID]['authFlag'] = $this->iAuthFlag;
		$this->aUsersDB[$this->sAuthNoun]['authFlag'] = $this->iAuthFlag;
		$this->aUsersDB[$this->sAuthNoun]['session'] = $sSID;
		$this->aUsersDB[$this->sAuthNoun]['lastIP'] = $sIP;
		// save dbs
		$this->fSaveSessionDB(); $this->fSaveUserDB();
		// by default set cookie to secure only
		$iSSLcookie = (isset($this->aDB['sessionCookieSSLonly']) && false == $this->aDB['sessionCookieSSLonly']) ? 0 : 1;

		// authentication session cookie
		setcookie($this->fGetDefaultString('SssS_s'), // name
				$sSID,											// value
				$iExpires,
				'/', $this->aDB['sessionCookieDomain'],		// path & domain
				$iSSLcookie);								// ssl switch
	} // fNewSession


	/**
	 * fReadSessionDB($sPath)
	 * Function_description_here
	 *
	 * @param string $sPath path of plist holding session credentials
	 *
	 * @return bool true on success, false on fail
	 *
	 *
	 * @uses SssS_Plist
	 *
	 * @version 20090716_223651 (CC)2007 Luke JZ aka SwissalpS
	 */
	function fReadSessionDB($sPath = null) {
		if (!$sPath || '' == $sPath) {
			if (!isset($this->aDB)) return false;
			$sPath = (isset($this->aDB['sessionsPlistPath']))
				? $this->aDB['sessionsPlistPath'] : '';
		} // no path
		$s = @file_get_contents_safely($sPath);
		$s = (!$s) ? '' : $s;
		$o = new SssS_Plist($s);
		$a = $o->a_plist;
		if (!$a) $a = array();
		$this->oSessionPlist = $o;
		// remove expired
		$aClean = array();
		$iCurrTime = time();
		foreach ($a as $sKey => $mValue) {
			if ($iCurrTime < $mValue['expire']) $aClean[$sKey] = $mValue;
		} // foreach
		$this->aSessionDB = $aClean;
		$this->fSaveSessionDB($sPath);
		return true;
	} // fReadSessionDB


	/**
	 * fReadSettings($sPath)
	 * Function_description_here
	 *
	 * @param string $sPath parameter_description_here
	 *
	 * @return void return_description_here
	 *
	 *
	 * @uses something - uses_desciption_here
	 *
	 * @version 20090725_163950 (CC)2007 Luke JZ aka SwissalpS
	 */
	function fReadSettings($sPath) {
		$s = @file_get_contents_safely($sPath);
		if (!$s) return false;
		$o = new SssS_Plist($s);
		$a = $o->a_plist;
    if (!isset($a['diePath'])) $a['diePath'] = 'SomeStaticInfo.html';
    if (!isset($a['errorIncludePathSSL'])) $a['errorIncludePathSSL'] = 'ErrorSSL.html';
	if (!isset($a['onlySSL']) || false !== $a['onlySSL']) $a['onlySSL'] = true;
	if (!isset($a['bAllowSimpleLogon']) || true !== $a['bAllowSimpleLogon']) $a['bAllowSimpleLogon'] = false;
		if (!isset($a['bAllowSimpleSession']) || true !== $a['bAllowSimpleSession']) $a['bAllowSimpleSession'] = false;
		if (!isset($a['sessionCookieSSLonly']) || false !== $a['sessionCookieSSLonly']) $a['sessionCookieSSLonly'] = true;
		if (!isset($a['sessionCookieDomain'])) $a['sessionCookieDomain'] = 'localhost';
		if (!isset($a['sessionsPlistPath'])) $a['sessionsPlistPath'] = 'data/sessions.plist';
		if (!isset($a['usersPlistPath'])) $a['usersPlistPath'] = 'data/users.plist';
	/*	if (!isset($a[''])) $a[] = '';
		if (!isset($a[''])) $a[] = '';
		if (!isset($a[''])) $a[] = '';
		if (!isset($a[''])) $a[] = '';
		if (!isset($a['']) || false !== $a[]) $a[] = false;
		if (!isset($a[''])) $a[] = '';

*/
		$this->oDB = $o;
		$this->aDB = $a;
		return true;
	} // fReadSettings


	/**
	 * fReadUserDB($sPath)
	 * Function_description_here
	 *
	 * @param string $sPath path of plist holding credentials
	 *
	 * @return bool true on success, false on fail
	 *
	 *
	 * @uses SssS_Plist
	 *
	 * @version 20090716_223651 (CC)2007 Luke JZ aka SwissalpS
	 */
	function fReadUserDB($sPath = null) {
		if (!$sPath || '' == $sPath) {
			if (!isset($this->aDB, $this->aDB['usersPlistPath'])) return false;
			$sPath = $this->aDB['usersPlistPath'];
		} // no path
		$s = @file_get_contents_safely($sPath);
		if (!$s) return false;
		$o = new SssS_Plist($s);
		$a = $o->a_plist;
		if (!$a || 0 == count($a)) return false;
  // apply defaults TODO
		$this->oUsersPlist = $o;
		$this->aUsersDB = $a;
		return true;
	} // fReadUserDB


	/**
	 * fRedirect2errorSSL()
	 * Function_description_here
	 *
	 * @param mixed $( parameter_description_here
	 *
	 * @return void return_description_here
	 *
	 *
	 * @uses something - uses_desciption_here
	 *
	 * @version 20090725_141824 (CC)2009 Luke JZ aka SwissalpS
	 */
	function fRedirect2errorSSL() {
		$sSSLerrorPath = 'ErrorSSL.html';
		if (isset($this->aDB, $this->aDB['errorIncludePathSSL'])) {
			$sSSLeP = $this->aDB['errorIncludePathSSL'];
			if (is_file($sSSLeP) && is_readable($sSSLeP)) $sSSLerrorPath = $sSSLeP;
		} // get path from db
		include($sSSLerrorPath);
		exit();
	} // fRedirect2errorSSL


	/**
	 * fReset()
	 * resets values of some vars
	 *
	 * @return void
	 *
	 * @version 20090716_201419 (CC)2009 Luke JZ aka SwissalpS
	 */
	function fReset() {
		$this->bInitiated		= false;
		$this->iAuthLevel		= 0;
		$this->sAuthNoun		= null;
		$this->bAuthenticated	= false;
		$this->bAuthenticated1	= false;
		$this->bAuthenticatedS	= false;
		$this->bAuthenticatedC	= false;
		$this->bAuthenticated4	= false;
		$this->sPath			= null;
		$this->oUsersPlist		= null;
		$this->aUsersDB			= null;
		$this->oDB				= null;
		$this->aDB				= null;
		$this->oSessionPlist	= null;
		$this->aSessionDB		= null;
		$this->aUserInfo		= null;
		$this->oXtend			= new SssS_Xtend();
	} // fReset


	/**
	 * fSaveSessionDB($sPath)
	 * Function_description_here
	 *
	 * @param string $sPath parameter_description_here (defaults to: '')
	 *
	 * @return void return_description_here
	 *
	 * @version 20091024_010245 + file_put_contents_safely
	 * @version 20090720_014342 (CC)2009 Luke JZ aka SwissalpS
	 */
	function fSaveSessionDB($sPath = null) {
		if (!$sPath || '' == $sPath) {
			if (!isset($this->aDB)) return false;
			$sPath = (isset($this->aDB['sessionsPlistPath']))
				? $this->aDB['sessionsPlistPath'] : '';
		} // no path
		$s = $this->oSessionPlist->initFromArray($this->aSessionDB);
		return @file_put_contents_safely($sPath, $s, 'wb');
	} // fSaveSessionDB


	/**
	 * fSaveSettings($sPath)
	 * Function_description_here
	 *
	 * @param string $sPath parameter_description_here (defaults to: '')
	 *
	 * @return void return_description_here
	 *
	 * @version 20091024_010245 + file_put_contents_safely
	 * @version 20090725_165101 (CC)2009 Luke JZ aka SwissalpS
	 */
	function fSaveSettings($sPath = null) {
		if (!$sPath || '' == $sPath) {
			if (!isset($this->sPath)) return false;
			$sPath = $this->sPath;
		} // no path
		$s = $this->oDB->initFromArray($this->aDB);
		return @file_put_contents_safely($sPath, $s, 'wb');
	} // fSaveSettings


	/**
	 * fSaveUserDB($sPath)
	 * Function_description_here
	 *
	 * @param string $sPath parameter_description_here (defaults to: '')
	 *
	 * @return void return_description_here
	 *
	 * @version 20091024_010245 + file_put_contents_safely
	 * @version 20090720_014342 (CC)2009 Luke JZ aka SwissalpS
	 */
	function fSaveUserDB($sPath = null) {
		if (!$sPath || '' == $sPath) {
			if (!isset($this->aDB, $this->aDB['usersPlistPath'])) return false;
			$sPath = $this->aDB['usersPlistPath'];
		} // no path
		$s = $this->oUsersPlist->initFromArray($this->aUsersDB);
		return file_put_contents_safely($sPath, $s, 'wb');
	} // fSaveUserDB

	function __toString() { return print_r($this, true); } // __toString

} // Authenticator
/* * * *\ SssS_Authenticator.liz (CC) Luke JZ aka SwissalpS /* * * */
?>
