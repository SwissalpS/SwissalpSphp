<?php
/* * * *
 * * SssS_LogRotator.inc
 * *
 * * @version 20101128_212847 (CC) Luke JZ aka SwissalpS
 * * * */
//error_reporting(E_ALL); // 0); //
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_ShellScriptRunnerForDummies.inc');
} // if not autoincluding but on php's include path


//$oTest = new SssS_LogRotator('/path/to/logs', 3, 50);
//$oTest->run();

class SssS_LogRotator {

	/**
	 * @protected array holds runtime errors
	 */
	protected $aErrors;


	/**
	 * __construct($sLogDir, $iNumOfOld = 3, $iMaxKBytes = 500)
	 * construct the object
	 *
	 * @param string $sLogDir The path to the directory to work on (subdirs included)
	 * @param int $iNumOfOld how many rotation files to keep (defaults to: 3 minimum 1)
	 * @param int $iMaxKBytes how large may the files be before we rotote them (defaults to: 500)
	 *
	 * @package __SssS__ - __package_desciption_here__
	 * @author Luke JZ aka SwissalpS <SwissalpS@LukeZimmerman.com>
	 * @license __license_url_here__
	 * @copyright 2007 Luke JZ aka SwissalpS
	 * @version 20101128_162929 (CC)2007 Luke JZ aka SwissalpS
	 */
	function __construct($sLogDir, $iNumOfOld = 3, $iMaxKBytes = 500) {

		$this->aErrors = array();

		$this->sLogDir = $sLogDir;

		$this->iNumOfOld = Max(1, $iNumOfOld);

		$this->iMaxKBytes = $iMaxKBytes;

	} // __construct


	/**
	 * run()
	 * __Function_description_here__
	 *
	 * @return bool if success or fail
	 *
	 * @uses SssS_ShellScriptRunnerForDummies - to run the shell script: du -k -a logDir
	 *
	 * @package __SssS__ - __package_desciption_here__
	 * @author Luke JZ aka SwissalpS <SwissalpS@LukeZimmerman.com>
	 * @copyright 2007 Luke JZ aka SwissalpS
	 * @version 20101128_154135 (CC)2007 Luke JZ aka SwissalpS
	 */
	function run() {

		$this->aErrors = array();

		static $oRunner = null; if (!$oRunner) $oRunner = new SssS_ShellScriptRunnerForDummies;

		$sScript = 'du -a -k "' . $this->sLogDir . '"';

		$oRunner->doScript($sScript);

		if (0 != $oRunner->iRes()) $this->aErrors[] = 'Error gathering files: ' . $oRunner->sLline();

		$aLines = explode("\n", $oRunner->sOut());

		// first filter by size
		$aPottentials = array();
		$aToRotate = array();
		foreach ($aLines as $sLine) {

			$aParts = explode("\t", $sLine);

			if (!isset($aParts[1])) continue;

			$sPathFile = $aParts[1];

			if (is_dir($sPathFile) or is_link($sPathFile)) continue;

			if ($this->iMaxKBytes > intval($aParts[0])) continue;

			$sFile = basename($sPathFile);

			// ignore invisibles
			if ('.' == $sFile{0}) continue;

			// ignore those with tailing numbers
			if (is_numeric(substr($sFile, -1))) continue;

			$sPath = dirname($sPathFile) . '/';

			$aToRotate[] = array('file' => $sFile, 'path' => $sPath);

			for ($i = $this->iNumOfOld; $i > 1; $i--) {

				//remove old?

				$sOrig = $sPathFile . ($i-1); $sDest = $sPathFile . $i;

				if (is_file($sOrig)) {

					echo 'mv ' . $sOrig . ' ' . $sFile . $i . chr(10);
					$oRunner->moveOrRename($sOrig, $sDest);

					if (0 != $oRunner->iRes()) $this->aErrors[] = 'Error moving file ' . $sOrig . ' : ' . $oRunner->sLline();

				} // if orig exists

			} // for N to 1

			$sOrig = $sPathFile; $sDest = $sPathFile . 1;
			echo 'mv ' . $sFile . ' ' . $sFile . 1 . chr(10);
			$oRunner->moveOrRename($sOrig, $sDest);

			if (0 != $oRunner->iRes()) $this->aErrors[] = 'Error moving file ' . $sOrig . ' : ' . $oRunner->sLline();

		} //

		return empty($this->aErrors);

	} // run


	/**
	 * getErrors()
	 * array of errors that occured
	 *
	 * @return array of errors that occured
	 *
	 * @package __SssS__ - __package_desciption_here__
	 * @author Luke JZ aka SwissalpS <SwissalpS@LukeZimmerman.com>
	 * @copyright 2007 Luke JZ aka SwissalpS
	 * @version 20101128_170056 (CC)2007 Luke JZ aka SwissalpS
	 */
	function getErrors() {

			return $this->aErrors;

	} // getErrors

} // SssS_LogRotator
/* * * *\ SssS_LogRotator (CC) Luke JZ aka SwissalpS /* * * */
?>
