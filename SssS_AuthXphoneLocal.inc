<?php
/* * * *
 * * SssS_AuthXphoneLocal.inc
 * *
 * * authentication on iPhone
 * * detect where the caller is from: our local machine only, lan only, www enabling seperation of allowed tasks. -> per zone authentication.
 * * uses SssS_PSL
 * * uses SssS_Xphone
 * * version 20090728_163945 (CC) Luke JZ aka SwissalpS
 * * * */
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_Xphone.inc');
	require_once('SssS_Authenticator.inc');
} // if not autoincluding but on php's include path

class SssS_AuthXphone extends SssS_Authenticator {
    function SssS_AuthXphone($sPath) {
                 $this->SssS_Authenticator($sPath);
    } // SssS_AuthXphone
	/**
	 * fReset()
	 * resets values of some vars
	 *
	 * @return void
	 *
	 * @version 20090801_034519 (CC)2007 Luke JZ aka SwissalpS
	 */
	function fReset() {
		$this->bInitiated		= false;
		$this->iAuthLevel		= 0;
		$this->sAuthNoun		= null;
		$this->bAuthenticated	= false;
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
		$this->oXtend = $this->oXphone = new SssS_XPhone();
	} // fReset

	function addLoginPage() {
		// prefix title
		$this->oXphone->setTitle('Login: ' . $this->oXphone->getTitle());
		$this->oXphone->addLabel('Login');
		$iC = $this->oXphone->addRectangleWithContentDIV();
		$iF = $this->oXphone->addForm($iC);
		$this->oXphone->addHeader('Alias ', $iF);
		$this->oXphone->addTag('input', $iF, 'type="text" name="'
				. $this->fGetDefaultString('SssS_nom') . '"');
		$this->oXphone->addHeader('Password ', $iF);
		$this->oXphone->addTag('input', $iF, 'type="password" name="'
				. $this->fGetDefaultString('SssS_pass') . '"');
		$this->oXphone->addTag('input', $iF, 'type="submit" class="submit"');
		return $iF; // return form id so tokens can be added.
	} // addLoginPage
} // SssS_AuthXphone
/* * * *\ SssS_AuthXphoneLocal.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
