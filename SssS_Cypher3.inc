<?php
/* * * *
 * * SssS_Cypher3.inc
 * * Intermediate class for SssS_Cypher3mcrypt() and SssS_Cypher3opensslCLI()
 * *
 * * DO NOT USE DIRECTLY!!
 * * Instantiate SssS_Cypher3mcrypt() or SssS_Cypher3opensslCLI() objects instead.
 * *
 * * @version 20131019_083530 (CC) Luke JZ aka SwissalpS
 * * * */
// a little english lesson: the cipher is the adjective while the verb is spelt cypher
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_Cypher2.inc');
} // if not autoincluding but on php's include path

if (!defined('NL')) define('NL', chr(10));

class SssS_Cypher3 extends SssS_Cypher2 {

    protected $hCipherSizeBlock;
    protected $hCipherSizeKey;
    protected $hCipherSizeIV;
    protected $hCipherHMAC;

    // constructor do not use! Instead instantiate objects of classes
	// SssS_Cypher3opensslCLI or SssS_Cypher3mcrypt
    public function SssS_Cypher3($aArgs = false) {

        // initialize the parents
		$this->SssS_Cypher2($aArgs);

    } // SssS_Cypher3


	// alias to cleanupCypher2()
	function cleanupCypher3() {

		return $this->cleanupCypher2();

	} // cleanupCypher3


    /*
     * Creates a sub-key from a master key for a specific purpose.
     * $sMaster contains e.g. the password
     * $sPurpose cotains the salt-data
     * $iBytes defines the length of the returned key
     * The same set of parameters produce the same key on each call.
     * returns string of $iBytes length or FALSE
     */
    static function createSubkey($sMaster, $sPurpose, $iBytes) {

        $sSource = hash_hmac('sha512', $sPurpose, $sMaster, true);
        if ($iBytes > strlen($sSource)) {

			//throw new Exception('Subkey too big.', E_USER_ERROR);

			// TODO: check PHP version, and trigger error if old
            //trigger_error('Subkey too big.', E_USER_ERROR);
            return false; // fail safe

        } // if hash is shorter than requested length

        return substr($sSource, 0, $iBytes);

    } // createSubkey


	// returns NULL and sets the satus array values in honour of
	// SssS_ShellScriptRunner parent calss' way of passing information.
    function fail($sFailText, $iCode) {

        $this->cleanupCypher2();
        $this->fReset();
        $this->aResults['fullOutput'] = $sFailText;
        $this->aResults['lastLine'] = $iCode;
        $this->aResults['scriptExitCode'] = $iCode;

        return null;

    } // fail


    /*
     * Returns a random binary string of length $iByteLength bytes
     * or throws an Exception
     */
    static function secureRandom($iByteLength) {

        if (function_exists('mcrypt_create_iv')) {

            return mcrypt_create_iv($iByteLength, MCRYPT_DEV_URANDOM);

        } elseif (function_exists('openssl_random_pseudo_bytes')) {

            return openssl_random_pseudo_bytes($iByteLength);

        } else {

            // warning's will be output unles the error suppression operator is used. errors such as
            // "open_basedir restriction in effect", "Permission denied", "No such file or directory", etc.
            $rF = @fopen('/dev/urandom', 'rb');

            if ($rF !== true && $rF !== false) { // surprisingly faster than !is_bool() or is_resource()
                $sRandom = fread($rF, $iByteLength);

                fclose($rF);

                return $sRandom;

            } else {

				// TODO: if does not yoet support Exceptions then tripgger error
                throw new Exception('failed to open /dev/urandom', -3);
                return str_repeat('1', $iByteLength); // fallback

            } // if able to read from /dev/urandom

        } // if got mcrypt, openssl or none of those

    } // secureRandom


    /*
     * Compares two strings in constant time.
     */
    static function slowEquals($sA, $sB) {

        $iDiff = strlen($sA) ^ strlen($sB);

        for($i = 0; strlen($sA) > $i && strlen($sB) > $i; $i++) {

            $iDiff |= ord($sA[$i]) ^ ord($sB[$i]);

        } // loop

        return 0 === $iDiff;

    } // slowEquals


    // returns some 'salt'-data for generating subkey for authentication
	// ideal method to override in a subclass
    static function subKeyDataAuthentication() {

        return 'AuthEntiCatiOn';

    } // subKeyDataAuthentication


    // returns some 'salt'-data for generating subkey for encryption
	// ideal method to override in a subclass
    static function subKeyDataEncryption() {

        return 'EncrYptiOn';

    } // subKeyDataEncryption

	// Test SssS_Cypher3mcrypt and SssS_Cypher3openSSLcli
	// first self-tests each class, if successfull with both
	// tests if data encrypted with one, can be decrypted with the other.
	// This test may take a while.... run over lunch-break.
	// Also note that this method outputs debug information to std-out
	// Returns a multilevel array containing error messages.
	static function testCypher3($bSkipLongTest = false) {

		$aResults = array();

		// include the classes
		if (!defined('AUTO_INCLUDING')) {

			require_once('SssS_Cypher3mcrypt.inc');
			require_once('SssS_Cypher3openSSLcli.inc');

		} // if not autoincluding

		// perform self tests
		$bStop = false;
		echo 'Self-Test of SssS_Cypher3mcrypt starting...' . NL;
		$aResults['SelfTestMcrypt'] = SssS_Cypher3mcrypt::testCypher3mcrypt($bSkipLongTest);

		echo 'Self-Test of SssS_Cypher3openSSLcli starting...' . NL;
		$aResults['SelfTestOpenSSLcli'] = SssS_Cypher3openSSLcli::testCypher3openSSLcli($bSkipLongTest);

		// evaluate results
		if (!empty($aResults['SelfTestMcrypt'])) {

			$bStop = true;
			echo '  FAILED Self-Test of SssS_Cypher3mcrypt' . NL;

		} // if there were errors
		if (!empty($aResults['SelfTestOpenSSLcli'])) {

			$bStop = true;
			echo '  FAILED Self-Test of SssS_Cypher3openSSLcli' . NL;

		} // if there were errors

		// abbort if self tests didn't both pass
		if ($bStop) return $aResults;

		$oCm = SssS_Cypher3mcrypt::sharedInstance();
		$oCo = SssS_Cypher3openSSLcli::sharedInstance();

		$aCipherCodes = array_keys($oCm->hCipherHMAC);

		$sData = self::secureRandom(510);
		$sPassword = self::secureRandom(24);

		$aResults['CrossTestOpenSSLcli2mcrypt'] = array();

		echo 'Trying all cipher-codes.' . NL
				. 'Encrypting with SssS_Cypher3openSSLcli and '
				. 'decrypting with SssS_Cypher3mcrypt' . NL;

		forEach ($aCipherCodes as $sCipherCode) {

			echo NL . 'Trying cipher-code: ' . $sCipherCode . NL;

			$sDataEncryptedEncoded = $oCo->encryptWithClearTextPass($sData, $sCipherCode, $sPassword);
			if (null === $sDataEncryptedEncoded) {

				echo '--FAILED TO ENCRYPT' . NL;
				$aResults['CrossTestOpenSSLcli2mcrypt'][$sCipherCode] = 'FAILED TO ENCRYPT';
				continue;

			} // if error encrypting

			echo '  OK, encrypted' . NL;

			$sDataEncrypted = base64_decode($sDataEncryptedEncoded);

			$sDataDecrypted = $oCm->decryptWithClearTextPass($sDataEncrypted, $sPassword);
			if (null === $sDataDecrypted) {

				echo '--FAILED to decrypt' . NL;
				$aResults['CrossTestOpenSSLcli2mcrypt'][$sCipherCode] = 'FAILED TO DECRYPT';
				continue;

			} // if error decrypting

			echo '  OK, decrypted' . NL;

			// check if decrypted data matches original
			if ($sDataDecrypted == $sData) {

				echo '++PASS ++++' . NL;

			} else {

				echo '--FAIL ----' . NL;
				$aResults['CrossTestOpenSSLcli2mcrypt'][$sCipherCode] = 'DECRYPTED DATA DOES NOT MATCH ORIGINAL';

			} // if decrypted data matches original data

		} // loop each aCypherCodes

		$aResults['CrossTestMcrypt2openSSLcli'] = array();

		echo NL . NL . 'Encrypting with mcrypt and decrypting with openssl' . NL;

		forEach ($aCipherCodes as $sCipherCode) {

			echo NL . 'Trying cipher-code: ' . $sCipherCode . NL;

			$sDataEncryptedEncoded = $oCm->encryptWithClearTextPass($sData, $sCipherCode, $sPassword);
			if (null === $sDataEncryptedEncoded) {

				echo '--FAILED TO ENCRYPT' . NL;
				$aResults['CrossTestMcrypt2openSSLcli'][$sCipherCode] = 'FAILED TO ENCRYPT';
				continue;

			} // if error encrypting

			echo '  OK, encrypted' . NL;

			$sDataEncrypted = base64_decode($sDataEncryptedEncoded);

			$sDataDecrypted = $oCo->decryptWithClearTextPass($sDataEncrypted, $sPassword);
			if (null === $sDataDecrypted) {

				echo '--FAILED to decrypt' . NL;
				$aResults['CrossTestMcrypt2openSSLcli'][$sCipherCode] = 'FAILED TO DECRYPT';
				continue;

			} // if error decrypting

			echo '  OK, decrypted' . NL;

			// check if decrypted data matches original
			if ($sDataDecrypted == $sData) {

				echo '++PASS ++++ ' . NL;

			} else {

				echo '--FAIL ---- ' . NL;
				$aResults['CrossTestMcrypt2openSSLcli'][$sCipherCode] = 'DECRYPTED DATA DOES NOT MATCH ORIGINAL';

			} // if decrypted data matches original data

		} // loop each aCypherCodes

		return $aResults;

	} // testCypher3


	/* * * * *  *   PRIVATE FUNCTIONS   *  * * * * */

    // these are no longer needed as we hash the password ourselves and pass iv and key to openssl/mcrypt instead of the actual password
	private function fClearTextPassBit($sPassword = '') {
        throw new Exception('invalid call to private function', -2);
		//return '-pass pass:' . escapeshellarg($sPassword);
	} // fClearTextPassBit

	private function fFilePassBit($sPassPath = '') {
        throw new Exception('invalid call to private function', -3);
		//return '-pass file:' . escapeshellarg($sPassPath);
	} // fFilePassBit

} // SssS_Cypher3

/* * * *\ SssS_Cypher3.inc (CC)2013 Luke JZ aka SwissalpS /* * * */
?>
