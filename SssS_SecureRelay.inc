<?php
/* * * *
 * * SssS_SecureRelay.inc
 * *
 * * Analyze task.plist given in POST and relay to destination or act
 plist
 	tasks <array>
 		(task)
 			name/action
 			arguments such as time and target MC
 * * @version 20090821_233256 + file_get/put_contents_safely
 * * @version 20090821_233256 (CC) Luke JZ aka SwissalpS
 * * * */
// subclasses should override executeTask and relayTask possibly also initCypher, cleanupCypher and myCypherCode
// instantiate your subclass like so:
//$oCR = new SssS_SecureRelaySubclass('Auth.plist', 'myNounInUserDBinAuthPlist'[, 'passwordSalt'][, 'checksumSalt']);
// that's all, let it rip

if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_Authenticator.inc');
	require_once('SssS_ShellScriptRunner.inc');
} // if not autoincluding but on php's include path

class SssS_SecureRelay { // extends SssS_Cypher2 {

	// subclasses override these for more security
	// trailing / on folders please
	var $sCuePlistPath		= 'data/taskCue.plist';
	var $sScriptPoolPath	= 'scriptPool/';
	var $sCargoBasketPath	= 'data/basket/';
	var $sCueRelayPlistPath	= 'data/relayCue.plist';

	// subclass to adapt to servers capabilities and specs
	private function relayTask($a) {
		return 'relayTask has not been defined yet, nothing done';
	} // relayTask

	// subclass to adapt to servers capabilities and specs
	private function executeTask($a) {
		if (isset($a['cargo']['taskType'])) {
			switch ($a['cargo']['taskType']) {
				// subclass or remove this one in production mode
				case 'runArbitraryCode' :
					return $this->runArbitraryCode($a);
				// run a script from the local pool
				// e.g. start/stop cue watcher
				case 'runPoolScript' :
					return $this->runPoolScript($a);
				case 'listPoolScripts' :
					return $this->listPoolScripts($a);
				case 'storeCargoInBasket' :
					return $this->storeCargoInBasket($a);
				case 'storeCargoInBasketEncrypted' :
					return $this->storeCargoInBasketEncrypted($a);
				case 'encryptFileAndRelay' :
					return $this->encryptFileAndRelay($a);
				case 'cueRsyncTask' :
					return $this->cueRsyncTask($a);
				case 'getLastIP' :
					return $this->getLastIP($a);
			} // switch case
		} // if valid task
		return false; //'executeTask has not been defined yet, nothing done';
	} // executeTask

	// subclass this to adapt to your subclassed SssS_Cypher | SssS_Cypher2
	private function initCypher() {
		require_once('SssS_Cypher2.inc'); // this should be a subclass of SssS_Cypher or SssS_Cypher2 where initCiphers and/or cypherForKey have been
		return new SssS_Cypher2(array('tmpPath' => '/tmp/'));  // data/
	} // initCypher

	// subclass this to adapt to your subclassed SssS_Cypher / SssS_Cypher2
	private function cleanupCypher($oC) { if ('SssS_Cypher2' == get_class($oC)) $oC->cleanupCypher2(); /* remove the files used for cypher */ /* subclasses using SssS_Cypher will remove 'manually' here */ } // cleanupCypher

	// subclass this to choose another cypher code or to adapt to the code your SssS_Cypher(/2) subclass uses
	private function myCypherCode() { return 'c12472937914a70e4a5b9b22'; } // myCypherCode

	// remove this one in production mode
	private function runArbitraryCode($aTask) {
		if (isset($aTask['cargo']['shellScript'])) {
			$oSSR = new SssS_ShellScriptRunner();
			return $oSSR->doScript($aTask['cargo']['shellScript']);
		} // if got shellScript
		return false;
	} // runArbitraryCode

	// subclasses set $sScriptPoolPath to adjust path
	private function runPoolScript($aTask) {
		if (isset($aTask['cargo']['poolScriptName'])) {
			$sPath = $this->sScriptPoolPath . $aTask['cargo']['poolScriptName'];
			if (file_exists($sPath)
					&& is_file($sPath)
					&& is_executable($sPath)) {
				$oSSR = new SssS_ShellScriptRunner();
				return $oSSR->doScript($sPath);
			} // if valid name
		} // if got script name
		return false;
	} // runPoolScript

	private function listPoolScripts($aTask) {
		$aScripts = array();
		$sPoolDir = $this->trailSlashThis($this->sScriptPoolPath);
		if (is_dir($sPoolDir)) {
			if ($rDir = opendir($sPoolDir)) {
				while (($sFile = readdir($rDir)) !== false) {
					if (is_file($sPoolDir . $sFile)) {
						$aScripts[] = $sFile;
					} // if is a file
				} // while loop
				closedir($rDir);
			} else {
				// failed to open
				$this->aOut['comments'][] = 'listPoolScripts couldn\'t open sScriptPoolPath: ' . $sPoolDir;
			} // if opened
		} else {
			// dir not found
			$this->aOut['comments'][] = 'listPoolScripts didn\'t find sScriptPoolPath: ' . $sPoolDir;
		} // if is valid dir
		return $aScripts;
	} // listPoolScripts

	// subclasses set $sCargoBasketPath to adjust path
	private function storeCargoInBasket($aTask) {
		if (isset($aTask['cargo']['fileName'])) {
			$sPath = $this->sCargoBasketPath . $aTask['cargo']['fileName'];
			$oP = new SssS_Plist($aTask);
			$res = @file_put_contents_safely($sPath, $oP->XML(), 'wb');
			return ((false === $res)
					? false // 'failed to write to basket'
					: $res . ' bytes written');
		} // if got arguments needed
		return false;
	} // storeCargoInBasket

	// subclass to adjust encrypting
	private function encryptFileAndRelay($aTask) {
		if (isset($aTask['cargo']['inFilePath'], $aTask['cargo']['targetUID'])
				&& file_exists($aTask['cargo']['inFilePath'])
				&& is_readable($aTask['cargo']['inFilePath'])) {

			// add checksums
			$s = $this->sChecksumSalt . $aTask['cargo'];
			$aTask['checksums']['md5'] = md5($s);
			$aTask['checksums']['sha1'] = sha1($s);

			// add target
			$aTask['trace']['targetUID'] = $aTask['cargo']['targetUID'];

			// add relay info
			$aTask['trace']['relays'][] = array(
									'relayUID' => $this->aSs['myUID'],
									'relayTSin' => gmdate('U'),
									'relayTSout' => gmdate('U'));

			// encrypt
			$oC = $this->initCypher();
			$sEncrypted = $oC->encryptWithClearTextPassFromFile(
					$this->myCypherCode(),
					$a['trace']['senderTS'] . $this->sPasswordSalt,
					$aTask['cargo']['inFilePath']);

			unset($aTask['cargo']);
			$aTask['cargo'] = $sEncrypted;

			return $this->relayTask($aTask);
		} // if got path and readable
	} // encryptFileAndRelay

	// subclasses set $sCuePlistPath to adjust path
	private function cueRsyncTask($aTask) {} // cueRsyncTask

	private function getLastIP($aTask) {
		if (isset($aTask['cargo']['IPofUID'])) {
			// search thru aUsersDB to find uid and it's last recorded ip
		} // if got uid
		if (isset($aTask['cargo']['IPofNoun'],
				$this->oA->aUsersDB[$aTask['cargo']['IPofNoun']],
				$this->oA->aUsersDB[$aTask['cargo']['IPofNoun']]['lastIP'])) {
			return $this->oA->aUsersDB[$aTask['cargo']['IPofNoun']]['lastIP'];
		} // if got uid
		return false;
	} // getLastIP

	// initializer and runner
	public function SssS_SecureRelay($sAuthPlistPath, $sMyNoun, $sPasswordSalt = 'passwordSalt', $sChecksumSalt = 'checksumSalt') {
		$this->sPasswordSalt = strval($sPasswordSalt);
		$this->sChecksumSalt = strval($sChecksumSalt);
		// todo, check auth file or extend authenticator to give reply and act upon it
		$this->oA = new SssS_Authenticator($sAuthPlistPath);
		//$this->oX &= $this->oA->oXtend;
		$this->aOut = array(
				'authResult'	=> 'authentication failed',
				'tasksResults'	=> array(),
				'comments'		=> array());

		if ($this->oA->fAuthenticate()) {
			$this->aOut['authResult'] = 'authenticated';
			$sMyNoun = strval($sMyNoun);
			$this->aSs = array('myNoun' => $sMyNoun, 'myUID' => $this->oA->aUsersDB[$sMyNoun]['uid']);
			$this->analyzeTask(); // check for post SssS_l -> list of lockets and/or tasks
		} else {
			//$this->aOut['authResult'] = $this->oA->authError;
		} // if logged on or not

		$oP = new SssS_Plist($this->aOut);
		echo $oP->XML();
	} // SssS_SecureRelay

	private function analyzeTask() {
		//
		$aPost = $this->oA->aPost; $aCookies = $this->oA->aCookies;

		if (isset($aPost['SssS_l'])) { // plist of tasks
			$oPl = new SssS_Plist(stripslashes($aPost['SssS_l']));
			$aP = $oPl->a_plist;
			if (isset($aP['tasks']) && 'array' == gettype($aP['tasks'])) {
				foreach ($aP['tasks'] as $a) {
					if (isset(
							$a['cargo'],
							$a['checksums'], $a['checksums']['sha1'], $a['checksums']['md5'],
							$a['trace'], $a['trace']['targetUID'], $a['trace']['senderUID'], $a['trace']['senderTS'])) {
						// make sure there is a relays entry
						if (!isset($a['trace']['relays'])) $a['trace']['relays'] = array();
						// now let's have a look at the cargo, which can contain further lockets and tasks
						// first, is this for us or to be relayed as is?
						if ($a['trace']['targetUID'] == $this->aSs['myUID']) {
							// for me
							if ($aCargo = $this->unpackCargo(
									$a['cargo'], $this->oA->fGetTokenFor($a['trace']['senderTS']),
									$a['checksums']['sha1'], $a['checksums']['md5'])) {
								$a['cargo'] = $aCargo;
								$a['trace']['targetTSin'] = gmdate('U');

								// do task
								$mTaskResult = $this->executeTask($a);
								$this->aOut['tasksResults'][] = $mTaskResult; //((false !== $mTaskResult) && (nil !== $mTaskResult)) ? $mTaskResult : 'failed to execute task, dropped';
							} else {
								// reg error
								$this->aOut['tasksResults'][] = 'failed to unpack cargo';
							} // if unpacked ok or not
						} else {
							// repackage and send off or put in cron que
							$a['trace']['relays'][] = array(
									'relayUID' => $this->aSs['myUID'],
									'relayTSin' => gmdate('U'),
									'relayTSout' => gmdate('U'));
							$mRelayResult = $this->relayTask($a);
							$this->aOut['tasksResults'][] = ($mRelayResult) ? $mRelayResult : 'failed to relay, packige dropped'; //'cued@Noun' or task result
						} // if is for me or to relay
					} else {
						// reg error
						$this->aOut['tasksResults'][] = 'invalid task headers';
					} // if got headers or not
				} // foreach task
			} else {
				// reg error
				$this->aOut['tasksResults'][] = 'no tasks found';
			} // if got tasks or not
		} else {
			// hmm what do we do about that? for the time being:
			$this->aOut['tasksResults'][] = 'no tasks found, authenticator has logged your ip';
		} // if got payload (locket) post or not
	} // analyzeTask

	private function checksumsTest($sCargo, $sSha1, $sMd5) {
		// if not encrypted just reply true
		if ('array' == gettype($sCargo)) return true;
		$s = $this->sChecksumSalt . $sCargo;
		return ((sha1($s) == $sSha1) && (md5($s) == $sMd5));
	} // checksumsTest

	private function unpackCargo($sCargo, $sPass, $sSha1, $sMd5) {
		if ('array' == gettype($sCargo)) return $sCargo;
		if ($this->checksumsTest($sCargo, $sSha1, $sMd5)) {
			$oC = $this->initCypher();
			$sC = $oC->decryptWithClearTextPass($sCargo, $this->myCypherCode(), $sPass . $this->sPasswordSalt);
			$this->cleanupCypher($oC);
			$oPc = new SssS_Plist($sC);
			return $oPc->a_plist;
		} else return false;
	} // unpackCargo

	static function trailSlashThis($s) {
		$s .= ('/' != substr($s, -1)) ? '/' : '';
		return $s;
	} // trailSlashThis

	function __toString() { return print_r($this, true); } // __toString

} // SssS_SecureRelay
/* * * *\ SssS_SecureRelay (CC) Luke JZ aka SwissalpS /* * * */
?>
