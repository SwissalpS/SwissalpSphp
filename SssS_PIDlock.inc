<?php
/* * * *
 * * SssS_PIDlock.inc
 * * for cli scripts to make sure they are only executed once
 * * and so prevent inherant conflicts
 * *
 * * @version 20091023_225258 (CC) Luke JZ aka SwissalpS
 * * * */
/*
usage:
// for quick use with defaults:
	$oPidLock = new SssS_PIDlock();
	if($oPidLock->isLocked()) die('  already running. ');
	## your magic ##
	$oPidLock->unLock();
	exit;

// to use customized:
	$oPidLock = new SssS_PIDlock('scratch/path', , ,'.pid');
	// the rest as shown above
*/
require_once('SssS_FileGetPutContents.inc');

class SssS_PIDlock {

	private $sPID = null;

	private $sLockPathFile = '';

	public function SssS_PIDlock($sPath = null,
			$sName = '',
			$sExtScript = '.php',
			$sExtLock = '.lock') {

		if (defined('LOCK_FILE')) {

			$this->sLockPathFile = LOCK_FILE;

			return;

		} // if defined LOCK_FILE

		if (!$sPath) $sPath = '/var/run/';
		$sPath .= ('/' != substr($sPath, -1)) ? '/' : ''; // add trailing /

		if (!$sName || '' == $sName)
				$sName = basename((isset($argv[0])
						? $argv[0]
						: (isset($_SERVER['SCRIPT_FILENAME'])
								? $_SERVER['SCRIPT_FILENAME']
								: (isset($_SERVER['SCRIPT_NAME'])
										? $_SERVER['SCRIPT_NAME']
										: 'SwissalpS_pid_lock_file')))
						, $sExtScript) . $sExtLock;
		if ('/' == $sName{0}) $sName = substr($sName, 1);

		$this->sLockPathFile = $sPath . $sName;

		// some prefer this, so it's in here too
		define('LOCK_FILE', $this->sLockPathFile);

	} // SssS_PIDlock

	public function isLocked() {
		// If lock file exists, check if stale. If exists and is not stale, return TRUE
		// Else, create lock file and return FALSE

		if(file_exists($this->sLockPathFile)) {
			// check if it's stale
			$sPID = $this->getPIDfromFile();

			// get all PIDs - thanks to the users posting on php.net
			$aPIDs = explode("\n", trim(`ps -e | awk '{print $1}'`));

			// if PID is still active, return true
			if (in_array($sPID, $aPIDs)) return true;

			// ok, must be stale, renew it
			unlink($this->sLockPathFile);
		} // if already exists

		@file_put_contents_safely($this->sLockPathFile, $this->getPID()
				. "\n", 'wb');
		return false;
	} // isLocked

	public function getPID() {
		if (!$this->sPID) $this->sPID = getmypid();
		return $this->sPID;
	} // getPID

	public function getPIDfromFile() {
		return trim(@file_get_contents_safely($this->sLockPathFile));
	} // getPIDfromFile

	public function getPIDLockPathFile() {
		return $this->sLockPathFile;
	} // getPIDLockPathFile

	public function unLock() {
		if ($this->sLockPathFile) @unlink($this->sLockPathFile);
		$this->sLockPathFile = null;
		unset($this->sLockPathFile);
	} // unLock

	function __toString() { return print_r($this, true); } // __toString

} // SssS_PIDlock
/* * * *\ SssS_PIDlock.inc… (CC) Luke JZ aka SwissalpS /* * * */
?>
