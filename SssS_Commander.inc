<?php
/* * * *
 * * SssS_Commander.inc
 * * parses commands, executes them by calling for it's own
 * * functions or that of it's delegate
 * * initializes from plist and kills app on failure
 * *
 * * @version 20091030_102154 (CC) Luke JZ aka SwissalpS
 * * * */
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_PlistPrefs.inc');
} // if not autoincluding but on php's include path

class SssS_Commander extends SssS_PlistPrefs {

	var $oDelegate = null; // the delegate who might be able to help out

	function SssS_Commander($sSettingsPlistPath, $oDelegate) {
		// init super or die
		$this->SssS_PlistPrefs($sSettingsPlistPath);

		// hook up the delegate
		$this->oDelegate = $oDelegate;

		// init some defaults
		$this->initDefaults();

	} // SssS_Commander

	protected function initDefaults() {

		// keys can be used to alias commands
		$this->getOrSet('translationCommands', array(
				'help' => 'help', '???' => 'help', '?' => 'help',
				'hilfe' => 'help', 'exit' => 'exit'));

		// keys must be translated commands
		$this->getOrSet('translationHelp', array(
				'help' => ' syntax: /(help | hilfe | ??? | ?) [command] ',
				'exit' => 'to log out'));

	} // initDefaults

	function command_exit($aArgs = null) {
		if ($this->oDelegate) $this->oDelegate->shutdown();
		exit();
	} // command_exit

	function command_help($aArgs = null, $oUser = null) {
		$sOut		= null;
		$sCommand	= array_shift($aArgs);
		$sOut 		= $this->translateHelp($sCommand);

		if (!$sOut) {
			// no valid command -> dump it all
			$sOut = '';
			$sDescr = '+ = added; - = not yet implemented;'
					. ' ~ = partly implemented' . "\n"
					. '* = requires elevated priveliges;'
					. ' ** = requires very high priveliges' . "\n\n";
			$sOut .= $sDescr;
			$aC = $this->get('translationCommands');
			$aOut = array();
			foreach ($aC as $sAlias => $sCommand) {
				if (isset($aOut[$sCommand])) {
					// add alias
					$aOut[$sCommand]['aAliases'][] = $sAlias;
				} else {
					// add command
					$mHelp = $this->translateHelp($sCommand);
					$mHelp = (null === $mHelp) ? "(-) no_help_written_yet" : $mHelp;
					$aOut[$sCommand] = array('sHelp' => $mHelp,
											'aAliases' => array());
				} // if alias or command
			} // foreach alias
			foreach ($aOut as $sFunction => $aHelp) {
				$sAliasses = (0 == count($aHelp['aAliases'])) ? '' : ' (' . implode(', ', $aHelp['aAliases']) . ')';
				$sOut .= SssS_AnsiEscapes::gimme('bold')
						. $sFunction . SssS_AnsiEscapes::gimme()
						. $sAliasses . "\n" . '    ' . $aHelp['sHelp'] . "\n";
			} // foreach command

			$sOut .= "\n" . $sDescr
			. 'syntax: /(help | hilfe | ???) [command]' . "\n";
		} // if not got command

		return $sOut;
	} // command_help

	function isAllowed($oUser = null, $iMin = null,	$iMustBe = null) {
		if (!$oUser) return false;
		if (!($aUperms = $oUser->get('permissions'))) return false;

		// god and murphy may do anything :-)
		if (isset($aUperms['system']) && $aUperms['system'] > 2047) return true;

		$bAllowed = false;

		// for others it depends on the room
		$sUIDroom = (isset($this->oDelegate->sUIDroom)) ? $this->oDelegate->sUIDroom : null;

		if ($sUIDroom && ($iMin || $iMustBe) && isset($aPerms[$sUIDroom])) {
			// if need minimum
			if ($iMin && $aPerms[$sUIDroom] >= $iMin) $bAllowed = true;
			// if need to be
			if ($iMustBe && ($aPerms[$sUIDroom] & $iMustBe)) $bAllowed = true;
		} // if got room uid

		return $bAllowed;
	} // isAllowed

	function runCommandLine($sCommandLine = '') {
		$aArgs = explode(' ', $sCommandLine);
		// remove command from args
		$sCommand = array_shift($aArgs);

		if ($mRes = $this->translateCommand($sCommand)) {
			// reply with output
			// translate command
			$sCommand = 'command_' . $mRes;
			if (is_callable(array($this, $sCommand), false)) {
				$sOut = call_user_func(array($this, $sCommand), $aArgs);
			} elseif ($this->oDelegate &&
					is_callable(array($this->oDelegate, $sCommand), false)) {
				$sOut = call_user_func(array($this->oDelegate, $sCommand), $aArgs);
			} else {
				// tell user no go
				$sOut = $sCommand . ' is not a valid command (1
';
			} // if can call me, delegate or no function found
		} else {
			// tell user no go
			$sOut = $sCommand . ' is not a valid command (2
';
		} // if a registered command or not

		return $sOut;
	} // runCommandLine

	function translateCommand($sCommand = '') {
		$s = strtolower($sCommand);
		$aC = $this->get('translationCommands');
		if (isset($aC[$s])) return $aC[$s];
		return null;
	} // translateCommand

	function translateHelp($sCommand = '') {
		if (! $s = $this->translateCommand($sCommand)) return null;

		$aH = $this->get('translationHelp');
		if (isset($aH[$s])) return $aH[$s];
		return null;
	} // translateHelp
} // SssS_Commander
/* * * *\ SssS_Commander.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
