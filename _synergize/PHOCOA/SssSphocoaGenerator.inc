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
 * * @version 20110619_205946 (CC) Luke JZ aka SwissalpS ver 2 plists with all keys being used as searches
 * * @version 20100501_081510 (CC) Luke JZ aka SwissalpS
 * * * */
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_Plist.inc');
	require_once('SssS_CLI_Tools.inc');
	require_once('SssS_ShellScriptRunnerForDummies.inc');
} // if not autoincluding but on php's include path

if (!defined('DIR_SEP')) define('DIR_SEP', DIRECTORY_SEPARATOR);

class SssSphocoaGenerator {

	const sAppleScriptletBitNew = '/SssSphocoaGen/newSssSphocoaProjectFromPlist.app/Contents/Resources/newSssSphocoaProject';
	const sAppleScriptletBitInPlace = '/SssSphocoaGen/newSssSphocoaProjectFromPlist.app/Contents/Resources/remakeTemplatedFilesWithInfoFromPlist';

	// __construct()
	public function SssSphocoaGenerator() {} // SssSphocoaGenerator

	// dealloc
	static public function bail() {	echo chr(10) . 'SssSphocoaGenerator exiting...' . chr(10); exit(0); } // bail

	// main cli 'inlet'
	static public function runCLI() {
		$a = $_SERVER['argv'];
		$sInvocation = array_shift($a); // get $0 path
		//echo chr(10) . $sInvocation . '<-sInvocation-';
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

			else $aPaths[] = array($s, $bUseAllDefaults, $sPathDest, $sAppName);
				// got a path to a plist (if correct syntax etc) theoretically could be a string xml:plist

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
						chr(10), $iYes, count($aPaths));

		// debug fails if any
		if (!empty($aFails)) echo 'failed paths: ' . print_r($aFails, true);

		self::bail();
	} // runCLI


	// cli 'inlet'
	static public function runCLIinPlaceReplacement() {
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
		echo sprintf('%1$ssuccessful in %2$s out of %3$s files%1$s',
						chr(10), $iYes, $iCount);

		// debug fails if any
		if (!empty($aFails)) echo 'failed paths: ' . print_r($aFails, true);

		self::bail();

	} // runCLIinPlaceReplacement


	// cli 'inlet'
	static public function runCLIupdatePlistVersion1To2() {

		$a = $_SERVER['argv'];
		$sInvocation = array_shift($a); // get $0 path
		$sPathInvocationBase = self::parseInvocationBase($sInvocation);

		$aFails = array(); $iYes = 0; $iCount = count($a);

		while (0 < count($a)) {

			$mPlist = self::trimOrUnescapeShellArgument(array_shift($a));

			if (self::updatePlistVersion1To2($mPlist, null, $sPathInvocationBase))
				$iYes++; // count success

			else
				$aFails[] = array('mPlist' => $mPlist,
					 'sPathInvocationBase' => $sPathInvocationBase);

		} // while have arguments

		// good bye
		echo sprintf('%1$ssuccessfully updated %2$s out of %3$s files%1$s',
						chr(10), $iYes, $iCount);

		// debug fails if any
		if (!empty($aFails)) echo 'failed paths: ' . print_r($aFails, true);

		self::bail();

	} // runCLIupdatePlistVersion1To2


	// default values
	static public function defaultPlist($iVersion = 1) {

		$a = array();

		switch($iVersion) {

			case 1 :

				$sConf = 'SssSphocoaGeneratorDefault1.conf'; break;

			case 2 :
			default :

				$sConf = 'SssSphocoaGeneratorDefault.conf'; break;

		} // switch version

		@include($sConf);

		return new SssS_Plist($a);

	} // defaultPlist


	// main interactive wizard
	static public function makeNewSssSphocoaProject($bUseAllDefaults = false) {

		self::makeNewSssSphocoaProjectWithPlist(self::defaultPlist(0), $bUseAllDefaults);

	} // makeNewSssSphocoaProject


	// make from settings in a plist file interactively changing or complementing arguments
	static public function makeNewSssSphocoaProjectWithPlist(
			$mPlist = null, // SssS_Plist object a path or a plist xml string or an array
			$bUseAllDefaults = false,
			$sPathDest = false,
			$sPathInvocationBase = null, // alternative root calculated from invocation path
			$sAppName = false)
	{

		static $rts = null; static $ats = null; static $gi = null;
		static $nl = null; if (!$nl) {
			$nl = chr(10);
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
				$oP->getOrSet('DEFAULT_BASE_TEMPLATE_DIR', $sPathInvocationBase);

		} else { // is not an SssS_Plist or subclass
			// mPlist is plist string or path
			if (('string' == $sArgType) || ('array' == $sArgType)) {
				$oP = new SssS_Plist($mPlist);
				if ($sPathInvocationBase)
					$oP->getOrSet('DEFAULT_BASE_TEMPLATE_DIR', $sPathInvocationBase);

			} // if string (path or xml) or array as mPlist
			else {
				$oP = self::defaultPlist(0);
				if ($sPathInvocationBase)
					$oP->set('DEFAULT_BASE_TEMPLATE_DIR', $sPathInvocationBase);

			} // if mPlist type handling
		} // if no useable mPlist

		if (2 > $oP->getInt('SssSphocoaGeneratorVersion', 1)) {

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
		echo $nl .  $nl . '     SssSphocoaGenerator::makeNewSssSphocoaProjectWithPlist()' . $nl;

		$oRunner = self::sharedSssS_ShellScriptRunnerForDummies();

		$sPathOrig = ($sPathDefaultBaseTemplate)
			? SssS_CLI_Tools::$rts($sPathDefaultBaseTemplate, DIR_SEP) : ($sPathInvocationBase)
				? $sPathInvocationBase : '';

		$sPathDest = ($sPathDest)
				?  SssS_CLI_Tools::$ats($sPathDest, DIR_SEP) . $sAppName
				: $oP->getOrSet('PHOCOA_APP_CONTAINER_DIR', $sPathCurentWorkingDirectory . DIR_SEP . $sAppName);

	if ($bUseAllDefaults) {
		echo 'not interacting for values...' . $nl;
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
			echo 'original template path not found: ' . $sPathOrig . $nl;
			return false;
		} // if no source;

	// get app name
		// ##PHOCOA_PROJECT_NAME##
		$sAppName = SssS_CLI_Tools::$gi(
								$nl . 'Name of new App (sAppName):', $sAppName);
		$oP->set('sAppName', $sAppName);

	// get target path
		// ##PHOCOA_APP_CONTAINER_DIR##
		$sPathDest = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
										$nl . 'Path of new App (PHOCOA_APP_CONTAINER_DIR):',
										substr($sPathDest, 0, strlen($sPathDest) - strlen(strrchr($sPathDest, DIR_SEP))) . DIR_SEP
										. $sAppName), DIR_SEP);
		$oP->set('PHOCOA_APP_CONTAINER_DIR', $sPathDest);

		echo $nl;

		if ($oRunner->isSomethingAtPath($sPathDest)) {
			echo 'destination already exists: ' . $sPathDest . $nl;
			return false;
		} // if destination already exists;

	// set target app-root
		// APP_ROOT', '##PHOCOA_APP_DIR##
		$sPathDestAppRoot = $sPathDest . DIR_SEP . $sAppName;
		$oP->set('PHOCOA_APP_DIR', $sPathDestAppRoot);

	// get app type
		// for application delegate
		$sAppType = SssS_CLI_Tools::getChoiceCLI('Type of app (cli|web|both)',
				array('cli' => 'Command Line Tool', 'web' => 'WFWebApplication',
				'both' => 'both types'), 'both types');
		//echo 'currently only supporting cli, so set to cli' . $nl . $nl;
		//$sAppType = 'cli';

	// httpd.conf takes also servername, ip and port
		if ('Command Line Tool' != $sAppType) {
			$sHTTPip = SssS_CLI_Tools::$gi($nl . 'IP (SERVER_IP):', $sHTTPip);
			$oP->set('SERVER_IP', $sHTTPip);
			$iHTTPPort = intval(
					SssS_CLI_Tools::$gi($nl . 'port (SERVER_PORT): ', $iHTTPPort));
			$oP->set('SERVER_PORT', $iHTTPPort);
			$sHTTPHost = SssS_CLI_Tools::$gi(
									$nl . 'hostname (SERVER_NAME):', $sHTTPHost);
			$oP->set('SERVER_NAME', $sHTTPHost);
			echo $nl;
		} // if not cli

	// ask for log dir
		$sPathLog = $sPathDest . DIR_SEP . 'log';
		$sPathLog = SssS_CLI_Tools::$gi($nl . 'log path:', $sPathLog);

	// ask if uses propel
		$sUsePropel = SssS_CLI_Tools::getChoiceCLI('Use propel for db?',
				array('no' => 'NO', 'yes' => 'YES'), 'yes');
		if ('yes' == strtolower($sUsePropel)) {
			// get db props
			echo $nl;
		// get propel path
			$sPathPropelFrameWork = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
				$nl . 'path to propel', $sPathPropelFrameWork));
			echo $nl;
			$sDBtype = SssS_CLI_Tools::getChoiceCLI('Choose database type (PROPEL_DATABASE):',
					array('pgsql' => 'Postgres', 'mysql' => 'MySQl',
						  'mssql' => 'mssql', 'sqllite' => 'sqllite',
						   'ldap' => 'ldap'), $sDBtype);
			$oP->set('PROPEL_DATABASE', $sDBtype);
			$sDBname = SssS_CLI_Tools::$gi($nl . 'DB name:', $sDBname);
			$oP->set('DB_NAME', $sDBname);
			$sDBuser = SssS_CLI_Tools::$gi($nl . 'DB username:', $sDBuser);
			$oP->set('DB_USER', $sDBuser);
			$sDBpass = SssS_CLI_Tools::$gi($nl . 'DB password:', $sDBpass);
			$oP->set('DB_PASS', $sDBpass);
			$sDBhost = SssS_CLI_Tools::$gi($nl . 'DB hostname:', $sDBhost);
			$oP->set('DB_HOST', $sDBhost);
			$sPathPropelGenerator = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi($nl . 'path to propel-gen binary', $sPathPropelGenerator));
			$oP->set('PROPEL_GEN_BIN', $sPathPropelGenerator);

		} // if propel

		echo $nl;

	} // if ask for anything

	// copy the bare-bone structure
		// make destination path
		$bRes = $oRunner->makePath($sPathDest);
		$bPHPcompatiblePathDest = true;
		if ((!is_dir($sPathDest)) && $bRes) {
			echo $nl . $nl . 'destination path is not PHP compatible, problems'
					. ' to be expected' . $nl;
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
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
			return false;
		} // if copy error

		// copy app dir -> Project dir and app dir MUST bear same name
		$sPathOrigAppRoot = $sPathOrig . DIR_SEP
									. substr(strrchr($sPathOrig, DIR_SEP), 1);
		$bRes = $oRunner->copyPreserving($sPathOrigAppRoot,	$sPathDestAppRoot);
		if (!$bRes) {
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
			return false;
		} // if copy error
		// also here PHP fails
		//if (false === copy($sPathOrig, $sPathDest)) {
		//	echo 'error making copy';
		//	return false;
		//} // if copy fail

		// copy bin dir
		//if (!$oRunner->copyPreserving($sPathOrig . '/AppRoot/bin', $sPathDestAppRoot . '/bin')) { echo 'error with copy' . $nl; var_dump($oRunner->aResults); return false; } // if copy error

		// copy replicator dir holding info and applescript after the plists
		$sPathGen = $sPathDest . DIR_SEP . 'SssSphocoaGen';
		if (!$oRunner->copyPreserving(
						$sPathOrig . DIR_SEP . 'SssSphocoaGen', $sPathGen)) {
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
			//return false;
		} // if copy error
		$sPathGen .= DIR_SEP;

		// copy runner.phps.SssStemplate -> sAppName
		$sPathRunner = $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP . $sAppName;
		$sPathRunnerOrig = $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP
													. 'runner.phps.SssStemplate';
		if (!$oRunner->copyPreserving($sPathRunnerOrig, $sPathRunner)) {//moveOrRename
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
			return false;
		} // if rename error

		// make sure there isn't any old symlink or empty directory...
		$oRunner->doScript('rm -dfv "' . $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP . 'run"');
		// make a symlink for easy running
		$bRes = $oRunner->symlinkToAt($sPathRunner, $sPathDestAppRoot . DIR_SEP
													. 'bin' . DIR_SEP . 'run');
		if (!$bRes) {
			echo 'error with alias creation' . $nl; var_dump($oRunner->aResults);
			return false;
		} // if symlink error

	// config files

		$sPathConf = $sPathDestAppRoot . DIR_SEP . 'conf' . DIR_SEP;

		// copy propel-conf.xml.SssStemplate -> sAppName-conf.xml
		//$sPathPropelConf = $sPathConf . $sAppName . '-conf.xml';
		//$sPathPropelConfOrig = $sPathConf . 'propel-conf.xml.SssStemplate';
		//if (!$oRunner->copyPreserving($sPathPropelConfOrig, $sPathPropelConf)) {
		//	echo 'error with copy' . $nl; var_dump($oRunner->aResults);
		//	return false;
		//} // if rename error

		// .htaccess.SssStemplate
		$sPathHTAconf = $sPathConf . '.htaccess';
		$sPathHTAconfOrig = $sPathConf . '.htaccess.SssStemplate';
		if (!$oRunner->copyPreserving($sPathHTAconfOrig, $sPathHTAconf)) {
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// httpd.conf.SssStemplate
		$sPathHTTPdConf = $sPathConf . 'httpd.conf';
		$sPathHTTPdConfOrig = $sPathConf . 'httpd.conf.SssStemplate';
		if (!$oRunner->copyPreserving($sPathHTTPdConfOrig, $sPathHTTPdConf)) {
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
			//return false;
		} // if copy error

		// copy SssSphocoaApp.conf.SssStemplate
		$sPathWebAppConf = $sPathConf . 'SssSphocoaApp.conf';
		$sPathWebAppConfSyml = $sPathConf . 'webapp.conf';
		$sPathWebAppConfOrig = $sPathConf . 'SssSphocoaApp.conf.SssStemplate';
		if (!$oRunner->copyPreserving($sPathWebAppConfOrig, $sPathWebAppConf)) {
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
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
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
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
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
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

	} // makeNewSssSphocoaProjectWithPlist


	// make from settings in a plist file interactively changing or complementing arguments
	static public function makeNewSssSphocoaProjectWithPlist1(
			$mPlist = null, // SssS_Plist object a path or a plist xml string or an array
			$bUseAllDefaults = false,
			$sPathDest = false,
			$sPathInvocationBase = null, // alternative root calculated from invocation path
			$sAppName = false)
	{

		static $rts = null; static $ats = null; static $gi = null;
		static $nl = null; if (!$nl) {
			$nl = chr(10);
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
		echo $nl .  $nl . '     SssSphocoaGenerator::makeNewSssSphocoaProjectWithPlist1()' . $nl;

		$oRunner = self::sharedSssS_ShellScriptRunnerForDummies();

		$sPathOrig = ($sPathDefaultBaseTemplate)
			? SssS_CLI_Tools::$rts($sPathDefaultBaseTemplate, DIR_SEP) : ($sPathInvocationBase)
				? $sPathInvocationBase : '';

		$sPathDest = ($sPathDest)
				?  SssS_CLI_Tools::$ats($sPathDest, DIR_SEP) . $sAppName
				: $oP->getOrSet('sPathDest', $sPathCurentWorkingDirectory . DIR_SEP . $sAppName);

	if ($bUseAllDefaults) {
		echo 'not interacting for values...' . $nl;
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
			echo 'original template path not found: ' . $sPathOrig . $nl;
			return false;
		} // if no source;

	// get app name
		// ##PHOCOA_PROJECT_NAME##
		$sAppName = SssS_CLI_Tools::$gi(
								$nl . 'Name of new App (sAppName):', $sAppName);
		$oP->set('sAppName', $sAppName);

	// get target path
		// ##PHOCOA_APP_CONTAINER_DIR##
		$sPathDest = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
										$nl . 'Path of new App (sPathDest):',
										substr($sPathDest, 0, strlen($sPathDest) - strlen(strrchr($sPathDest, DIR_SEP))) . DIR_SEP
										. $sAppName), DIR_SEP);
		$oP->set('sPathDest', $sPathDest);

		echo $nl;

		if ($oRunner->isSomethingAtPath($sPathDest)) {
			echo 'destination already exists: ' . $sPathDest . $nl;
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
		//echo 'currently only supporting cli, so set to cli' . $nl . $nl;
		//$sAppType = 'cli';

	// httpd.conf takes also servername, ip and port
		if ('Command Line Tool' != $sAppType) {
			$sHTTPip = SssS_CLI_Tools::$gi($nl . 'IP (sHTTPip):', $sHTTPip);
			$oP->set('sHTTPip', $sHTTPip);
			$iHTTPPort = intval(
					SssS_CLI_Tools::$gi($nl . 'port (iHTTPPort): ', $iHTTPPort));
			$oP->set('iHTTPPort', $iHTTPPort);
			$sHTTPHost = SssS_CLI_Tools::$gi(
									$nl . 'hostname (sHTTPHost):', $sHTTPHost);
			$oP->set('sHTTPHost', $sHTTPHost);
			echo $nl;
		} // if not cli

	// ask if uses propel
		$sPathLog = $sPathDest . DIR_SEP . 'log';
		$sUsePropel = SssS_CLI_Tools::getChoiceCLI('Use propel for db?',
				array('no' => 'NO', 'yes' => 'YES'), 'yes');
		if ('yes' == strtolower($sUsePropel)) {
			// get db props
			$sPathLog = SssS_CLI_Tools::$gi(
											$nl . 'log path:', $sPathLog);
			$oP->set('sPathLog', $sPathLog);
			echo $nl;
		// get propel path
			$sPathPropelFrameWork = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi(
				$nl . 'path to propel', $sPathPropelFrameWork));
			echo $nl;
			$sDBtype = SssS_CLI_Tools::getChoiceCLI('Choose database type:',
					array('pgsql' => 'Postgres', 'mysql' => 'MySQl',
						  'mssql' => 'mssql', 'sqllite' => 'sqllite',
						   'ldap' => 'ldap'), $sDBtype);
			$oP->set('sDBtype', $sDBtype);
			$sDBname = SssS_CLI_Tools::$gi($nl . 'DB name:', $sDBname);
			$oP->set('sDBname', $sDBname);
			$sDBuser = SssS_CLI_Tools::$gi($nl . 'DB username:', $sDBuser);
			$oP->set('sDBuser', $sDBuser);
			$sDBpass = SssS_CLI_Tools::$gi($nl . 'DB password:', $sDBpass);
			$oP->set('sDBpass', $sDBpass);
			$sDBhost = SssS_CLI_Tools::$gi($nl . 'DB hostname:', $sDBhost);
			$oP->set('sDBhost', $sDBhost);
			$sPathPropelGenerator = SssS_CLI_Tools::$rts(SssS_CLI_Tools::$gi($nl . 'path to propel-gen binary', $sPathPropelGenerator));
			$oP->set('sPathPropelGenerator', $sPathPropelGenerator);

		} // if propel

		echo $nl;

	} // if ask for anything

	// copy the bare-bone structure
		// make destination path
		$bRes = $oRunner->makePath($sPathDest);
		$bPHPcompatiblePathDest = true;
		if ((!is_dir($sPathDest)) && $bRes) {
			echo $nl . $nl . 'destination path is not PHP compatible, problems'
					. ' to be expected' . $nl;
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
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
			return false;
		} // if copy error

		// copy app dir -> Project dir and app dir MUST bear same name
		$sPathOrigAppRoot = $sPathOrig . DIR_SEP
									. substr(strrchr($sPathOrig, DIR_SEP), 1);
		$bRes = $oRunner->copyPreserving($sPathOrigAppRoot,	$sPathDestAppRoot);
		if (!$bRes) {
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
			return false;
		} // if copy error
		// also here PHP fails
		//if (false === copy($sPathOrig, $sPathDest)) {
		//	echo 'error making copy';
		//	return false;
		//} // if copy fail

		// copy bin dir
		//if (!$oRunner->copyPreserving($sPathOrig . '/AppRoot/bin', $sPathDestAppRoot . '/bin')) { echo 'error with copy' . $nl; var_dump($oRunner->aResults); return false; } // if copy error

		// copy replicator dir holding info and applescript after the plists
		$sPathGen = $sPathDest . DIR_SEP . 'SssSphocoaGen';
		if (!$oRunner->copyPreserving(
						$sPathOrig . DIR_SEP . 'SssSphocoaGen', $sPathGen)) {
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
			//return false;
		} // if copy error
		$sPathGen .= DIR_SEP;

		// copy runner.phps.SssStemplate -> sAppName
		$sPathRunner = $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP . $sAppName;
		$sPathRunnerOrig = $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP
													. 'runner.phps.SssStemplate';
		if (!$oRunner->copyPreserving($sPathRunnerOrig, $sPathRunner)) {//moveOrRename
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
			return false;
		} // if rename error

		// make sure there isn't any old symlink or empty directory...
		$oRunner->doScript('rm -dfv "' . $sPathDestAppRoot . DIR_SEP . 'bin' . DIR_SEP . 'run"');
		// make a symlink for easy running
		$bRes = $oRunner->symlinkToAt($sPathRunner, $sPathDestAppRoot . DIR_SEP
													. 'bin' . DIR_SEP . 'run');
		if (!$bRes) {
			echo 'error with alias creation' . $nl; var_dump($oRunner->aResults);
			return false;
		} // if symlink error

	// config files

		$sPathConf = $sPathDestAppRoot . DIR_SEP . 'conf' . DIR_SEP;

		// copy propel-conf.xml.SssStemplate -> sAppName-conf.xml
		//$sPathPropelConf = $sPathConf . $sAppName . '-conf.xml';
		//$sPathPropelConfOrig = $sPathConf . 'propel-conf.xml.SssStemplate';
		//if (!$oRunner->copyPreserving($sPathPropelConfOrig, $sPathPropelConf)) {
		//	echo 'error with copy' . $nl; var_dump($oRunner->aResults);
		//	return false;
		//} // if rename error

		// .htaccess.SssStemplate
		$sPathHTAconf = $sPathConf . '.htaccess';
		$sPathHTAconfOrig = $sPathConf . '.htaccess.SssStemplate';
		if (!$oRunner->copyPreserving($sPathHTAconfOrig, $sPathHTAconf)) {
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
			//return false;
		} // if rename error

		// httpd.conf.SssStemplate
		$sPathHTTPdConf = $sPathConf . 'httpd.conf';
		$sPathHTTPdConfOrig = $sPathConf . 'httpd.conf.SssStemplate';
		if (!$oRunner->copyPreserving($sPathHTTPdConfOrig, $sPathHTTPdConf)) {
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
			//return false;
		} // if rename error

		// copy SssSphocoaApp.conf.SssStemplate
		$sPathWebAppConf = $sPathConf . 'SssSphocoaApp.conf';
		$sPathWebAppConfSyml = $sPathConf . 'webapp.conf';
		$sPathWebAppConfOrig = $sPathConf . 'SssSphocoaApp.conf.SssStemplate';
		if (!$oRunner->copyPreserving($sPathWebAppConfOrig, $sPathWebAppConf)) {
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
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
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
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
			echo 'error with copy' . $nl; var_dump($oRunner->aResults);
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


	static function inPlaceRefreshTemplatesInPlist(
			$mPlist = null,
			$aFiles = null,
			$sPathInvocationBase = null)
	{
		if (!$mPlist) return null;

		static $rts = null; static $ats = null; static $gi = null;
		static $nl = null; if (!$nl) {
			$nl = chr(10);
			$rts = 'removeTrailingSlash';
			$ats = 'addTrailingSlash';
			$gi = 'getInputCLI';
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
				$oP = self::defaultPlist(0);
				if ($sPathInvocationBase)
					$oP->set('DEFAULT_BASE_TEMPLATE_DIR', null);
			} // if use defaults

		} // if mPlist is is a SssS_Plist subclass or not

		if (2 > $oP->getInt('SssSphocoaGeneratorVersion', 1)) {

			self::warnVersion();

			return self::inPlaceRefreshTemplatesInPlist1($oP, $aFiles, $sPathInvocationBase);

		} // if not a version 2 plist given -> do old method

		// make sure the base dir is set
		$oP->getOrSet('PHOCOA_APP_CONTAINER_DIR', $sPathInvocationBase);

		// merge given plist over defaults plist and hold in $oP
		$oPd = self::defaultPlist(0);
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

		} // foreach path

		return true;

	} // inPlaceRefreshTemplatesInPlist


	static function inPlaceRefreshTemplatesInPlist1(
			$mPlist = null,
			$aFiles = null,
			$sPathInvocationBase = null)
	{
		if (!$mPlist) return null;

		static $rts = null; static $ats = null; static $gi = null;
		static $aSearches = null;
		static $nl = null; if (!$nl) {
			$nl = chr(10);
			$rts = 'removeTrailingSlash';
			$ats = 'addTrailingSlash';
			$gi = 'getInputCLI';
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
				$oP = self::defaultPlist();
				if ($sPathInvocationBase)
					$oP->set('sPathDefaultBaseTemplate', null);
			} // if use defaults

		} // if mPlist is is a SssS_Plist subclass or not

		$oP->getOrSet('sPathDefaultBaseTemplate', $sPathInvocationBase);

		$oPd = self::defaultPlist();
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
		} // foreach path

		return true;
	} // inPlaceRefreshTemplatesInPlist1 // refreshSetOfConfigToPlistValues or something of the kind to make an alternative .conf for different deployment or testing
	// scan current approot for any files ending with .SssStemplate
	// for each make a copy removing the suffix and overwrite existing


	static function findTemplatesInProject($sPathOrig = null) {

		$oRunner = self::sharedSssS_ShellScriptRunnerForDummies();

		if (!$oRunner->isFolder($sPathOrig)) return array();

		// make sure find gets it that we are looking at a folder....
		$oRunner->doScript('find -L "' . $sPathOrig . '" -name "*.SssStemplate"');

		return explode("\n", $oRunner->sOut());

	} // findTemplatesInProject


	static public function parseInvocationBase($sInvocation) {

		$sPathInvocationBase = null;

		if (false !== strstr($sInvocation, self::sAppleScriptletBitInPlace)) {
			$sPathInvocationBase = substr($sInvocation, 0, strlen($sInvocation) - strlen(self::sAppleScriptletBitInPlace));
		} // if got an optional source path because run from winthin realpath()
		// AppleScript.app in a SssSphocoaGen directory.
		// That directory could be used as sPathDefaultBaseTemplate
		// if sPathDefaultBaseTemplate is missing in the plist

		return $sPathInvocationBase;

	} // parseInvocationBase


	static function sharedSssS_ShellScriptRunnerForDummies() {
		static $oRunner = null; if (!$oRunner)
			$oRunner = new SssS_ShellScriptRunnerForDummies();
		return $oRunner;
	} // sharedSssS_ShellScriptRunnerForDummies


	static function showHelpCLI() {

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


	static public function trimOrUnescapeShellArgument($s) {

		return ('"' == $s{0} || "'" == $s{0})
					? trim($s, "\" \t'") 		// trim whitespace and ' "
					: str_replace('\\', '', $s);// or remove \ escapes --> this endangers sending clear xml plist todo: check xml header() {

	} // trimOrUnescapeShellArgument


	static public function updatePlistVersion1To2($sPlistPath) {

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


	static public function warnVersion() {

		echo chr(10) . chr(10) . '! ! ! Warning, you are using an old version plist ! ! !' . chr(10) . ' update with ....' . chr(10) . chr(10);

	} // warnVersion

} // SssSphocoaGenerator

/* * * *\ SssSphocoaGenerator.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
