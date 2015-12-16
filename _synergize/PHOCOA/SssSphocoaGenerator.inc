<?php
/* * * *
 * * SssS/_synergize/PHOCOA/SssSphocoaGenerator.inc
 * *
 * * to be run as cli
 * * guides thru the setup of a copy from template structure
 * * modifying as few options as needed
 * *
 * * developed and tested on a mac w OS X 10.5.x
 * * written mainly for own use... don't have much \ in my paths ;-)
 * * for now no smarty, propel support -> targeting cli app
 * *
 * * #version 20151213_170326 (CC) Luke JZ aka SwissalpS ver 4 plists keys have changed, added template fixer functions
 * * @version ?? (CC) Luke JZ aka SwissalpS ver 3 plists base64-encode passwords
 * * @version 20110619_205946 (CC) Luke JZ aka SwissalpS ver 2 plists with all keys being used as searches
 * * @version 20100501_081510 (CC) Luke JZ aka SwissalpS
 * * * */
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_Plist.inc');
	require_once('SssS_CLI_Tools.inc');
	require_once('SssS_ShellScriptRunnerForDummies.inc');
} // if not autoincluding but on php's include path

if (!defined('DIR_SEP')) define('DIR_SEP', DIRECTORY_SEPARATOR);
if (!defined('NL')) define('NL', chr(10));

class SssSphocoaGenerator {

	const sAppleScriptletBitNew = '/SssSphocoaGen/newSssSphocoaProjectFromPlist.app/Contents/Resources/newSssSphocoaProject';
	const sAppleScriptletBitInPlace = '/SssSphocoaGen/newSssSphocoaProjectFromPlist.app/Contents/Resources/remakeTemplatedFilesWithInfoFromPlist';

	// __construct()
	public function SssSphocoaGenerator() {} // SssSphocoaGenerator

	// dealloc
	public static function bail() {	echo NL . 'SssSphocoaGenerator exiting...' . NL; exit(0); } // bail

	// main cli 'inlet'
	public static function runCLI() {
		$a = $_SERVER['argv'];
		$sInvocation = array_shift($a); // get $0 path
		//echo NL . $sInvocation . '<-sInvocation-';
		$sPathInvocationBase = self::parseInvocationBase($sInvocation);

		$bUseAllDefaults = $sPathDest = $sAppName = false;
		$aPaths = array();
		while (0 < count($a)) {
			$s = array_shift($a);

			if ('help' == strtolower($s)) {
				self::showHelpCLI();
				self::bail();
			} // if help passed as argument

			if (('-target' == $s) && (0 < count($a)))
				$sPathDest = array_shift($a);
				// setting new sPathDest destination dir

			else if ((('-appName' == $s)
				|| ('-appname' == $s)) && (0 < count($a)))
				$sAppName = array_shift($a);
				// setting an app name by command line (not yet implemented) might never as editing the plists is so simple and gui-able

			else if (('-noInteraction' == $s)
				|| ('-nointeraction' == $s)) $bUseAllDefaults = true;
				// just use defaults for all files from now on

			else if (('-withInteraction' == $s)
				|| ('-withinteraction' == $s)) $bUseAllDefaults = false;
				// ask for each item for all files from now on

			else {
				$aPaths[] = array($s, $bUseAllDefaults, $sPathDest, $sAppName);
				// got a path to a plist (if correct syntax etc) theoretically could be a string xml:plist
				$sAppName = $sPathDest = false;
			} // if path or modifier

		} // loop all args building aPaths array

		$aFails = array(); $iYes = 0;
		foreach ($aPaths as $aPath) {
			list($sPath, $bUseAllDefaults, $sPathDest, $sAppName) = $aPath;
			// paths in $a may come quoted or escaped
			$sPath = self::trimOrUnescapeShellArgument($sPath);

			if (self::makeNewSssSphocoaProjectWithPlist($sPath, $bUseAllDefaults, $sPathDest, $sPathInvocationBase, $sAppName))
				$iYes++; // count success

			else
				$aFails[] = array('sPath' => $sPath,
						'bUseAllDefaults' => $bUseAllDefaults,
							  'sPathDest' => $sPathDest);
				// record fails for debugging at end

		} // loop all paths generating a project at each given set of values in plist or flagged form

		// good bye
		echo sprintf('%1$ssuccessful in %2$s out of %3$s files%1$s',
						NL, $iYes, count($aPaths));

		// debug fails if any
		if (!empty($aFails)) echo 'failed paths: ' . print_r($aFails, true);

		self::bail();
	} // runCLI


	// cli 'inlet'
	public static function runCLIinPlaceReplacement() {
		$a = $_SERVER['argv'];
		$sInvocation = array_shift($a); // get $0 path
		$sPathInvocationBase = self::parseInvocationBase($sInvocation);

		$aFails = array(); $iYes = 0; $iCount = count($a);
		while (0 < count($a)) {
			$s = array_shift($a);
			$mPlist = self::trimOrUnescapeShellArgument($s);
			if (self::inPlaceRefreshTemplatesInPlist($mPlist, null, $sPathInvocationBase))
				$iYes++; // count success

			else
				$aFails[] = array('mPlist' => $mPlist,
					 'sPathInvocationBase' => $sPathInvocationBase);

		} // while have arguments

		// good bye
		echo sprintf('%1$ssuccessful in %2$s out of %3$s plist files%1$s',
						NL, $iYes, $iCount);

		// debug fails if any
		if (!empty($aFails)) echo 'failed paths: ' . print_r($aFails, true);

		self::bail();

	} // runCLIinPlaceReplacement


	// cli 'inlet'
	public static function runCLIupdatePlistVersion1To2() {

		self::updatePlistVersion(1, 2);

	} // runCLIupdatePlistVersion1To2


	// cli 'inlet'
	public static function runCLIupdatePlistVersion1To3() {

		self::updatePlistVersion(1, 3);

	} // runCLIupdatePlistVersion1To3


	// cli 'inlet'
	public static function runCLIupdatePlistVersion2To3() {

		self::runCLIupdatePlistVersion(2, 3);

	} // runCLIupdatePlistVersion2To3


	public static function runCLIupdatePlistVersion3To4() {

		self::runCLIupdatePlistVersion(3, 4);

	} // runCLIupdatePlistVersion3To4


	protected static function runCLIupdatePlistVersion($iFromVersion = 1, $iToVersion = 4) {

		$a = $_SERVER['argv'];
		$sInvocation = array_shift($a); // get $0 path
		$sPathInvocationBase = self::parseInvocationBase($sInvocation);

		if (1 == $iFromVersion) {

			$aFails = array(); $iYes = 0; $iCount = count($a);

			foreach($a as $mPlist) {

				$mPlist = self::trimOrUnescapeShellArgument($mPlist);

				if (self::updatePlistVersion1To2($mPlist, null, $sPathInvocationBase)) {

					$iYes++; // count success

				} else {

					$aFails[] = array('mPlist' => $mPlist,
						 'sPathInvocationBase' => $sPathInvocationBase);

				} // if errors

			} // while have arguments

			// status message
			echo sprintf('%1$ssuccessfully updated %2$s out of %3$s files from version %4$s to %5$s%1$s',
							NL, $iYes, $iCount, $iFromVersion, 2);

			// debug fails if any
			if (!empty($aFails)) {

				echo 'failed paths: ' . print_r($aFails, true);

				self::bail();

			} // if there were errors

			$iFromVersion++;

		} // if canvert from version 1

		// is this all?
		if (2 == $iToVersion) self::bail();

		if (2 == $iFromVersion) {

			$aFails = array(); $iYes = 0; $iCount = count($a);

			foreach($a as $mPlist) {

				$mPlist = self::trimOrUnescapeShellArgument($mPlist);

				if (self::updatePlistVersion2To3($mPlist, null, $sPathInvocationBase)) {

					$iYes++; // count success

				} else {

					$aFails[] = array('mPlist' => $mPlist,
						 'sPathInvocationBase' => $sPathInvocationBase);

				} // if errors

			} // while have arguments

			// status message
			echo sprintf('%1$ssuccessfully updated %2$s out of %3$s files from version %4$s to %5$s%1$s',
							NL, $iYes, $iCount, $iFromVersion, 3);

			// debug fails if any
			if (!empty($aFails)) echo 'failed paths: ' . print_r($aFails, true);

			$iFromVersion++;

		} // if canvert from version 2

		// is this all?
		if (3 == $iToVersion) self::bail();

		if (3 == $iFromVersion) {

			$aFails = array(); $iYes = 0; $iCount = count($a);

			foreach($a as $mPlist) {

				$mPlist = self::trimOrUnescapeShellArgument($mPlist);

				if (self::updatePlistVersion3To4($mPlist, null, $sPathInvocationBase)) {

					$iYes++; // count success

				} else {

					$aFails[] = array('mPlist' => $mPlist,
						 'sPathInvocationBase' => $sPathInvocationBase);

				} // if errors

			} // while have arguments

			// status message
			echo sprintf('%1$ssuccessfully updated %2$s out of %3$s files from version %4$s to %5$s%1$s',
							NL, $iYes, $iCount, $iFromVersion, 4);

			// debug fails if any
			if (!empty($aFails)) {
				echo 'failed paths: ' . print_r($aFails, true);
			} // if got fails

			$iFromVersion++;

		} // if canvert from version 2

		// is this all?
		//if (4 == $iToVersion)

		// goodbye
		self::bail();

	} // runCLIupdatePlistVersion


	public static function runCLIupdateTemplatesVersion1To2() {

		$a = $_SERVER['argv'];
		$sInvocation = array_shift($a); // get $0 path
		$sPathInvocationBase = self::parseInvocationBase($sInvocation);

		foreach ($a as $sPathBase) {

			self::updateTemplatesVersion1To2(self::trimOrUnescapeShellArgument($sPathBase));

		} // loop all given paths

		self::bail();

	} // runCLIupdateTemplatesVersion1To2


	public static function runCLIupdateTemplatesVersion2To4() {

		$a = $_SERVER['argv'];
		$sInvocation = array_shift($a); // get $0 path
		$sPathInvocationBase = self::parseInvocationBase($sInvocation);

		foreach ($a as $sPathBase) {

			self::updateTemplatesVersion2Or3To4(self::trimOrUnescapeShellArgument($sPathBase));

		} // loop all given paths

		self::bail();

	} // runCLIupdateTemplatesVersion2To4


	// default values
	public static function defaultPlist($iVersion = 1) {

		$a = array();

		switch($iVersion) {

			case 1 :

				$sConf = 'SssSphocoaGeneratorDefault1.conf'; break;

			case 2 :

				$sConf = 'SssSphocoaGeneratorDefault2.conf'; break;

			case 3 :

				$sConf = 'SssSphocoaGeneratorDefault3.conf'; break;

			default :

				$sConf = 'SssSphocoaGeneratorDefault.conf'; break;

		} // switch version

		@include($sConf);

		return new SssS_Plist($a);

	} // defaultPlist


	// main interactive wizard
	public static function makeNewSssSphocoaProject($bUseAllDefaults = false) {

		self::makeNewSssSphocoaProjectWithPlist(self::defaultPlist(0), $bUseAllDefaults);

	} // makeNewSssSphocoaProject


	// make from settings in a plist file interactively changing or complementing arguments
	public static function makeNewSssSphocoaProjectWithPlist(
			$mPlist = null, // SssS_Plist object a path or a plist xml string or an array
			$bUseAllDefaults = false,
			$sPathDest = false,
			$sPathInvocationBase = null, // alternative root calculated from invocation path
			$sAppName = false) {

		static $ats = null; static $gi = null; static $iVersion = null;
		static $rts = null; if (!$rts) {

			$rts = 'removeTrailingSlash';
			$ats = 'addTrailingSlash';
			$gi = 'getInputCLI';
			$iVersion = 4;

		} // first run

		if ($sPathInvocationBase)
			$sPathInvocationBase = SssS_CLI_Tools::$rts($sPathInvocationBase, DIR_SEP);

		$sArgType = gettype($mPlist);
		if (('object' == $sArgType) && is_subclass_of($mPlist, 'SssS_Plist')) {
			$oP = $mPlist;
			if ($sPathInvocationBase)
				$oP->getOrSet('PATH_DIR_DEFAULT_BASE_TEMPLATE', $sPathInvocationBase);

		} else { // is not an SssS_Plist or subclass
			// mPlist is plist string or path
			if (('string' == $sArgType) || ('array' == $sArgType)) {
				$oP = new SssS_Plist($mPlist);
				if ($sPathInvocationBase)
					$oP->getOrSet('PATH_DIR_DEFAULT_BASE_TEMPLATE', $sPathInvocationBase);

			} // if string (path or xml) or array as mPlist
			else {
				$oP = self::defaultPlist($iVersion);
				if ($sPathInvocationBase)
					$oP->set('PATH_DIR_DEFAULT_BASE_TEMPLATE', $sPathInvocationBase);

			} // if mPlist type handling
		} // if no useable mPlist

		if ($iVersion > $oP->getInt('SssSphocoaGeneratorVersion', 1)) {

			self::warnVersion();

			return self::makeNewSssSphocoaProjectWithPlist3($oP, $bUseAllDefaults, $sPathDest, $sPathInvocationBase, $sAppName);

		} // if old version plist

		// some default paths

		// what to copy... (will ask for confirmation)
		$sPathDefaultBaseTemplate = $oP->get('PATH_DIR_DEFAULT_BASE_TEMPLATE');

		// where to point frameworks to (~required fields)
		$sPathPhocoaFrameWork	= $oP->get('PATH_DIR_SRC_PHOCOA');
		$sPathSmartyFrameWork	= $oP->get('PATH_DIR_SRC_SMARTY');
		$sPathPropelFrameWork	= $oP->get('PATH_DIR_SRC_PROPEL');
		$sPathPropelGenerator	= $oP->get('PATH_BIN_PROPEL', 'propel');
		$sPathHordeFrameWork	= $oP->get('PATH_DIR_SRC_HORDE');
		$sPathSssSFrameWork		= $oP->get('PATH_DIR_SRC_SwissalpSphp');
        $sPathPearLog			= $oP->get('PATH_DIR_SRC_PEAR_LOG');
		$sPathPhing				= $oP->get('PATH_DIR_SRC_PHING');

		// default appName (will ask for confirmation)
		if (!$sAppName) $sAppName = $oP->get('PHOCOA_PROJECT_NAME', 'PhocoaApp');

		// where are we?
		$sPathCurentWorkingDirectory = getcwd();

		// welcome....
		echo NL .  NL . '     SssSphocoaGenerator::makeNewSssSphocoaProjectWithPlist()' . NL;

		$oRunner = self::sharedSssS_ShellScriptRunnerForDummies();

		$sPathOrig = ($sPathDefaultBaseTemplate)
			? SssS_CLI_Tools::$rts($sPathDefaultBaseTemplate, DIR_SEP) : ($sPathInvocationBase)
				? $sPathInvocationBase : '';

		$sPathDest = ($sPathDest)
				?  SssS_CLI_Tools::$ats($sPathDest, DIR_SEP) . $sAppName
				: $oP->getOrSet('PATH_DIR_PHOCOA_APP_CONTAINER', $sPathCurentWorkingDirectory . DIR_SEP . $sAppName);

		// some other default values
		// for propel -conf.xml (will ask for confirmation)
		$sDBtype = $oP->get('DB_TYPE', 'sqlite');
		$sDBfile = $oP->get('DB_SQLITE_FILE', $sPathDest . DIR_SEP . 'runtime' . DIR_SEP . 'db' . DIR_SEP . 'SssSphocoaAppBasic.sq3');
		$sDBhost = $oP->get('DB_HOST', 'localhost'); //phocoaDBhost';
		$sDBuser = $oP->get('DB_USER'); //DBuser';
		$sDBpass = $oP->get('DB_PASS'); //DBpass';
		$sDBname = $oP->get('DB_NAME'); //DBname'; not required for sqlite

		// for apache httpd.conf (will ask for confirmation)
		$sHTTPip	= $oP->get('HTTPD_IP', '127.0.0.1');
		$sHTTPHost	= $oP->get('HTTPD_NAME', 'localhost');
		$iHTTPPort	= $oP->get('HTTPD_PORT', 80);

		if ($bUseAllDefaults) {
			echo 'not interacting for values...' . NL;
			$sPathLog = $sPathDest . DIR_SEP . 'log';
			$sPathDestAppRoot = $sPathDest . DIR_SEP . $sAppName;
		} else {
			// get template path
			$bRepeat = true;
			while ($bRepeat) {

				// normally gets parts from sPathPhocoaFrameWork. we have our templates
				$sPathOrig = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
						'Path of template Base Structure (PATH_DIR_DEFAULT_BASE_TEMPLATE):',
												$sPathOrig), DIR_SEP);
				$oP->set('PATH_DIR_DEFAULT_BASE_TEMPLATE', $sPathOrig);

				// PHP is not letting me use paths with spaces...best not to have unsoported
				// chars as this will only make things worse down the line
				//if (!is_dir(escapeshellarg($sPathOrig))) {
				//	echo 'original template path not found: ' . escapeshellarg($sPathOrig);
				//	return false;
				//} // if no source
				if (!$oRunner->isFolder($sPathOrig)) {

					echo 'original template path not found: ' . $sPathOrig . NL;
					$sRetry = SssS_CLI_Tools::getChoiceCLI('Fix or exit?', array('fix' => 'Let me give another path', 'exit' => 'Abbort creation'), 'fix');
					if (in_array($sRetry, array('Abbort creation', 'exit'))) {

						return false;

					} // if abbort

				} else {

					$bRepeat = false;

				} // if no source;

			} // loop until valid path given


			// get app name
			// ##PHOCOA_PROJECT_NAME##
			$sAppName = SssS_CLI_Tools::$gi(
									NL . 'Name of new App (PHOCOA_PROJECT_NAME):', $sAppName);
			$oP->set('PHOCOA_PROJECT_NAME', $sAppName);

			// get target path
			$bRepeat = true;
			while ($bRepeat) {

				// ##PATH_DIR_PHOCOA_APP_CONTAINER##
				$sPathDest = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
												NL . 'Path of new App (PATH_DIR_PHOCOA_APP_CONTAINER):',
												substr($sPathDest, 0, strlen($sPathDest) - strlen(strrchr($sPathDest, DIR_SEP))) . DIR_SEP
												. $sAppName), DIR_SEP);
				$oP->set('PATH_DIR_PHOCOA_APP_CONTAINER', $sPathDest);

				echo NL;

				if ($oRunner->isSomethingAtPath($sPathDest)) {

					echo 'destination already exists: ' . $sPathDest . NL;
					$sRetry = SssS_CLI_Tools::getChoiceCLI('Fix or exit?',
					array('fix' => 'Let me give another path', 'exit' => 'Abbort creation'), 'fix');
					if (in_array($sRetry, array('Abbort creation', 'exit'))) return false;

				} else {

					$bRepeat = false;

				} // if destination already exists;

			} // loop until valid path given

			// set target app-root
			// APP_ROOT', '##PATH_DIR_PHOCOA_APP##
			$sPathDestAppRoot = $sPathDest . DIR_SEP . $sAppName;
			$oP->set('PATH_DIR_PHOCOA_APP', $sPathDestAppRoot);

			// get app type
			// for application delegate
			$sAppType = SssS_CLI_Tools::getChoiceCLI('Type of app (cli|web|both)',
					array('cli' => 'Command Line Tool', 'web' => 'WFWebApplication',
					'both' => 'both types'), 'both');
			//echo 'currently only supporting cli, so set to cli' . NL . NL;
			//$sAppType = 'cli';

			// httpd.conf takes also servername, ip and port
			if (!in_array($sAppType, array('Command Line Tool', 'cli'))) {

				$sHTTPip = SssS_CLI_Tools::$gi(NL . 'IP (HTTPD_IP):', $sHTTPip);
				$oP->set('HTTPD_IP', $sHTTPip);
				$iHTTPPort = intval(
						SssS_CLI_Tools::$gi(NL . 'port (HTTPD_PORT): ', $iHTTPPort));
				$oP->set('HTTPD_PORT', $iHTTPPort);
				$sHTTPHost = SssS_CLI_Tools::$gi(
										NL . 'hostname (HTTPD_NAME):', $sHTTPHost);
				$oP->set('HTTPD_NAME', $sHTTPHost);
				echo NL;

			} // if not cli

			// ask for log dir
			$sPathLog = $sPathDest . DIR_SEP . 'log';
			$sPathLog = SssS_CLI_Tools::$gi(NL . 'log path:', $sPathLog);

			// ask if uses propel
			$sUsePropel = SssS_CLI_Tools::getChoiceCLI('Use propel for db?',
					array('no' => 'NO', 'yes' => 'YES'), 'yes');
			if ('yes' == strtolower($sUsePropel)) {
				// get db props
				echo NL;
				// get propel path
				$sPathPropelFrameWork = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
					NL . 'path to propel', $sPathPropelFrameWork));
				echo NL;
				$sDBtype = SssS_CLI_Tools::getChoiceCLI('Choose database type (DB_TYPE):',
						array('pgsql' => 'Postgres', 'mysql' => 'MySQl',
							  'mssql' => 'mssql', 'sqlite' => 'sqlite',
							   'ldap' => 'ldap'), $sDBtype);
				$oP->set('DB_TYPE', $sDBtype);

				if ('sqlite' == $sDBtype) {
					$sDBfile = SssS_CLI_Tools::$gi(NL . 'absolute path to database file:', $sDBfile);
					$oP->set('DB_SQLITE_FILE', $sDBfile);
					$sDBname = 'default';
					$sDBuser = '';
					$sDBpass = '';
				} else {
					$sDBname = SssS_CLI_Tools::$gi(NL . 'DB name:', $sDBname);
					$sDBuser = SssS_CLI_Tools::$gi(NL . 'DB username:', $sDBuser);
					$sDBpass = SssS_CLI_Tools::$gi(NL . 'DB password:', base64_decode($sDBpass));
					$sDBhost = SssS_CLI_Tools::$gi(NL . 'DB hostname/ip:', $sDBhost);
					$oP->set('DB_HOST', $sDBhost);
				} // if sqlite or other type
				$oP->set('DB_NAME', $sDBname);
				$oP->set('DB_USER', $sDBuser);
				$oP->set('DB_PASS', base64_encode($sDBpass));
				$sPathPropelGenerator = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(NL . 'path to propel executable', $sPathPropelGenerator));
				$oP->set('PATH_BIN_PROPEL', $sPathPropelGenerator);

			} // if propel

			echo NL;

		} // if ask for anything

		// copy the bare-bone structure
		// make destination path
		$bRes = $oRunner->makePath($sPathDest);
		$bPHPcompatiblePathDest = true;
		if ((!is_dir($sPathDest)) && $bRes) {
			echo NL . NL . 'destination path is not PHP compatible, problems'
					. ' to be expected' . NL;
			$bPHPcompatiblePathDest = false;
		} // if php compatible path destination

		// make log dir
		$oRunner->makePath($sPathLog);
		// make runtime dir
		//$oRunner->makePath($sPathDest . DIR_SEP . 'runtime');
		// copy runtime dir
		$bRes = $oRunner->copyPreserving($sPathOrig . DIR_SEP . 'runtime',
								 		 $sPathDest . DIR_SEP . 'runtime');
		if (!$bRes) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			return false;
		} // if copy error

		// copy app dir -> Project dir and app dir MUST bear same name
		$sPathOrigAppRoot = $sPathOrig . DIR_SEP
									. substr(strrchr($sPathOrig, DIR_SEP), 1);
		$bRes = $oRunner->copyPreserving($sPathOrigAppRoot,	$sPathDestAppRoot);
		if (!$bRes) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			return false;
		} // if copy error
		// also here PHP fails
		//if (false === copy($sPathOrig, $sPathDest)) {
		//	echo 'error making copy';
		//	return false;
		//} // if copy fail

		// copy bin dir
		//if (!$oRunner->copyPreserving($sPathOrig . '/AppRoot/bin', $sPathDestAppRoot . '/bin')) { echo 'error with copy' . NL; var_dump($oRunner->aResults); return false; } // if copy error

		// copy replicator dir holding info and applescript after the plists
		$sPathGen = $sPathDest . DIR_SEP . 'SssSphocoaGen';
		if (!$oRunner->copyPreserving(
						$sPathOrig . DIR_SEP . 'SssSphocoaGen', $sPathGen)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error
		$sPathGen .= DIR_SEP;

		// copy runner.phps.SssStemplate -> sAppName
		$sPathRunner = $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP . $sAppName;
		$sPathRunnerOrig = $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP
													. 'runner.phps.SssStemplate';
		if (!$oRunner->copyPreserving($sPathRunnerOrig, $sPathRunner)) {//moveOrRename
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			return false;
		} // if rename error

		// make sure there isn't any old symlink or empty directory...
		$oRunner->doScript('rm -dfv "' . $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP . 'run"');
		// make a symlink for easy running
		$bRes = $oRunner->symlinkToAt($sPathRunner, $sPathDestAppRoot . DIR_SEP
													. 'bin' . DIR_SEP . 'run');
		if (!$bRes) {
			echo 'error with alias creation' . NL; var_dump($oRunner->aResults);
			return false;
		} // if symlink error

		// config files

		$sPathConf = $sPathDestAppRoot . DIR_SEP . 'conf' . DIR_SEP;

		// copy propel-conf.xml.SssStemplate -> sAppName-conf.xml
		//$sPathPropelConf = $sPathConf . $sAppName . '-conf.xml';
		//$sPathPropelConfOrig = $sPathConf . 'propel-conf.xml.SssStemplate';
		//if (!$oRunner->copyPreserving($sPathPropelConfOrig, $sPathPropelConf)) {
		//	echo 'error with copy' . NL; var_dump($oRunner->aResults);
		//	return false;
		//} // if rename error

		// .htaccess.SssStemplate
		$sPathHTAconf = $sPathConf . '.htaccess';
		$sPathHTAconfOrig = $sPathConf . '.htaccess.SssStemplate';
		if (!$oRunner->copyPreserving($sPathHTAconfOrig, $sPathHTAconf)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// httpd.conf.SssStemplate
		$sPathHTTPdConf = $sPathConf . 'httpd.conf';
		$sPathHTTPdConfOrig = $sPathConf . 'httpd.conf.SssStemplate';
		if (!$oRunner->copyPreserving($sPathHTTPdConfOrig, $sPathHTTPdConf)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// copy SssSphocoaApp.conf.SssStemplate
		$sPathWebAppConf = $sPathConf . 'SssSphocoaApp.conf';
		$sPathWebAppConfSyml = $sPathConf . 'webapp.conf';
		$sPathWebAppConfOrig = $sPathConf . 'SssSphocoaApp.conf.SssStemplate';
		if (!$oRunner->copyPreserving($sPathWebAppConfOrig, $sPathWebAppConf)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// first remove old symlink
		$oRunner->doScript('rm -dfv "' . $sPathWebAppConfSyml . '";');
		// make symlink to SssSphocoaApp.conf for compatability with webapp.conf
		$oRunner->symlinkToAt($sPathWebAppConf, $sPathWebAppConfSyml);

		// copy AppleScripts php shell runner (on apple dir_sep is /)
		$sPathASres = $sPathGen . 'newSssSphocoaProjectFromPlist.app/Contents/Resources/';
		$sPathASnew = $sPathASres . 'newSssSphocoaProject';
		$sPathASnewOrig = $sPathASres . 'newSssSphocoaProject.SssStemplate';
		if (!$oRunner->copyPreserving($sPathASnewOrig, $sPathASnew)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// first remove old symlink
		$oRunner->doScript('rm -dfv "' . $sPathGen . 'newSssSphocoaProject";');
		// make symlink to newSssSphocoaProject in SssSphocoaGen
		$oRunner->symlinkToAt($sPathASnew, $sPathGen . 'newSssSphocoaProject');

		// copy inPlace AppleScripts php shell runner (on apple dir_sep is /)
		$sPathASres = $sPathGen . 'remakeTemplatedFilesWithInfoFromPlist.app/Contents/Resources/';
		$sPathASipNew = $sPathASres . 'remakeTemplatedFilesWithInfoFromPlist';
		$sPathASipNewOrig = $sPathASres . 'remakeTemplatedFilesWithInfoFromPlist.SssStemplate';
		if (!$oRunner->copyPreserving($sPathASipNewOrig, $sPathASipNew)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// first remove old symlink
		$oRunner->doScript('rm -dfv "' . $sPathGen . 'remakeTemplatedFilesWithInfoFromPlist";');
		// make symlink to newSssSphocoaProject in SssSphocoaGen
		$oRunner->symlinkToAt($sPathASipNew, $sPathGen . 'remakeTemplatedFilesWithInfoFromPlist');


		// let's save a plist for remake and one for useAsTemplate
		$oP->set('LOG_DIR', $sPathLog);
		$oP->saveTo($sPathGen . 'useTemplateThatMadeMe.plist');

		$oP->set('PATH_DIR_DEFAULT_BASE_TEMPLATE', $sPathDest);
		$oP->set('PATH_DIR_PHOCOA_APP_CONTAINER', $sPathDest);
		$oP->saveTo($sPathGen . 'useMeToRebuildInPlace.plist');

		// and so we can reuse another method
		$mRes = self::inPlaceRefreshTemplatesInPlist($oP, null, $sPathDest);

		// and one more with changed name
		$oP->set('PHOCOA_PROJECT_NAME', $oP->get('PHOCOA_PROJECT_NAME') . 'Copy');
		$oP->saveTo($sPathGen . 'useMeAsTemplate.plist');

		return $mRes;

	} // makeNewSssSphocoaProjectWithPlist

	// make from settings in a plist file interactively changing or complementing arguments
	public static function makeNewSssSphocoaProjectWithPlist1(
			$mPlist = null, // SssS_Plist object a path or a plist xml string or an array
			$bUseAllDefaults = false,
			$sPathDest = false,
			$sPathInvocationBase = null, // alternative root calculated from invocation path
			$sAppName = false) {

		static $ats = null; static $gi = null;
		static $rts = null; if (!$rts) {

			$rts = 'removeTrailingSlash';
			$ats = 'addTrailingSlash';
			$gi = 'getInputCLI';

		} // first run

		if ($sPathInvocationBase)
			$sPathInvocationBase = SssS_CLI_Tools::$rts($sPathInvocationBase, DIR_SEP);

		$sArgType = gettype($mPlist);
		if (('object' == $sArgType) && is_subclass_of($mPlist, 'SssS_Plist')) {
			$oP = $mPlist;
			if ($sPathInvocationBase)
				$oP->getOrSet('sPathDefaultBaseTemplate', $sPathInvocationBase);

		} else { // is not an SssS_Plist or subclass
			// mPlist is plist string or path
			if (('string' == $sArgType) || ('array' == $sArgType)) {
				$oP = new SssS_Plist($mPlist);
				if ($sPathInvocationBase)
					$oP->getOrSet('sPathDefaultBaseTemplate', $sPathInvocationBase);

			} // if string (path or xml) or array as mPlist
			else {
				$oP = self::defaultPlist();
				if ($sPathInvocationBase)
					$oP->set('sPathDefaultBaseTemplate', $sPathInvocationBase);

			} // if mPlist type handling
		} // if no useable mPlist

		// some default paths

		// what to copy... (will ask for confirmation)
		$sPathDefaultBaseTemplate = $oP->get('sPathDefaultBaseTemplate');

		// where to point frameworks to (~required fields)
		$sPathPhocoaFrameWork	= $oP->get('sPathPhocoaFrameWork');
		$sPathSmartyFrameWork	= $oP->get('sPathSmartyFrameWork');
		$sPathPropelFrameWork	= $oP->get('sPathPropelFrameWork');
		$sPathPropelGenerator	= $oP->get('sPathPropelGenerator', 'propel-gen');
		$sPathHordeFrameWork	= $oP->get('sPathHordeFrameWork');
		$sPathSssSFrameWork		= $oP->get('sPathSssSFrameWork');
        $sPathPearLog			= $oP->get('sPathPearLog');
		$sPathPhing				= $oP->get('sPathPhing');

		// some other default values
		// for propel -conf.xml (will ask for confirmation)
		$sDBtype = $oP->get('sDBtype', 'mysql');
		$sDBhost = $oP->get('sDBhost', 'localhost'); //phocoaDBhost';
		$sDBuser = $oP->get('sDBuser'); //DBuser';
		$sDBpass = $oP->get('sDBpass'); //DBpass';
		$sDBname = $oP->get('sDBname'); //DBname';

		// for apache httpd.conf (will ask for confirmation)
		$sHTTPip	= $oP->get('sHTTPip', '127.0.0.1');
		$sHTTPHost	= $oP->get('sHTTPHost', 'localhost');
		$iHTTPPort	= $oP->get('iHTTPPort', 80);

		// default appName (will ask for confirmation)
		if (!$sAppName) $sAppName = $oP->get('sAppName', 'PhocoaApp');

		// where are we?
		$sPathCurentWorkingDirectory = getcwd();

		// welcome....
		echo NL .  NL . '     SssSphocoaGenerator::makeNewSssSphocoaProjectWithPlist1()' . NL;

		$oRunner = self::sharedSssS_ShellScriptRunnerForDummies();

		$sPathOrig = ($sPathDefaultBaseTemplate)
			? SssS_CLI_Tools::$rts($sPathDefaultBaseTemplate, DIR_SEP) : ($sPathInvocationBase)
				? $sPathInvocationBase : '';

		$sPathDest = ($sPathDest)
				?  SssS_CLI_Tools::$ats($sPathDest, DIR_SEP) . $sAppName
				: $oP->getOrSet('sPathDest', $sPathCurentWorkingDirectory . DIR_SEP . $sAppName);

	if ($bUseAllDefaults) {
		echo 'not interacting for values...' . NL;
		$sPathLog = $sPathDest . DIR_SEP . 'log';
		$sPathDestAppRoot = $sPathDest . DIR_SEP . $sAppName;
	} else {
	// get template path
		// normally gets parts from sPathPhocoaFrameWork. we have our templates
		$sPathOrig = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
				'Path of template Base Structure (sPathDefaultBaseTemplate):',
										$sPathOrig), DIR_SEP);
		$oP->set('sPathDefaultBaseTemplate', $sPathOrig);

		// PHP is not letting me use paths with spaces...best not to have unsoported
		// chars as this will only make things worse down the line
		//if (!is_dir(escapeshellarg($sPathOrig))) {
		//	echo 'original template path not found: ' . escapeshellarg($sPathOrig);
		//	return false;
		//} // if no source
		if (!$oRunner->isFolder($sPathOrig)) {
			echo 'original template path not found: ' . $sPathOrig . NL;
			return false;
		} // if no source;

	// get app name
		// ##PHOCOA_PROJECT_NAME##
		$sAppName = SssS_CLI_Tools::$gi(
								NL . 'Name of new App (sAppName):', $sAppName);
		$oP->set('sAppName', $sAppName);

	// get target path
		// ##PHOCOA_APP_CONTAINER_DIR##
		$sPathDest = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
										NL . 'Path of new App (sPathDest):',
										substr($sPathDest, 0, strlen($sPathDest) - strlen(strrchr($sPathDest, DIR_SEP))) . DIR_SEP
										. $sAppName), DIR_SEP);
		$oP->set('sPathDest', $sPathDest);

		echo NL;

		if ($oRunner->isSomethingAtPath($sPathDest)) {
			echo 'destination already exists: ' . $sPathDest . NL;
			return false;
		} // if destination already exists;

	// set target app-root
		// APP_ROOT', '##PHOCOA_APP_DIR##
		$sPathDestAppRoot = $sPathDest . DIR_SEP . $sAppName;

	// get app type
		// for application delegate
		$sAppType = SssS_CLI_Tools::getChoiceCLI('Type of app (cli|web|both)',
				array('cli' => 'Command Line Tool', 'web' => 'WFWebApplication',
				'both' => 'both types'), 'both types');
		//echo 'currently only supporting cli, so set to cli' . NL . NL;
		//$sAppType = 'cli';

	// httpd.conf takes also servername, ip and port
		if ('Command Line Tool' != $sAppType) {
			$sHTTPip = SssS_CLI_Tools::$gi(NL . 'IP (sHTTPip):', $sHTTPip);
			$oP->set('sHTTPip', $sHTTPip);
			$iHTTPPort = intval(
					SssS_CLI_Tools::$gi(NL . 'port (iHTTPPort): ', $iHTTPPort));
			$oP->set('iHTTPPort', $iHTTPPort);
			$sHTTPHost = SssS_CLI_Tools::$gi(
									NL . 'hostname (sHTTPHost):', $sHTTPHost);
			$oP->set('sHTTPHost', $sHTTPHost);
			echo NL;
		} // if not cli

	// ask if uses propel
		$sPathLog = $sPathDest . DIR_SEP . 'log';
		$sUsePropel = SssS_CLI_Tools::getChoiceCLI('Use propel for db?',
				array('no' => 'NO', 'yes' => 'YES'), 'yes');
		if ('yes' == strtolower($sUsePropel)) {
			// get db props
			$sPathLog = SssS_CLI_Tools::$gi(
											NL . 'log path:', $sPathLog);
			$oP->set('sPathLog', $sPathLog);
			echo NL;
		// get propel path
			$sPathPropelFrameWork = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
				NL . 'path to propel', $sPathPropelFrameWork));
			echo NL;
			$sDBtype = SssS_CLI_Tools::getChoiceCLI('Choose database type:',
					array('pgsql' => 'Postgres', 'mysql' => 'MySQl',
						  'mssql' => 'mssql', 'sqllite' => 'sqllite',
						   'ldap' => 'ldap'), $sDBtype);
			$oP->set('sDBtype', $sDBtype);
			$sDBname = SssS_CLI_Tools::$gi(NL . 'DB name:', $sDBname);
			$oP->set('sDBname', $sDBname);
			$sDBuser = SssS_CLI_Tools::$gi(NL . 'DB username:', $sDBuser);
			$oP->set('sDBuser', $sDBuser);
			$sDBpass = SssS_CLI_Tools::$gi(NL . 'DB password:', $sDBpass);
			$oP->set('sDBpass', $sDBpass);
			$sDBhost = SssS_CLI_Tools::$gi(NL . 'DB hostname:', $sDBhost);
			$oP->set('sDBhost', $sDBhost);
			$sPathPropelGenerator = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(NL . 'path to propel-gen binary', $sPathPropelGenerator));
			$oP->set('sPathPropelGenerator', $sPathPropelGenerator);

		} // if propel

		echo NL;

	} // if ask for anything

	// copy the bare-bone structure
		// make destination path
		$bRes = $oRunner->makePath($sPathDest);
		$bPHPcompatiblePathDest = true;
		if ((!is_dir($sPathDest)) && $bRes) {
			echo NL . NL . 'destination path is not PHP compatible, problems'
					. ' to be expected' . NL;
			$bPHPcompatiblePathDest = false;
		} // if php compatible path destination

		// make log dir
		$oRunner->makePath($sPathLog);
		// make runtime dir
		//$oRunner->makePath($sPathDest . DIR_SEP . 'runtime');
		// copy runtime dir
		$bRes = $oRunner->copyPreserving($sPathOrig . DIR_SEP . 'runtime',
								 		 $sPathDest . DIR_SEP . 'runtime');
		if (!$bRes) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			return false;
		} // if copy error

		// copy app dir -> Project dir and app dir MUST bear same name
		$sPathOrigAppRoot = $sPathOrig . DIR_SEP
									. substr(strrchr($sPathOrig, DIR_SEP), 1);
		$bRes = $oRunner->copyPreserving($sPathOrigAppRoot,	$sPathDestAppRoot);
		if (!$bRes) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			return false;
		} // if copy error
		// also here PHP fails
		//if (false === copy($sPathOrig, $sPathDest)) {
		//	echo 'error making copy';
		//	return false;
		//} // if copy fail

		// copy bin dir
		//if (!$oRunner->copyPreserving($sPathOrig . '/AppRoot/bin', $sPathDestAppRoot . '/bin')) { echo 'error with copy' . NL; var_dump($oRunner->aResults); return false; } // if copy error

		// copy replicator dir holding info and applescript after the plists
		$sPathGen = $sPathDest . DIR_SEP . 'SssSphocoaGen';
		if (!$oRunner->copyPreserving(
						$sPathOrig . DIR_SEP . 'SssSphocoaGen', $sPathGen)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error
		$sPathGen .= DIR_SEP;

		// copy runner.phps.SssStemplate -> sAppName
		$sPathRunner = $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP . $sAppName;
		$sPathRunnerOrig = $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP
													. 'runner.phps.SssStemplate';
		if (!$oRunner->copyPreserving($sPathRunnerOrig, $sPathRunner)) {//moveOrRename
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			return false;
		} // if rename error

		// make sure there isn't any old symlink or empty directory...
		$oRunner->doScript('rm -dfv "' . $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP . 'run"');
		// make a symlink for easy running
		$bRes = $oRunner->symlinkToAt($sPathRunner, $sPathDestAppRoot . DIR_SEP
													. 'bin' . DIR_SEP . 'run');
		if (!$bRes) {
			echo 'error with alias creation' . NL; var_dump($oRunner->aResults);
			return false;
		} // if symlink error

	// config files

		$sPathConf = $sPathDestAppRoot . DIR_SEP . 'conf' . DIR_SEP;

		// copy propel-conf.xml.SssStemplate -> sAppName-conf.xml
		//$sPathPropelConf = $sPathConf . $sAppName . '-conf.xml';
		//$sPathPropelConfOrig = $sPathConf . 'propel-conf.xml.SssStemplate';
		//if (!$oRunner->copyPreserving($sPathPropelConfOrig, $sPathPropelConf)) {
		//	echo 'error with copy' . NL; var_dump($oRunner->aResults);
		//	return false;
		//} // if rename error

		// .htaccess.SssStemplate
		$sPathHTAconf = $sPathConf . '.htaccess';
		$sPathHTAconfOrig = $sPathConf . '.htaccess.SssStemplate';
		if (!$oRunner->copyPreserving($sPathHTAconfOrig, $sPathHTAconf)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if rename error

		// httpd.conf.SssStemplate
		$sPathHTTPdConf = $sPathConf . 'httpd.conf';
		$sPathHTTPdConfOrig = $sPathConf . 'httpd.conf.SssStemplate';
		if (!$oRunner->copyPreserving($sPathHTTPdConfOrig, $sPathHTTPdConf)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if rename error

		// copy SssSphocoaApp.conf.SssStemplate
		$sPathWebAppConf = $sPathConf . 'SssSphocoaApp.conf';
		$sPathWebAppConfSyml = $sPathConf . 'webapp.conf';
		$sPathWebAppConfOrig = $sPathConf . 'SssSphocoaApp.conf.SssStemplate';
		if (!$oRunner->copyPreserving($sPathWebAppConfOrig, $sPathWebAppConf)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if rename error

		// first remove old symlink
		$oRunner->doScript('rm -dfv "' . $sPathWebAppConfSyml . '";');
		// make symlink to SssSphocoaApp.conf for compatability with webapp.conf
		$oRunner->symlinkToAt($sPathWebAppConf, $sPathWebAppConfSyml);

		// copy AppleScripts php shell runner (on apple dir_sep is /)
		$sPathASres = $sPathGen . 'newSssSphocoaProjectFromPlist.app/Contents/Resources/';
		$sPathASnew = $sPathASres . 'newSssSphocoaProject';
		$sPathASnewOrig = $sPathASres . 'newSssSphocoaProject.SssStemplate';
		if (!$oRunner->copyPreserving($sPathASnewOrig, $sPathASnew)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if rename error

		// first remove old symlink
		$oRunner->doScript('rm -dfv "' . $sPathGen . 'newSssSphocoaProject";');
		// make symlink to newSssSphocoaProject in SssSphocoaGen
		$oRunner->symlinkToAt($sPathASnew, $sPathGen . 'newSssSphocoaProject');

		// copy inPlace AppleScripts php shell runner (on apple dir_sep is /)
		$sPathASres = $sPathGen . 'remakeTemplatedFilesWithInfoFromPlist.app/Contents/Resources/';
		$sPathASipNew = $sPathASres . 'remakeTemplatedFilesWithInfoFromPlist';
		$sPathASipNewOrig = $sPathASres . 'remakeTemplatedFilesWithInfoFromPlist.SssStemplate';
		if (!$oRunner->copyPreserving($sPathASipNewOrig, $sPathASipNew)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if rename error

		// first remove old symlink
		$oRunner->doScript('rm -dfv "' . $sPathGen . 'remakeTemplatedFilesWithInfoFromPlist";');
		// make symlink to newSssSphocoaProject in SssSphocoaGen
		$oRunner->symlinkToAt($sPathASipNew, $sPathGen . 'remakeTemplatedFilesWithInfoFromPlist');

		$aSearches = array('##PHOCOA_BASE_DIR##', '##PHOCOA_PROJECT_NAME##',
				'##PHOCOA_APP_CONTAINER_DIR##', '##PHOCOA_APP_DIR##',
				'##SwissalpS_FRAMEWORK_DIR##', '##PEAR_LOG_FRAMEWORK_DIR##',
				'##PROPEL_FRAMEWORK_DIR##', '##HORDE_FRAMEWORK_DIR##',
				'##PHING_PATH##', '##DATE_STAMP##',
				'##SERVER_IP##', '##SERVER_PORT##', '##SERVER_NAME##',
				'##LOG_DIR##', '##DB_NAME##', '##PROPEL_DATABASE##',
				'##DB_USER##', '##DB_HOST##', '##DB_PASS##',
				'##SMARTY_DIR##', '##DIR_SEP##', '##PROPEL_GEN_BIN##', '##PHP_BIN##',
				'/*##RUN_BLOCKER##', '##RUN_BLOCKER##*/'); // last 2 need no replacement

		$aReplacements = array($sPathPhocoaFrameWork, $sAppName,
				$sPathDest, $sPathDestAppRoot,
				$sPathSssSFrameWork, $sPathPearLog,
				$sPathPropelFrameWork, $sPathHordeFrameWork,
				$sPathPhing, gmdate('Ymd_His'),
				$sHTTPip, $iHTTPPort, $sHTTPHost,
				$sPathLog, $sDBname, $sDBtype,
				$sDBuser, $sDBhost, $sDBpass,
				$sPathSmartyFrameWork, DIR_SEP, $sPathPropelGenerator,
				SssS_ShellScriptRunner::getPHPinterpreterPath()); // delete last 2

		// setup config file
		// shell script runner
		// .htaccess needs only ##PHOCOA_APP_DIR##
		// httpd.conf setup if needed or not
		// propel-conf.xml ##LOG_DIR## ##DB_NAME## ##PROPEL_DATABASE## ##DB_USER## ##DB_HOST## ##DB_PASS##
		// AppleScript droplet
		foreach (self::findTemplatesInProject($sPathDest)  as $sPath) {//array($sPathWebAppConf, //webapp.conf',
						//$sPathRunner, $sPathPropelConf,
						//$sPathHTAconf, $sPathHTTPdConf,
						//$sPathASnew, $sPathASipNew)
			//file_put_contents_safely($sPath,
			//		str_replace($aSearches, $aReplacements,
			//		file_get_contents_safely($sPath)), 'wb');
			$sPath = trim($sPath);
			if (empty($sPath)) continue;
			$sNameTemplate = basename($sPath);
			$sName = basename($sPath, '.SssStemplate');

			// exceptions
			if ('runner.phps' == $sName) $sName = $sAppName;
			//if ('propel-conf.xml' == $sName) $sName = $sAppName . '-conf.xml';

			$sPath = dirname($sPath) . DIR_SEP;
			file_put_contents_safely($sPath . $sName,
					str_replace($aSearches, $aReplacements,
					file_get_contents_safely($sPath . $sNameTemplate)), 'wb');

		} // foreach path replace all tokens in these files

		echo '--done, thanks & have fun--';


		// let's save a plist for remake and one for useAsTemplate
		$oP->set('sPathLog', null);
		$oP->saveTo($sPathGen . 'useTemplateThatMadeMe.plist');

		$oP->set('sPathDefaultBaseTemplate', $sPathDest);
		$oP->set('sPathDest', null);
		$oP->set('sAppName', $oP->get('sAppName') . 'Copy');

		$oP->saveTo($sPathGen . 'useMeAsTemplate.plist');

		return true;
	} // makeNewSssSphocoaProjectWithPlist1


	// make from settings in a plist file interactively changing or complementing arguments
	public static function makeNewSssSphocoaProjectWithPlist2(
			$mPlist = null, // SssS_Plist object a path or a plist xml string or an array
			$bUseAllDefaults = false,
			$sPathDest = false,
			$sPathInvocationBase = null, // alternative root calculated from invocation path
			$sAppName = false) {

		static $ats = null; static $gi = null; static $iVersion = null;
		static $rts = null; if (!$rts) {

			$rts = 'removeTrailingSlash';
			$ats = 'addTrailingSlash';
			$gi = 'getInputCLI';
			$iVersion = 2;

		} // first run

		if ($sPathInvocationBase)
			$sPathInvocationBase = SssS_CLI_Tools::$rts($sPathInvocationBase, DIR_SEP);

		$sArgType = gettype($mPlist);
		if (('object' == $sArgType) && is_subclass_of($mPlist, 'SssS_Plist')) {
			$oP = $mPlist;
			if ($sPathInvocationBase)
				$oP->getOrSet('DEFAULT_BASE_TEMPLATE_DIR', $sPathInvocationBase);

		} else { // is not an SssS_Plist or subclass
			// mPlist is plist string or path
			if (('string' == $sArgType) || ('array' == $sArgType)) {
				$oP = new SssS_Plist($mPlist);
				if ($sPathInvocationBase)
					$oP->getOrSet('DEFAULT_BASE_TEMPLATE_DIR', $sPathInvocationBase);

			} // if string (path or xml) or array as mPlist
			else {
				$oP = self::defaultPlist($iVersion);
				if ($sPathInvocationBase)
					$oP->set('DEFAULT_BASE_TEMPLATE_DIR', $sPathInvocationBase);

			} // if mPlist type handling
		} // if no useable mPlist

		if ($iVersion > $oP->getInt('SssSphocoaGeneratorVersion', 1)) {

			self::warnVersion();

			return self::makeNewSssSphocoaProjectWithPlist1($oP, $bUseAllDefaults, $sPathDest, $sPathInvocationBase, $sAppName);

		} // if old version plist

		// some default paths

		// what to copy... (will ask for confirmation)
		$sPathDefaultBaseTemplate = $oP->get('DEFAULT_BASE_TEMPLATE_DIR');

		// where to point frameworks to (~required fields)
		$sPathPhocoaFrameWork	= $oP->get('FRAMEWORK_DIR');
		$sPathSmartyFrameWork	= $oP->get('SMARTY_DIR');
		$sPathPropelFrameWork	= $oP->get('PROPEL_FRAMEWORK_DIR');
		$sPathPropelGenerator	= $oP->get('PROPEL_GEN_BIN', 'propel-gen');
		$sPathHordeFrameWork	= $oP->get('HORDE_FRAMEWORK_DIR');
		$sPathSssSFrameWork		= $oP->get('SwissalpS_FRAMEWORK_DIR');
        $sPathPearLog			= $oP->get('PEAR_LOG_FRAMEWORK_DIR');
		$sPathPhing				= $oP->get('PHING_PATH');

		// some other default values
		// for propel -conf.xml (will ask for confirmation)
		$sDBtype = $oP->get('PROPEL_DATABASE', 'mysql');
		$sDBhost = $oP->get('DB_HOST', 'localhost'); //phocoaDBhost';
		$sDBuser = $oP->get('DB_USER'); //DBuser';
		$sDBpass = $oP->get('DB_PASS'); //DBpass';
		$sDBname = $oP->get('DB_NAME'); //DBname';

		// for apache httpd.conf (will ask for confirmation)
		$sHTTPip	= $oP->get('SERVER_IP', '127.0.0.1');
		$sHTTPHost	= $oP->get('SERVER_NAME', 'localhost');
		$iHTTPPort	= $oP->get('SERVER_PORT', 80);

		// default appName (will ask for confirmation)
		if (!$sAppName) $sAppName = $oP->get('PHOCOA_PROJECT_NAME', 'PhocoaApp');

		// where are we?
		$sPathCurentWorkingDirectory = getcwd();

		// welcome....
		echo NL .  NL . '     SssSphocoaGenerator::makeNewSssSphocoaProjectWithPlist()' . NL;

		$oRunner = self::sharedSssS_ShellScriptRunnerForDummies();

		$sPathOrig = ($sPathDefaultBaseTemplate)
			? SssS_CLI_Tools::$rts($sPathDefaultBaseTemplate, DIR_SEP) : ($sPathInvocationBase)
				? $sPathInvocationBase : '';

		$sPathDest = ($sPathDest)
				?  SssS_CLI_Tools::$ats($sPathDest, DIR_SEP) . $sAppName
				: $oP->getOrSet('PHOCOA_APP_CONTAINER_DIR', $sPathCurentWorkingDirectory . DIR_SEP . $sAppName);

	if ($bUseAllDefaults) {
		echo 'not interacting for values...' . NL;
		$sPathLog = $sPathDest . DIR_SEP . 'log';
		$sPathDestAppRoot = $sPathDest . DIR_SEP . $sAppName;
	} else {
	// get template path
		$bRepeat = true;
		while ($bRepeat) {

			// normally gets parts from sPathPhocoaFrameWork. we have our templates
			$sPathOrig = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
					'Path of template Base Structure (sPathDefaultBaseTemplate):',
											$sPathOrig), DIR_SEP);
			$oP->set('sPathDefaultBaseTemplate', $sPathOrig);

			// PHP is not letting me use paths with spaces...best not to have unsoported
			// chars as this will only make things worse down the line
			//if (!is_dir(escapeshellarg($sPathOrig))) {
			//	echo 'original template path not found: ' . escapeshellarg($sPathOrig);
			//	return false;
			//} // if no source
			if (!$oRunner->isFolder($sPathOrig)) {

				echo 'original template path not found: ' . $sPathOrig . NL;
				$sRetry = SssS_CLI_Tools::getChoiceCLI('Fix or exit?',
				array('fix' => 'Let me give another path', 'exit' => 'Abbort creation'), 'fix');
				if (in_array($sRetry, array('Abbort creation', 'exit'))) return false;

			} else {

				$bRepeat = false;

			} // if no source;

		} // loop until valid path given


	// get app name
		// ##PHOCOA_PROJECT_NAME##
		$sAppName = SssS_CLI_Tools::$gi(
								NL . 'Name of new App (sAppName):', $sAppName);
		$oP->set('sAppName', $sAppName);

	// get target path
		$bRepeat = true;
		while ($bRepeat) {

			// ##PHOCOA_APP_CONTAINER_DIR##
			$sPathDest = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
											NL . 'Path of new App (PHOCOA_APP_CONTAINER_DIR):',
											substr($sPathDest, 0, strlen($sPathDest) - strlen(strrchr($sPathDest, DIR_SEP))) . DIR_SEP
											. $sAppName), DIR_SEP);
			$oP->set('PHOCOA_APP_CONTAINER_DIR', $sPathDest);

			echo NL;

			if ($oRunner->isSomethingAtPath($sPathDest)) {

				echo 'destination already exists: ' . $sPathDest . NL;
				$sRetry = SssS_CLI_Tools::getChoiceCLI('Fix or exit?',
				array('fix' => 'Let me give another path', 'exit' => 'Abbort creation'), 'fix');
				if (in_array($sRetry, array('Abbort creation', 'exit'))) return false;

			} else {

				$bRepeat = false;

			} // if destination already exists;

		} // loop until valid path given

	// set target app-root
		// APP_ROOT', '##PHOCOA_APP_DIR##
		$sPathDestAppRoot = $sPathDest . DIR_SEP . $sAppName;
		$oP->set('PHOCOA_APP_DIR', $sPathDestAppRoot);

	// get app type
		// for application delegate
		$sAppType = SssS_CLI_Tools::getChoiceCLI('Type of app (cli|web|both)',
				array('cli' => 'Command Line Tool', 'web' => 'WFWebApplication',
				'both' => 'both types'), 'both');
		//echo 'currently only supporting cli, so set to cli' . NL . NL;
		//$sAppType = 'cli';

	// httpd.conf takes also servername, ip and port
		if (!in_array($sAppType, array('Command Line Tool', 'cli'))) {

			$sHTTPip = SssS_CLI_Tools::$gi(NL . 'IP (SERVER_IP):', $sHTTPip);
			$oP->set('SERVER_IP', $sHTTPip);
			$iHTTPPort = intval(
					SssS_CLI_Tools::$gi(NL . 'port (SERVER_PORT): ', $iHTTPPort));
			$oP->set('SERVER_PORT', $iHTTPPort);
			$sHTTPHost = SssS_CLI_Tools::$gi(
									NL . 'hostname (SERVER_NAME):', $sHTTPHost);
			$oP->set('SERVER_NAME', $sHTTPHost);
			echo NL;

		} // if not cli

	// ask for log dir
		$sPathLog = $sPathDest . DIR_SEP . 'log';
		$sPathLog = SssS_CLI_Tools::$gi(NL . 'log path:', $sPathLog);

	// ask if uses propel
		$sUsePropel = SssS_CLI_Tools::getChoiceCLI('Use propel for db?',
				array('no' => 'NO', 'yes' => 'YES'), 'yes');
		if ('yes' == strtolower($sUsePropel)) {
			// get db props
			echo NL;
		// get propel path
			$sPathPropelFrameWork = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
				NL . 'path to propel', $sPathPropelFrameWork));
			echo NL;
			$sDBtype = SssS_CLI_Tools::getChoiceCLI('Choose database type (PROPEL_DATABASE):',
					array('pgsql' => 'Postgres', 'mysql' => 'MySQl',
						  'mssql' => 'mssql', 'sqllite' => 'sqllite',
						   'ldap' => 'ldap'), $sDBtype);
			$oP->set('PROPEL_DATABASE', $sDBtype);
			$sDBname = SssS_CLI_Tools::$gi(NL . 'DB name:', $sDBname);
			$oP->set('DB_NAME', $sDBname);
			$sDBuser = SssS_CLI_Tools::$gi(NL . 'DB username:', $sDBuser);
			$oP->set('DB_USER', $sDBuser);
			$sDBpass = SssS_CLI_Tools::$gi(NL . 'DB password:', $sDBpass);
			$oP->set('DB_PASS', $sDBpass);
			$sDBhost = SssS_CLI_Tools::$gi(NL . 'DB hostname:', $sDBhost);
			$oP->set('DB_HOST', $sDBhost);
			$sPathPropelGenerator = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(NL . 'path to propel-gen binary', $sPathPropelGenerator));
			$oP->set('PROPEL_GEN_BIN', $sPathPropelGenerator);

		} // if propel

		echo NL;

	} // if ask for anything

	// copy the bare-bone structure
		// make destination path
		$bRes = $oRunner->makePath($sPathDest);
		$bPHPcompatiblePathDest = true;
		if ((!is_dir($sPathDest)) && $bRes) {
			echo NL . NL . 'destination path is not PHP compatible, problems'
					. ' to be expected' . NL;
			$bPHPcompatiblePathDest = false;
		} // if php compatible path destination

		// make log dir
		$oRunner->makePath($sPathLog);
		// make runtime dir
		//$oRunner->makePath($sPathDest . DIR_SEP . 'runtime');
		// copy runtime dir
		$bRes = $oRunner->copyPreserving($sPathOrig . DIR_SEP . 'runtime',
								 		 $sPathDest . DIR_SEP . 'runtime');
		if (!$bRes) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			return false;
		} // if copy error

		// copy app dir -> Project dir and app dir MUST bear same name
		$sPathOrigAppRoot = $sPathOrig . DIR_SEP
									. substr(strrchr($sPathOrig, DIR_SEP), 1);
		$bRes = $oRunner->copyPreserving($sPathOrigAppRoot,	$sPathDestAppRoot);
		if (!$bRes) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			return false;
		} // if copy error
		// also here PHP fails
		//if (false === copy($sPathOrig, $sPathDest)) {
		//	echo 'error making copy';
		//	return false;
		//} // if copy fail

		// copy bin dir
		//if (!$oRunner->copyPreserving($sPathOrig . '/AppRoot/bin', $sPathDestAppRoot . '/bin')) { echo 'error with copy' . NL; var_dump($oRunner->aResults); return false; } // if copy error

		// copy replicator dir holding info and applescript after the plists
		$sPathGen = $sPathDest . DIR_SEP . 'SssSphocoaGen';
		if (!$oRunner->copyPreserving(
						$sPathOrig . DIR_SEP . 'SssSphocoaGen', $sPathGen)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error
		$sPathGen .= DIR_SEP;

		// copy runner.phps.SssStemplate -> sAppName
		$sPathRunner = $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP . $sAppName;
		$sPathRunnerOrig = $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP
													. 'runner.phps.SssStemplate';
		if (!$oRunner->copyPreserving($sPathRunnerOrig, $sPathRunner)) {//moveOrRename
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			return false;
		} // if rename error

		// make sure there isn't any old symlink or empty directory...
		$oRunner->doScript('rm -dfv "' . $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP . 'run"');
		// make a symlink for easy running
		$bRes = $oRunner->symlinkToAt($sPathRunner, $sPathDestAppRoot . DIR_SEP
													. 'bin' . DIR_SEP . 'run');
		if (!$bRes) {
			echo 'error with alias creation' . NL; var_dump($oRunner->aResults);
			return false;
		} // if symlink error

	// config files

		$sPathConf = $sPathDestAppRoot . DIR_SEP . 'conf' . DIR_SEP;

		// copy propel-conf.xml.SssStemplate -> sAppName-conf.xml
		//$sPathPropelConf = $sPathConf . $sAppName . '-conf.xml';
		//$sPathPropelConfOrig = $sPathConf . 'propel-conf.xml.SssStemplate';
		//if (!$oRunner->copyPreserving($sPathPropelConfOrig, $sPathPropelConf)) {
		//	echo 'error with copy' . NL; var_dump($oRunner->aResults);
		//	return false;
		//} // if rename error

		// .htaccess.SssStemplate
		$sPathHTAconf = $sPathConf . '.htaccess';
		$sPathHTAconfOrig = $sPathConf . '.htaccess.SssStemplate';
		if (!$oRunner->copyPreserving($sPathHTAconfOrig, $sPathHTAconf)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// httpd.conf.SssStemplate
		$sPathHTTPdConf = $sPathConf . 'httpd.conf';
		$sPathHTTPdConfOrig = $sPathConf . 'httpd.conf.SssStemplate';
		if (!$oRunner->copyPreserving($sPathHTTPdConfOrig, $sPathHTTPdConf)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// copy SssSphocoaApp.conf.SssStemplate
		$sPathWebAppConf = $sPathConf . 'SssSphocoaApp.conf';
		$sPathWebAppConfSyml = $sPathConf . 'webapp.conf';
		$sPathWebAppConfOrig = $sPathConf . 'SssSphocoaApp.conf.SssStemplate';
		if (!$oRunner->copyPreserving($sPathWebAppConfOrig, $sPathWebAppConf)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// first remove old symlink
		$oRunner->doScript('rm -dfv "' . $sPathWebAppConfSyml . '";');
		// make symlink to SssSphocoaApp.conf for compatability with webapp.conf
		$oRunner->symlinkToAt($sPathWebAppConf, $sPathWebAppConfSyml);

		// copy AppleScripts php shell runner (on apple dir_sep is /)
		$sPathASres = $sPathGen . 'newSssSphocoaProjectFromPlist.app/Contents/Resources/';
		$sPathASnew = $sPathASres . 'newSssSphocoaProject';
		$sPathASnewOrig = $sPathASres . 'newSssSphocoaProject.SssStemplate';
		if (!$oRunner->copyPreserving($sPathASnewOrig, $sPathASnew)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// first remove old symlink
		$oRunner->doScript('rm -dfv "' . $sPathGen . 'newSssSphocoaProject";');
		// make symlink to newSssSphocoaProject in SssSphocoaGen
		$oRunner->symlinkToAt($sPathASnew, $sPathGen . 'newSssSphocoaProject');

		// copy inPlace AppleScripts php shell runner (on apple dir_sep is /)
		$sPathASres = $sPathGen . 'remakeTemplatedFilesWithInfoFromPlist.app/Contents/Resources/';
		$sPathASipNew = $sPathASres . 'remakeTemplatedFilesWithInfoFromPlist';
		$sPathASipNewOrig = $sPathASres . 'remakeTemplatedFilesWithInfoFromPlist.SssStemplate';
		if (!$oRunner->copyPreserving($sPathASipNewOrig, $sPathASipNew)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// first remove old symlink
		$oRunner->doScript('rm -dfv "' . $sPathGen . 'remakeTemplatedFilesWithInfoFromPlist";');
		// make symlink to newSssSphocoaProject in SssSphocoaGen
		$oRunner->symlinkToAt($sPathASipNew, $sPathGen . 'remakeTemplatedFilesWithInfoFromPlist');


		// let's save a plist for remake and one for useAsTemplate
		$oP->set('LOG_DIR', $sPathLog);
		$oP->saveTo($sPathGen . 'useTemplateThatMadeMe.plist');

		$oP->set('DEFAULT_BASE_TEMPLATE_DIR', $sPathDest);
		$oP->set('PHOCOA_APP_CONTAINER_DIR', $sPathDest);
		$oP->set('PHOCOA_PROJECT_NAME', $oP->get('PHOCOA_PROJECT_NAME') . 'Copy');

		$oP->saveTo($sPathGen . 'useMeAsTemplate.plist');

		// and so we can reuse another method
		return self::inPlaceRefreshTemplatesInPlist($oP, null, $sPathDest);

	} // makeNewSssSphocoaProjectWithPlist2


	public static function makeNewSssSphocoaProjectWithPlist3(
			$mPlist = null, // SssS_Plist object a path or a plist xml string or an array
			$bUseAllDefaults = false,
			$sPathDest = false,
			$sPathInvocationBase = null, // alternative root calculated from invocation path
			$sAppName = false) {

		static $ats = null; static $gi = null; static $iVersion = null;
		static $rts = null; if (!$rts) {

			$rts = 'removeTrailingSlash';
			$ats = 'addTrailingSlash';
			$gi = 'getInputCLI';
			$iVersion = 3;

		} // first run

		if ($sPathInvocationBase)
			$sPathInvocationBase = SssS_CLI_Tools::$rts($sPathInvocationBase, DIR_SEP);

		$sArgType = gettype($mPlist);
		if (('object' == $sArgType) && is_subclass_of($mPlist, 'SssS_Plist')) {
			$oP = $mPlist;
			if ($sPathInvocationBase)
				$oP->getOrSet('DEFAULT_BASE_TEMPLATE_DIR', $sPathInvocationBase);

		} else { // is not an SssS_Plist or subclass
			// mPlist is plist string or path
			if (('string' == $sArgType) || ('array' == $sArgType)) {
				$oP = new SssS_Plist($mPlist);
				if ($sPathInvocationBase)
					$oP->getOrSet('DEFAULT_BASE_TEMPLATE_DIR', $sPathInvocationBase);

			} // if string (path or xml) or array as mPlist
			else {
				$oP = self::defaultPlist($iVersion);
				if ($sPathInvocationBase)
					$oP->set('DEFAULT_BASE_TEMPLATE_DIR', $sPathInvocationBase);

			} // if mPlist type handling
		} // if no useable mPlist

		if ($iVersion > $oP->getInt('SssSphocoaGeneratorVersion', 1)) {

			self::warnVersion();

			return self::makeNewSssSphocoaProjectWithPlist2($oP, $bUseAllDefaults, $sPathDest, $sPathInvocationBase, $sAppName);

		} // if old version plist

		// some default paths

		// what to copy... (will ask for confirmation)
		$sPathDefaultBaseTemplate = $oP->get('DEFAULT_BASE_TEMPLATE_DIR');

		// where to point frameworks to (~required fields)
		$sPathPhocoaFrameWork	= $oP->get('FRAMEWORK_DIR');
		$sPathSmartyFrameWork	= $oP->get('SMARTY_DIR');
		$sPathPropelFrameWork	= $oP->get('PROPEL_FRAMEWORK_DIR');
		$sPathPropelGenerator	= $oP->get('PROPEL_GEN_BIN', 'propel-gen');
		$sPathHordeFrameWork	= $oP->get('HORDE_FRAMEWORK_DIR');
		$sPathSssSFrameWork		= $oP->get('SwissalpS_FRAMEWORK_DIR');
        $sPathPearLog			= $oP->get('PEAR_LOG_FRAMEWORK_DIR');
		$sPathPhing				= $oP->get('PHING_PATH');

		// some other default values
		// for propel -conf.xml (will ask for confirmation)
		$sDBtype = $oP->get('PROPEL_DATABASE', 'mysql');
		$sDBhost = $oP->get('DB_HOST', 'localhost'); //phocoaDBhost';
		$sDBuser = $oP->get('DB_USER'); //DBuser';
		$sDBpass = $oP->get('DB_PASS'); //DBpass';
		$sDBname = $oP->get('DB_NAME'); //DBname';

		// for apache httpd.conf (will ask for confirmation)
		$sHTTPip	= $oP->get('SERVER_IP', '127.0.0.1');
		$sHTTPHost	= $oP->get('SERVER_NAME', 'localhost');
		$iHTTPPort	= $oP->get('SERVER_PORT', 80);

		// default appName (will ask for confirmation)
		if (!$sAppName) $sAppName = $oP->get('PHOCOA_PROJECT_NAME', 'PhocoaApp');

		// where are we?
		$sPathCurentWorkingDirectory = getcwd();

		// welcome....
		echo NL .  NL . '     SssSphocoaGenerator::makeNewSssSphocoaProjectWithPlist()' . NL;

		$oRunner = self::sharedSssS_ShellScriptRunnerForDummies();

		$sPathOrig = ($sPathDefaultBaseTemplate)
			? SssS_CLI_Tools::$rts($sPathDefaultBaseTemplate, DIR_SEP) : ($sPathInvocationBase)
				? $sPathInvocationBase : '';

		$sPathDest = ($sPathDest)
				?  SssS_CLI_Tools::$ats($sPathDest, DIR_SEP) . $sAppName
				: $oP->getOrSet('PHOCOA_APP_CONTAINER_DIR', $sPathCurentWorkingDirectory . DIR_SEP . $sAppName);

	if ($bUseAllDefaults) {
		echo 'not interacting for values...' . NL;
		$sPathLog = $sPathDest . DIR_SEP . 'log';
		$sPathDestAppRoot = $sPathDest . DIR_SEP . $sAppName;
	} else {
	// get template path
		$bRepeat = true;
		while ($bRepeat) {

			// normally gets parts from sPathPhocoaFrameWork. we have our templates
			$sPathOrig = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
					'Path of template Base Structure (sPathDefaultBaseTemplate):',
											$sPathOrig), DIR_SEP);
			$oP->set('sPathDefaultBaseTemplate', $sPathOrig);

			// PHP is not letting me use paths with spaces...best not to have unsoported
			// chars as this will only make things worse down the line
			//if (!is_dir(escapeshellarg($sPathOrig))) {
			//	echo 'original template path not found: ' . escapeshellarg($sPathOrig);
			//	return false;
			//} // if no source
			if (!$oRunner->isFolder($sPathOrig)) {

				echo 'original template path not found: ' . $sPathOrig . NL;
				$sRetry = SssS_CLI_Tools::getChoiceCLI('Fix or exit?',
				array('fix' => 'Let me give another path', 'exit' => 'Abbort creation'), 'fix');
				if (in_array($sRetry, array('Abbort creation', 'exit'))) return false;

			} else {

				$bRepeat = false;

			} // if no source;

		} // loop until valid path given


	// get app name
		// ##PHOCOA_PROJECT_NAME##
		$sAppName = SssS_CLI_Tools::$gi(
								NL . 'Name of new App (sAppName):', $sAppName);
		$oP->set('sAppName', $sAppName);

	// get target path
		$bRepeat = true;
		while ($bRepeat) {

			// ##PHOCOA_APP_CONTAINER_DIR##
			$sPathDest = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
											NL . 'Path of new App (PHOCOA_APP_CONTAINER_DIR):',
											substr($sPathDest, 0, strlen($sPathDest) - strlen(strrchr($sPathDest, DIR_SEP))) . DIR_SEP
											. $sAppName), DIR_SEP);
			$oP->set('PHOCOA_APP_CONTAINER_DIR', $sPathDest);

			echo NL;

			if ($oRunner->isSomethingAtPath($sPathDest)) {

				echo 'destination already exists: ' . $sPathDest . NL;
				$sRetry = SssS_CLI_Tools::getChoiceCLI('Fix or exit?',
				array('fix' => 'Let me give another path', 'exit' => 'Abbort creation'), 'fix');
				if (in_array($sRetry, array('Abbort creation', 'exit'))) return false;

			} else {

				$bRepeat = false;

			} // if destination already exists;

		} // loop until valid path given

	// set target app-root
		// APP_ROOT', '##PHOCOA_APP_DIR##
		$sPathDestAppRoot = $sPathDest . DIR_SEP . $sAppName;
		$oP->set('PHOCOA_APP_DIR', $sPathDestAppRoot);

	// get app type
		// for application delegate
		$sAppType = SssS_CLI_Tools::getChoiceCLI('Type of app (cli|web|both)',
				array('cli' => 'Command Line Tool', 'web' => 'WFWebApplication',
				'both' => 'both types'), 'both');
		//echo 'currently only supporting cli, so set to cli' . NL . NL;
		//$sAppType = 'cli';

	// httpd.conf takes also servername, ip and port
		if (!in_array($sAppType, array('Command Line Tool', 'cli'))) {

			$sHTTPip = SssS_CLI_Tools::$gi(NL . 'IP (SERVER_IP):', $sHTTPip);
			$oP->set('SERVER_IP', $sHTTPip);
			$iHTTPPort = intval(
					SssS_CLI_Tools::$gi(NL . 'port (SERVER_PORT): ', $iHTTPPort));
			$oP->set('SERVER_PORT', $iHTTPPort);
			$sHTTPHost = SssS_CLI_Tools::$gi(
									NL . 'hostname (SERVER_NAME):', $sHTTPHost);
			$oP->set('SERVER_NAME', $sHTTPHost);
			echo NL;

		} // if not cli

	// ask for log dir
		$sPathLog = $sPathDest . DIR_SEP . 'log';
		$sPathLog = SssS_CLI_Tools::$gi(NL . 'log path:', $sPathLog);

	// ask if uses propel
		$sUsePropel = SssS_CLI_Tools::getChoiceCLI('Use propel for db?',
				array('no' => 'NO', 'yes' => 'YES'), 'yes');
		if ('yes' == strtolower($sUsePropel)) {
			// get db props
			echo NL;
		// get propel path
			$sPathPropelFrameWork = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
				NL . 'path to propel', $sPathPropelFrameWork));
			echo NL;
			$sDBtype = SssS_CLI_Tools::getChoiceCLI('Choose database type (PROPEL_DATABASE):',
					array('pgsql' => 'Postgres', 'mysql' => 'MySQl',
						  'mssql' => 'mssql', 'sqllite' => 'sqllite',
						   'ldap' => 'ldap'), $sDBtype);
			$oP->set('PROPEL_DATABASE', $sDBtype);
			$sDBname = SssS_CLI_Tools::$gi(NL . 'DB name:', $sDBname);
			$oP->set('DB_NAME', $sDBname);
			$sDBuser = SssS_CLI_Tools::$gi(NL . 'DB username:', $sDBuser);
			$oP->set('DB_USER', $sDBuser);
			$sDBpass = SssS_CLI_Tools::$gi(NL . 'DB password:', base64_decode($sDBpass));
			$oP->set('DB_PASS', base64_encode($sDBpass));
			$sDBhost = SssS_CLI_Tools::$gi(NL . 'DB hostname:', $sDBhost);
			$oP->set('DB_HOST', $sDBhost);
			$sPathPropelGenerator = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(NL . 'path to propel-gen binary', $sPathPropelGenerator));
			$oP->set('PROPEL_GEN_BIN', $sPathPropelGenerator);

		} // if propel

		echo NL;

	} // if ask for anything

	// copy the bare-bone structure
		// make destination path
		$bRes = $oRunner->makePath($sPathDest);
		$bPHPcompatiblePathDest = true;
		if ((!is_dir($sPathDest)) && $bRes) {
			echo NL . NL . 'destination path is not PHP compatible, problems'
					. ' to be expected' . NL;
			$bPHPcompatiblePathDest = false;
		} // if php compatible path destination

		// make log dir
		$oRunner->makePath($sPathLog);
		// make runtime dir
		//$oRunner->makePath($sPathDest . DIR_SEP . 'runtime');
		// copy runtime dir
		$bRes = $oRunner->copyPreserving($sPathOrig . DIR_SEP . 'runtime',
								 		 $sPathDest . DIR_SEP . 'runtime');
		if (!$bRes) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			return false;
		} // if copy error

		// copy app dir -> Project dir and app dir MUST bear same name
		$sPathOrigAppRoot = $sPathOrig . DIR_SEP
									. substr(strrchr($sPathOrig, DIR_SEP), 1);
		$bRes = $oRunner->copyPreserving($sPathOrigAppRoot,	$sPathDestAppRoot);
		if (!$bRes) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			return false;
		} // if copy error
		// also here PHP fails
		//if (false === copy($sPathOrig, $sPathDest)) {
		//	echo 'error making copy';
		//	return false;
		//} // if copy fail

		// copy bin dir
		//if (!$oRunner->copyPreserving($sPathOrig . '/AppRoot/bin', $sPathDestAppRoot . '/bin')) { echo 'error with copy' . NL; var_dump($oRunner->aResults); return false; } // if copy error

		// copy replicator dir holding info and applescript after the plists
		$sPathGen = $sPathDest . DIR_SEP . 'SssSphocoaGen';
		if (!$oRunner->copyPreserving(
						$sPathOrig . DIR_SEP . 'SssSphocoaGen', $sPathGen)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error
		$sPathGen .= DIR_SEP;

		// copy runner.phps.SssStemplate -> sAppName
		$sPathRunner = $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP . $sAppName;
		$sPathRunnerOrig = $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP
													. 'runner.phps.SssStemplate';
		if (!$oRunner->copyPreserving($sPathRunnerOrig, $sPathRunner)) {//moveOrRename
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			return false;
		} // if rename error

		// make sure there isn't any old symlink or empty directory...
		$oRunner->doScript('rm -dfv "' . $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP . 'run"');
		// make a symlink for easy running
		$bRes = $oRunner->symlinkToAt($sPathRunner, $sPathDestAppRoot . DIR_SEP
													. 'bin' . DIR_SEP . 'run');
		if (!$bRes) {
			echo 'error with alias creation' . NL; var_dump($oRunner->aResults);
			return false;
		} // if symlink error

	// config files

		$sPathConf = $sPathDestAppRoot . DIR_SEP . 'conf' . DIR_SEP;

		// copy propel-conf.xml.SssStemplate -> sAppName-conf.xml
		//$sPathPropelConf = $sPathConf . $sAppName . '-conf.xml';
		//$sPathPropelConfOrig = $sPathConf . 'propel-conf.xml.SssStemplate';
		//if (!$oRunner->copyPreserving($sPathPropelConfOrig, $sPathPropelConf)) {
		//	echo 'error with copy' . NL; var_dump($oRunner->aResults);
		//	return false;
		//} // if rename error

		// .htaccess.SssStemplate
		$sPathHTAconf = $sPathConf . '.htaccess';
		$sPathHTAconfOrig = $sPathConf . '.htaccess.SssStemplate';
		if (!$oRunner->copyPreserving($sPathHTAconfOrig, $sPathHTAconf)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// httpd.conf.SssStemplate
		$sPathHTTPdConf = $sPathConf . 'httpd.conf';
		$sPathHTTPdConfOrig = $sPathConf . 'httpd.conf.SssStemplate';
		if (!$oRunner->copyPreserving($sPathHTTPdConfOrig, $sPathHTTPdConf)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// copy SssSphocoaApp.conf.SssStemplate
		$sPathWebAppConf = $sPathConf . 'SssSphocoaApp.conf';
		$sPathWebAppConfSyml = $sPathConf . 'webapp.conf';
		$sPathWebAppConfOrig = $sPathConf . 'SssSphocoaApp.conf.SssStemplate';
		if (!$oRunner->copyPreserving($sPathWebAppConfOrig, $sPathWebAppConf)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// first remove old symlink
		$oRunner->doScript('rm -dfv "' . $sPathWebAppConfSyml . '";');
		// make symlink to SssSphocoaApp.conf for compatability with webapp.conf
		$oRunner->symlinkToAt($sPathWebAppConf, $sPathWebAppConfSyml);

		// copy AppleScripts php shell runner (on apple dir_sep is /)
		$sPathASres = $sPathGen . 'newSssSphocoaProjectFromPlist.app/Contents/Resources/';
		$sPathASnew = $sPathASres . 'newSssSphocoaProject';
		$sPathASnewOrig = $sPathASres . 'newSssSphocoaProject.SssStemplate';
		if (!$oRunner->copyPreserving($sPathASnewOrig, $sPathASnew)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// first remove old symlink
		$oRunner->doScript('rm -dfv "' . $sPathGen . 'newSssSphocoaProject";');
		// make symlink to newSssSphocoaProject in SssSphocoaGen
		$oRunner->symlinkToAt($sPathASnew, $sPathGen . 'newSssSphocoaProject');

		// copy inPlace AppleScripts php shell runner (on apple dir_sep is /)
		$sPathASres = $sPathGen . 'remakeTemplatedFilesWithInfoFromPlist.app/Contents/Resources/';
		$sPathASipNew = $sPathASres . 'remakeTemplatedFilesWithInfoFromPlist';
		$sPathASipNewOrig = $sPathASres . 'remakeTemplatedFilesWithInfoFromPlist.SssStemplate';
		if (!$oRunner->copyPreserving($sPathASipNewOrig, $sPathASipNew)) {
			echo 'error with copy' . NL; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// first remove old symlink
		$oRunner->doScript('rm -dfv "' . $sPathGen . 'remakeTemplatedFilesWithInfoFromPlist";');
		// make symlink to newSssSphocoaProject in SssSphocoaGen
		$oRunner->symlinkToAt($sPathASipNew, $sPathGen . 'remakeTemplatedFilesWithInfoFromPlist');


		// let's save a plist for remake and one for useAsTemplate
		$oP->set('LOG_DIR', $sPathLog);
		$oP->saveTo($sPathGen . 'useTemplateThatMadeMe.plist');

		$oP->set('DEFAULT_BASE_TEMPLATE_DIR', $sPathDest);
		$oP->set('PHOCOA_APP_CONTAINER_DIR', $sPathDest);
		$oP->set('PHOCOA_PROJECT_NAME', $oP->get('PHOCOA_PROJECT_NAME') . 'Copy');

		$oP->saveTo($sPathGen . 'useMeAsTemplate.plist');

		// and so we can reuse another method
		return self::inPlaceRefreshTemplatesInPlist($oP, null, $sPathDest);

	} // makeNewSssSphocoaProjectWithPlist3


	public static function inPlaceRefreshTemplatesInPlist(
			$mPlist = null,
			$aFiles = null,
			$sPathInvocationBase = null) {

		if (!$mPlist) return null;

		static $ats = null; static $gi = null; static $iVersion = null;
		static $rts = null; if (!$rts) {

			$rts = 'removeTrailingSlash';
			$ats = 'addTrailingSlash';
			$gi = 'getInputCLI';
			$iVersion = 4;

		} // first run

		$sArgType = gettype($mPlist);
		if ('object' == $sArgType && is_subclass_of($mPlist, 'SssS_Plist')) {

			$oP = $mPlist;

		} else { // is not an SssS_Plist or subclass
			// mPlist is plist string or path
			if (('string' == $sArgType) || ('array' == $sArgType)) {

				$oP = new SssS_Plist($mPlist);
				// if string (path or xml) or array as mPlist
			} else {

				$oP = self::defaultPlist($iVersion);
				if ($sPathInvocationBase)
					$oP->set('PATH_DIR_DEFAULT_BASE_TEMPLATE', null);

			} // if use defaults

		} // if mPlist is is a SssS_Plist subclass or not

		if ($iVersion > $oP->getInt('SssSphocoaGeneratorVersion', 1)) {

			self::warnVersion();

			return self::inPlaceRefreshTemplatesInPlist3($oP, $aFiles, $sPathInvocationBase);

		} // if not a version 4 plist given -> do older method

		// make sure the base dir is set
		$oP->getOrSet('PATH_DIR_PHOCOA_APP_CONTAINER', $sPathInvocationBase);

		// merge given plist over defaults plist and hold in $oP
		$oPd = self::defaultPlist($iVersion);
		$a = array_merge($oPd->a_plist, $oP->a_plist);
		$oP->set('', $a);

		// collect the information (for in-place replacement)
		$sPathOrig = SssS_CLI_Tools::$rts($a['PATH_DIR_PHOCOA_APP_CONTAINER'], DIR_SEP);
		$sAppName = basename($sPathOrig);
		$sPathDestAppRoot = $sPathOrig . DIR_SEP . $sAppName;
		$sPathGen = $sPathOrig . DIR_SEP . 'SssSphocoaGen';

		if (empty($aFiles)) $aFiles = self::findTemplatesInProject($sPathOrig);

		if (empty($aFiles))	return 0;

		// double check existance of major variables
		$oP->getOrSet('DIR_SEP', DIR_SEP);
		$oP->getOrSet('DATE_STAMP', gmdate('Ymd_His'));
		$oP->getOrSet('PHOCOA_PROJECT_NAME', $sAppName);
		$oP->getOrSet('PATH_DIR_PHOCOA_APP', $sPathDestAppRoot);
		$oP->getOrSet('PATH_DIR_PHOCOA_APP_CONTAINER', $sPathOrig);
		$oP->getOrSet('PATH_DIR_LOG', $sPathOrig . DIR_SEP . 'log');
		$oP->getOrSet('PHP_BIN', SssS_ShellScriptRunner::getPHPinterpreterPath());

		$a = $oP->a_plist;	$aSearches = array(); $aReplacements = array();
		forEach ($a as $sKey => $mVal) {

			if (is_array($mVal)) continue; // skip arrays

			$aSearches[] = '##' . $sKey . '##';

			// special case password(s)
			if ('DB_PASS' == $sKey) {

				// decode password
				$aReplacements[] = base64_decode($mVal);

				// don't trim password! could end with a DIR_SEP
				continue;

			} // if password

			// remove trailing slash only if not only slash!
			if (is_string($mVal) && 1 < strLen($mVal)) {

				$aReplacements[] = SssS_CLI_Tools::$rts($mVal, DIR_SEP);

			} else {

				$aReplacements[] = $mVal;

			} // if string and longer than 1 char -> remove trailing slash

		} // loop each entry

		// make sure run blocker blocks get removed
		$aSearches[] = '/*##RUN_BLOCKER##';
		$aSearches[] = '##RUN_BLOCKER##*/';

		$iCountTemplatesDone = 0;
		foreach ($aFiles as $sPath) {
			$sPath = trim($sPath);
			if (empty($sPath)) continue;
			$sNameTemplate = basename($sPath);
			$sName = basename($sPath, '.SssStemplate');
			if ('runner.phps' == $sName) $sName = $sAppName;
			$sPath = dirname($sPath) . DIR_SEP;
			$sIn = file_get_contents_safely($sPath . $sNameTemplate);
			$sOut = str_replace($aSearches, $aReplacements, $sIn);
			file_put_contents_safely($sPath . $sName, $sOut, 'wb');

			// replicate mode of original
			chmod($sPath . $sName, filePerms($sPath . $sNameTemplate));

			$iCountTemplatesDone++;

		} // foreach path

		return $iCountTemplatesDone;

	} // inPlaceRefreshTemplatesInPlist


	public static function inPlaceRefreshTemplatesInPlist3(
			$mPlist = null,
			$aFiles = null,
			$sPathInvocationBase = null) {

		if (!$mPlist) return null;

		static $ats = null; static $gi = null; static $iVersion = null;
		static $rts = null; if (!$rts) {

			$rts = 'removeTrailingSlash';
			$ats = 'addTrailingSlash';
			$gi = 'getInputCLI';
			$iVersion = 3;

		} // first run

		$sArgType = gettype($mPlist);
		if ('object' == $sArgType && is_subclass_of($mPlist, 'SssS_Plist')) {

			$oP = $mPlist;

		} else { // is not an SssS_Plist or subclass
			// mPlist is plist string or path
			if (('string' == $sArgType) || ('array' == $sArgType)) {

				$oP = new SssS_Plist($mPlist);
				// if string (path or xml) or array as mPlist
			} else {

				$oP = self::defaultPlist($iVersion);
				if ($sPathInvocationBase)
					$oP->set('DEFAULT_BASE_TEMPLATE_DIR', null);

			} // if use defaults

		} // if mPlist is is a SssS_Plist subclass or not

		if ($iVersion > $oP->getInt('SssSphocoaGeneratorVersion', 1)) {

			self::warnVersion();

			return self::inPlaceRefreshTemplatesInPlist2($oP, $aFiles, $sPathInvocationBase);

		} // if not a version 3 plist given -> do older method

		// make sure the base dir is set
		$oP->getOrSet('PHOCOA_APP_CONTAINER_DIR', $sPathInvocationBase);

		// merge given plist over defaults plist and hold in $oP
		$oPd = self::defaultPlist($iVersion);
		$a = array_merge($oPd->a_plist, $oP->a_plist);
		$oP->set('', $a);

		// collect the information (for in-place replacement)
		$sPathOrig = SssS_CLI_Tools::$rts($a['PHOCOA_APP_CONTAINER_DIR'], DIR_SEP);
		$sAppName = basename($sPathOrig);
		$sPathDestAppRoot = $sPathOrig . DIR_SEP . $sAppName;
		$sPathGen = $sPathOrig . DIR_SEP . 'SssSphocoaGen';

		if (empty($aFiles)) $aFiles = self::findTemplatesInProject($sPathOrig);

		if (empty($aFiles))	return 0;

		// double check existance of major variables
		$oP->getOrSet('DIR_SEP', DIR_SEP);
		$oP->getOrSet('DATE_STAMP', gmdate('Ymd_His'));
		$oP->getOrSet('PHOCOA_PROJECT_NAME', $sAppName);
		$oP->getOrSet('PHOCOA_APP_DIR', $sPathDestAppRoot);
		$oP->getOrSet('PHOCOA_APP_CONTAINER_DIR', $sPathOrig);
		$oP->getOrSet('LOG_DIR', $sPathOrig . DIR_SEP . 'log');
		$oP->getOrSet('PHP_BIN', SssS_ShellScriptRunner::getPHPinterpreterPath());

		$a = $oP->a_plist;	$aSearches = array(); $aReplacements = array();
		forEach ($a as $sKey => $mVal) {

			if (is_array($mVal)) continue; // skip arrays

			$aSearches[] = '##' . $sKey . '##';

			// special case password(s)
			if ('DB_PASS' == $sKey) {

				// decode password
				$aReplacements[] = base64_decode($mVal);

				// don't trim password! could end with a DIR_SEP
				continue;

			} // if password

			// remove trailing slash only if not only slash!
			if (is_string($mVal) && 1 < strLen($mVal)) {

				$aReplacements[] = SssS_CLI_Tools::$rts($mVal, DIR_SEP);

			} else {

				$aReplacements[] = $mVal;

			} // if string and longer than 1 char -> remove trailing slash

		} // loop each entry

		// make sure run blocker blocks get removed
		$aSearches[] = '/*##RUN_BLOCKER##';
		$aSearches[] = '##RUN_BLOCKER##*/';

		$iCountTemplatesDone = 0;

		foreach ($aFiles as $sPath) {
			$sPath = trim($sPath);
			if (empty($sPath)) continue;
			$sNameTemplate = basename($sPath);
			$sName = basename($sPath, '.SssStemplate');
			if ('runner.phps' == $sName) $sName = $sAppName;
			//if ('propel-conf.xml' == $sName) $sName = $sAppName . '-conf.xml';
			$sPath = dirname($sPath) . DIR_SEP;
			$sIn = file_get_contents_safely($sPath . $sNameTemplate);
			$sOut = str_replace($aSearches, $aReplacements, $sIn);
			file_put_contents_safely($sPath . $sName, $sOut, 'wb');

			// replicate mode of original
			chmod($sPath . $sName, filePerms($sPath . $sNameTemplate));

			$iCountTemplatesDone++;

		} // foreach path

		return $iCountTemplatesDone;

	} // inPlaceRefreshTemplatesInPlist3


	public static function inPlaceRefreshTemplatesInPlist2(
			$mPlist = null,
			$aFiles = null,
			$sPathInvocationBase = null) {

		if (!$mPlist) return null;

		static $ats = null; static $gi = null; static $iVersion = null;
		static $rts = null; if (!$rts) {

			$rts = 'removeTrailingSlash';
			$ats = 'addTrailingSlash';
			$gi = 'getInputCLI';
			$iVersion = 2;

		} // first run

		$sArgType = gettype($mPlist);
		if ('object' == $sArgType && is_subclass_of($mPlist, 'SssS_Plist')) {

			$oP = $mPlist;

		} else { // is not an SssS_Plist or subclass
			// mPlist is plist string or path
			if (('string' == $sArgType) || ('array' == $sArgType)) {

				$oP = new SssS_Plist($mPlist);
				// if string (path or xml) or array as mPlist
			} else {

				$oP = self::defaultPlist($iVersion);
				if ($sPathInvocationBase)
					$oP->set('DEFAULT_BASE_TEMPLATE_DIR', null);

			} // if use defaults

		} // if mPlist is is a SssS_Plist subclass or not

		if ($iVersion > $oP->getInt('SssSphocoaGeneratorVersion', 1)) {

			self::warnVersion();

			return self::inPlaceRefreshTemplatesInPlist1($oP, $aFiles, $sPathInvocationBase);

		} // if not a version 2 plist given -> do old method

		// make sure the base dir is set
		$oP->getOrSet('PHOCOA_APP_CONTAINER_DIR', $sPathInvocationBase);

		// merge given plist over defaults plist and hold in $oP
		$oPd = self::defaultPlist($iVersion);
		$a = array_merge($oPd->a_plist, $oP->a_plist);
		$oP->set('', $a);

		// collect the information (for in-place replacement)
		$sPathOrig = SssS_CLI_Tools::$rts($a['PHOCOA_APP_CONTAINER_DIR'], DIR_SEP);
		$sAppName = basename($sPathOrig);
		$sPathDestAppRoot = $sPathOrig . DIR_SEP . $sAppName;
		$sPathGen = $sPathOrig . DIR_SEP . 'SssSphocoaGen';

		if (empty($aFiles))
				$aFiles = self::findTemplatesInProject($sPathOrig);

		if (empty($aFiles))	return 0;

		// double check existance of major variables
		$oP->getOrSet('DIR_SEP', DIR_SEP);
		$oP->getOrSet('DATE_STAMP', gmdate('Ymd_His'));
		$oP->getOrSet('PHOCOA_PROJECT_NAME', $sAppName);
		$oP->getOrSet('PHOCOA_APP_DIR', $sPathDestAppRoot);
		$oP->getOrSet('PHOCOA_APP_CONTAINER_DIR', $sPathOrig);
		$oP->getOrSet('LOG_DIR', $sPathOrig . DIR_SEP . 'log');
		$oP->getOrSet('PHP_BIN', SssS_ShellScriptRunner::getPHPinterpreterPath());

		$a = $oP->a_plist;	$aSearches = array(); $aReplacements = array();
		forEach ($a as $sKey => $mVal) {

			if (is_array($mVal)) continue; // skip arrays

			$aSearches[] = '##' . $sKey . '##';

			// remove trailing slash only if not only slash!
			if (is_string($mVal) && 1 < strLen($mVal)) {

				$aReplacements[] = SssS_CLI_Tools::$rts($mVal, DIR_SEP);

			} else {

				$aReplacements[] = $mVal;

			} // if string and longer than 1 char -> remove trailing slash

		} // loop each entry

		// make sure run blocker blocks get removed
		$aSearches[] = '/*##RUN_BLOCKER##';
		$aSearches[] = '##RUN_BLOCKER##*/';

		$iCountTemplatesDone = 0;

		foreach ($aFiles as $sPath) {
			$sPath = trim($sPath);
			if (empty($sPath)) continue;
			$sNameTemplate = basename($sPath);
			$sName = basename($sPath, '.SssStemplate');
			if ('runner.phps' == $sName) $sName = $sAppName;
			//if ('propel-conf.xml' == $sName) $sName = $sAppName . '-conf.xml';
			$sPath = dirname($sPath) . DIR_SEP;
			$sIn = file_get_contents_safely($sPath . $sNameTemplate);
			$sOut = str_replace($aSearches, $aReplacements, $sIn);
			file_put_contents_safely($sPath . $sName, $sOut, 'wb');

			//if ('#!' == subStr($sOut, 0, 2)) { } // if got shebang -> executable
			// instead replicate original
			chmod($sPath . $sName, filePerms($sPath . $sNameTemplate));

			$iCountTemplatesDone++;

		} // foreach path

		return $iCountTemplatesDone;

	} // inPlaceRefreshTemplatesInPlist2


	public static function inPlaceRefreshTemplatesInPlist1(
			$mPlist = null,
			$aFiles = null,
			$sPathInvocationBase = null) {

		if (!$mPlist) return null;

		static $ats = null; static $gi = null; static $iVersion = null;
		static $aSearches = null;
		static $rts = null; if (!$rts) {

			$rts = 'removeTrailingSlash';
			$ats = 'addTrailingSlash';
			$gi = 'getInputCLI';
			$iVersion = 1;
			$aSearches = array('##PHOCOA_BASE_DIR##', '##PHOCOA_PROJECT_NAME##',
				'##PHOCOA_APP_CONTAINER_DIR##', '##PHOCOA_APP_DIR##',
				'##SwissalpS_FRAMEWORK_DIR##', '##PEAR_LOG_FRAMEWORK_DIR##',
				'##PROPEL_FRAMEWORK_DIR##', '##HORDE_FRAMEWORK_DIR##',
				'##PHING_PATH##', '##DATE_STAMP##',
				'##SERVER_IP##', '##SERVER_PORT##', '##SERVER_NAME##',
				'##LOG_DIR##', '##DB_NAME##', '##PROPEL_DATABASE##',
				'##DB_USER##', '##DB_HOST##', '##DB_PASS##',
				'##SMARTY_DIR##', '##DIR_SEP##', '##PROPEL_GEN_BIN##', '##PHP_BIN##',
				'/*##RUN_BLOCKER##', '##RUN_BLOCKER##*/'); // last 2 need no replacement

		} // first run

		$sArgType = gettype($mPlist);
		if ('object' == $sArgType && is_subclass_of($mPlist, 'SssS_Plist'))
			$oP = $mPlist;

		else { // is not an SssS_Plist or subclass
			// mPlist is plist string or path
			if (('string' == $sArgType) || ('array' == $sArgType))
				$oP = new SssS_Plist($mPlist);
			// if string (path or xml) or array as mPlist

			else {
				$oP = self::defaultPlist($iVersion);
				if ($sPathInvocationBase)
					$oP->set('sPathDefaultBaseTemplate', null);
			} // if use defaults

		} // if mPlist is is a SssS_Plist subclass or not

		$oP->getOrSet('sPathDefaultBaseTemplate', $sPathInvocationBase);

		$oPd = self::defaultPlist($iVersion);
		$a = array_merge($oPd->a_plist, $oP->a_plist);
		$oP->set('', $a);

		// collect the information
		$sPathOrig = SssS_CLI_Tools::$rts($a['sPathDefaultBaseTemplate'], DIR_SEP);
		$sAppName = basename($sPathOrig);
		$sPathDestAppRoot = $sPathOrig . DIR_SEP . $sAppName;
		$sPathGen = $sPathOrig . DIR_SEP . 'SssSphocoaGen';

		if (empty($aFiles))
				$aFiles = self::findTemplatesInProject($sPathOrig);

		if (empty($aFiles))	return 0;

		$aReplacements = array(
			SssS_CLI_Tools::$rts($a['sPathPhocoaFrameWork']),
			$sAppName, $sPathOrig, $sPathDestAppRoot,
			SssS_CLI_Tools::$rts($a['sPathSssSFrameWork']),
			SssS_CLI_Tools::$rts($a['sPathPearLog']),
			SssS_CLI_Tools::$rts($a['sPathPropelFrameWork']),
			SssS_CLI_Tools::$rts($a['sPathHordeFrameWork']),
			SssS_CLI_Tools::$rts($a['sPathPhing']),
			gmdate('Ymd_His'),
			$oP->get('sHTTPip', '127.0.0.1'), $oP->get('iHTTPPort', 80),
			$oP->get('sHTTPHost', 'localhost'),
			$oP->get('sPathLog', $sPathOrig . DIR_SEP . 'log'),
			$oP->get('sDBname', 'phocoa'), $oP->get('sDBtype', 'mysql'),
			$oP->get('sDBuser', 'phocoa'), $oP->get('sDBhost', 'phocoa'),
			$oP->get('sDBpass', 'phocoa'),
			SssS_CLI_Tools::$rts($a['sPathSmartyFrameWork']),
			(isset($a['DIR_SEP']) ? $a['DIR_SEP'] : DIR_SEP), // todo: read this earlier
			$oP->get('sPathPropelGenerator', 'propel-gen'),
			SssS_ShellScriptRunner::getPHPinterpreterPath()); // delete last 2

		$iCountTemplatesDone = 0;

		foreach ($aFiles as $sPath) {
			$sPath = trim($sPath);
			if (empty($sPath)) continue;
			$sNameTemplate = basename($sPath);
			$sName = basename($sPath, '.SssStemplate');
			if ('runner.phps' == $sName) $sName = $sAppName;
			//if ('propel-conf.xml' == $sName) $sName = $sAppName . '-conf.xml';
			$sPath = dirname($sPath) . DIR_SEP;
			file_put_contents_safely($sPath . $sName,
					str_replace($aSearches, $aReplacements,
					file_get_contents_safely($sPath . $sNameTemplate)), 'wb');
			$iCountTemplatesDone++;
		} // foreach path

		return $iCountTemplatesDone;
	} // inPlaceRefreshTemplatesInPlist1 // refreshSetOfConfigToPlistValues or something of the kind to make an alternative .conf for different deployment or testing
	// scan current approot for any files ending with .SssStemplate
	// for each make a copy removing the suffix and overwrite existing


	public static function findTemplatesInProject($sPathOrig = null) {

		$oRunner = self::sharedSssS_ShellScriptRunnerForDummies();

		if (!$oRunner->isFolder($sPathOrig)) return array();

		// make sure find gets it that we are looking at a folder....
		$oRunner->doScript('find -L "' . $sPathOrig . '" -name "*.SssStemplate"');

		return explode("\n", $oRunner->sOut());

	} // findTemplatesInProject


	public static function parseInvocationBase($sInvocation) {

		$sPathInvocationBase = null;

		if (false !== strstr($sInvocation, self::sAppleScriptletBitInPlace)) {
			$sPathInvocationBase = substr($sInvocation, 0, strlen($sInvocation) - strlen(self::sAppleScriptletBitInPlace));
		} // if got an optional source path because run from winthin realpath()
		// AppleScript.app in a SssSphocoaGen directory.
		// That directory could be used as sPathDefaultBaseTemplate
		// if sPathDefaultBaseTemplate is missing in the plist

		return $sPathInvocationBase;

	} // parseInvocationBase


	public static function sharedSssS_ShellScriptRunnerForDummies() {
		static $oRunner = null; if (!$oRunner)
			$oRunner = new SssS_ShellScriptRunnerForDummies();
		return $oRunner;
	} // sharedSssS_ShellScriptRunnerForDummies


	public static function showHelpCLI() {

		static $sHelpText = null; if (!$sHelpText) $sHelpText =
		'syntax for SssSphocoaGenerator::runCLI()
		newSssSphocoaProject [args] <plist> [[args] <plist>] ...]
	the following arguments are valid:
		help	shows this text. no other arguments are parsed when this
				argument is present.

		-target <destination_dir>	ignore the target in plist and use this

		-appName <appName>	override plist appName

		-noInteraction	use all defaults, don\'t ask for confirmation

		-withInteraction (default) prompt for each value

		arguments are not case sensitive, values are.
	your_script needs to make sure SssSphocoaGenerator.inc is loaded
';

		echo $sHelpText;

	} // showHelpCLI


	public static function trimOrUnescapeShellArgument($s) {

		return ('"' == $s{0} || "'" == $s{0})
					? trim($s, "\" \t'") 		// trim whitespace and ' "
					: str_replace('\\', '', $s);// or remove \ escapes --> this endangers sending clear xml plist todo: check xml header() {

	} // trimOrUnescapeShellArgument


	public static function updatePlistVersion1To2($sPlistPath) {

		// only proceed if file exists
		if (!is_file($sPlistPath)) return false;

		// init the plist
		$oP = new SssS_Plist($sPlistPath);
		$aOrig = $oP->a_plist;

		// only continue if not empty
		if (empty($aOrig)) return false;

		// backup
		$oP->saveTo($sPlistPath . '~');

		$aNew = array();
		forEach ($aOrig as $sKey => $mVal) {

			switch($sKey) {

				case 'sPathPropelGenerator' : $aNew['PROPEL_GEN_BIN'] = $mVal; break;

				case 'sPathDest' : $aNew['PHOCOA_APP_CONTAINER_DIR'] = $mVal; break;

				case 'sPathDefaultBaseTemplate' : $aNew['DEFAULT_BASE_TEMPLATE_DIR'] = $mVal; break;

				case 'sPathPhocoaFrameWork' : $aNew['FRAMEWORK_DIR'] = $mVal; break;

				case 'sPathSssSFrameWork' : $aNew['SwissalpS_FRAMEWORK_DIR'] = $mVal; break;

				case 'sPathSmartyFrameWork' : $aNew['SMARTY_DIR'] = $mVal; break;

				case 'sPathPropelFrameWork' : $aNew['PROPEL_FRAMEWORK_DIR'] = $mVal; break;

				case 'sPathHordeFrameWork' : $aNew['HORDE_FRAMEWORK_DIR'] = $mVal; break;

				case 'sPathPearLog' : $aNew['PEAR_LOG_FRAMEWORK_DIR'] = $mVal; break;

				case 'sPathPhing' : $aNew['PHING_PATH'] = $mVal; break;

				case 'sDBtype' : $aNew['PROPEL_DATABASE'] = $mVal; break;

				case 'sDBhost' : $aNew['DB_HOST'] = $mVal; break;

				case 'sDBuser' : $aNew['DB_USER'] = $mVal; break;

				case 'sDBpass' : $aNew['DB_PASS'] = $mVal; break;

				case 'sDBname' : $aNew['DB_NAME'] = $mVal; break;

				case 'sHTTPip' : $aNew['SERVER_IP'] = $mVal; break;

				case 'sHTTPHost' : $aNew['SERVER_NAME'] = $mVal; break;

				case 'iHTTPPort' : $aNew['SERVER_PORT'] = $mVal; break;

				case 'sAppName' : $aNew['PHOCOA_PROJECT_NAME'] = $mVal; break;

				case 'sPathLog' : $aNew['LOG_DIR'] = $mVal; break;

				default :

					$aNew[$sKey] = $mVal;

				break;

			} // switch known keys

		} // loop each entry

		// re-init
		$oP->initFromArray($aNew);

		// set version
		$oP->setInt('SssSphocoaGeneratorVersion', 2);

		// save
		$oP->save();

		return true;

	} // updatePlistVersion1To2


	// we need to encode passwords as they can contain special characters that can mess up the plist
	public static function updatePlistVersion2To3($sPlistPath) {

		// only proceed if file exists
		if (!is_file($sPlistPath)) return false;

		// init the plist
		$oP = new SssS_Plist($sPlistPath);
		$aOrig = $oP->a_plist;

		// only continue if not empty
		if (empty($aOrig)) return false;

		// backup
		$oP->saveTo($sPlistPath . '~');

		// is a password set at all?
		if (isset($aOrig['DB_PASS'])) {

			$oP->setString('DB_PASS', base64_encode($oP->getString('DB_PASS')));

		} // if got password at all

		// set version
		$oP->setInt('SssSphocoaGeneratorVersion', 3);

		// save
		$oP->save();

		return true;

	} // updatePlistVersion2To3


	// this requires templates to be updated too!
	public static function updatePlistVersion3To4($sPlistPath) {

		// only proceed if file exists
		if (!is_file($sPlistPath)) return false;

		// init the plist
		$oP = new SssS_Plist($sPlistPath);
		$aOrig = $oP->a_plist;

		// only continue if not empty
		if (empty($aOrig)) return false;

		// backup
		$oP->saveTo($sPlistPath . '~');

		$aNew = array();
		forEach ($aOrig as $sKey => $mVal) {

			switch($sKey) {

				case 'PROPEL_GEN_BIN' : $aNew['PATH_BIN_PROPEL'] = $mVal; break;

				case 'PHOCOA_APP_DIR' : $aNew['PATH_DIR_PHOCOA_APP'] = $mVal; break;

				case 'PHOCOA_APP_CONTAINER_DIR' : $aNew['PATH_DIR_PHOCOA_APP_CONTAINER'] = $mVal; break;

				case 'DEFAULT_BASE_TEMPLATE_DIR' : $aNew['PATH_DIR_DEFAULT_BASE_TEMPLATE'] = $mVal; break;

				case 'FRAMEWORK_DIR' : $aNew['PATH_DIR_SRC_PHOCOA'] = $mVal; break;

				case 'SwissalpS_FRAMEWORK_DIR' : $aNew['PATH_DIR_SRC_SwissalpSphp'] = $mVal; break;

				case 'SMARTY_DIR' : $aNew['PATH_DIR_SRC_SMARTY'] = $mVal; break;

				case 'PROPEL_FRAMEWORK_DIR' : $aNew['PATH_DIR_SRC_PROPEL'] = $mVal; break;

				case 'HORDE_FRAMEWORK_DIR' : $aNew['PATH_DIR_SRC_HORDE'] = $mVal; break;

				case 'PEAR_LOG_FRAMEWORK_DIR' : $aNew['PATH_DIR_SRC_PEAR_LOG'] = $mVal; break;

				case 'PHING_PATH' : $aNew['PATH_DIR_SRC_PHING'] = $mVal; break;

				case 'PROPEL_DATABASE' : $aNew['DB_TYPE'] = $mVal; break;

				case 'SERVER_IP' : $aNew['HTTPD_IP'] = $mVal; break;

				case 'SERVER_NAME' : $aNew['HTTPD_NAME'] = $mVal; break;

				case 'SERVER_PORT' : $aNew['HTTPD_PORT'] = $mVal; break;

				case 'LOG_DIR' : $aNew['PATH_DIR_LOG'] = $mVal; break;

				default :

					$aNew[$sKey] = $mVal;

				break;

			} // switch known keys

		} // loop each entry

		// re-init
		$oP->initFromArray($aNew);

		// set version
		$oP->setInt('SssSphocoaGeneratorVersion', 4);

		// save
		$oP->save();

		return true;

	} // updatePlistVersion3To4


	public static function updateTemplatesVersion1To2($sPathOrig = null) {

		//TODO: if null path, use pwd/cwd
		$aTemplates = self::findTemplatesInProject($sPathOrig);

		if (empty($aTemplates)) return false;

		$sAppName = basename($sPathOrig);

		$aSearches = array(
			'##sPathPropelGenerator##',
			'##sPathDest##',
			'##sPathDefaultBaseTemplate##',
			'##sPathPhocoaFrameWork##',
			'##sPathSssSFrameWork##',
			'##sPathSmartyFrameWork##',
			'##sPathPropelFrameWork##',
			'##sPathHordeFrameWork##',
			'##sPathPearLog##',
			'##sPathPhing##',
			'##sDBtype##',
			'##sDBhost##',
			'##sDBuser##',
			'##sDBpass##',
			'##sDBname##',
			'##sHTTPip##',
			'##sHTTPHost##',
			'##iHTTPPort##',
			'##sAppName##',
			'##sPathLog##',
			);
		$aReplacements = array(
			'##PROPEL_GEN_BIN##',
			'##PHOCOA_APP_CONTAINER_DIR##',
			'##DEFAULT_BASE_TEMPLATE_DIR##',
			'##FRAMEWORK_DIR##',
			'##SwissalpS_FRAMEWORK_DIR##',
			'##SMARTY_DIR##',
			'##PROPEL_FRAMEWORK_DIR##',
			'##HORDE_FRAMEWORK_DIR##',
			'##PEAR_LOG_FRAMEWORK_DIR##',
			'##PHING_PATH##',
			'##PROPEL_DATABASE##',
			'##DB_HOST##',
			'##DB_USER##',
			'##DB_PASS##',
			'##DB_NAME##',
			'##SERVER_IP##',
			'##SERVER_NAME##',
			'##SERVER_PORT##',
			'##PHOCOA_PROJECT_NAME##',
			'##LOG_DIR##',
			);

		$iCountTemplatesDone = 0;
		foreach ($aTemplates as $sPathFile) {

			$sPath = trim($sPathFile);
			if (empty($sPath)) continue;

			// backup
			copy($sPath, $sPath . '~');

			$sNameTemplate = basename($sPath);
			$sName = basename($sPath, '.SssStemplate');
			if ('runner.phps' == $sName) $sName = $sAppName;
			$sPath = dirname($sPath) . DIR_SEP;
			$sIn = file_get_contents_safely($sPath . $sNameTemplate);
			$sOut = str_replace($aSearches, $aReplacements, $sIn);
			file_put_contents_safely($sPath . $sName, $sOut, 'wb');

			// replicate mode of original
			chmod($sPath . $sName, filePerms($sPath . $sNameTemplate));

			$iCountTemplatesDone++;

		} // loop each file

		return $iCountTemplatesDone;

	} // updateTemplatesVersion1To2


	public static function updateTemplatesVersion1To3($sPathOrig = null) {
		return self::updateTemplatesVersion1To2($sPathOrig);
		return self::updateTemplatesVersion2To3($sPathOrig);
	} // updateTemplatesVersion1To3


	public static function updateTemplatesVersion1To4($sPathOrig = null) {
		if (self::updateTemplatesVersion1To2($sPathOrig))
		return self::updateTemplatesVersion2Or3To4($sPathOrig);
	} // updateTemplatesVersion1To4


	public static function updateTemplatesVersion2To3($sPathOrig = null) {
		// nothing changed with keywords, nothing to do
		return true;
	} // updateTemplatesVersion2To3


	public static function updateTemplatesVersion2To4($sPathOrig = null) {
		return self::updateTemplatesVersion2Or3To4($sPathOrig);
	} // updateTemplatesVersion2To4


	public static function updateTemplatesVersion3To4($sPathOrig = null) {
		return self::updateTemplatesVersion2Or3To4($sPathOrig);
	} // updateTemplatesVersion3To4


	public static function updateTemplatesVersion2Or3To4($sPathOrig = null) {

		//TODO: if null path, use pwd/cwd
		$aTemplates = self::findTemplatesInProject($sPathOrig);

		if (empty($aTemplates)) return false;

		$sAppName = basename($sPathOrig);

		$aSearches = array(
			'##PROPEL_GEN_BIN##',
			'##PHOCOA_APP_DIR##',
			'##PHOCOA_APP_CONTAINER_DIR##',
			'##DEFAULT_BASE_TEMPLATE_DIR##',
			'##FRAMEWORK_DIR##',
			'##SwissalpS_FRAMEWORK_DIR##',
			'##SMARTY_DIR##',
			'##PROPEL_FRAMEWORK_DIR##',
			'##HORDE_FRAMEWORK_DIR##',
			'##PEAR_LOG_FRAMEWORK_DIR##',
			'##PHING_PATH##',
			'##PROPEL_DATABASE##',
			'##SERVER_IP##',
			'##SERVER_NAME##',
			'##SERVER_PORT##',
			'##LOG_DIR##',
			);
		$aReplacements = array(
			'##PATH_BIN_PROPEL##',
			'##PATH_DIR_PHOCOA_APP##',
			'##PATH_DIR_PHOCOA_APP_CONTAINER##',
			'##PATH_DIR_DEFAULT_BASE_TEMPLATE##',
			'##PATH_DIR_SRC_PHOCOA##',
			'##PATH_DIR_SRC_SwissalpSphp##',
			'##PATH_DIR_SRC_SMARTY##',
			'##PATH_DIR_SRC_PROPEL##',
			'##PATH_DIR_SRC_HORDE##',
			'##PATH_DIR_SRC_PEAR_LOG##',
			'##PATH_DIR_SRC_PHING##',
			'##DB_TYPE##',
			'##HTTPD_IP##',
			'##HTTPD_NAME##',
			'##HTTPD_PORT##',
			'##PATH_DIR_LOG##',
			);

		$iCountTemplatesDone = 0;
		foreach ($aTemplates as $sPathFile) {

			$sPath = trim($sPathFile);
			if (empty($sPath)) continue;

			// backup
			copy($sPath, $sPath . '~');

			$sNameTemplate = basename($sPath);
			$sName = basename($sPath, '.SssStemplate');
			if ('runner.phps' == $sName) $sName = $sAppName;
			$sPath = dirname($sPath) . DIR_SEP;
			$sIn = file_get_contents_safely($sPath . $sNameTemplate);
			$sOut = str_replace($aSearches, $aReplacements, $sIn);
			file_put_contents_safely($sPath . $sName, $sOut, 'wb');

			// replicate mode of original
			chmod($sPath . $sName, filePerms($sPath . $sNameTemplate));

			$iCountTemplatesDone++;

		} // loop each file

		return $iCountTemplatesDone;

	} // updateTemplatesVersion2Or3To4


	public static function warnVersion() {

		echo NL . NL . '! ! ! Warning, you are using an old version plist ! ! !' . NL . ' update with ....' . NL . NL;

	} // warnVersion

} // SssSphocoaGenerator

/* * * *\ SssSphocoaGenerator.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
