<?php
/* * * *
 * * SssS_RsyncLocation.inc
 * *
 * * version 20090731_031456 (CC) Luke JZ aka SwissalpS
 * * * */

class SssS_RsyncLocation {
	function SssS_RsyncLocation($sUser = false, $sIP = false, $sPath = false) {
		// if no path is given, we assume current pwd, but only if no ip
		if (!$sPath || '' == $sPath) {
			if ($sIP && '' != $sIP) {
				$sPath = '';
			} else { // local
				$sPath = '.';
			} // if local or remote
		} // if path MT or missing set a default depending on loc or rem
		// note: this is a quick fix, extremly short paths under 3 chars will fail to be checked on trailing /
		elseif ((3 < strlen($sPath))) {
			if ('/' != substr($sPath, -1)) $sPath .= '/';
		} // check for ending slash

		if ($sIP && '' == trim($sIP)) $sIP = false;
		if ($sUser && '' == trim($sUser)) $sUser = false;
		if ($sUser && !$sIP) $sIP = 'localhost'; // try localhost if user given but ip not set
		$this->sUser = $sUser;
		$this->sIP = $sIP; // may be hostname or ip
		$this->sPath = $sPath;
		//$this->bLocal = !isset($sIP);
		return $this;
	} // SssS_RsyncLocation

	function rsyncString() {
		// put the pieces together ready for insertion into rsync command line
		// return false if can't make a proper string
		$sRet = '';
		if ($this->sUser && $this->sIP) {
			$sRet = $this->sUser . '@' . $this->sIP . ':';
		} elseif ($this->sIP) {
			$sRet = $this->sIP . ':';
		} // else return false;

		$sRet .= escapeshellarg($this->sPath);

		return $sRet;
	} // rsyncString

	function __toString() { return print_r($this, true); } // __toString

} // SssS_RsyncLocation
/* * * *\ SssS_RsyncLocation.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
