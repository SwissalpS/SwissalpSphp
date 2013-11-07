<?php
/* * * *
 * * SssS_Cypher2.inc
 * * extends SssS_Cypher to never use cleartext on the cl
 * *
 * * to make sure the temporary files are deleted, call cleanupCypher2()
 * * @version 20100503_132059 + autoloading switch and system tmp dir
 * * @version 20091024_203818 + file_get/put_contents
 * * @version 20090815_213258 (CC) Luke JZ aka SwissalpS
 * * * */

/* usage example: * /
$sPass = 'ha ha';
$sContent = 'something secret';
$sCypherCode = 'c9055128424a70e4a5b9d0a'; // => 'des3';

$oCypher = new SssS_Cypher();

// encode
$sEnc = $oCypher->encryptWithClearTextPass($sContent, $sCypherCode, $sPass);

// decode
$sDec = $oCypher->decryptWithClearTextPass($sEnc, $sCypherCode, $sPass);

// output. $oCypher->aResults['scriptExitCode'] is 0 when all went well.
var_dump($sEnc, $sDec, $oCypher->aResults);
/* :usage example */
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_Cypher.inc');
} // if not autoincluding but on php's include path

class SssS_Cypher2 extends SssS_Cypher {

	var $sTP;
	protected $aSettings;

	public function SssS_Cypher2($aArgs = false) {
		$this->SssS_Cypher();

		$this->aSettings = array(
				'tmpPath'		=> sys_get_temp_dir(),
				'passPath'		=> false,
				'fileIn'		=> false,
				'fileOut'		=> false,
				'delPassPath'	=> false,
				'delFileIn'		=> false,
				'delFileOut'	=> false);

		// use aArgs where applicable
		if ('array' == gettype($aArgs)) {
			// base path where to make tmp folder - please set, must exist and be writeable
			if (isset($aArgs['tmpPath'])
					&& is_dir($aArgs['tmpPath'])
					&& is_writeable($aArgs['tmpPath']))
						$this->aSettings['tmpPath'] = $aArgs['tmpPath'];

			// password file path - file containing the password to be used
			if (isset($aArgs['passPath'])
					&& is_file($aArgs['passPath'])
					&& is_readable($aArgs['passPath']))
						$this->aSettings['passPath'] = $aArgs['passPath'];

			// file in path
			if (isset($aArgs['fileIn'])
					&& is_file($aArgs['passPath'])
					&& is_readable($aArgs['passPath']))
						$this->aSettings['fileIn'] = $aArgs['fileIn'];

			// file out path
			if (isset($aArgs['fileOut'])) {
				$sDir = dirname($aArgs['fileOut']);
				if (is_dir($sDir) && is_writeable($sDir))
						$this->aSettings['fileOut'] = $aArgs['fileOut'];
				unset($sDir);
			} // if probably got out path

			// delete pass file after action
			if (isset($aArgs['delPassPath'])
					&& true === $aArgs['delPassPath'])
						$this->aSettings['delPassPath'] = $aArgs['delPassPath'];

			// delete input file after action
			if (isset($aArgs['delFileIn'])
					&& true === $aArgs['delFileIn'])
						$this->aSettings['delFileIn'] = $aArgs['delFileIn'];

			// delete output file after action
			if (isset($aArgs['delFileOut'])
					&& true === $aArgs['delFileOut'])
						$this->aSettings['delFileOut'] = $aArgs['delFileOut'];

			//if (isset($aArgs[''])) $this->aSettings[''] = $aArgs[''];

			$this->sTP = $this->fMakeSafeUniqueFolderInPath($this->aSettings['tmpPath']);
		} // if got args
	} // SssS_Cypher2

	/* * * * *  *   /PUBLIC enCRYPTING FUNCTIONS\   *  * * * * */

	public function encryptWithClearTextPass($sContent = '', $mCypherCode = '', $sPassword = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delFileIn']	= true;
		$this->aSettings['delFileOut']	= true;
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->encryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithClearTextPass

	public function encryptWithClearTextPassFromFile($mCypherCode = '', $sPassword = '', $sPathIn = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delFileOut']	= true;
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		return $this->encryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithClearTextPassFromFile

	public function encryptWithClearTextPassFromFileToFile($mCypherCode = '', $sPassword = '', $sPathIn = '', $sPathOut = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		return $this->encryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithClearTextPassFromFileToFile

	public function encryptWithClearTextPassToFile($sContent = '', $mCypherCode = '', $sPassword = '', $sPathOut = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delFileIn']	= true;
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->encryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithClearTextPassToFile

	public function encryptWithFilePass($sContent = '', $mCypherCode = '', $sPassPath = '') {
		$sPassBit		= $this->fFilePassBit($sPassPath);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$this->aSettings['delFileIn']	= true;
		$this->aSettings['delFileOut']	= true;
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->encryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithFilePass

	public function encryptWithFilePassFromFile($mCypherCode = '', $sPassPath = '', $sPathIn = '') {
		$sPassBit		= $this->fFilePassBit($sPassPath);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$this->aSettings['delFileOut']	= true;
		return $this->encryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithFilePassFromFile

	public function encryptWithFilePassFromFileToFile($mCypherCode = '', $sPassPath = '', $sPathIn = '', $sPathOut = '') {
		$sPassBit		= $this->fFilePassBit($sPassPath);
		return $this->encryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithFilePassFromFileToFile

	public function encryptWithFilePassToFile($sContent = '', $mCypherCode = '', $sPassPath = '', $sPathOut = '') {
		$sPassBit		= $this->fFilePassBit($sPassPath);
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->encryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithFilePassToFile

	/* * * * *  *   \PUBLIC enCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PUBLIC deCRYPTING FUNCTIONS\   *  * * * * */

	public function decryptWithClearTextPass($sContent = '', $mCypherCode = '', $sPassword = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delFileIn']	= true;
		$this->aSettings['delFileOut']	= true;
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->decryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithClearTextPass

	public function decryptWithClearTextPassFromFile($mCypherCode = '', $sPassword = '', $sPathIn = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delFileOut']	= true;
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		return $this->decryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithClearTextPassFromFile

	public function decryptWithClearTextPassFromFileToFile($mCypherCode = '', $sPassword = '', $sPathIn = '', $sPathOut = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		return $this->decryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithClearTextPassFromFileToFile

	public function decryptWithClearTextPassToFile($sContent = '', $mCypherCode = '', $sPassword = '', $sPathOut = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delFileIn']	= true;
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->decryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithClearTextPassToFile

	public function decryptWithFilePass($sContent = '', $mCypherCode = '', $sPassPath = '') {
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sPassPath);
		$this->aSettings['delFileIn']	= true;
		$this->aSettings['delFileOut']	= true;
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->decryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithFilePass

	public function decryptWithFilePassFromFile($mCypherCode = '', $sPassPath = '', $sPathIn = '') {
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sPassPath);
		$this->aSettings['delFileOut']	= true;
		return $this->decryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithFilePassFromFile

	public function decryptWithFilePassFromFileToFile($mCypherCode = '', $sPassPath = '', $sPathIn = '', $sPathOut = '') {
		$sPassBit		= $this->fFilePassBit($sPassPath);
		return $this->decryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithFilePassFromFileToFile

	public function decryptWithFilePassToFile($sContent = '', $mCypherCode = '', $sPassPath = '', $sPathOut = '') {
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sPassPath);
		$this->aSettings['delFileIn']	= true;
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->decryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithFilePassToFile

	/* * * * *  *   \PUBLIC deCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   PRIVATE FUNCTIONS   *  * * * * */

	private function fClearTextPassBit($sPassword = '') {
		return '-pass pass:' . escapeshellarg($sPassword);
	} // fClearTextPassBit

	private function fFilePassBit($sPassPath = '') {
		return '-pass file:' . escapeshellarg($sPassPath);
	} // fFilePassBit

	/* * * * *  *   /PRIVATE enCRYPTING FUNCTIONS\   *  * * * * */

	private function encryptFromFile($mCypherCode = '', $sPassBit = '', $sPathIn = '') {
		$this->fReset();
		$this->aResults['fullOutput'] = 'Depricated call to private function encryptFromFile of class SssS_Cypher2. Nothing done!';
		$this->aResults['lastLine'] = 3;
		return $this->aResults['fullOutput'];
	} // encryptFromFile

	private function encryptFromFileToFile($mCypherCode = '', $sPassBit = '', $sPathIn = '', $sPathOut = '') {
		if (!is_file($sPathIn)) {
			$this->fReset();
			$this->aResults['fullOutput'] = 'invalid in path';
			$this->aResults['lastLine'] = 2;
			return $this->aResults['fullOutput'];
		} // if invalid in path
		$sInBit = escapeshellarg($sPathIn);
		$sCipher = $this->cypherForKey($mCypherCode);
		$sOutBit = ($sPathOut && ('' != $sPathOut))
				? ' -out ' . escapeshellarg($sPathOut) : '';
		$sScriptEncode = sprintf('openssl enc -%s -a -salt -in %s%s %s'
				, $sCipher, $sInBit, $sOutBit, $sPassBit);
		$this->doScript($sScriptEncode);
		//$this->cleanupCypher2();
//echo 'cl: ' . $sScriptEncode . '
//';
		return @file_get_contents_safely($sPathOut); //$this->aResults['fullOutput'];
	} // encryptFromFileToFile

	private function encryptThisToFile($sContent = '', $mCypherCode = '', $sPassBit = '', $sPathOut = '') {
		$this->fReset();
		$this->aResults['fullOutput'] = 'Depricated call to private function encryptThisToFile of class SssS_Cypher2. Nothing done!';
		$this->aResults['lastLine'] = 3;
		return $this->aResults['fullOutput'];
	} // encryptThisToFile

	private function encryptThis($sContent = '', $mCypherCode = '', $sPassBit = '') {
		$this->fReset();
		$this->aResults['fullOutput'] = 'Depricated call to private function encryptThisToFile of class SssS_Cypher2. Nothing done!';
		$this->aResults['lastLine'] = 3;
		return $this->aResults['fullOutput'];
	} // encryptThis

	/* * * * *  *   \PRIVATE enCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PRIVATE deCRYPTING FUNCTIONS\   *  * * * * */

	private function decryptFromFile($mCypherCode = '', $sPassBit = '', $sPathIn = '') {
		$this->fReset();
		$this->aResults['fullOutput'] = 'Depricated call to private function decryptFromFile of class SssS_Cypher2. Nothing done!';
		$this->aResults['lastLine'] = 3;
		return $this->aResults['fullOutput'];
	} // decryptFromFile

	private function decryptFromFileToFile($mCypherCode = '', $sPassBit = '', $sPathIn = '', $sPathOut = '') {
		if (!is_file($sPathIn)) {
			$this->fReset();
			$this->aResults['fullOutput'] = 'invalid in path';
			$this->aResults['lastLine'] = 2;
			return $this->aResults['fullOutput'];
		} // if invalid in path
		$sInBit = escapeshellarg($sPathIn);
		$sOutBit = ($sPathOut && ('' != $sPathOut))
				? ' -out ' . escapeshellarg($sPathOut) : '';
		$sCipher = $this->cypherForKey($mCypherCode);
		$sScriptDecode = sprintf('openssl enc -d -%s -a -in %s%s %s'
				, $sCipher, $sInBit, $sOutBit, $sPassBit);
		$this->doScript($sScriptDecode);
		return @file_get_contents_safely($sPathOut); // if "" == $this->aResults['fullOutput'];
	} // decryptFromFileToFile

	private function decryptThisToFile($sContent = '', $mCypherCode = '', $sPassBit = '', $sPathOut = '') {
		$this->fReset();
		$this->aResults['fullOutput'] = 'Depricated call to private function decryptThisToFile of class SssS_Cypher2. Nothing done!';
		$this->aResults['lastLine'] = 3;
		return $this->aResults['fullOutput'];
	} // decryptThisToFile

	private function decryptThis($sContent = '', $mCypherCode = '', $sPassBit = '') {
		$this->fReset();
		$this->aResults['fullOutput'] = 'Depricated call to private function decryptThis of class SssS_Cypher2. Nothing done!';
		$this->aResults['lastLine'] = 3;
		return $this->aResults['fullOutput'];
	} // decryptThis

	/* * * * *  *   \PRIVATE deCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PRIVATE FUNCTIONS 4 SUBCLASSING\   *  * * * * */

	// subclass this for more obfuscation
	private function cypherForKey($sKey = '') {
		$sCipher = 'des3';
		if (isset($this->hCiphers, $this->hCiphers[$sKey])) $sCipher = $this->hCiphers[$sKey];
		return $sCipher;
	} // cypherForKey

	// subclass this for more obfuscation
	private function initCiphers() {
		$aCiphers = array('aes-128-cbc', 'aes-128-ecb', 'aes-192-cbc', 'aes-192-ecb', 'aes-256-cbc', 'aes-256-ecb', 'base64', 'bf', 'bf-cbc', 'bf-cfb', 'bf-ecb', 'bf-ofb', 'cast', 'cast-cbc', 'cast5-cbc', 'cast5-cfb', 'cast5-ecb', 'cast5-ofb', 'des', 'des-cbc', 'des-cfb', 'des-ecb', 'des-ede', 'des-ede-cbc', 'des-ede-cfb', 'des-ede-ofb', 'des-ede3', 'des-ede3-cbc', 'des-ede3-cfb', 'des-ede3-ofb', 'des-ofb', 'des3', 'desx', 'idea', 'idea-cbc', 'idea-cfb', 'idea-ecb', 'idea-ofb', 'rc2', 'rc2-40-cbc', 'rc2-64-cbc', 'rc2-cbc', 'rc2-cfb', 'rc2-ecb', 'rc2-ofb', 'rc4', 'rc4-40');
		$aKeys = array('c16656459864a70e4a5b9acd', 'c2045100124a70e4a5b9af4', 'c1371656364a70e4a5b9b00', 'c8203878634a70e4a5b9b0b', 'c12472937914a70e4a5b9b22', 'c6056705394a70e4a5b9b2d', 'c17844071144a70e4a5b9b38', 'c9648511854a70e4a5b9b43', 'c3996862294a70e4a5b9b4e', 'c8413803914a70e4a5b9b5a', 'c12520960834a70e4a5b9b65', 'c13237995704a70e4a5b9b70', 'c20738177034a70e4a5b9b7b', 'c16550030104a70e4a5b9b87', 'c499833734a70e4a5b9b92', 'c8454988194a70e4a5b9b9d', 'c4410822034a70e4a5b9ba8', 'c16104793824a70e4a5b9bb4', 'c12116392854a70e4a5b9bbf', 'c8110343934a70e4a5b9bcb', 'c9898186794a70e4a5b9bd6', 'c17935449364a70e4a5b9be1', 'c11861465694a70e4a5b9bed', 'c15616643904a70e4a5b9bf8', 'c12132073984a70e4a5b9c03', 'c13616428694a70e4a5b9c0e', 'c8078337254a70e4a5b9c1a', 'c17729865414a70e4a5b9c25', 'c13873505044a70e4a5b9c30', 'c18390697884a70e4a5b9c3b', 'c15121712554a70e4a5b9c46', 'c9055128424a70e4a5b9d0a', 'c20435798004a70e4a5b9d23', 'c16493368914a70e4a5b9d31', 'c17259007054a70e4a5b9d3d', 'c11433899434a70e4a5b9d48', 'c1075237824a70e4a5b9d53', 'c13628241714a70e4a5b9d5e', 'c21082411284a70e4a5b9d6a', 'c5072100114a70e4a5b9d75', 'c567209144a70e4a5b9d80', 'c12128535634a70e4a5b9d8b', 'c18310095814a70e4a5b9d96', 'c21305386184a70e4a5b9da1', 'c7203729254a70e4a5b9dac', 'c18809929544a70e4a5b9db8', 'c8285537894a70e4a5b9dc3');
		$this->hCiphers = array_combine($aKeys, $aCiphers);
	} // initCiphers

	/* * * * *  *   \PRIVATE FUNCTIONS 4 SUBCLASSING/   *  * * * * */

	/* * * * *  *   /HELPER FUNCTIONS\   *  * * * * */

	public function cleanupCypher2() {
		if (isset($this->sTP) && file_exists($this->sTP)) {
			// delete everything overwriting data
			$this->doScript('rm -fPr ' . escapeshellarg($this->sTP));
		} // if temp path exists
	} // cleanupCypher2

	/* * * * *  *   \HELPER FUNCTIONS/   *  * * * * */

} // SssS_Cypher2
/* * * *\ SssS_Cypher2.inc (CC)2009 Luke JZ aka SwissalpS /* * * */
?>
