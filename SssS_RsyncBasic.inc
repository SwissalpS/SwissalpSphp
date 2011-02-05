<?php
/* * * *
 * * SssS_RsyncBasic.inc
 * *
 * * @version 20091024_203029 + file_get/put_contents_safely
 * * @version 20090731_031507 (CC) Luke JZ aka SwissalpS
 * * * */
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_Cypher.inc');
	require_once('SssS_RsyncLocation.inc');
} // if not autoincluding but on php's include path

class SssS_RsyncBasic extends SssS_Cypher {
	/**
	 * SssS_RsyncBasic($oRLocA = null, $oRLocB = null, $aAuth = null, $sTransMethod = null, $aExcludePatterns = null, $bSudo = false)
	 * Function_description_here
	 *
	 * @param object $oRLocA primary SssS_RsyncLocation object can be set after initial construction of this object but is vital
	 * @param object $oRLocB secondary SssS_RsyncLocation object can be set after initial construction of this object but is vital
	 * @param array $aAuth hash of authorization info:
	 * 	aAuth['method'] may be one of three: writePass2tempFile, readPassFromFileIn, deleteFileOutAfterTransfer.
	 * 	depending on which method is used, some of these other keys may not be required:
	 * 		pathIn, pathOut, password, cryptMethodID
	 *  read more about this in SssS_RsyncBasic::fInitAuth
	 *
	 * @param string $sTransMethod one of 'Backup', 'Archive', 'Restore', 'Sync' where Backup and Archive are identical
	 * @param mixed $aExcludePatterns string or array. string points to a file containing the patterns, array contains strings of exclude patterns
	 * @param boolean $bSudo if sudo should be prepended: only for cli where user is ready to respond to password challenge, this script assumes to be run as super user already anyway. (defaults to: false)
	 *
	 * @return void not yet decided what and when...
	 *
	 * @uses something - uses_desciption_here
	 *
	 * @version 20090804_153728 (CC)2007 Luke JZ aka SwissalpS
	 */
	function SssS_RsyncBasic($oRLocA = null, $oRLocB = null, $aAuth = null, $sTransMethod = null, $aExcludePatterns = null, $bSudo = false) {
		$this->SssS_Cypher();
		$this->bSudo = ($bSudo) ? true : false;
		$this->fSetRsyncLocations($oRLocA, $oRLocB);
		$this->fInitAuth($aAuth);
		$this->fSetTransMethod($sTransMethod);
		$this->fSetRsyncExcludePatterns($aExcludePatterns);
		$this->bRsyncCompress = true;
		$this->bRsyncVerbose = true;
		$this->fInitPassPathFile4ssh();
	} // SssS_RsyncBasic

	public function doTransfer() {
		if ($this->oRsyncLocationA->rsyncString()
				&& $this->oRsyncLocationB->rsyncString()
				&& $this->sTransMethod) {
			$this->sRsyncScript = ($this->bSudo) ? 'sudo ' : '';
			$this->sRsyncScript .= 'rsync';
			$this->fAddTransMethodBit();
			$this->fAddPasswordBit(); // and write to file if needed
			$this->fAddExclusions();
			$sScriptBase = $this->sRsyncScript;
			$this->fAddPathBits();
		echo 'the rsync line: ' . $this->sRsyncScript;
			$aResults = $this->doScript($this->sRsyncScript);

			if ('Sync' == $this->sTransMethod) {
				$aResults = array($aResults);
				$this->sRsyncScript = $sScriptBase;
				$this->fAddPathBits(true);
				$aResults []= $this->doScript($this->sRsyncScript);
			} // if sync


			$this->fCleanUpRsyncPassFile();

			return $aResults;
		} // if minimal args set
		return false;
	} // doTransfer

	public function fSetRsyncExcludePatterns($aExcludePatterns = false) {
		$this->aRsyncExcludePatterns = array();
		if ($aExcludePatterns) {
			if ('string' == gettype($aExcludePatterns)) {
				// TODO: make paths point to files with exclude patterns using rsync's --exclude-from=FILE
				$sCheckedEP = escapeshellarg($aExcludePatterns);
				if ($sCheckedEP) $this->aRsyncExcludePatterns = array(' --exclude-from=' . $sCheckedEP);
			} elseif ('array' == gettype($aExcludePatterns)) {
				foreach($aExcludePatterns as $sEP) {
					$sCheckedEP = escapeshellarg($sEP);
					if ($sCheckedEP) $this->aRsyncExcludePatterns []= ' --exclude=' . $sCheckedEP;
				} // foreach pattern check
			} // array or string
		} // if patterns given
	} // fSetRsyncExcludePatterns

	public function fSetRsyncLocationA($oRLocA = null) {
		$this->oRsyncLocationA = null;
		if ($oRLocA && method_exists($oRLocA, 'SssS_RsyncLocation')) $this->oRsyncLocationA = $oRLocA;
	} // fSetRsyncLocationA

	public function fSetRsyncLocationB($oRLocB = null) {
		$this->oRsyncLocationB = null;
		if ($oRLocB && method_exists($oRLocB, 'SssS_RsyncLocation')) $this->oRsyncLocationB = $oRLocB;
	} // fSetRsyncLocationB

	public function fSetRsyncLocations($oRLocA = null, $oRLocB = null) {
		$this->fSetRsyncLocationA($oRLocA);
		$this->fSetRsyncLocationB($oRLocB);
	} // fSetRsyncLocations

	public function fSetTransMethod($sTransMethod = false) {
		$this->sTransMethod = false;
		if ($sTransMethod and in_array($sTransMethod, array('Backup', 'Archive', 'Restore', 'Sync'))) $this->sTransMethod = $sTransMethod;
	} // fSetTransMethod

	private function fAddTransMethodBit() {
		$sBit = ' -au' . (($this->bRsyncVerbose) ? 'v' : 'q') . (($this->bRsyncCompress) ? 'z' : '');
		switch ($this->sTransMethod) {
			case 'Sync' :
			case 'Backup' :
			case 'Archive' :
				$sBit .= ' -e ssh';
			break;
			case 'Restore' :
				$sBit .= 'H --del -e ssh'; // use --delete where --del won't
			break;
		} // switch trans method
		$this->sRsyncScript .= $sBit;
	} // fAddTransMethodBit

	private function fAddPasswordBit() {
		$sPB = '';
		if ($this->sAuthMethod) $sPB = ' --password-file=' . escapeshellarg($this->sPassFileOut);
		switch ($this->sAuthMethod) {
			case 'writePass2tempFile' : // write to temporary file given in fileOut or -In. and DELETE after transfer
/*This option allows you to provide  a  password  in  a  file  for
accessing  a  remote rsync daemon. Note that this option is only
useful when accessing an rsync daemon using the built in  trans-
port,  not  when using a remote shell as the transport. The file
must not be world readable. It should contain just the  password
as a single line. */
				file_put_contents_safely($this->sPassFileOut, $this->sPass, 'wb');
				$this->doScript((($this->bSudo) ? 'sudo ' : '') . 'chmod go-r ' . escapeshellarg($this->sPassFileOut));
			break;
			case 'readPassFromFileIn' : // make this the path rsync will read from, DON'T delete it after transfer
			break;
			case 'deleteFileOutAfterTransfer' : // use pass to open pathIn which we write to pathOut for rsync to read from and DELETE after transfer
				// write sPass to file (sPassPathFile) and use, then delete in cleanup
				file_put_contents_safely($this->sPassPathFile, $this->sPass, 'wb');
				$this->doScript(
					(($this->bSudo) ? 'sudo ' : '')
					. 'chmod go-r ' . escapeshellarg($this->sPassPathFile)
				);
				$this->decryptWithFilePassFromFileToFile($this->sPassCryptMethodID, $this->sPassPathFile, $this->sPassFileIn, $this->sPassFileOut);

				// to use sPass without writing it to file:
				//$this->decryptWithClearTextPassFromFileToFile($this->sPassCryptMethodID, $this->sPass, $this->sPassFileIn, $this->sPassFileOut);
			break;
			default : $sPB = '';
		} // switch
		$this->sRsyncScript .= $sPB;
	} // fAddPasswordBit

	function fCleanUpRsyncPassFile() {
		switch ($this->sAuthMethod) {
			case 'writePass2tempFile' : // delete passFileOut
				unset($this->sPassFileOut);
			break;
			case 'readPassFromFileIn' : // DON'T delete
			break;
			case 'deleteFileOutAfterTransfer' : // delete passFileOut & passPathFile
				unset($this->sPassPathFile);
				unset($this->sPassFileOut);
			break;
			default:
		} // switch
	} // fCleanUpRsyncPassFile

	function fAddExclusions() {
        $sEx = implode('', $this->aRsyncExcludePatterns);
        $this->sRsyncScript .= $sEx;
    } // fAddExclusions

	function fAddPathBits($bReverse = false) {
		if ($bReverse) {
			$sPathBit = ' ' . $this->oRsyncLocationB->rsyncString() . ' ' . $this->oRsyncLocationA->rsyncString();
		} else {
			$sPathBit = ' ' . $this->oRsyncLocationA->rsyncString() . ' ' . $this->oRsyncLocationB->rsyncString();
		} // if reverse ie sync

		$this->sRsyncScript .= $sPathBit;
	} // fAddPathBits

	// subclass and modify location to some place where as few users as possible may read.
	private function fInitPassPathFile4ssh() {
		$this->sPassPathFile = '/tmp/SssS_frog';
	} // fInitPassPathFile4ssh

	public function fInitAuth($aAuth = null) {
		$this->sAuthMethod = null;  // this is only set after init.
		$this->sPass = null;
		$this->sPassCryptMethodID = null; //
		$this->sPassFileIn = null; // path to hashed file to be opened with pass or if no pass is given it is used as password-file
		$this->sPassFileOut = null;
		if (isset($aAuth, $aAuth['method'])) {
			switch ($aAuth['method']) {
				case 'writePass2tempFile' : // write to temporary file given in fileOut or -In.
					if ($aAuth['password']) {
						// check the path
						if (isset($aAuth['pathIn'])) {
							$this->sPassFileOut = $aAuth['pathIn'];
						} elseif (isset($aAuth['pathOut'])) {
							$this->sPassFileOut = $aAuth['pathOut'];
						} else {
							list($sPath, $sFile) = $this->fGetSafeUniqueFolderAndFileInPath('/tmp');
							$this->sPassFileOut = $sPath . $sFile;
						} // if no path given
						$this->sAuthMethod = $aAuth['method'];
						$this->sPass = $aAuth['password'];
					} // if pass given
					break;
				case 'readPassFromFileIn' : // make this the path rsync will read from, DON'T delete it after transfer
					if (($aAuth['pathIn'] || $aAuth['pathOut'])) {
						if ($aAuth['pathIn'] && '' != $aAuth['pathIn']) {
							$this->sPassFileOut = $aAuth['pathIn'];
							$this->sAuthMethod = $aAuth['method'];
						} elseif ($aAuth['pathOut']) {
							$this->sPassFileOut = $aAuth['pathOut'];
							$this->sAuthMethod = $aAuth['method'];
						} // if which path
					} // if path given
					break;
				case 'deleteFileOutAfterTransfer' : // use pass to open pathIn which we write to pathOut for rsync to read from and DELETE after transfer
					if (isset($aAuth['pathIn'], $aAuth['pathOut'], $aAuth['password'])) {
						$this->sPassFileOut = $aAuth['pathOut'];
						$this->sPassFileIn = $aAuth['pathIn'];
						$this->sPass = $aAuth['password'];
						$this->sPassCryptMethodID = ($aAuth['cryptMethodID']) ? $aAuth['cryptMethodID'] : 'default';
						$this->sAuthMethod = $aAuth['method'];
					} // if parameters all given
				default : // no method -> no need to check more as user might be running in terminal or local without password
			} // switch case
		} // if auth given
	} // fInitAuth
}
/* * * *\ SssS_RsyncBasic.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
