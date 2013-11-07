<?php
/* * * *
 * * SssS_Cypher3mcrypt.inc
 * * Extends SssS_Cypher2 to follow state-of-the art guidelines.
 * *
 * * Mainly by adding an authentication hash to the encrypted data for a way
 * * to check for tampering
 * *
 * * Ciphertext format: [___24_BYTE_CIPHER_MODE_LENGTH_CODE___][____HMAC____]
 * * 			[[____IV____][___1_BYTE_PAD_LENGTH___][____CIPHERTEXT____]].
 * *
 * * [___24_BYTE_CIPHER_MODE_LENGTH_CODE___]: identifies the cipher method,
 * * 		mode, key-, block- and hash-lengths. It can be obfuscated further
 * * 		in a subclass
 * * [____HMAC____]: hash of IV, pad length, pad and encrypted data seeded with
 * * 		a hash of the password.
 * * [____IV____]: purely random initialization vector.
 * * [___1_BYTE_PAD_LENGTH___]: char value indicates length of random padding.
 * * [____CIPHERTEXT____]: encrypted data seeded with IV and hash of password.
 * *
 * * For best security use cbc mode over ecb. Also prefer AES(Rijndael) over
 * * tripple DES, DES nad CAST (in that order). If speed is more important
 * * the list is probably inverse. Please try not to use DES, it's not 'secure'
 * * but tripple DES can be considered secure at this time.
 * *
 * * The result is compatible with SssS_Cypher3openSSLcli.
 * * If you have large files to encrypt, consider using SssS_Cypher3openSSLcli,
 * * but read the notes first as there is a 'ps-security-hole'.
 * *
 * * To remove any temporary traces, run cleanupCypher3() after you are done
 * * even if there was no error!!
 * *
 * * @version 20131027_133820 (CC) Luke JZ aka SwissalpS
 * * * */
// a little english lesson: the cipher is the adjective while the verb is spelt cypher
/*
 * USAGE (using default settings):
 * // instantiate object
 * $oCypher3 = SssS_Cypher3mcrypt::sharedInstance();
 * // encrypt something with AES-256-cbc-128bit block, 192bit key
 * // and save to $sPathOut
 * $mRes = $oCypher3->encryptWithClearTextPassToFile('some super secret data',
 *													'c1371656364a70e4a5b9b00c',
 *													'Password:Monkey123',
 *													$sPathOut);
 * // check for error
 * if (null === $mRes) echo $oCypher3->sOut() . ' ' . $oCypher3->iRes();
 * // cleanup
 * $oCypher3->cleanupCypher3();
 */
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_Cypher3.inc');
} // if not autoincluding but on php's include path

class SssS_Cypher3mcrypt extends SssS_Cypher3 {

    protected $hCipherModes;

    // constructor
	// throws Exception if mcrypt-module is unavailable
    public function SssS_Cypher3mcrypt($aArgs = false) {

        // check if mcrypt functions exist at all
        if (!function_exists('mcrypt_module_open')) {

            throw new Exception('can not use mcrypt functions, use SssS_Cypher3opensslCLI instead.', 1);

        } // if mcrypt is not available

        // initialize the parents
		$this->SssS_Cypher3($aArgs);

    } // SssS_Cypher3mcrypt

	// a singleton/shared object with default settings
	static function sharedInstance() {

		static $o = null;
		if (!$o) $o = new self();

		return $o;

	} // sharedInstance


	// test code
	// returns array with error messages
	// also outputs status to std-out
    static function testCypher3mcrypt($bSkipLongTest = false) {

        $aSmallDataSets = array(
                'Some Data',
                "Some Data With Trailing Nulls\x00\x00",
                'mcrypt can not work with empty data, so this is just another test',
                'Some other super secret stuff'
            );

        $sPassword = self::secureRandom(16);

        $aResults = array();

        $oCypher3 = self::sharedInstance();

        $aCipherCodes = array_keys($oCypher3->hCiphers);
        $iCountCodes = count($aCipherCodes);
        $iCount = 0;

        forEach ($aCipherCodes as $sCipherCode) {

            echo 'Testing cipher code ' . ++$iCount . '/' . $iCountCodes . ': ' . $sCipherCode . NL;

            forEach ($aSmallDataSets as $sData) {

                echo '  Testing data: ' . bin2hex($sData) . NL;

                $mRes = $oCypher3->encryptWithClearTextPass($sData, $sCipherCode, $sPassword);

                if (null === $mRes) {

                    $sMsg = 'FAIL: could not encrypt | ' . $oCypher3->sOut() . ' ' . $oCypher3->iRes();
                    $aResults[] = array('cipherCode' => $sCipherCode, 'result' => $sMsg, 'data' => bin2hex($sData), 'password' => $sPassword);

                    echo '    ' . $sMsg . NL;

                    continue;

                } // if failed to encrypt

                $sDataEncrypted = base64_decode($mRes);

                $sDataDecrypted = $oCypher3->decryptWithClearTextPass($sDataEncrypted, $sPassword);

                if (null === $sDataDecrypted) {

                    $sMsg = 'FAIL: decryption failed | ' . $oCypher3->sOut() . ' ' . $oCypher3->iRes();
                    $aResults[] = array('cipherCode' => $sCipherCode, 'result' => $sMsg, 'data' => bin2hex($sData), 'password' => $sPassword);

                    echo '    ' . $sMsg . NL;

                    continue;

                } // if error decrypting

                if ($sDataDecrypted !== $sData) {

                    $sMsg = 'FAIL: decrypted data does not match original';
                    $aResults[] = array('cipherCode' => $sCipherCode, 'result' => $sMsg, 'data' => bin2hex($sData), 'password' => $sPassword);

                    echo '    ' . $sMsg . NL;

                } else {

                    echo '    PASS' . NL;

                } // if decrypted data is not identical to original

            } // loop each aSmallDataSets

        } // loop each aCipherCodes

        if (0 == count($aResults)) {

            echo 'PASS TEST A' . NL . NL;

        } else {

            echo 'FAIL TEST A, ABORTING' . NL;

            return $aResults;

        } // if TEST A passed or not

        // TEST B - check all methods

        $sTmp1 = $oCypher3->fMakeSafeUniqueFileInPath($oCypher3->sTP);
        $sTmp2 = $oCypher3->fMakeSafeUniqueFileInPath($oCypher3->sTP);
        $sTmp3 = $oCypher3->fMakeSafeUniqueFileInPath($oCypher3->sTP);

        $sPathTmp1 = $oCypher3->sTP . $sTmp1;
        $sPathTmp2 = $oCypher3->sTP . $sTmp2;
        $sPathTmp3 = $oCypher3->sTP . $sTmp3;

        file_put_contents_safely($sPathTmp1, $sData, 'wb');
        $sDataEncryptedBase64 = $oCypher3->encryptWithClearTextPassFromFile($sCipherCode, $sPassword, $sPathTmp1);

        file_put_contents_safely($sPathTmp2, base64_decode($sDataEncryptedBase64), 'wb');
        $sDataDecrypted = $oCypher3->decryptWithClearTextPassFromFile($sPassword, $sPathTmp2);

        if ($sData !== $sDataDecrypted) {

            $sMsg = 'FAIL encryptWithClearTextPassFromFile and decrypWithClearTextPassFromFile, abborting.';

            echo $sMsg . NL;

            $aResults[] = $sMsg;

            return $aResults;

        } // if failed

        echo 'PASS combo 1' . NL;

        file_put_contents_safely($sPathTmp1, $sData, 'wb');
        $oCypher3->encryptWithClearTextPassFromFileToFile($sCipherCode, $sPassword, $sPathTmp1, $sPathTmp2);

        $oCypher3->decryptWithClearTextPassFromFileToFile($sPassword, $sPathTmp2, $sPathTmp3);
        $sDataDecrypted = file_get_contents_safely($sPathTmp3);

        if ($sData !== $sDataDecrypted) {

            $sMsg = 'FAIL encryptWithClearTextPassFromFileToFile and decrypWithClearTextPassFromFileToFile, abborting.';

            echo $sMsg . NL;

            $aResults[] = $sMsg;

            return $aResults;

        } // if failed

        echo 'PASS combo 2' . NL;

        $oCypher3->encryptWithClearTextPassToFile($sData, $sCipherCode, $sPassword, $sPathTmp1);
        $sDataEncrypted = file_get_contents_safely($sPathTmp1);

        $oCypher3->decryptWithClearTextPassToFile($sDataEncrypted, $sPassword, $sPathTmp2);
        $sDataDecrypted = file_get_contents_safely($sPathTmp2);

        if ($sData !== $sDataDecrypted) {

            $sMsg = 'FAIL encryptWithClearTextPassToFile and decrypWithClearTextPassToFile, abborting.';

            echo $sMsg . NL;

            $aResults[] = $sMsg;

            return $aResults;

        } // if failed

        echo 'PASS combo 3' . NL;

        file_put_contents_safely($sPathTmp1, $sPassword, 'wb');
        $sDataEncryptedBase64 = $oCypher3->encryptWithFilePass($sData, $sCipherCode, $sPathTmp1);
        $sDataEncrypted = base64_decode($sDataEncryptedBase64);

        $sDataDecrypted = $oCypher3->decryptWithFilePass($sDataEncrypted, $sPathTmp1);

        if ($sData !== $sDataDecrypted) {

            $sMsg = 'FAIL encryptWithFilePass and decrypWithFilePass, abborting.';

            echo $sMsg . NL;

            $aResults[] = $sMsg;

            return $aResults;

        } // if failed

        echo 'PASS combo 4' . NL;

        file_put_contents_safely($sPathTmp1, $sPassword, 'wb');
        file_put_contents_safely($sPathTmp2, $sData, 'wb');
        $sDataEncryptedBase64 = $oCypher3->encryptWithFilePassFromFile($sCipherCode, $sPathTmp1, $sPathTmp2);

        file_put_contents_safely($sPathTmp2, base64_decode($sDataEncryptedBase64), 'wb');
        $sDataDecrypted = $oCypher3->decryptWithFilePassFromFile($sPathTmp1, $sPathTmp2);

        if ($sData !== $sDataDecrypted) {

            $sMsg = 'FAIL encryptWithFilePassFromFile and decrypWithFilePassFromFile, abborting.';

            echo $sMsg . NL;

            $aResults[] = $sMsg;

            return $aResults;

        } // if failed

        echo 'PASS combo 5' . NL;

        file_put_contents_safely($sPathTmp1, $sPassword, 'wb');
        file_put_contents_safely($sPathTmp2, $sData, 'wb');
        $oCypher3->encryptWithFilePassFromFileToFile($sCipherCode, $sPathTmp1, $sPathTmp2, $sPathTmp3);

        $oCypher3->decryptWithFilePassFromFileToFile($sPathTmp1, $sPathTmp3, $sPathTmp2);
        $sDataDecrypted = file_get_contents_safely($sPathTmp2);

        if ($sData !== $sDataDecrypted) {

            $sMsg = 'FAIL encryptWithFilePassFromFileToFile and decrypWithFilePassFromFileToFile, abborting.';

            echo $sMsg . NL;

            $aResults[] = $sMsg;

            return $aResults;

        } // if failed

        echo 'PASS combo 6' . NL;

        file_put_contents_safely($sPathTmp1, $sPassword, 'wb');
        $oCypher3->encryptWithFilePassToFile($sData, $sCipherCode, $sPathTmp1, $sPathTmp2);
        $sDataEncrypted = file_get_contents_safely($sPathTmp2);

        $oCypher3->decryptWithFilePassToFile($sDataEncrypted, $sPathTmp1, $sPathTmp2);

        $sDataDecrypted = file_get_contents_safely($sPathTmp2);

        if ($sData !== $sDataDecrypted) {

            $sMsg = 'FAIL encryptWithFilePassToFile and decrypWithFilePassToFile, abborting.';

            echo $sMsg . NL;

            $aResults[] = $sMsg;

            return $aResults;

        } // if failed

        echo 'PASS combo 7' . NL . NL;

        echo 'PASS TEST B: basic methods, now starting test of meddled data' . NL . NL;

        $sDataEncryptedBase64 = $oCypher3->encryptWithClearTextPass($sData, $sCipherCode, $sPassword);

        $sDataEncrypted = base64_decode($sDataEncryptedBase64);
        $sDataEncryptedTampered = $sDataEncrypted . 'z';
        $sDataDecrypted = $oCypher3->decryptWithClearTextPass($sDataEncryptedTampered, $sPassword);

		if (null === $sDataDecrypted) {

            if ('HMAC missmatch' !== substr($oCypher3->sOut(), 0, 14)) {

				echo 'WARNING test passed but not with expected message. | '
					. $oCypher3->sOut() . ' (' . $oCypher3->iRes() . ')' . NL;

            } // if not what expected

        } // if passed by failing

        if (null === $sDataDecrypted) {

            echo 'PASS tamper test 1' . NL;

        } else {

            $sMsg = 'FAIL tamper test 1, abborting.';

            echo $sMsg . NL;

            $aResults[] = $sMsg;

			return $aResults;

        } // if passed or not

        $sDataEncryptedTampered = $sDataEncrypted;
        $sDataEncryptedTampered[0] = chr((ord($sDataEncrypted[0]) + 1) % 256);
        $sDataDecrypted = $oCypher3->decryptWithClearTextPass($sDataEncryptedTampered, $sPassword);

        if (null === $sDataDecrypted) {

            if ('invalid cipher-code' !== substr($oCypher3->sOut(), 0, 19)) {

				echo 'WARNING test passed bot not with expected message. | '
					. $oCypher3->sOut() . ' (' . $oCypher3->iRes() . ')' . NL;

            } // if not what expected

        } // if passed by failing

        if (null === $sDataDecrypted) {

            echo 'PASS tamper test 2' . NL;

        } else {

            $sMsg = 'FAIL tamper test 2, abborting.';

            echo $sMsg . NL;

            $aResults[] = $sMsg;

			return $aResults;

        } // if passed or not

        echo 'PASS TEST C: test of meddled data, now checking if wrong password affects' . NL . NL;

        // we need to rebuild temp paths as they may have been cleared..
        $sTmp1 = $oCypher3->fMakeSafeUniqueFileInPath($oCypher3->sTP);
        $sTmp2 = $oCypher3->fMakeSafeUniqueFileInPath($oCypher3->sTP);
        $sTmp3 = $oCypher3->fMakeSafeUniqueFileInPath($oCypher3->sTP);

        $sPathTmp1 = $oCypher3->sTP . $sTmp1;
        $sPathTmp2 = $oCypher3->sTP . $sTmp2;
        $sPathTmp3 = $oCypher3->sTP . $sTmp3;

        // check if wrong password fails especially at very long lengths
        $sPasswordLong = self::secureRandom(256);
        file_put_contents_safely($sPathTmp1, $sPasswordLong, 'wb');

        $oCypher3->encryptWithFilePassToFile($sData, $sCipherCode, $sPathTmp1, $sPathTmp2);
        $sDataEncrypted = file_get_contents_safely($sPathTmp2);

        $sDataDecrypted = $oCypher3->decryptWithClearTextPass($sDataEncrypted, $sPasswordLong);

        if (null === $sDataDecrypted) {

            $sMsg = 'FAIL cannot work with 256-byte-long password, bailing';

			echo $sMsg . NL;

			$aResults[] = $sMsg;

            return $aResults;

        } // if failed without tampering

        $sPasswordLongWrong = self::secureRandom(256);

        $sDataDecrypted = $oCypher3->decryptWithClearTextPass($sDataEncrypted, $sPasswordLongWrong);

        if (null === $sDataDecrypted) {

            if ('HMAC missmatch' !== substr($oCypher3->sOut(), 0, 14)) {

				echo 'WARNING test passed but not with expected message | '
					. $oCypher3->sOut() . ' (' . $oCypher3->iRes() . ')' . NL;

            } // if not what we expected

        } // if passed by failing

        if (null === $sDataDecrypted) {

            echo 'PASS long and very wrong password' . NL;

        } else {

			$sMsg = 'FAIL long and very wrong password';

			echo $sMsg . NL;

            $aResults[] = $sMsg;

			return $aResults;

        } // if pass or not

        $sPasswordLongWrong = $sPasswordLong;
        $sPasswordLongWrong[255] = chr((ord($sPasswordLongWrong[255]) + 1) % 256);

        $sDataDecrypted = $oCypher3->decryptWithClearTextPass($sDataEncrypted, $sPasswordLongWrong);

        if (null === $sDataDecrypted) {

            if ('HMAC missmatch' !== substr($oCypher3->sOut(), 0, 14)) {

				echo 'WARNING test passed but not with expected message | '
					. $oCypher3->sOut() . ' (' . $oCypher3->iRes() . ')' . NL;

            } // if not what we expected

        } // if passed by failing

        if (null === $sDataDecrypted) {

            echo 'PASS';

        } else {

            echo 'FAIL';
            $aResults[] = 'FAIL long and slightly wrong password';

        } // if pass or not

        echo ' long and slightly wrong password' . NL;

        if (0 < count($aResults)) return $aResults;

        echo 'PASS TEST D' . NL . NL;

        echo 'TEST E: Testing null tolerance' . NL;

		$sData = 'a' . chr(0) . chr(0) . chr(0) . chr(0) . chr(0) . chr(0) . chr(0) . chr(0) . 'b';
		$iLength = 10;

		if (10 !== strlen($sData)) {

			echo 'FAIL' . NL;
			$aResults[] = 'FAIL PHP truncates strings at null';
			return $aResults;

		} // if php already truncates input data

		// TODO: test against all cipher-codes?

		$sDataEncrypted = base64_decode($oCypher3->encryptWithClearTextPass($sData, $sCipherCode, $sPasswordLong));

		$sDataDecrypted = $oCypher3->decryptWithClearTextPass($sDataEncrypted, $sPasswordLong);

		if (10 !== strlen($sDataDecrypted)) {

			echo 'FAIL' . NL;
			$aResults[] = 'FAIL decrypted data is truncated at null';
			return $aResults;

		} // if decrypted data is truncated

		if ($sData !== $sDataDecrypted) {

			echo 'FAIL' . NL;
			$aResults[] = 'FAIL decrypted data does not match original data with nulls';
			return $aResults;

		} // if decrypted data is not same as original

        echo 'PASS TEST E' . NL . NL;

		if (!$bSkipLongTest) {

			echo 'TEST F: Testing against larger files and all cipher codes' . NL;

			// make sure our temp files exist
			$sTmp1 = $oCypher3->fMakeSafeUniqueFileInPath($oCypher3->sTP);
			$sTmp2 = $oCypher3->fMakeSafeUniqueFileInPath($oCypher3->sTP);
			$sTmp3 = $oCypher3->fMakeSafeUniqueFileInPath($oCypher3->sTP);

			$sPathTmp1 = $oCypher3->sTP . $sTmp1;
			$sPathTmp2 = $oCypher3->sTP . $sTmp2;
			$sPathTmp3 = $oCypher3->sTP . $sTmp3;

			$aResults = array();
			$aBaseFileSizes = array(1024, 5 * 1024, 32 * 1024);
			$iCountBaseFileSizes = count($aBaseFileSizes);
			echo 'About to test ' . $iCountBaseFileSizes . ' base file sizes.' . NL;

			forEach ($aBaseFileSizes as $iFilesize) {

				// write random data to temp file 1
				file_put_contents_safely($sPathTmp1, self::secureRandom(1), 'wb');

				$iFilesizeWritten = 1;

				$iChunksize = 256;

				$iFullChunks = floor(($iFilesize - 1) / $iChunksize);

				for ($iCountChunks = 0; $iFullChunks < $iCountChunks; $iCountChunks++) {

					file_put_contents_safely($sPathTmp1, self::secureRandom($iChunksize));
					$iFilesizeWritten += $iChunksize;

				} // loop with iCountChunks to $iFullChunks

				// write remaining
				$iChunksize = $iFilesize - $iFilesizeWritten;
				file_put_contents_safely($sPathTmp1, self::secureRandom($iChunksize));
				$iFilesizeWritten += $iChunksize;

				$iChunksize = 1024 * 4;

				// add some extra to filesize
				for ($iAdded = 1; 260 > $iAdded; $iAdded++) {

					file_put_contents_safely($sPathTmp1, self::secureRandom($iAdded));
					 $iFilesizeWritten += $iAdded;

					echo '  Testing file size in bytes: ' . $iFilesizeWritten . NL;

					$iCount = 0;
					forEach ($aCipherCodes as $sCipherCode) {

						echo '    Testing cipher code ' . ++$iCount . '/' . $iCountCodes . ': ' . $sCipherCode . NL;

						$mRes = $oCypher3->encryptWithClearTextPassFromFileToFile($sCipherCode, $sPasswordLong, $sPathTmp1, $sPathTmp2);
						if (null === $mRes) {

							$sMsg = 'FAIL: could not encrypt | ' . $oCypher3->sOut() . ' ' . $oCypher3->iRes();
							$aResults[] = array('cipherCode' => $sCipherCode, 'result' => $sMsg);

							echo '    ' . $sMsg . NL;

							continue;

						} // if failed to encrypt

						$mRes = $oCypher3->decryptWithClearTextPassFromFileToFile($sPasswordLong, $sPathTmp2, $sPathTmp3);
						if (null === $mRes) {

							$sMsg = 'FAIL: decryption failed | ' . $oCypher3->sOut() . ' ' . $oCypher3->iRes();
							$aResults[] = array('cipherCode' => $sCipherCode, 'result' => $sMsg);

							echo '    ' . $sMsg . NL;

							continue;

						} // if error decrypting

						// now compare file contents
						$bSame = true;
						$rF1 = fopen($sPathTmp1, 'rb');
						$rF2 = fopen($sPathTmp3, 'rb');

						while (!feof($rF1) and !feof($rF2)) {

							if (fread($rF1, $iChunksize) !== fread($rF2, $iChunksize)) {

								$bSame = false;
								break;

							} // if not matching contents

						} // loop until either file has reached end

						// check that both files have been fully read
						if (feof($rF1) !== feof($rF2)) $bSame = false;

						// close the files
						fclose($rF1); fclose($rF2);

						if (!$bSame) {

						   $sMsg = 'FAIL: decrypted data does not match original';
						   $aResults[] = array('cipherCode' => $sCipherCode, 'result' => $sMsg);

						   echo '    ' . $sMsg . NL;

						} else {

						   echo '    PASS' . NL;

						} // if decrypted data is not identical to original

					} // loop each aCypherCodes

				} // loop with iAdded

			} // loop each base file size

			if (0 == count($aResults)) {

				echo 'TEST F PASSED (if there were no other errors above' . NL . NL;

			} else {

				echo 'TEST F FAILED' . NL;

			} // if TEST F passed or not

		} // if do long test

        $oCypher3->cleanupCypher3();

        return $aResults;

    } // testCypher3mcrypt

	/* * * * *  *   /PUBLIC enCRYPTING FUNCTIONS\   *  * * * * */

    // main functions
	// returns NULL or the encrypted data
    public function encrypt($sContent = '', $sCipherCode = '', $sPassword = '') {

        // if invalid cipher-code
        if (!isset($this->hCiphers[$sCipherCode]))
                return $this->fail('invalid cipher-code', 2);

		// if no data, no need to encrypt
		if (empty($sContent)) return $this->fail('no data to encrypt', -24);

		// if no password, no need to encrypt
		if (empty($sPassword)) return $this->fail('no password given', -25);

        // determine the algorithm, method and sizes from the cipher-code

		$sCipher        = $this->hCiphers[$sCipherCode];
        $sCipherHMAC    = $this->hCipherHMAC[$sCipherCode];
        $sMode          = $this->hCipherModes[$sCipherCode];
        $iByteSizeIV    = $this->hCipherSizeIV[$sCipherCode];
		$iByteSizeKey   = $this->hCipherSizeKey[$sCipherCode];
        $iByteSizeBlock = $this->hCipherSizeBlock[$sCipherCode];
        $iByteSizeHMAC  = strlen(hash_hmac($sCipherHMAC, '', '', true));

        // generate the key for encryption
        $sKeyEnc = self::createSubkey($sPassword, self::subKeyDataEncryption(), $iByteSizeKey);
        if (false === $sKeyEnc) return $this->fail('error making subkey for encryption', -11);

        // generate a random initialization vector and convert to string of hex values
        try {

            $sIV = self::secureRandom($iByteSizeIV);

        } catch(Exception $e) {

            return $this->fail('error getting IV: ' . $e->getMessage(), -12);

        } // try catch IV creation

        // pad contents as needed
        $iByteSizePad = $iByteSizeBlock - (strlen($sContent) % $iByteSizeBlock);

		// add an extra block of padding for more compatibilty with eg Java
		$iByteSizePad += $iByteSizeBlock;

		// unless someone does something silly like choosing a huge block-size
		// of 128 bytes instead of bits... there's trouble ahead in such a case
		// anyway. I just feel this needs to be addressed somehow.
		//if (255 < $iByteSizePad) $iByteSizePad -= $iByteSizeBlock;

		// this is always true since we now add one extra block
        //if (0 < $iByteSizePad) {
		try {

            // prepend the padding
            $sPad = self::secureRandom($iByteSizePad);

            $sContent = $sPad . $sContent;

		} catch (Exception $e) {

			return $this->fail('could not get random padding of length: '
							   . $iByteSizePad . ' | ' . $e->getMessage(), -33);

		} // try catch getting random pad

        //} // if needs paddinng

        // encrypt prepending IV and pad-length
		$sEncrypted = mcrypt_encrypt($sCipher, $sKeyEnc, $sContent, $sMode, $sIV);
        $sEncrypted = $sIV . chr($iByteSizePad) . $sEncrypted;

        // generate the key for authentication
        $sKeyAuth = self::createSubkey($sPassword,
									   self::subKeyDataAuthentication(),
									   $iByteSizeKey);
		if (false === $sKeyAuth) return $ths->fail('could not create authentication key', -36);

        // calculate the HMAC
        $sHMAC = hash_hmac($sCipherHMAC, $sEncrypted, $sKeyAuth, true);
        if ($iByteSizeHMAC > strlen($sHMAC)) return $this->fail(
                'hmac is shorter than ' . $iByteSizeHMAC . ' bytes!', -8);
        $sHMAC = substr($sHMAC, 0, $iByteSizeHMAC);

        // Apply the cipher-code and HMAC
        $sEncrypted = $sCipherCode . $sHMAC . $sEncrypted;

        return $sEncrypted;
        // [___24_BYTE_CIPHER_MODE_LENGTH_CODE___][____HMAC____][[____IV____][___1_BYTE_PAD_LENGTH___][____CIPHERTEXT____]]

    } // encrypt


    // cipher-code is in $sEncrypted
    // return NULL or the data decrypted
    public function decrypt($sEncrypted = '', $sPassword = '') {

		// no password - no need to decrypt
        if (empty($sPassword)) return $this->fail('no password given, abborting', -32);

        if (28 >= strlen($sEncrypted)) return $this->fail('input string is too short', -31);

        // Extract the cipher-code from the front of $sEnrcypted.
        $sCipherCode = substr($sEncrypted, 0, 24);
        $sEncrypted = substr($sEncrypted, 24);

        // if invalid cipher-code
        if (!isset($this->hCiphers[$sCipherCode]))
                return $this->fail('invalid cipher-code', -17);

		$sCipher        = $this->hCiphers[$sCipherCode];
        $sCipherHMAC    = $this->hCipherHMAC[$sCipherCode];
        $sMode          = $this->hCipherModes[$sCipherCode];
        $iByteSizeIV    = $this->hCipherSizeIV[$sCipherCode];
		$iByteSizeKey   = $this->hCipherSizeKey[$sCipherCode];
        $iByteSizeBlock = $this->hCipherSizeBlock[$sCipherCode];
        $iByteSizeHMAC  = strlen(hash_hmac($sCipherHMAC, '', '', true));

        // extract HMAC
        $sHMAC = substr($sEncrypted, 0, $iByteSizeHMAC);
        $sEncrypted = substr($sEncrypted, $iByteSizeHMAC);

        // regenerate the authentication sub-key
        $sKeyAuth = self::createSubkey($sPassword, self::subKeyDataAuthentication(), $iByteSizeKey);
		if (false === $sKeyAuth) return $this->fail('could not create authentication key', -23);

        // make sure the HMAC matches. If not the file has been tampered with!
        $sHMAC1 = hash_hmac($sCipherHMAC, $sEncrypted, $sKeyAuth, true);
        if ($iByteSizeHMAC > strlen($sHMAC1)) return $this->fail(
                'hmac is shorter than ' . $iByteSizeHMAC . ' bytes!', -20);

        $sHMAC1 = substr($sHMAC1, 0, $iByteSizeHMAC);

		// if HMACs do not match
        if (!self::slowEquals($sHMAC, $sHMAC1))
				return $this->fail('HMAC missmatch, aborting', -21);

        // extract IV
        $sIV = substr($sEncrypted, 0, $iByteSizeIV);
        $sEncrypted = substr($sEncrypted, $iByteSizeIV);

        // read padding length
        $iByteSizePad = ord($sEncrypted[0]);
        $sEncrypted = substr($sEncrypted, 1);

        // regenerate the encryption sub-key
        $sKeyEnc = self::createSubkey($sPassword, self::subKeyDataEncryption(), $iByteSizeKey);
        if (false === $sKeyEnc) return $this->fail('error making subkey for decryption', -22);

        // Do the decryption.
        $sPlaintext = mcrypt_decrypt($sCipher, $sKeyEnc, $sEncrypted, $sMode, $sIV);

        // Remove the padding.
        $sPlaintext = substr($sPlaintext, $iByteSizePad);

        return $sPlaintext;

    } // decrypt







    // returns NULL or $sContent encrypted and base64 encoded
	public function encryptWithClearTextPass($sContent = '',
											 $sCipherCode = '',
											 $sPassword = '') {

		// encrypt
		$sEncrypted = $this->encrypt($sContent, $sCipherCode, $sPassword);

        return (null === $sEncrypted) ? NULL : base64_encode($sEncrypted);

	} // encryptWithClearTextPass


    // returns NULL or base64 encoded encrypted data taken from $sPathIn
	public function encryptWithClearTextPassFromFile($sCipherCode = '',
													 $sPassword = '',
													 $sPathIn = '') {

        $sContent = @file_get_contents_safely($sPathIn);
        if (false === $sContent) return $this->fail('unable to read from '
													. $sPathIn, -35);

		$sEncrypted = $this->encrypt($sContent, $sCipherCode, $sPassword);

        return (null === $sEncrypted) ? null : base64_encode($sEncrypted);

	} // encryptWithClearTextPassFromFile


    // returns NULL or TRUE the encrypted data in $sPathOut is NOT base64 encoded
	public function encryptWithClearTextPassFromFileToFile($sCipherCode = '',
														   $sPassword = '',
														   $sPathIn = '',
														   $sPathOut = '') {

        $sContent = @file_get_contents_safely($sPathIn);
        if (false === $sContent) return $this->fail('unable to read from '
													. $sPathIn, -34);

        $sEncrypted = $this->encrypt($sContent, $sCipherCode, $sPassword);
		if (null === $sEncrypted) return null;

        $mRes = @file_put_contents_safely($sPathOut, $sEncrypted, 'wb');

        return (false !== $mRes) ? true : null;

	} // encryptWithClearTextPassFromFileToFile


    // returns NULL or TRUE the encrypted data in $sPathOut is NOT base64 encoded
	public function encryptWithClearTextPassToFile($sContent = '',
												   $sCipherCode = '',
												   $sPassword = '',
												   $sPathOut = '') {

        $sEncrypted = $this->encrypt($sContent, $sCipherCode, $sPassword);
		if (null === $sEncrypted) return null;

		$mRes = @file_put_contents_safely($sPathOut, $sEncrypted, 'wb');

        return (false !== $mRes) ? true : null;

	} // encryptWithClearTextPassToFile


    // returns NULL or the encrypted data base64 encoded
	public function encryptWithFilePass($sContent = '',
										$sCipherCode = '',
										$sPassPath = '') {

		$sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword)
				return $this->fail('could not read password from file: '
								   . $sPassPath, -37);

        $sEncrypted = $this->encrypt($sContent, $sCipherCode, $sPassword);

        return (null === $sEncrypted) ? null : base64_encode($sEncrypted);

	} // encryptWithFilePass


    // returns NULL or the base64 encoded and encrypted data
	public function encryptWithFilePassFromFile($sCipherCode = '',
												$sPassPath = '',
												$sPathIn = '') {

		$sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword)
				return $this->fail('could not read password from file: '
								   . $sPassPath, -38);

        $sContent = @file_get_contents_safely($sPathIn);
		if (false === $sContent)
				return $this->fail('could not read content from file: '
								   . $sPathIn, -39);

        $sEncrypted = $this->encrypt($sContent, $sCipherCode, $sPassword);

        return (null === $sEncrypted) ? null : base64_encode($sEncrypted);

	} // encryptWithFilePassFromFile


    // returns NULL or TRUE. The encrypted data in $sPathOut is NOT base64 encoded
	public function encryptWithFilePassFromFileToFile($sCipherCode = '',
													  $sPassPath = '',
													  $sPathIn = '',
													  $sPathOut = '') {

		$sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword)
				return $this->fail('could not read password from file: '
								   . $sPassPath, -40);

        $sContent = @file_get_contents_safely($sPathIn);
		if (false === $sContent)
				return $this->fail('could not read content from file: '
								   . $sPathIn, -41);

        $sEncrypted = $this->encrypt($sContent, $sCipherCode, $sPassword);
		if (null === $sEncrypted) return null;

        $mRes = @file_put_contents_safely($sPathOut, $sEncrypted, 'wb');
		if (false === $mRes)
				return $this->fail('could not write encrypted data to file: '
								   . $sPathOut, -42);

        return true;

	} // encryptWithFilePassFromFileToFile


    // returns NULL or TRUE the data in $sPathOut is NOT base64 encoded
	public function encryptWithFilePassToFile($sContent = '',
											  $sCipherCode = '',
											  $sPassPath = '',
											  $sPathOut = '') {

		$sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword)
				return $this->fail('could not read password from file: '
								   . $sPassPath, -43);

        $sEncrypted = $this->encrypt($sContent, $sCipherCode, $sPassword);
		if (null === $sEncrypted) return null;

		$mRes = @file_put_contents_safely($sPathOut, $sEncrypted, 'wb');
		if (false === $mRes)
				return $this->fail('could not write encrypted data to file: '
								   . $sPathOut, -44);

        return true;

	} // encryptWithFilePassToFile

	/* * * * *  *   \PUBLIC enCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PUBLIC deCRYPTING FUNCTIONS\   *  * * * * */

    // returns NULL or the decrypted data
	public function decryptWithClearTextPass($sEncrypted = '',
											 $sPassword = '') {

		return $this->decrypt($sEncrypted, $sPassword);

	} // decryptWithClearTextPass


    // returns NULL or the decrypted data
	public function decryptWithClearTextPassFromFile($sPassword = '',
													 $sPathIn = '') {

        $sEncrypted = @file_get_contents_safely($sPathIn);
		if (false === $sEncrypted)
				return $this->fail('could not read encrypted data from file: '
								   . $sPathIn, -45);

		$sContent = $this->decrypt($sEncrypted, $sPassword);

        return $sContent;

	} // decryptWithClearTextPassFromFile


    // returns NULL or TRUE
	public function decryptWithClearTextPassFromFileToFile($sPassword = '',
														   $sPathIn = '',
														   $sPathOut = '') {

        $sEncrypted = @file_get_contents_safely($sPathIn);
		if (false === $sEncrypted)
				return $this->fail('could not read encrypted data from file: '
								   . $sPathIn, -46);

		$sContent = $this->decrypt($sEncrypted, $sPassword);
		if (null === $sContent) return null;

        $mRes = @file_put_contents_safely($sPathOut, $sContent, 'wb');
		if (false === $mRes)
				return $this->fail('could not write decrypted content to file: '
								   . $sPathOut, -47);

        return true;

	} // decryptWithClearTextPassFromFileToFile


    // returns NULL or the decrypted data which also is in $sPathOut
	public function decryptWithClearTextPassToFile($sEncrypted = '',
												   $sPassword = '',
												   $sPathOut = '') {

		$sContent = $this->decrypt($sEncrypted, $sPassword);
		if (null === $sContent) return null;

        $mRes = @file_put_contents_safely($sPathOut, $sContent, 'wb');
		if (false === $mRes)
				return $this->fail('could not write decrypted content to file: '
								   . $sPathOut, -48);

        return true;

	} // decryptWithClearTextPassToFile


    // returns NULL or the decrypted data
	public function decryptWithFilePass($sEncrypted = '', $sPassPath = '') {

		$sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword)
				return $this->fail('could not read password from file: '
								   . $sPassPath, -49);

        $sContent = $this->decrypt($sEncrypted, $sPassword);

        return $sContent;

	} // decryptWithFilePass


    // returns NULL or decrypted data
	public function decryptWithFilePassFromFile($sPassPath = '', $sPathIn = '') {

        $sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword)
				return $this->fail('could not read password from file: '
								   . $sPassPath, -50);

        $sEncrypted = @file_get_contents_safely($sPathIn);
		if (false === $sEncrypted)
				return $this->fail('could not read encrypted data from file: '
								   . $sPathIn, -51);

        $sContent = $this->decrypt($sEncrypted, $sPassword);

        return $sContent;

	} // decryptWithFilePassFromFile


    // returns NULL or TRUE
	public function decryptWithFilePassFromFileToFile($sPassPath = '',
													  $sPathIn = '',
													  $sPathOut = '') {

		$sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword)
				return $this->fail('could not read password from file: '
								   . $sPassPath, -52);

        $sEncrypted = @file_get_contents_safely($sPathIn);
		if (false === $sEncrypted)
				return $this->fail('could not read encrypted data from file: '
								   . $sPathIn, -53);

        $sContent = $this->decrypt($sEncrypted, $sPassword);
		if (null === $sContent) return null;

		$mRes = @file_put_contents_safely($sPathOut, $sContent, 'wb');
		if (false === $mRes)
				return $this->fail('could not write decrypted content to file: '
								   . $sPathOut, -54);

        return true;

	} // decryptWithFilePassFromFileToFile


    // returns NULL or the decrypted content which is also in $sPathOut
	public function decryptWithFilePassToFile($sEncrypted = '',
											  $sPassPath = '',
											  $sPathOut = '') {

		$sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword)
				return $this->fail('could not read password from file: '
								   . $sPassPath, -55);

        $sContent = $this->decrypt($sEncrypted, $sPassword);
		if (null === $sContent) return null;

		$mRes = @file_put_contents_safely($sPathOut, $sContent, 'wb');
		if (false === $mRes)
				return $this->fail('could not write decrypted content to file: '
								   . $sPathOut, -56);

        return true;

	} // decryptWithFilePassToFile

	/* * * * *  *   \PUBLIC deCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   PRIVATE FUNCTIONS   *  * * * * */

	/* * * * *  *   /PRIVATE enCRYPTING FUNCTIONS\   *  * * * * */

	// alias to encryptWithClearTextPassFromFileToFile()
	private function encryptFromFileToFile($sCipherCode = '',
										   $sPassword = '',
										   $sPathIn = '',
										   $sPathOut = '') {

        return $this->encryptWithClearTextPassFromFileToFile($sCipherCode,
															 $sPassword,
															 $sPathIn,
															 $sPathOut);

	} // encryptFromFileToFile

	/* * * * *  *   \PRIVATE enCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PRIVATE deCRYPTING FUNCTIONS\   *  * * * * */

    // [___24_BYTE_CIPHER_MODE_LENGTH_CODE___][____HMAC____][[____IV____][___1_BYTE_PAD_LENGTH___][____CIPHERTEXT____]]
	// alias to decryptWithClearTextPassFromFileToFile()
	private function decryptFromFileToFile($sPassword = '', $sPathIn = '', $sPathOut = '') {

        return $this->decryptWithClearTextPassFromFileToFile($sPassword, $sPathIn, $sPathOut);

	} // decryptFromFileToFile

	/* * * * *  *   \PRIVATE deCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PRIVATE FUNCTIONS 4 SUBCLASSING\   *  * * * * */

	// subclass this for more obfuscation
	protected function initCiphers() {

		$aCiphers = array(
            // AES-256 is MCRYPT_RIJNDAEL_128 NOT RIJNDAEL_256!!!
                MCRYPT_RIJNDAEL_128, MCRYPT_RIJNDAEL_128,
                MCRYPT_RIJNDAEL_128, MCRYPT_RIJNDAEL_128,
                MCRYPT_RIJNDAEL_128, MCRYPT_RIJNDAEL_128,
                // cast-128-cbc with openssl (cast5)
                MCRYPT_CAST_128,
                // (cast6) not supported by openssl
                MCRYPT_CAST_128, MCRYPT_CAST_128,
				//MCRYPT_CAST_256, MCRYPT_CAST_256,

                MCRYPT_DES, MCRYPT_DES,
				//MCRYPT_3DES, MCRYPT_3DES,
				MCRYPT_TRIPLEDES, MCRYPT_TRIPLEDES
            );

        $aCipherModes = array(
                MCRYPT_MODE_CBC, MCRYPT_MODE_ECB,
                MCRYPT_MODE_CBC, MCRYPT_MODE_ECB,
                MCRYPT_MODE_CBC, MCRYPT_MODE_ECB,
                MCRYPT_MODE_CBC,
                MCRYPT_MODE_CBC, MCRYPT_MODE_ECB,
                //MCRYPT_MODE_CBC, MCRYPT_MODE_ECB,
                MCRYPT_MODE_CBC, MCRYPT_MODE_ECB,
                //MCRYPT_MODE_CBC, MCRYPT_MODE_ECB,
                MCRYPT_MODE_CBC, MCRYPT_MODE_ECB
            );

        $aSizeBlocks = array(
                16, 16, 16, 16, 16, 16,
                8,
				8, 8,
                //16, 16,
                8, 8,
                //8, 8,
                8, 8
            );

        $aSizeKeys = array(
                16, 16, 24, 24, 32, 32,
                16,
				16, 16,
                //32, 32,
                8, 8,
				//24, 24,
				24, 24,
            );

        $aSizeIVs = array(
                16, 16, 16, 16, 16, 16,
                8,
				8, 8,
                //16, 16,
                8, 8,
				//8, 8,
				8, 8
            );

        $aCiphersHMAC = array(
                'sha256', 'sha256', 'sha256', 'sha256', 'sha256', 'sha256',
                'sha256',
                'sha256', 'sha256',
                //'sha256', 'sha256',
                'sha256', 'sha256',
                //'sha256', 'sha256',
                'sha256', 'sha256'
            );

        // all have uniform length: 24 bytes
		$aKeys = array(
                'c16656459864a70e4a5b9acd', 'c2045100124a70e4a5b9af4c',
                'c1371656364a70e4a5b9b00c', 'c8203878634a70e4a5b9b0bc',
                'c12472937914a70e4a5b9b22', 'c6056705394a70e4a5b9b2dc',

                'c16550030104a70e4a5b9b87',

                'c499833734a70e4a5b9b92c4', 'c4410822034a70e4a5b9ba8c',
				//'c77474c0033a911e76c47b12', 'c47234fe70c324d343be2026',

                'c8110343934a70e4a5b9bcbc', 'c17935449364a70e4a5b9be1',
				//'cf01eb1e81796a23a566cd7c', 'c4363322bb31af9b13d1cfa3',
				'c8085cbf6afa5e5f6bdb99c9', 'cfc37cf15b866f71ea4d5011'
            );

        $this->hCipherSizeBlock = array_combine($aKeys, $aSizeBlocks);
        $this->hCipherSizeKey   = array_combine($aKeys, $aSizeKeys);
        $this->hCipherSizeIV    = array_combine($aKeys, $aSizeIVs);
        $this->hCipherHMAC      = array_combine($aKeys, $aCiphersHMAC);
        $this->hCipherModes     = array_combine($aKeys, $aCipherModes);
		$this->hCiphers         = array_combine($aKeys, $aCiphers);

	} // initCiphers

	/* * * * *  *   \PRIVATE FUNCTIONS 4 SUBCLASSING/   *  * * * * */

} // SssS_Cypher3mcrypt

/* * * *\ SssS_Cypher3mcrypt.inc (CC)2013 Luke JZ aka SwissalpS /* * * */
?>
