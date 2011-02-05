<?php
/* * * *
 * * SssS_PlistPrefs.inc !!!!!!! still heavily in alpha !!!!!!!!!!
 * *adds save and load functionality to SssS_Plist
 * * @version 20091024_043205 (CC) Luke JZ aka SwissalpS
 * * * */
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_Plist.inc');
} // if not autoincluding but on php's include path

if (version_compare('5', phpversion(), '>')) {
	// pre v5 : we trigger_error() instead
} else {
	if (!defined('AUTO_INCLUDING')) {
		require_once('_external/CustomException.inc');
		require_once('_synergize/PHOCOA/SssSException.inc');
	} // if not autoincluding but on php's include path
	class SssSPlistPrefsException extends SssSException {
		var $s_path = '';
	} // SssSPlistException
} // if at least PHP 5

// extends the base class
class SssS_PlistPrefs extends SssS_Plist {

	/**
	 * constructs the SssS_PlistPrefs instance from the given path
	 *
	 * NOTE: expects apple's plist format. Will read most plists but not all! especially with certain DATA TODO
	 * works best with booleans, path strings, numbers, arrays of those and hashes that have keys with php friendly names and values
	 *
	 * @param string $s_path must be valid read & write path
	 *
	 * @uses SssS_Plist::SssS_Plist init and extend
	 */
	// constructor
	function SssS_PlistPrefs($s_path,
			$b_outPlistKeyOnOwnLine = false,
			$b_identifyDataAndDatesInOutputArray = true,
			$bPathCheckCreate = true) {

		//first time around we can't use load method
		$this->SssS_Plist($s_path, null, $b_outPlistKeyOnOwnLine,
				$b_identifyDataAndDatesInOutputArray, $bPathCheckCreate);
		// could not read or create a new file?
		if (!$this->s_path || ('' == $this->s_path)) {
			$s = chr(10) . get_class($this)
				. '::_construct path error, can not init prefs from path: -->'
				. $s_path . "<--\n pwd: " . getcwd() . "\n";
			if (version_compare('5', phpversion(), '>')) {
				var_dump(debug_backtrace(false));
				trigger_error($s, E_USER_ERROR);

			} else {
				$e = new SssSPlistPrefsException($s);
				$e->s_path = $s_path;
				throw $e;

			} // if pre Exceptions

			return null;
		} // if not good
	} // constructor
	/*
	public function set($aNew) {
		if ('array' != gettype($aNew)) return false;
		return $this->initFromArray($aNew);
	} // set
	*/
	public function setDefaults() {
	} //

	public function resetDefaults() {
		// only reset the values in myDefaults
// TODO: traverse down to lowest branch 		foreach($this->myDefaults() as $
		$this->a_plist = $this->a_plist;
		return true;
	} // resetDefaults

} // SssS_PlistPrefs
/* * *\ SssS_PlistPrefs (CC)2009 Luke JZ aka SwissalpS /* * */
?>
