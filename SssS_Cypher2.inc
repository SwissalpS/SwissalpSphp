<?php
/* * * *
 * * SssS_Cypher2.inc
 * * extends SssS_Cypher to never use cleartext on the cl
 * *
 * * to make sure the temporary files are deleted, call cleanupCypher2()
 * *
 * * DEPRICATED CLASS, use SssS_Cypher3openSSLcli or SssS_Cypher3mcrypt instead
 * * not all the settings options are respected and they will never be as I have
 * * stopped working on this class. SssS_Cypher3 subclasses are the way to go.
 * *
 * * @version 20131106_153049 changed spelling of cypher to cipher when adj.
 * * @version 20131103_013848 + base64decodeFile() + base64encodeFile()
 * * @version 20131103_012737 + allways make tmp dir + check rm -P
 * * @version 20100503_132059 + autoloading switch and system tmp dir
 * * @version 20091024_203818 + file_get/put_contents
 * * @version 20090815_213258 (CC) Luke JZ aka SwissalpS
 * * * */
// a little english lesson: the cipher is the adjective while the verb is spelt cypher
/* usage example: * /
$sPass = 'ha ha';
$sContent = 'something secret';
$sCipherCode = 'c9055128424a70e4a5b9d0a'; // => 'des3';

$oCypher = new SssS_Cypher2();

// encode
$sEnc = $oCypher->encryptWithClearTextPass($sContent, $sCipherCode, $sPass);

// decode
$sDec = $oCypher->decryptWithClearTextPass($sEnc, $sCipherCode, $sPass);

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

		} // if got args

		$this->sTP = $this->fMakeSafeUniqueFolderInPath($this->aSettings['tmpPath']);

	} // SssS_Cypher2

	/* * * * *  *   /PUBLIC enCRYPTING FUNCTIONS\   *  * * * * */

	public function encryptWithClearTextPass($sContent = '', $sCipherCode = '', $sPassword = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delFileIn']	= true;
		$this->aSettings['delFileOut']	= true;
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->encryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithClearTextPass

	public function encryptWithClearTextPassFromFile($sCipherCode = '', $sPassword = '', $sPathIn = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delFileOut']	= true;
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		return $this->encryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithClearTextPassFromFile

	public function encryptWithClearTextPassFromFileToFile($sCipherCode = '', $sPassword = '', $sPathIn = '', $sPathOut = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		return $this->encryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithClearTextPassFromFileToFile

	public function encryptWithClearTextPassToFile($sContent = '', $sCipherCode = '', $sPassword = '', $sPathOut = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delFileIn']	= true;
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->encryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithClearTextPassToFile

	public function encryptWithFilePass($sContent = '', $sCipherCode = '', $sPassPath = '') {
		$sPassBit		= $this->fFilePassBit($sPassPath);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$this->aSettings['delFileIn']	= true;
		$this->aSettings['delFileOut']	= true;
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->encryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithFilePass

	public function encryptWithFilePassFromFile($sCipherCode = '', $sPassPath = '', $sPathIn = '') {
		$sPassBit		= $this->fFilePassBit($sPassPath);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$this->aSettings['delFileOut']	= true;
		return $this->encryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithFilePassFromFile

	public function encryptWithFilePassFromFileToFile($sCipherCode = '', $sPassPath = '', $sPathIn = '', $sPathOut = '') {
		$sPassBit		= $this->fFilePassBit($sPassPath);
		return $this->encryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithFilePassFromFileToFile

	public function encryptWithFilePassToFile($sContent = '', $sCipherCode = '', $sPassPath = '', $sPathOut = '') {
		$sPassBit		= $this->fFilePassBit($sPassPath);
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->encryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithFilePassToFile

	/* * * * *  *   \PUBLIC enCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PUBLIC deCRYPTING FUNCTIONS\   *  * * * * */

	public function decryptWithClearTextPass($sContent = '', $sCipherCode = '', $sPassword = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delFileIn']	= true;
		$this->aSettings['delFileOut']	= true;
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->decryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithClearTextPass

	public function decryptWithClearTextPassFromFile($sCipherCode = '', $sPassword = '', $sPathIn = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delFileOut']	= true;
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		return $this->decryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithClearTextPassFromFile

	public function decryptWithClearTextPassFromFileToFile($sCipherCode = '', $sPassword = '', $sPathIn = '', $sPathOut = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		return $this->decryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithClearTextPassFromFileToFile

	public function decryptWithClearTextPassToFile($sContent = '', $sCipherCode = '', $sPassword = '', $sPathOut = '') {
		$sTmpPassFile	= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sTmpPassFile);
		$this->aSettings['delFileIn']	= true;
		$this->aSettings['delPassPath']	= true;
		@file_put_contents_safely($sTmpPassFile, $sPassword, 'wb');
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->decryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithClearTextPassToFile

	public function decryptWithFilePass($sContent = '', $sCipherCode = '', $sPassPath = '') {
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sPassPath);
		$this->aSettings['delFileIn']	= true;
		$this->aSettings['delFileOut']	= true;
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->decryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithFilePass

	public function decryptWithFilePassFromFile($sCipherCode = '', $sPassPath = '', $sPathIn = '') {
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sPassPath);
		$this->aSettings['delFileOut']	= true;
		return $this->decryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithFilePassFromFile

	public function decryptWithFilePassFromFileToFile($sCipherCode = '', $sPassPath = '', $sPathIn = '', $sPathOut = '') {
		$sPassBit		= $this->fFilePassBit($sPassPath);
		return $this->decryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithFilePassFromFileToFile

	public function decryptWithFilePassToFile($sContent = '', $sCipherCode = '', $sPassPath = '', $sPathOut = '') {
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPassBit		= $this->fFilePassBit($sPassPath);
		$this->aSettings['delFileIn']	= true;
		@file_put_contents_safely($sPathIn, $sContent, 'wb');
		return $this->decryptFromFileToFile($sCipherCode, $sPassBit, $sPathIn, $sPathOut);
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

	private function encryptFromFile($sCipherCode = '', $sPassBit = '', $sPathIn = '') {
		$this->fReset();
		$this->aResults['fullOutput'] = 'Depricated call to private function encryptFromFile of class SssS_Cypher2. Nothing done!';
		$this->aResults['lastLine'] = 3;
		return $this->aResults['fullOutput'];
	} // encryptFromFile

	private function encryptFromFileToFile($sCipherCode = '', $sPassBit = '', $sPathIn = '', $sPathOut = '') {
		if (!is_file($sPathIn)) {
			$this->fReset();
			$this->aResults['fullOutput'] = 'invalid in path';
			$this->aResults['lastLine'] = 2;
			return $this->aResults['fullOutput'];
		} // if invalid in path
		$sInBit = escapeshellarg($sPathIn);
		$sCipher = $this->cipherForKey($sCipherCode);
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

	private function encryptThisToFile($sContent = '', $sCipherCode = '', $sPassBit = '', $sPathOut = '') {
		$this->fReset();
		$this->aResults['fullOutput'] = 'Depricated call to private function encryptThisToFile of class SssS_Cypher2. Nothing done!';
		$this->aResults['lastLine'] = 3;
		return $this->aResults['fullOutput'];
	} // encryptThisToFile

	private function encryptThis($sContent = '', $sCipherCode = '', $sPassBit = '') {
		$this->fReset();
		$this->aResults['fullOutput'] = 'Depricated call to private function encryptThisToFile of class SssS_Cypher2. Nothing done!';
		$this->aResults['lastLine'] = 3;
		return $this->aResults['fullOutput'];
	} // encryptThis

	/* * * * *  *   \PRIVATE enCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PRIVATE deCRYPTING FUNCTIONS\   *  * * * * */

	private function decryptFromFile($sCipherCode = '', $sPassBit = '', $sPathIn = '') {
		$this->fReset();
		$this->aResults['fullOutput'] = 'Depricated call to private function decryptFromFile of class SssS_Cypher2. Nothing done!';
		$this->aResults['lastLine'] = 3;
		return $this->aResults['fullOutput'];
	} // decryptFromFile

	private function decryptFromFileToFile($sCipherCode = '', $sPassBit = '', $sPathIn = '', $sPathOut = '') {
		if (!is_file($sPathIn)) {
			$this->fReset();
			$this->aResults['fullOutput'] = 'invalid in path';
			$this->aResults['lastLine'] = 2;
			return $this->aResults['fullOutput'];
		} // if invalid in path
		$sInBit = escapeshellarg($sPathIn);
		$sOutBit = ($sPathOut && ('' != $sPathOut))
				? ' -out ' . escapeshellarg($sPathOut) : '';
		$sCipher = $this->cipherForKey($sCipherCode);
		$sScriptDecode = sprintf('openssl enc -d -%s -a -in %s%s %s'
				, $sCipher, $sInBit, $sOutBit, $sPassBit);
		$this->doScript($sScriptDecode);
		return @file_get_contents_safely($sPathOut); // if "" == $this->aResults['fullOutput'];
	} // decryptFromFileToFile

	private function decryptThisToFile($sContent = '', $sCipherCode = '', $sPassBit = '', $sPathOut = '') {
		$this->fReset();
		$this->aResults['fullOutput'] = 'Depricated call to private function decryptThisToFile of class SssS_Cypher2. Nothing done!';
		$this->aResults['lastLine'] = 3;
		return $this->aResults['fullOutput'];
	} // decryptThisToFile

	private function decryptThis($sContent = '', $sCipherCode = '', $sPassBit = '') {
		$this->fReset();
		$this->aResults['fullOutput'] = 'Depricated call to private function decryptThis of class SssS_Cypher2. Nothing done!';
		$this->aResults['lastLine'] = 3;
		return $this->aResults['fullOutput'];
	} // decryptThis

	/* * * * *  *   \PRIVATE deCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PRIVATE FUNCTIONS 4 SUBCLASSING\   *  * * * * */

	// subclass this for more obfuscation
	protected function cipherForKey($sKey = '') {
		$sCipher = 'des3';
		if (isset($this->hCiphers, $this->hCiphers[$sKey])) $sCipher = $this->hCiphers[$sKey];
		return $sCipher;
	} // cipherForKey


	// alias to cipherForKey() for backward compatibility
	protected function cypherForKey($sKey = '') { return $this->cipherForKey($sKey); } // cypherForKey

	// subclass this for more obfuscation
	protected function initCiphers() {

		$aCiphers = array('aes-128-cbc', 'aes-128-ecb', 'aes-192-cbc', 'aes-192-ecb', 'aes-256-cbc', 'aes-256-ecb', 'base64', 'bf', 'bf-cbc', 'bf-cfb', 'bf-ecb', 'bf-ofb', 'cast', 'cast-cbc', 'cast5-cbc', 'cast5-cfb', 'cast5-ecb', 'cast5-ofb', 'des', 'des-cbc', 'des-cfb', 'des-ecb', 'des-ede', 'des-ede-cbc', 'des-ede-cfb', 'des-ede-ofb', 'des-ede3', 'des-ede3-cbc', 'des-ede3-cfb', 'des-ede3-ofb', 'des-ofb', 'des3', 'desx', 'idea', 'idea-cbc', 'idea-cfb', 'idea-ecb', 'idea-ofb', 'rc2', 'rc2-40-cbc', 'rc2-64-cbc', 'rc2-cbc', 'rc2-cfb', 'rc2-ecb', 'rc2-ofb', 'rc4', 'rc4-40');

		$aKeys = array('c16656459864a70e4a5b9acd', 'c2045100124a70e4a5b9af4', 'c1371656364a70e4a5b9b00', 'c8203878634a70e4a5b9b0b', 'c12472937914a70e4a5b9b22', 'c6056705394a70e4a5b9b2d', 'c17844071144a70e4a5b9b38', 'c9648511854a70e4a5b9b43', 'c3996862294a70e4a5b9b4e', 'c8413803914a70e4a5b9b5a', 'c12520960834a70e4a5b9b65', 'c13237995704a70e4a5b9b70', 'c20738177034a70e4a5b9b7b', 'c16550030104a70e4a5b9b87', 'c499833734a70e4a5b9b92', 'c8454988194a70e4a5b9b9d', 'c4410822034a70e4a5b9ba8', 'c16104793824a70e4a5b9bb4', 'c12116392854a70e4a5b9bbf', 'c8110343934a70e4a5b9bcb', 'c9898186794a70e4a5b9bd6', 'c17935449364a70e4a5b9be1', 'c11861465694a70e4a5b9bed', 'c15616643904a70e4a5b9bf8', 'c12132073984a70e4a5b9c03', 'c13616428694a70e4a5b9c0e', 'c8078337254a70e4a5b9c1a', 'c17729865414a70e4a5b9c25', 'c13873505044a70e4a5b9c30', 'c18390697884a70e4a5b9c3b', 'c15121712554a70e4a5b9c46', 'c9055128424a70e4a5b9d0a', 'c20435798004a70e4a5b9d23', 'c16493368914a70e4a5b9d31', 'c17259007054a70e4a5b9d3d', 'c11433899434a70e4a5b9d48', 'c1075237824a70e4a5b9d53', 'c13628241714a70e4a5b9d5e', 'c21082411284a70e4a5b9d6a', 'c5072100114a70e4a5b9d75', 'c567209144a70e4a5b9d80', 'c12128535634a70e4a5b9d8b', 'c18310095814a70e4a5b9d96', 'c21305386184a70e4a5b9da1', 'c7203729254a70e4a5b9dac', 'c18809929544a70e4a5b9db8', 'c8285537894a70e4a5b9dc3');

		$this->hCiphers = array_combine($aKeys, $aCiphers);

	} // initCiphers

	/* * * * *  *   \PRIVATE FUNCTIONS 4 SUBCLASSING/   *  * * * * */

	/* * * * *  *   /HELPER FUNCTIONS\   *  * * * * */

    public function base64decodeFile($sPathFileIn, $sPathFileOut) {

        if (!(file_exists($sPathFileIn) && is_file($sPathFileIn))) return false;

        $sIn = escapeshellarg($sPathFileIn);
        $sOut = escapeshellarg($sPathFileOut);

        $sScriptDecode = sprintf('openssl enc -base64 -d -in %s -out %s', $sIn, $sOut);
        $this->doScript($sScriptDecode);

        return (0 === $this->iRes());

    } // base64decodeFile


    public function base64encodeFile($sPathFileIn, $sPathFileOut) {

        if (!(file_exists($sPathFileIn) && is_file($sPathFileIn))) return false;

        $sIn = escapeshellarg($sPathFileIn);
        $sOut = escapeshellarg($sPathFileOut);

        $sScriptEncode = sprintf('openssl enc -base64 -in %s -out %s', $sIn, $sOut);
        $this->doScript($sScriptEncode);

        return (0 === $this->iRes());

    } // base64encodeFile


	public function cleanupCypher2() {

		static $bRmHasOptionP = null;

		if (isset($this->sTP) && file_exists($this->sTP)) {

			// find out if this systems rm command supports option P
			if (null === $bRmHasOptionP) {

				$sTmpFileEscaped = escapeshellarg($this->sTP . 'SssS_Cypher2testOptionP');
				$this->doScript('touch ' . $sTmpFileEscaped . ';rm -P ' . $sTmpFileEscaped);
				$bRmHasOptionP = (0 === $this->iRes());

				if (!$bRmHasOptionP) {

					// cleanup test file
					$this->doScript('rm ' . $sTmpFileEscaped);

					echo 'rm does not support option "P", will emulate this myself.' . chr(10);

				} // if option P is not supported

			} // if first run

			// if option P is supported (OS X), we can use the shortcut
			if ($bRmHasOptionP) {

				// delete everything overwriting data
				$this->doScript('rm -fPr ' . escapeshellarg($this->sTP));
				$bRes = (0 === $this->iRes());

			} else {

				// recurseivly overwrite files with 0xFF, 0x00 and 0xFF
				$bRes = $this->secureRecursiveDeleteFolder($this->sTP);
				if (false === $bRes) return false;

			} // if rm has option -P or not

			if (true !== $bRes) {

				throw new Exception('error cleaning up. Manually remove dir: ' . $this->sTP);

			} // if error cleaning up

			// make new temp path
			$this->sTP = $this->fMakeSafeUniqueFolderInPath($this->aSettings['tmpPath']);

		} // if temp path exists

		return true;

	} // cleanupCypher2


	function secureRecursiveDeleteFolder($sPath) {

		if (!file_exists($sPath)) return false;
		if (!is_dir($sPath)) return false;

		$rD = opendir($sPath);
		if (false === $rD) return false;

		$sPath = rtrim($sPath, DIRECTORY_SEPARATOR) . DIRECTORY_SEPARATOR;

		while (false !== ($sF = readdir($rD))) {

			if ('.' == $sF) continue;
			if ('..' == $sF) continue;

			$sPathFile = $sPath . $sF;
			if (is_dir($sPathFile)) {

				$bRes = $this->secureRecursiveDeleteFolder($sPathFile);

			} else {

				// overwrite file with 0x00, 0xFF, 0x00

				$iFileSize = filesize($sPathFile);
				$sZeros = str_repeat(chr(0), $iFileSize);
				$s255s = str_repeat(chr(0xFF), $iFileSize);

				file_put_contents_safely($sPathFile, $sZeros, 'wb');
				file_put_contents_safely($sPathFile, $s255s, 'wb');
				file_put_contents_safely($sPathFile, $sZeros, 'wb');

				$bRes = @unlink($sPathFile);

			} // if is dir or file

			if (false === $bRes) return false;

		} // loop all files in folder

		return @rmdir($sPath);

	} // secureRecursiveDeleteFolder

	/* * * * *  *   \HELPER FUNCTIONS/   *  * * * * */

} // SssS_Cypher2
/* * * *\ SssS_Cypher2.inc (CC)2009 Luke JZ aka SwissalpS /* * * */
?>
