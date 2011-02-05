<?php
/* * * *
 * * SssS_RsyncTask.inc - !!!!!alpha!!!!
 * * self persistant rsync task
 * *
 * * reads its parameters from plist file
 * * writes its parameters to plist file
 * * can be modified or executed with this class
 * * it may be protected by a password as it is bound to contain sensitive information
 * * also possible to write out as self executing shell script using cl arguments
 * * you may subclass fDecypherPassword to have an encrypted password in datastore but then you'll have that code in a php file or you call an external app to do so. At some point you'll just have to settle down ;-)
 * *
 * * birthed for cron job situations running as 'root' so they will be chmoded to 700 thus keeping the cleartext main password reasonably safe and out of the 'normaly' expected location. In a cron scenario you have to put the password(s) somewhere or use static ips and certificates. --> TODO :-)
 * *
 * * @version 20091024_202644 + file_get/put_contents_safely
 * * @version 20090804_004243 (CC) Luke JZ aka SwissalpS
 * * * */
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_Plist.inc');
	require_once('SssS_RsyncBasic.inc');
} // if not autoincluding but on php's include path

class SssS_RsyncTask extends SssS_RsyncBasic {
	function SssS_RsyncTask($sPathToMyPlist = '', $sPassWord = '', $sCyperID = '') {
		if ($this->fInitRsyncTask($sPathToMyPlist, $sPassWord, $sCyperID)) {
			 // if initiated, that is read uncyphered portions
			//if (
		} else {
			// probably new file
			return $this->fGlueRsyncTask();
		} //
	} // SssS_RsyncTask


	function fInitRsyncTask($sPathToMyPlist = '', $sPassWord = '', $sCyperID = '') {
		$bInit = false; // true; // TODO make sure this is only set true when actually true
		$this->fResetRsyncTask();
		$sPP = trim($sPathToMyPlist);
		if ('' != $sPP) {
			if ($sRaw = @file_get_contents_safely($sPP)) {
				// something read, try to read plist
				$oP = new SssS_Plist($sRaw);
				$aP = &$oP->a_plist;
				$this->aSettingsRsyncTask['myPath'] = $sPP;
				if (isset($aP['protection'], $aP['protection']['passinself']) && false != $aP['protection']['passinself'] && 'false' != $aP['protection']['passinself'] ) {
					// passinself
					$this->aSettingsRsyncTask['base']['protection']['passinself'] = $aP['protection']['passinself'];
				} else {
					// password must have been passed for this to work
					$this->aSettingsRsyncTask['base']['passinself'] = false;
					if (isset($aP['protection']['pass']) && $this->fValidatePassAgainstHash($sPassWord, $aP['protection']['pass'])) {
						$this->aSettingsRsyncTask['passedPass'] = array($sPassWord, $sCyperID);
					} // if passed pass confirms with hash in plist
				} // if passinself

				if (isset($aP['protection']['cypher'])
						&& 'string' == gettype($aP['protection']['cypher'])) {
					$this->aSettingsRsyncTask['base']['protection']['cypher']
						= $aP['protection']['cypher'];
				} // if cypherinself

				if (isset($aP['workdir']) && @is_dir($aP['workdir'])
						&& is_writable($aP['workdir'])) {
					$this->aSettingsRsyncTask['base']['workdir']	= $aP['workdir'];
				} // if valid workdir

				if (isset($aP['dataenc'])) $bInit = $this->fCrackRsyncTask($aP['dataenc']); // false; // true;
			} else {
				// file not existant or problem reading -> continue with password and cypher
				if ('string' == gettype($sPassWord) && '' != $sPassWord) {
					$sCyperID = ('string' == gettype($sCyperID)) ? $sCyperID : '';
					$this->aSettingsRsyncTask['passedPass'] = array($sPassWord, $sCyperID);
				}
				$this->aSettingsRsyncTask['myPath'] = $sPP; // ftb maybe later consolidate with above
			} // path readable or not
		} // if path not MT
		return $bInit;
	} // fInitRsyncTask

	function fGlueRsyncTask() {
		// encrypt and save to file
		if (!isset($this->aSettingsRsyncTask['myPath']) || (!$this->aSettingsRsyncTask['myPath']) || ('' == $this->aSettingsRsyncTask['myPath'])) return false;

		if ($this->aSettingsRsyncTask['passedPass']) {
			// password was passed on init
			list($sPass, $sCypher) = $this->aSettingsRsyncTask['passedPass'];
		} elseif ($this->aSettingsRsyncTask['base']['protection']['passinself']) {
			// use password from plist
			$sPass = $this->fDecypherPassword(
					$this->aSettingsRsyncTask['base']['protection']['passinself']);
			$sCypher = $this->aSettingsRsyncTask['base']['protection']['cypher'];
		} else return false; //

		if (!isset($this->oPRT)) $this->oPRT = new SssS_Plist();
		$sPRT = $this->oPRT->initFromArray($this->aSettingsRsyncTask['rsyncBasic']);
		if (!isset($this->aSettingsRsyncTask['cracker'])) {
			// prepare the workdir by creating a unique folder and unique file
			list($sPath, $sFilePass) = $this->fGetSafeUniqueFolderAndFileInPath($this->aSettingsRsyncTask['base']['workdir']);
			$this->aSettingsRsyncTask['cracker']['sTmpPlistPath'] = $sPath . $this->fMakeSafeUniqueFileInPath($sPath);
			$this->aSettingsRsyncTask['cracker']['sTmpEncodedPath'] = $sPath . $this->fMakeSafeUniqueFileInPath($sPath);
			$this->aSettingsRsyncTask['cracker']['sTmpPassPath'] = $sPath . $sFilePass;
			$this->aSettingsRsyncTask['cracker']['sUworkPath'] = $sPath;
		} // if not yet created temp folder

		// write data, password to temp location for openssl action with least possibility of spying
		@file_put_contents_safely($this->aSettingsRsyncTask['cracker']['sTmpPassPath'], $sPass,
				'wb');
		@file_put_contents_safely($this->aSettingsRsyncTask['cracker']['sTmpPlistPath'], $sPRT,
				'wb');
		// encrypt data to temp encoded file
		$this->encryptWithFilePassFromFileToFile($sCypher,
				$this->aSettingsRsyncTask['cracker']['sTmpPassPath'],
				$this->aSettingsRsyncTask['cracker']['sTmpPlistPath'],
				$this->aSettingsRsyncTask['cracker']['sTmpEncodedPath']);
		// clean up 1
		// just put something else in there while we work on the task
		$sRandom = '';
		for ($i = 0; strlen($sPass) > $i; $i++)
				{ $sRandom .= chr(rand(32, 126)); }
		@file_put_contents_safely(
				$this->aSettingsRsyncTask['cracker']['sTmpPassPath'],
						$sRandom, 'wb');
		$sRandom = '';
		for ($i = 0; strlen($sPRT) > $i; $i++)
				{ $sRandom .= chr(rand(32, 126)); }
		@file_put_contents_safely(
				$this->aSettingsRsyncTask['cracker']['sTmpPlistPath'],
						$sRandom, 'wb');

		// save to file
		$a = $this->aSettingsRsyncTask['base'];
		$a['dataenc'] = @file_get_contents_safely(
				$this->aSettingsRsyncTask['cracker']['sTmpEncodedPath']);
		$oP = new SssS_Plist();
		$s = $oP->initFromArray($a);
		@file_put_contents_safely($this->aSettingsRsyncTask['myPath'], $s, 'wb');

		// clean up 2
		@file_put_contents_safely(
				$this->aSettingsRsyncTask['cracker']['sTmpEncodedPath'],
				md5('$sRandom'), 'wb');

		return true;
	} // fGlueRsyncTask

	private function fCrackRsyncTask($sDataEncrypted) {
		if ($this->aSettingsRsyncTask['passedPass']) {
			// password was passed on init
			list($sPass, $sCypher) = $this->aSettingsRsyncTask['passedPass'];
		} elseif ($this->aSettingsRsyncTask['base']['protection']['passinself']) {
			// use password from plist
			$sPass = $this->fDecypherPassword(
					$this->aSettingsRsyncTask['base']['protection']['passinself']);
			$sCypher = $this->aSettingsRsyncTask['base']['protection']['cypher'];
		} else return false; //

		// prepare the workdir by creating a unique folder and unique file
		list($sPath, $sFilePass) = $this->fGetSafeUniqueFolderAndFileInPath($this->aSettingsRsyncTask['base']['workdir']);
		$sTmpPlistPath = $sPath . $this->fMakeSafeUniqueFileInPath($sPath);
		$sTmpEncodedPath = $sPath . $this->fMakeSafeUniqueFileInPath($sPath);
		$sTmpPassPath = $sPath . $sFilePass;

		// write enc data, password to temp location for openssl action with least possibility of spying
		@file_put_contents_safely($sTmpPassPath, $sPass, 'wb');
		@file_put_contents_safely($sTmpEncodedPath, $sDataEncrypted, 'wb');
		// decrypt data to temp plist
		$this->decryptWithFilePassFromFileToFile($sCypher, $sTmpPassPath,
				$sTmpEncodedPath, $sTmpPlistPath);
		$sTmpPlistContents = file_get_contents_safely($sTmpPlistPath);
	echo '
	$sTmpPlistContents:
' . $sTmpPlistContents . '
	';/**/
		$this->oPRT = new SssS_Plist($sTmpPlistContents);
	//var_dump($this->oPRT);
		// clean up
		$sRandom = '';
		for ($i = 0; strlen($sPass) > $i; $i++)
				{ $sRandom .= chr(rand(32, 126)); }
		// put something else in there while we work on the task
		@file_put_contents_safely($sTmpPassPath, $sRandom, 'wb');
		$sRandom = '';
		for ($i = 0; strlen($sDataEncrypted) > $i; $i++)
				{ $sRandom .= chr(rand(32, 126)); }
		@file_put_contents_safely($sTmpEncodedPath, $sRandom, 'wb');

		// save to settings to do final cleanup and other operations
		$this->aSettingsRsyncTask['cracker'] = array(
				'sTmpPassPath' => $sTmpPassPath,
				'sTmpEncodedPath' => $sTmpEncodedPath,
				'sTmpPlistPath' => $sTmpPlistPath,
				'sUworkPath' => $sPath);

		if (!$this->oPRT){//isset($this->oPRT->a_plist)) {
			$this->fClearTraces();
			return false;
		} //
	//echo ' -- loaded  -- ';
		// try to init Locations and RsyncBasic part of this object
		$aPT = $this->oPRT->a_plist;
		if (!(isset($aPT['LocationA'], $aPT['LocationB']) && 'array' == gettype($aPT['LocationA']) && 'array' == gettype($aPT['LocationB']) && (0 < count($aPT['LocationA'])) && (0 < count($aPT['LocationB'])))) { $this->fClearTraces(); return false; } //

	//var_dump($this->aSettingsRsyncTask['rsyncBasic'], $aPT);
		$aSRB = array_merge($this->aSettingsRsyncTask['rsyncBasic'], $aPT);
		$this->aSettingsRsyncTask['rsyncBasic'] = $aSRB;

		$this->oRLocA = new SssS_RsyncLocation($aSRB['LocationA']['sUser'], $aSRB['LocationA']['sIP'], $aSRB['LocationA']['sPath']);
		$this->oRLocB = new SssS_RsyncLocation($aSRB['LocationB']['sUser'], $aSRB['LocationB']['sIP'], $aSRB['LocationB']['sPath']);
		$this->SssS_RsyncBasic($this->oRLocA, $this->oRLocB, $aSRB['aAuth'], $aSRB['sTransMethod'], $aSRB['aExcludePatterns'], $aSRB['bSudo']);

		return true; // ? true : false;
	} // fCrackRsyncTask

	// subclass this to adapt your defaults and distribution of where to keep defaults
	function fResetRsyncTask() {
		$this->aSettingsRsyncTask = array(
				'passedPass' => false,
				'myPath' => '',
				'uniqueSafeDir' => false,
				'base' => array(
					'protection' => array(
						'pass' => 'a94a8fe5ccb19ba61c4c0873d391e987982fbbd3',  // sha1 of test
						'cypher' => 'des3',
						'passinself' => 'test'), // false or the password
					'workdir' => '/tmp',
					'dataenc' => ''), // encrypted plist containing all the info required to run RsyncBasic commands
				'rsyncBasic' => array( // decrypted array of plist
					'LocationA' => array('sUser' => false, 'sIP' => false, 'sPath' => false),
					'LocationB' => array('sUser' => false, 'sIP' => false, 'sPath' => false),
					'aAuth' => false,
					'sTransMethod' => 'Archive', // same as backup
					'aExcludePatterns' => array(),
					'bSudo' => false // only makes sense from cli where you can respond to sudo password
				));
	} // fResetRsyncTask

	// subclass this
	function fValidatePassAgainstHash($sPass, $sHash) {
		return ($sHash == sha1($sPass));
	} // fValidatePassAgainstHash

	// subclass this
	function fDecypherPassword($sPass) {
		return $sPass;
	} // fDecypherPassword

	function fClearTraces() {
		if (isset($this->aSettingsRsyncTask['cracker'])) {
			if (isset($this->aSettingsRsyncTask['cracker']['sUworkPath']) && file_exists($this->aSettingsRsyncTask['cracker']['sUworkPath'])) {
				// delete everything overwriting data
				$this->doScript('rm -fPr ' . escapeshellarg($this->aSettingsRsyncTask['cracker']['sUworkPath']));
				if (0 != $this->aResults['scriptExitCode']) {
					// try one by one
					if (isset($this->aSettingsRsyncTask['cracker']['sTmpPlistPath'])) $this->doScript('rm -fP ' . escapeshellarg($this->aSettingsRsyncTask['cracker']['sTmpPlistPath']));
					if (isset($this->aSettingsRsyncTask['cracker']['sTmpEncodedPath'])) $this->doScript('rm -fP ' . escapeshellarg($this->aSettingsRsyncTask['cracker']['sTmpEncodedPath']));
					if (isset($this->aSettingsRsyncTask['cracker']['sTmpPassPath'])) $this->doScript('rm -fP ' . escapeshellarg($this->aSettingsRsyncTask['cracker']['sTmpPassPath']));
				} // if deleting dir worked
			} //
		} // if got cracker settings
	} // fClearTraces

	// override the SssS_RsyncBasic function
	private function fInitPassPathFile4ssh() {
		if (isset($this->aSettingsRsyncTask['cracker'], $this->aSettingsRsyncTask['cracker']['sUworkPath'])) {
			$this->sPassPathFile = $this->aSettingsRsyncTask['cracker']['sUworkPath'] . $this->fMakeSafeUniqueFileInPath($this->aSettingsRsyncTask['cracker']['sUworkPath']);
		} else {
			if (isset($this->aSettingsRsyncTask['base'], $this->aSettingsRsyncTask['base']['workdir'])) {
				$sPath = $this->aSettingsRsyncTask['base']['workdir'];
			} else {
				$sPath = '/tmp/';
			} // if workdir set

			list($sPath, $sFile) = $this->fGetSafeUniqueFolderAndFileInPath($sPath);
			$this->sPassPathFile = $sPath . $sFile;
		} // if cracker set
	} // fInitPassPathFile4ssh

	// subclass to adapt path for inclusion of 'this' class
	// 20100503_161103 SwissalpS + $sPHPinterpreterPath
	public function writeShellRunnerTo($sPathFile, $sPermCode = '700',
												$sPHPinterpreterPath = null) {
		if (!$sPHPinterpreterPath)
				$sPHPinterpreterPath = self::getPHPinterpreterPath();
		$sMyPath = (!isset($this->aSettingsRsyncTask['myPath']) || (!$this->aSettingsRsyncTask['myPath'])) ? '' : $this->aSettingsRsyncTask['myPath'];
		$sOut = sprintf('#!%2$s
<?php
require_once("SssS_RsyncTask.inc");
$oRT = new SssS_RsyncTask("%1$s");
$res = $oRT->doTransfer();
echo chr(10) . "transfer " . (($res) ? "succeeded" : "failed");
$oRT->fClearTraces();
echo chr(10) . $res["scriptExitCode"]; //$oRT->aResults["scriptExitCode"]; //"SssS_RsyncTask (%1$s) exited with code:"
?>', $sMyPath, $sPHPinterpreterPath);
		file_put_contents_safely($sPathFile, $sOut, 'wb');
		$this->doScript('chmod ' . $sPermCode . ' ' . escapeshellarg($sPathFile));
	} // writeShellRunnerTo
} // SssS_RsyncTask
/* * * *\ SssS_RsyncTask.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
