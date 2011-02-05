<?php
/* * * *
 * * SssS_PrettySecureLogon.inc
 * * pretty secure logon
 * *
 * * primarily written for non-browser based transfers
 * * e.g. in scripts for syncing small portions of data.
 * * in Cocoa apps for logon to services such as payments
 * * or registration processes.
 * *
 * * can also be seen as a https post with cookies using curl
 * *
 * * @version 20091024_203207 + file_get/put_contents_safely
 * * @version 20090822_002147 (CC) Luke JZ aka SwissalpS
 * * * */
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_ShellScriptRunner.inc');
} // if not autoincluding but on php's include path

class SssS_PrettySecureLogon extends SssS_ShellScriptRunner {
	//to be used to logon or transmit sensitive information
	/**
	 * SssS_PrettySecureLogon($aNouns, $sURL, $aVerbs, $sCookieJarPath)
	 * constructor
	 *
	 * @param array $aNouns key value pairs that will be posted and cookied (paramaters you don't want to risk being injected) e.g. username, uid, pass. Avoid '=-@<' characters and urlencode where needed in some cases also '%'
	 * @param string $sCApemPath path to certificate. if left out, will accept all -> insecure
	 * @param string $sURL 'https://' is added if missing.
	 * @param array $aVerbs (optional) POST only key value pairs
	 * @param string $sCookieJarPath (optional) path to write recieved cookies to. Check that it's writable as no error is issued on fail. Is only written to if cookies are recieved in reply. This method attempts to delete the file in order to notify if it was used. Please don't put this file in areas like /tmp where others may read it. The format of the cookie file is: domain, tailmatch, path, secure, expires, name, value
	 *
	 * think: nouns must authenticate at url for verbs to be executed. The result ends up in the cookie-jar, sResult and iResult (only after fExecute)
	 *
	 * @version 20090718_223144 (CC)2007 Luke JZ aka SwissalpS
	 */
	function SssS_PrettySecureLogon($aNouns = null, $sURL = null, $sCApemPath = null, $aVerbs = null, $sCookieJarPath = null) {
		$this->SssS_ShellScriptRunner();
		$this->fPrepareCApath(&$sCApemPath);
		$this->fPrepareCookieJarPath(&$sCookieJarPath);
		$this->fPrepareNouns(&$aNouns);
		$this->fPrepareURL(&$sURL);
		$this->fPrepareVerbs(&$aVerbs);
	} // SssS_PrettySecureLogon

	/**
	 * fExecute()
	 * execute the curl call and return results
	 *
	 * @return void return_description_here
	 *
	 *
	 * @uses something - uses_desciption_here
	 *
	 * @version 20090719_042158 (CC)2007 Luke JZ aka SwissalpS
	 */
	function fExecute() {
		$sCurlLine = 'curl ';
		$sCurlLine .= ($this->sCApemPath)
				//? '--cacert "' . $this->sCApemPath . '" '
				? '--cacert ' . escapeshellarg($this->sCApemPath) . ' '
				: '--insecure ';
		//$sCurlLine .= '-c "' . $this->sCookieJarPath . '" ';
		$sCurlLine .= '-c ' . escapeshellarg($this->sCookieJarPath) . ' ';

		$sCookies = '';
		$sFormstring = '';
		foreach ($this->aNouns as $sKey => $sValue) {
			$sCookies .= $sKey . '=' . $sValue . ';';
			$sFormstring .= '--form-string '
					. escapeshellarg($sKey . '2=' . $sValue) . ' ';
		} // foreach noun
		$sCookies = '-b ' . escapeshellarg(substr($sCookies, 0, -1)) . ' ';

		foreach ($this->aVerbs as $sKey => $sValue) {
			$sFormstring .= '--form-string '
					. escapeshellarg($sKey . '=' . $sValue) . ' ';
		} // foreach verb
		$sCurlLine .= $sCookies . $sFormstring;

		// finally the url
		$sCurlLine .= $this->sURL;

		//echo $sCurlLine;
		// TODO: write to file as shell script and execute that to keep the arguments from appearing in logs etc.
		// then remove the file again http://www.php.net/manual/en/keyword.parent.php
		$this->doScript($sCurlLine);

		// get cookie results
		$this->aResults['recievedCookies'] = (is_file($this->sCookieJarPath))
				? file_get_contents_safely($this->sCookieJarPath) : '';

		// for clarity...mainly...
		$this->aResults['curlExitCode']	&= $this->aResults['scriptExitCode'];

		return $this->aResults;
	} // fExecute


	/**
	 * fCleanArray($aIn, $sPrefixNumericalIndexes)
	 * Function_description_here
	 *
	 * @param array $aIn parameter_description_here
	 * @param string $sPrefixNumericalIndexes if keys are numerical or begin numerically, this is prefixed
	 *
	 * @return array the array of nouns cleaned up
	 *
	 * @version 20090719_051054 (CC)2007 Luke JZ aka SwissalpS
	 */
	private function fCleanArray($aIn = null, $sPrefixNumericalIndexes = 'p') {
		$aClean = array();
		if ($aIn && 'array' == gettype($aIn)) {
			foreach ($aIn as $mKey => $mValue) {
				// check key
				$sCleanKey = ('integer' == gettype($mKey)
					|| ('integer' == gettype($mKey{0}))) // first char
						? $sPrefixNumericalIndexes . strval($mKey)
						: $mKey;

				// check value. must be string
				switch (gettype($mValue)) {
					case 'boolean' :
						$sCleanValue = ($mValue) ? 'true' : 'false'; break;
					case 'NULL' :
						$sCleanValue = 'NULL'; break;
					default :
						$sCleanValue = strval($mValue);
				} // switch

				$aClean[$sCleanKey] = $sCleanValue;
			} // foreach
		} // if valid array

		return $aClean;
	} // fCleanArray


	/**
	 * fGetDefaultCookieJarPath()
	 * for easy subclassing
	 *
	 * @return string path to read & writeable location
	 *
	 * @version 20090719_172811 (CC)2007 Luke JZ aka SwissalpS
	 */
	private function fGetDefaultCookieJarPath() {
		return '/tmp/SssS_treats';
	} // fGetDefaultCookieJarPath


	/**
	 * fGetDefaultNouns()
	 * Function_description_her
	 *
	 * @return array return_description_here
	 *
	 * @version 20090719_165908 (CC)2007 Luke JZ aka SwissalpS
	 */
	private function fGetDefaultNouns() {
		return array(
		'SssS_noun' => 'stranger',
		'SssS_id'	=> '0',
		'SssS_p'	=> '0',
		'SssS_t'	=> fGetCurrentToken());
	} // fGetDefaultNouns


	/**
	 * fGetDefaultURL()
	 * intended for subclassing
	 *
	 * @return string url must beginn with https!
	 *
	 * @version 20090719_170225 (CC)2007 Luke JZ aka SwissalpS
	 */
	private function fGetDefaultURL() {
		// hint: append ' > outputFile' to direct output to file
		return 'https://cash.swissalps.biz/wg7wire.tmf?defurl=1';
	} // fGetDefaultURL

	/**
	 * fPrepareCApath($sCApemPath)
	 * Function_description_here
	 *
	 * @param string $sCApemPath parameter_description_here
	 *
	 * @return string/boolean the validated path or false
	 *
	 * @version 20090719_153231 (CC)2007 Luke JZ aka SwissalpS
	 */
	private function fPrepareCApath($sCApemPath = null) {
		$this->sCApemPath = false;

		if ($sCApemPath && ('' != trim($sCApemPath))) {
			$s = trim($sCApemPath);
			$this->sCApemPath = (file_exists($s) && is_file($s) && is_readable($s)) ? $s : false;
		} //

		return $this->sCApemPath;
	} // fPrepareCApath


	/**
	 * fPrepareCookieJarPath($sCookieJarPath)
	 * attempt to delete the file...
	 *
	 * @param string $sCookieJarPath path to file
	 *
	 * @return string 'validated path'
	 *
	 * @version 20090719_044748 (CC)2007 Luke JZ aka SwissalpS
	 */
	private function fPrepareCookieJarPath($sCookieJarPath = null) {
		$sDefault = $this->fGetDefaultCookieJarPath();

		if ($sCookieJarPath && ('' != trim($sCookieJarPath))) {
			$sCookieJarPath = trim($sCookieJarPath);
			$this->sCookieJarPath = $sCookieJarPath;

			if (file_exists($sCookieJarPath) && is_file($sCookieJarPath)) {
				if (!unlink($sCookieJarPath)) {
					// couldn't be deleted -> can't be written
					$this->sCookieJarPath = $sDefault;
				} // reset path
			} // file exists and isn't irregular
		} else {
			$this->sCookieJarPath = $sDefault;
		} // pre validate path

		return $this->sCookieJarPath;
	} // fPrepareCookieJarPath


	/**
	 * fPrepareNouns($aNouns, $sPrefixNumericalIndexes)
	 * Function_description_here
	 *
	 * @param array $aNouns parameter_description_here
	 * @param string $sPrefixNumericalIndexes if keys are numerical or begin numerically, this is prefixed
	 *
	 * @return array the array of nouns cleaned up
	 *
	 * @version 20090719_051054 (CC)2007 Luke JZ aka SwissalpS
	 */
	private function fPrepareNouns($aNouns = null, $sPrefixNumericalIndexes = 'p') {
		$aNouns = $this->fCleanArray($aNouns, $sPrefixNumericalIndexes);
		if (0 == count($aNouns)) $aNouns = $this->fGetDefaultNouns();
		return $this->aNouns = $aNouns;
	} // fPrepareNouns


	/**
	 * fPrepareURL($sURL)
	 * Function_description_here
	 *
	 * @param string $sURL parameter_description_here
	 *
	 * @return string return_description_here
	 *
	 * @version 20090719_162737 (CC)2007 Luke JZ aka SwissalpS
	 */
	private function fPrepareURL($sURL = null) {
		$sClean = '';
		$s = trim($sURL);
		if ($sURL && '' != $s) {
			switch (substr($s, 0, 7)) {
				case 'http://' : $sClean = 'https://' . substr($s, 7); break;
				case 'https:/' : $sClean = $s; break;
				default : $sClean = 'https://' . $s;
			} // switch
		} // if url
		if ('' == $sClean) $sClean = $this->fGetDefaultURL();
		return $this->sURL = $sClean;
	} // fPrepareURL


	/**
	 * fPrepareVerbs($aVerbs, $sPrefixNumericalIndexes)
	 * Function_description_here
	 *
	 * @param array $aVerbs parameter_description_here
	 *
	 * @return string return_description_here
	 *
	 * @version 20090719_162832 (CC)2007 Luke JZ aka SwissalpS
	 */
	private function fPrepareVerbs($aVerbs = null, $sPrefixNumericalIndexes = 'p') {
		return $this->aVerbs = $this->fCleanArray($aVerbs, $sPrefixNumericalIndexes);
	} // fPrepareVerbs
} // SssS_PrettySecureLogon
/* * * *\ SssS_PrettySecureLogon.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
