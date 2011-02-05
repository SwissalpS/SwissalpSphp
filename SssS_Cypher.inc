<?php
/* * * *
 * * SssS_Cypher.inc
 * *
 * * @version 20091024_204153 + file_get/put_contents_safely
 * * @version 20090730_030924 (CC) Luke JZ aka SwissalpS
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

/* cypher data without writing to disk (if you want, you can do that too)
 * use the file password instead of cleartext to avoid ps sniffers
 */

if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_ShellScriptRunner.inc');
} // if not autoincluding but on php's include path

class SssS_Cypher extends SssS_ShellScriptRunner {
	public function SssS_Cypher() {
		$this->SssS_ShellScriptRunner();
		$this->initCiphers();
	} // SssS_Cypher

	/* * * * *  *   /PUBLIC enCRYPTING FUNCTIONS\   *  * * * * */

	public function encryptWithClearTextPass($sContent = '', $mCypherCode = '', $sPassword = '') {
		$sPassBit = $this->fClearTextPassBit($sPassword);
		return $this->encryptThis($sContent, $mCypherCode, $sPassBit);
	} // encryptWithClearTextPass

	public function encryptWithClearTextPassFromFile($mCypherCode = '', $sPassword = '', $sPathIn = '') {
		$sPassBit = $this->fClearTextPassBit($sPassword);
		return $this->encryptFromFile($mCypherCode, $sPassBit, $sPathIn);
	} // encryptWithClearTextPassFromFile

	public function encryptWithClearTextPassFromFileToFile($mCypherCode = '', $sPassword = '', $sPathIn = '', $sPathOut = '') {
		$sPassBit = $this->fClearTextPassBit($sPassword);
		return $this->encryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithClearTextPassFromFileToFile

	public function encryptWithClearTextPassToFile($sContent = '', $mCypherCode = '', $sPassword = '', $sPathOut = '') {
		$sPassBit = $this->fClearTextPassBit($sPassword);
		return $this->encryptThisToFile($sContent, $mCypherCode, $sPassBit, $sPathOut);
	} // encryptWithClearTextPassToFile

	public function encryptWithFilePass($sContent = '', $mCypherCode = '', $sPassPath = '') {
		$sPassBit = $this->fFilePassBit($sPassPath);
		return $this->encryptThis($sContent, $mCypherCode, $sPassBit);
	} // encryptWithFilePass

	public function encryptWithFilePassFromFile($mCypherCode = '', $sPassPath = '', $sPathIn = '') {
		$sPassBit = $this->fFilePassBit($sPassPath);
		return $this->encryptFromFile($mCypherCode, $sPassBit, $sPathIn);
	} // encryptWithFilePassFromFile

	public function encryptWithFilePassFromFileToFile($mCypherCode = '', $sPassPath = '', $sPathIn = '', $sPathOut = '') {
		$sPassBit = $this->fFilePassBit($sPassPath);
		return $this->encryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // encryptWithFilePassFromFileToFile

	public function encryptWithFilePassToFile($sContent = '', $mCypherCode = '', $sPassPath = '', $sPathOut = '') {
		$sPassBit = $this->fFilePassBit($sPassPath);
		return $this->encryptThisToFile($sContent, $mCypherCode, $sPassBit, $sPathOut);
	} // encryptWithFilePassToFile

	/* * * * *  *   \PUBLIC enCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PUBLIC deCRYPTING FUNCTIONS\   *  * * * * */

	public function decryptWithClearTextPass($sContent = '', $mCypherCode = '', $sPassword = '') {
		$sPassBit = $this->fClearTextPassBit($sPassword);
		return $this->decryptThis($sContent, $mCypherCode, $sPassBit);
	} // decryptWithClearTextPass

	public function decryptWithClearTextPassFromFile($mCypherCode = '', $sPassword = '', $sPathIn = '') {
		$sPassBit = $this->fClearTextPassBit($sPassword);
		return $this->decryptFromFile($mCypherCode, $sPassBit, $sPathIn);
	} // decryptWithClearTextPassFromFile

	public function decryptWithClearTextPassFromFileToFile($mCypherCode = '', $sPassword = '', $sPathIn = '', $sPathOut = '') {
		$sPassBit = $this->fClearTextPassBit($sPassword);
		return $this->decryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithClearTextPassFromFileToFile

	public function decryptWithClearTextPassToFile($sContent = '', $mCypherCode = '', $sPassword = '', $sPathOut = '') {
		$sPassBit = $this->fClearTextPassBit($sPassword);
		return $this->decryptThisToFile($sContent, $mCypherCode, $sPassBit, $sPathOut);
	} // decryptWithClearTextPassToFile

	public function decryptWithFilePass($sContent = '', $mCypherCode = '', $sPassPath = '') {
		$sPassBit = $this->fFilePassBit($sPassPath);
		return $this->decryptThis($sContent, $mCypherCode, $sPassBit);
	} // decryptWithFilePass

	public function decryptWithFilePassFromFile($mCypherCode = '', $sPassPath = '', $sPathIn = '') {
		$sPassBit = $this->fFilePassBit($sPassPath);
		return $this->decryptFromFile($mCypherCode, $sPassBit, $sPathIn);
	} // decryptWithFilePassFromFile

	public function decryptWithFilePassFromFileToFile($mCypherCode = '', $sPassPath = '', $sPathIn = '', $sPathOut = '') {
		$sPassBit = $this->fFilePassBit($sPassPath);
		return $this->decryptFromFileToFile($mCypherCode, $sPassBit, $sPathIn, $sPathOut);
	} // decryptWithFilePassFromFileToFile

	public function decryptWithFilePassToFile($sContent = '', $mCypherCode = '', $sPassPath = '', $sPathOut = '') {
		$sPassBit = $this->fFilePassBit($sPassPath);
		return $this->decryptThisToFile($sContent, $mCypherCode, $sPassBit, $sPathOut);
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
		if (!is_file($sPathIn)) {
			$this->fReset();
			$this->aResults['fullOutput'] = 'invalid in path';
			$this->aResults['lastLine'] = 2;
			return $this->aResults['fullOutput'];
		} // if invalid in path
		$sInBit = escapeshellarg($sPathIn);
		$sCipher = $this->cypherForKey($mCypherCode);
		$sScriptEncode = sprintf('openssl enc -%s -a -salt -in %s %s'
				, $sCipher, $sInBit, $sPassBit);
		$this->doScript($sScriptEncode);
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
		return $this->aResults['fullOutput'];
	} // encryptFromFileToFile

	private function encryptThisToFile($sContent = '', $mCypherCode = '', $sPassBit = '', $sPathOut = '') {
		$sCipher = $this->cypherForKey($mCypherCode);
		$sContentQuoted = escapeshellarg($sContent);
		$sOutBit = ($sPathOut && ('' != $sPathOut))
				? ' -out ' . escapeshellarg($sPathOut) : '';
		$sScriptEncode = sprintf('echo %s | openssl enc -%s -a -salt%s %s'
				, $sContentQuoted, $sCipher, $sOutBit, $sPassBit);
		$this->doScript($sScriptEncode);
		return $this->aResults['fullOutput'];
	} // encryptThisToFile

	private function encryptThis($sContent = '', $mCypherCode = '', $sPassBit = '') {
		$sCipher = $this->cypherForKey($mCypherCode);
		$sContentQuoted = escapeshellarg($sContent);
		$sScriptEncode = sprintf('echo %s | openssl enc -%s -a -salt %s'
				, $sContentQuoted, $sCipher, $sPassBit);
		$this->doScript($sScriptEncode);
		return $this->aResults['fullOutput'];
	} // encryptThis

	/* * * * *  *   \PRIVATE enCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PRIVATE deCRYPTING FUNCTIONS\   *  * * * * */

	private function decryptFromFile($mCypherCode = '', $sPassBit = '', $sPathIn = '') {
		if (!is_file($sPathIn)) {
			$this->fReset();
			$this->aResults['fullOutput'] = 'invalid in path';
			$this->aResults['lastLine'] = 2;
			return $this->aResults['fullOutput'];
		} // if invalid in path
		$sInBit = escapeshellarg($sPathIn);
		$sCipher = $this->cypherForKey($mCypherCode);
		$sScriptDecode = sprintf('openssl enc -d -%s -a -in %s %s'
				, $sCipher, $sInBit, $sPassBit);
		$this->doScript($sScriptDecode);
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
		return $this->aResults['fullOutput'];
	} // decryptFromFileToFile

	private function decryptThisToFile($sContent = '', $mCypherCode = '', $sPassBit = '', $sPathOut = '') {
		$sOutBit = ($sPathOut && ('' != $sPathOut))
				? ' -out ' . escapeshellarg($sPathOut) : '';
		$sCipher = $this->cypherForKey($mCypherCode);
		$sContentQuoted = escapeshellarg($sContent);
		$sScriptDecode = sprintf('echo %s | openssl enc -d -%s -a%s %s'
				,$sContentQuoted, $sCipher, $sOutBit, $sPassBit);
		$this->doScript($sScriptDecode);
		return $this->aResults['fullOutput'];
	} // decryptThisToFile

	private function decryptThis($sContent = '', $mCypherCode = '', $sPassBit = '') {
		$sCipher = $this->cypherForKey($mCypherCode);
		$sContentQuoted = escapeshellarg($sContent);
		$sScriptDecode = sprintf('echo %s | openssl enc -d -%s -a %s'
				,$sContentQuoted, $sCipher, $sPassBit);
		$this->doScript($sScriptDecode);
		return $this->aResults['fullOutput'];
	} // decryptThis

	/* * * * *  *   \PRIVATE deCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PROTECTED FUNCTIONS 4 SUBCLASSING\   *  * * * * */

	// subclass this for more obfuscation
	protected function cypherForKey($sKey = '') {
		$sCipher = 'des3';
		if (isset($this->hCiphers, $this->hCiphers[$sKey])) $sCipher = $this->hCiphers[$sKey];
		return $sCipher;
	} // cypherForKey

	// subclass this for more obfuscation
	protected function initCiphers() {
		$aCiphers = array('aes-128-cbc', 'aes-128-ecb', 'aes-192-cbc', 'aes-192-ecb', 'aes-256-cbc', 'aes-256-ecb', 'base64', 'bf', 'bf-cbc', 'bf-cfb', 'bf-ecb', 'bf-ofb', 'cast', 'cast-cbc', 'cast5-cbc', 'cast5-cfb', 'cast5-ecb', 'cast5-ofb', 'des', 'des-cbc', 'des-cfb', 'des-ecb', 'des-ede', 'des-ede-cbc', 'des-ede-cfb', 'des-ede-ofb', 'des-ede3', 'des-ede3-cbc', 'des-ede3-cfb', 'des-ede3-ofb', 'des-ofb', 'des3', 'desx', 'idea', 'idea-cbc', 'idea-cfb', 'idea-ecb', 'idea-ofb', 'rc2', 'rc2-40-cbc', 'rc2-64-cbc', 'rc2-cbc', 'rc2-cfb', 'rc2-ecb', 'rc2-ofb', 'rc4', 'rc4-40');
		$aKeys = array('c16656459864a70e4a5b9acd', 'c2045100124a70e4a5b9af4', 'c1371656364a70e4a5b9b00', 'c8203878634a70e4a5b9b0b', 'c12472937914a70e4a5b9b22', 'c6056705394a70e4a5b9b2d', 'c17844071144a70e4a5b9b38', 'c9648511854a70e4a5b9b43', 'c3996862294a70e4a5b9b4e', 'c8413803914a70e4a5b9b5a', 'c12520960834a70e4a5b9b65', 'c13237995704a70e4a5b9b70', 'c20738177034a70e4a5b9b7b', 'c16550030104a70e4a5b9b87', 'c499833734a70e4a5b9b92', 'c8454988194a70e4a5b9b9d', 'c4410822034a70e4a5b9ba8', 'c16104793824a70e4a5b9bb4', 'c12116392854a70e4a5b9bbf', 'c8110343934a70e4a5b9bcb', 'c9898186794a70e4a5b9bd6', 'c17935449364a70e4a5b9be1', 'c11861465694a70e4a5b9bed', 'c15616643904a70e4a5b9bf8', 'c12132073984a70e4a5b9c03', 'c13616428694a70e4a5b9c0e', 'c8078337254a70e4a5b9c1a', 'c17729865414a70e4a5b9c25', 'c13873505044a70e4a5b9c30', 'c18390697884a70e4a5b9c3b', 'c15121712554a70e4a5b9c46', 'c9055128424a70e4a5b9d0a', 'c20435798004a70e4a5b9d23', 'c16493368914a70e4a5b9d31', 'c17259007054a70e4a5b9d3d', 'c11433899434a70e4a5b9d48', 'c1075237824a70e4a5b9d53', 'c13628241714a70e4a5b9d5e', 'c21082411284a70e4a5b9d6a', 'c5072100114a70e4a5b9d75', 'c567209144a70e4a5b9d80', 'c12128535634a70e4a5b9d8b', 'c18310095814a70e4a5b9d96', 'c21305386184a70e4a5b9da1', 'c7203729254a70e4a5b9dac', 'c18809929544a70e4a5b9db8', 'c8285537894a70e4a5b9dc3');
		$this->hCiphers = array_combine($aKeys, $aCiphers);
	} // initCiphers

	/* * * * *  *   \PROTECTED FUNCTIONS 4 SUBCLASSING/   *  * * * * */

	/* * * * *  *   /HELPER FUNCTIONS\   *  * * * * */

	// make a unique folder with rwx perms for owner only (700)
	// make a unique file with rwx perms for owner only (700)
	function fGetSafeUniqueFolderAndFileInPath($sPath = '') {
		$sUdir = $this->fMakeSafeUniqueFolderInPath($sPath);
		if (!$sUdir) return array(false, false);
		$sUfile = $this->fMakeSafeUniqueFileInPath($sUdir);
		return array($sUdir, $sUfile);
	} // fGetSafeUniqueFolderAndFileInPath

	// make a unique folder with rwx perms for owner only (700)
	function fMakeSafeUniqueFolderInPath($sPath = '') {
		if ('' == trim($sPath)) $sPath = './';
		$sPath .= ('/' != substr($sPath, -1)) ? '/' : '';
		$sUdir = '';
		while ('' == $sUdir || file_exists($sPath . $sUdir)) {
			$sUdir = 'd' . md5(time() . $sPath . $sUdir);
		} // while
		$sUdir = $sPath . $sUdir . '/';
		$this->doScript('mkdir -p -m 700 ' . escapeshellarg($sUdir));
		if ((0 != $this->aResults['scriptExitCode']) || (!file_exists($sUdir))) return false;
		return $sUdir;
	} // fMakeSafeUniqueFolderInPath

	// make a unique file with rwx perms for owner only (700)
	function fMakeSafeUniqueFileInPath($sPath = '') {
		$sPath .= ('/' != substr($sPath, -1)) ? '/' : '';
		$sUfile = '';
		while ('' == $sUfile || file_exists($sPath . $sUfile)) {
			$sUfile = 'f' . md5(time() . $sPath . $sUfile);
		} // while not unique filename
		$res = @file_put_contents_safely($sPath . $sUfile, '', 'wb');
		if (false === $res) return false;
		$this->doScript('chmod 700 ' . escapeshellarg($sPath . $sUfile));
		if (0 != $this->aResults['scriptExitCode']) return false;
		return $sUfile;
	} // fMakeSafeUniqueFileInPath

	/* * * * *  *   \HELPER FUNCTIONS/   *  * * * * */

} // SssS_Cypher
/* * * *\ SssS_Cypher.inc (CC)2009 Luke JZ aka SwissalpS /* * * */
?>
