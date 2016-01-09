<?php
/* * * *
 * * SwissalpS/PHOCOA/Communication/Mailer.inc
 * *
 * * @version 20101129_123108 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\Communication;

use WFLog;

if (!defined('SwissalpS_FRAMEWORK_DIR')) {
	define('SwissalpS_FRAMEWORK_DIR', dirname(dirname(dirname(__FILE__))));
} // if SwissalpS_FRAMEWORK_DIR not defined yet. i.e. not loaded by PHOCOA app

class Mailer {

	public $sSMTPserverString;
	public $iThePort;
	public $sUserNameString;
	public $sPasswordString;
	public $mUseSSL;
	public $sFromMeMail;
	public $sFromMeName;
	public $sLogFileName;

	function __construct($sSMTPserverString, $iThePort, $sUserNameString, $sPasswordString, $mUseSSL, $sFromMeMail, $sFromMeName, $sLogFileName = 'Mailer.log') {

		$this->sSMTPserverString = $sSMTPserverString;
		$this->iThePort = $iThePort;
		$this->sUserNameString = $sUserNameString;
		$this->sPasswordString = $sPasswordString;
		$this->mUseSSL = $mUseSSL;
		$this->sFromMeMail = $sFromMeMail;
		$this->sFromMeName = $sFromMeName;
		$this->sLogFileName = $sLogFileName;

	} // __construct


	function log($s) {

		WFLog::logToFile($this->sLogFileName, $s);

	} // log


	function sendMail($sToMail, $sToName, $sSubject, $sMessage) {

		return $this->sendMailXPM4($sToMail, $sToName, $sSubject, $sMessage);

		return $this->sendMailXPM3($sToMail, $sToName, $sSubject, $sMessage);

		$sMessage = wordWrap($sMessage, 70);

		$sHeaders = 'From: "' . $this->sFromMeName . '" <' . $this->sFromMeMail . '>';

		return mail('"' . $sToName . '" <' . $sToMail . '>', $sSubject, $sMessage, $sHeaders);

	} // sendMail


	protected function sendMailXPM3($sToMail, $sToName, $sSubject, $sMessage) {

		$sMessage = wordWrap($sMessage . '

Powered by XPertMailer', 70);

		require_once(SwissalpS_FRAMEWORK_DIR . '/_external/XPM3/XPM3_MAIL.php');

		$oM = new \XPM3_MAIL;

		if ($oM->Relay($this->sSMTPserverString, $this->sUserNameString, $this->sPasswordString, $this->iThePort, $this->bUseSSL)) {

			$oM->Delivery('relay');

			$oM->From($this->sFromMeMail, $this->sFromMeName);

			$oM->AddTo($sToEmail, $sToName, 'UTF-8');

			$oM->Text($sMessage, 'UTF-8');

			$mRes = $oM->Send($sSubject, 'UTF-8');

			$oM->Quit();

			//return print_r($oM->result, true);

			return print_r($mRes, true);

		} else {

			$this->log('Error, failed to connect to mailserver');

			return null;

		} // if failed to init connection to mailserver

	} // sendMailXPM3


	protected function sendMailXPM4($sToMail, $sToName, $sSubject, $sMessage) {

		$sMessage = wordWrap($sMessage . '

Powered by XPertMailer', 70);

		require_once(SwissalpS_FRAMEWORK_DIR . '/_external/XPM4/MAIL.php');

		$oM = new \MAIL;

// TODO: test what happens when there is no network at all!!!!!! does the try catch solve the problem? no, because the error is not an exception but an error triggered with trigger_error thus would need to setup an errorhandler function... I'm puzzled why phocoa didn't catch this, or did it and it just dies with the error? is this only debug mode? no, production or not, kills php.

		if ($rC = $oM->Connect($this->sSMTPserverString, $this->iThePort, $this->sUserNameString, $this->sPasswordString, $this->mUseSSL, 10, 'localhost')) {

			//$oM->Delivery('relay');

			$oM->From($this->sFromMeMail, $this->sFromMeName);

			$oM->AddTo($sToMail, $sToName, 'UTF-8');

			$oM->Text($sMessage, 'UTF-8');

			$oM->Subject($sSubject, 'UTF-8');

			$mRes = $oM->Send($rC);

			$oM->Disconnect();

			return print_r($mRes, true);

		} else {

			$this->log('Error, failed to connect to mailserver');

			return null;

		} // if failed to init connection to mailserver

	} // sendMailXPM4

} // Mailer
/* * * *\ SwissalpS/PHOCOA/Communication/Mailer.inc (CC) Luke JZ aka SwissalpS /* * * */
