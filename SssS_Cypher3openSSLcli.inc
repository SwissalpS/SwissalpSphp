<?php
/* * * *
 * * SssS_Cypher3openSSLcli.inc
 * * Extends SssS_Cypher2 to follow state-of-the art guidelines.
 * * Check out grc.com and defuse.ca for examples and security information.
 * *
 * * Main change: adding an authentication hash to the encrypted data for a way
 * * to check for tampering of encrypted data. This makes the output incompatible
 * * with the output of SssS_Cypher2.
 * *
 * * Ciphertext format: [___24_BYTE_CIPHER_MODE_LENGTH_CODE___][____HMAC____]
 * *                 [[____IV____][___1_BYTE_PAD_LENGTH___][____CIPHERTEXT____]]
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
 * * This is a great step forward in the security of the encrypted blob,
 * * however, this is not without consession: watching `ps` output, somebody
 * * could see the KEY AND IV being passed as shell arguments.
 * * It is not the password, but with those and access to this code, the blob
 * * can be decrypted without knowing the password used.
 * * If you can use mcrypt functions, do so with SssS_Cypher3mcrypt which is
 * * compatible with this class.
 * *
 * * The main reason to use this class, is to encrypt large files. You need
 * * space for two copies in the temporary location.
 * *
 * * Make sure you catch Exceptions and errors so you can run cleanupCypher3()
 * * to remove any temporary traces.
 * * Run cleanupCypher3() after you are done even if there was no error!
 * *
 * * @version 20131019_083530 (CC) Luke JZ aka SwissalpS
 * * * */
// a little english lesson: the cipher is the adjective while the verb is spelt cypher
/*
 * USAGE (using default settings):
 * // instantiate object
 * $oCypher3 = SssS_Cypher3openSSLcli::sharedInstance();
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

class SssS_Cypher3openSSLcli extends SssS_Cypher3 {

    // constructor
    public function SssS_Cypher3openSSLcli($aArgs = false) {

        // initialize the parents
		$this->SssS_Cypher3($aArgs);

    } // SssS_Cypher3openSSLcli


	// returns a singleton with default parameters
	static function sharedInstance() {

		static $o = null;
		if (!$o) $o = new self();

		return $o;

	} // sharedInstance


    // test code, this can take a while...
	// returns an array with errors
	// also outputs status report to std-out
    static function testCypher3openSSLcli($bSkipLongTest = false) {

        $aSmallDataSets = array(
                'Some Data',
                "Some Data With Trailing Nulls\x00\x00",
                'We no longer allow empty data, so here is some more data',
                'Some other super secret stuff'
            );

        $sPassword = self::secureRandom(16);

        $aResults = array();

        $oCypher3 = self::sharedInstance();

        $aCipherCodes = array_keys($oCypher3->hCiphers);
        $iCountCodes = count($aCipherCodes);
        $iCount = 0;

        forEach ($aCipherCodes as $sCipherCode) {

            echo 'Testing cipher-code ' . ++$iCount . '/' . $iCountCodes . ': ' . $sCipherCode . NL;

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

                    echo '    ' . $sMsg . NL;exit(-1);

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

            echo 'TEST A PASSED' . NL . NL;

        } else {

            echo 'TEST A FAILED, ABORTING' . NL;

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

        $sDataDecrypted = $oCypher3->decryptWithClearTextPassToFile($sDataEncrypted, $sPassword, $sPathTmp2);

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

            $sMsg = 'FAIL encryptWithFilePassFromFileToFile and decrypWithFilePassFromFileToFile, abborting. |' . $oCypher3->sOut() . '(' . $oCypher3->iRes() . ')';

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

            echo 'PASS tamper test 1' . NL;

        } else {

            echo 'FAIL tamper test 1' . NL;

        } // if passed or not

        $sDataEncryptedTampered = $sDataEncrypted;
        $sDataEncryptedTampered[0] = chr((ord($sDataEncrypted[0]) + 1) % 256);
        $sDataDecrypted = $oCypher3->decryptWithClearTextPass($sDataEncryptedTampered, $sPassword);
        if (null === $sDataDecrypted) {

            echo 'PASS tamper test 2' . NL;

        } else {

            echo 'FAIL tamper test 2' . NL;

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

            echo 'FAIL cannot work with 256-byte-long password, bailing' . NL;
            return $aResults;

        } // if failed without tampering

        $sPasswordLongWrong = self::secureRandom(256);
        $sDataDecrypted = $oCypher3->decryptWithClearTextPass($sDataEncrypted, $sPasswordLongWrong);

        if (null === $sDataDecrypted) {

            echo 'PASS';

        } else {

            echo 'FAIL';
            $aResults[] = 'FAIL long and very wrong password';

        } // if pass or not

        echo ' long and very wrong password' . NL;

        $sPasswordLongWrong = $sPasswordLong;
        $sPasswordLongWrong[255] = chr((ord($sPasswordLongWrong[255]) + 1) % 256);

        $sDataDecrypted = $oCypher3->decryptWithClearTextPass($sDataEncrypted, $sPasswordLongWrong);


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

			$sMsg = 'FAIL PHP truncates strings at null';
			echo $sMsg . NL;
			$aResults[] = $sMsg;
			return $aResults;

		} // if php already truncates input data

		// TODO: test against all cypher-codes?

		$sDataEncrypted = base64_decode($oCypher3->encryptWithClearTextPass($sData, $sCipherCode, $sPasswordLong));

		$sDataDecrypted = $oCypher3->decryptWithClearTextPass($sDataEncrypted, $sPasswordLong);


		if (null === $sDataDecrypted) throw new Exception('internal error', -999);

		if (10 !== strlen($sDataDecrypted)) {

			$sMsg = 'FAIL decrypted data is truncated at null';
			echo $sMsg . NL;
			$aResults[] = $sMsg;
			return $aResults;

		} // if decrypted data is truncated

		if ($sData !== $sDataDecrypted) {

			$sMsg = 'FAIL decrypted data does not match original data with nulls';
			echo $sMsg . NL;
			$aResults[] = $sMsg;
			return $aResults;

		} // if decrypted data is not same as original

        echo 'PASS TEST E' . NL . NL;

		if (!$bSkipLongTest) {

			echo 'TEST F: Testing against larger files and all cipher-codes' . NL;

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

						echo '    Testing cipher-code ' . ++$iCount . '/' . $iCountCodes . ': ' . $sCipherCode . NL;

						$mRes = $oCypher3->encryptWithClearTextPassFromFileToFile($sCipherCode, $sPasswordLong, $sPathTmp1, $sPathTmp2);
						if (null === $mRes) {

							$sMsg = 'FAIL: could not encrypt | ' . $oCypher3->sOut() . ' ' . $oCypher3->iRes();
							$aResults[] = array('cipherCode' => $sCipherCode, 'result' => $sMsg);

							echo '    ' . $sMsg . NL;

							continue;

						} // if failed to encrypt

						$mRes = $oCypher3->decryptWithClearTextPassFromFileToFile($sCipherCode, $sPasswordLong, $sPathTmp2, $sPathTmp3);
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

					} // loop each aCipherCodes

				} // loop with iAdded

			} // loop each base file size

			if (0 == count($aResults)) {

				echo 'TEST F PASSED (if there were no other errors above' . NL . NL;

			} else {

				echo 'TEST F FAILED' . NL;

			} // if TEST E passed or not

		} // if do long test

        $oCypher3->cleanupCypher2();

        return $aResults;

    } // testCypher3

	/* * * * *  *   /PUBLIC enCRYPTING FUNCTIONS\   *  * * * * */

	// returns NULL or the encrypted content base64 encoded
	public function encryptWithClearTextPass($sContent = '',
											 $sCipherCode = '',
											 $sPassword = '') {

		// if no data, no need to encrypt
		if (empty($sContent)) return $this->fail('no data to encrypt', -58);

		// prepare unique files to work with
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);

		// dump content to file
		$mRes = @file_put_contents_safely($sPathIn, $sContent, 'wb');
		if (false === $mRes) return $this->fail('could not write content to file: '
												. $sPathIn, -31);

		// encrypt
        $mRes = $this->encryptFromFileToFile($sCipherCode, $sPassword,
											 $sPathIn, $sPathOut);
		if (true !== $mRes) {

			@unlink($sPathIn); @unlink($sPathOut);

			return null; // the error is probably already set

		} // if encryption failed

        $sResRaw = @file_get_contents_safely($sPathOut);

        // remove the files, if they still exist
        @unlink($sPathIn); @unlink($sPathOut);

		// return encoded encrypted content or null
        return (false === $sResRaw) ? $this->fail('could not read from file: '
												  . $sPathOut, -32)
									: base64_encode($sResRaw);

	} // encryptWithClearTextPass


	// returns NULL or the encrypted content of $sPathIn base64 encoded
	public function encryptWithClearTextPassFromFile($sCipherCode = '',
													 $sPassword = '',
													 $sPathIn = '') {

		// prepare temporary file to use
		$sPathOut = $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);

		// encrypt
		$mRes = $this->encryptFromFileToFile($sCipherCode, $sPassword,
											 $sPathIn, $sPathOut);
		if (true !== $mRes) {

			@unlink($sPathOut);

			return null; // the error is probably already set

		} // if failed to encrypt

		// get encrypted content
        $sResRaw = @file_get_contents_safely($sPathOut);

		// remove temp file
        @unlink($sPathOut);

        return (false === $sResRaw) ? $this->fail('ould not read from file: '
												  . $sPathOut, -33)
									: base64_encode($sResRaw);

	} // encryptWithClearTextPassFromFile


    // returns NULL or TRUE the encrypted data in $sPathOut is NOT base64 encoded
	public function encryptWithClearTextPassFromFileToFile($sCipherCode = '',
														   $sPassword = '',
														   $sPathIn = '',
														   $sPathOut = '') {

		$mRes = $this->encryptFromFileToFile($sCipherCode, $sPassword,
											 $sPathIn, $sPathOut);

        return (true === $mRes) ? true : null;

	} // encryptWithClearTextPassFromFileToFile


    // returns NULL or TRUE the encrypted data in $sPathOut is NOT base64 encoded
	public function encryptWithClearTextPassToFile($sContent = '',
												   $sCipherCode = '',
												   $sPassword = '',
												   $sPathOut = '') {

		// if no data, no need to encrypt
		if (empty($sContent)) return $this->fail('no data to encrypt', -59);

		// prepare temporary file to use
		$sPathIn = $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);

		// dump contents to temporary file
		$mRes = @file_put_contents_safely($sPathIn, $sContent, 'wb');
		if (false === $mRes) return $this->fail('could not write to file: '
												. $sPathIn, -34);

		// encrypt
        $mRes = $this->encryptFromFileToFile($sCipherCode, $sPassword,
											 $sPathIn, $sPathOut);

		// remove temp file
		@unlink($sPathIn);

        return (true === $mRes) ? true : null;

	} // encryptWithClearTextPassToFile


    // returns NULL or the encrypted data base64 encoded
	public function encryptWithFilePass($sContent = '', $sCipherCode = '',
										$sPassPath = '') {

		// if no data, no need to encrypt
		if (empty($sContent)) return $this->fail('no data to encrypt', -60);

		// read the password from file
		$sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword) return $this->fail('could not read password from file: '
													 . $sPassPath, -35);

		// prepare some temporary files to use
		$sPathOut		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathIn		= $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);

		// dump content into file
        $mRes = @file_put_contents_safely($sPathIn, $sContent, 'wb');
		if (false === $mRes) {

			@unlink($sPathIn); @unlink($sPathOut);

			return $this->fail('could not write contents to file: '
							   . $sPathIn, -36);

		} // if failed to write to file

		// encrypt
		$mRes =  $this->encryptFromFileToFile($sCipherCode, $sPassword,
											  $sPathIn, $sPathOut);
		if (true !== $mRes) {

			@unlink($sPathIn); @unlink($sPathOut);

			return null;

		} // if failed to write to file

		// read ciphertext
        $sResRaw = @file_get_contents_safely($sPathOut);

        @unlink($sPathIn); @unlink($sPathOut);

        return (false === $sResRaw) ? $this->fail('could not read contents of file: '
												  . $sPathOut, -37)
									: base64_encode($sResRaw);

	} // encryptWithFilePass


    // returns NULL or the base64 encoded and encrypted data
	public function encryptWithFilePassFromFile($sCipherCode = '',
												$sPassPath = '',
												$sPathIn = '') {

		// read the password from file
		$sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword) return $this->fail('could not read password from file: '
													 . $sPassPath, -38);

		// prepare some temporary file to use
		$sPathOut = $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);

		// encrypt
        $mRes = $this->encryptFromFileToFile($sCipherCode, $sPassword,
											 $sPathIn, $sPathOut);
		if (true !== $mRes) {

			@unlink($sPathOut);

			return null;

		} // if failed to write to file

		// read ciphertext
        $sResRaw = @file_get_contents_safely($sPathOut);

        @unlink($sPathOut);

        return (false === $sResRaw) ? $this->fail('could not read contents of file: '
												  . $sPathOut, -39)
									: base64_encode($sResRaw);

	} // encryptWithFilePassFromFile


    // returns NULL or TRUE. The encrypted data in $sPathOut is NOT base64 encoded
	public function encryptWithFilePassFromFileToFile($sCipherCode = '',
													  $sPassPath = '',
													  $sPathIn = '',
													  $sPathOut = '') {

		// read the password from file
		$sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword) return $this->fail('could not read password from file: '
													 . $sPassPath, -40);

		// encrypt
        $mRes = $this->encryptFromFileToFile($sCipherCode, $sPassword,
											 $sPathIn, $sPathOut);

        return (true === $mRes) ? true : null;

	} // encryptWithFilePassFromFileToFile


    // returns NULL or TRUE the data in $sPathOut is NOT base64 encoded
	public function encryptWithFilePassToFile($sContent = '',
											  $sCipherCode = '',
											  $sPassPath = '',
											  $sPathOut = '') {

		// if no data, no need to encrypt
		if (empty($sContent)) return $this->fail('no data to encrypt', -61);

		// read the password from file
		$sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword) return $this->fail('could not read password from file: '
													 . $sPassPath, -41);

		// create a temporary file to use
		$sPathIn = $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);

		// dump contents to temp file
		$mRes = @file_put_contents_safely($sPathIn, $sContent, 'wb');
		if (false === $mRes) return $this->fail('could not write to file: '
												. $sPathIn, -42);

		// encrypt
		$mRes = $this->encryptFromFileToFile($sCipherCode, $sPassword,
											 $sPathIn, $sPathOut);

        @unlink($sPathIn);

        return (true === $mRes) ? true : null;

	} // encryptWithFilePassToFile

	/* * * * *  *   \PUBLIC enCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PUBLIC deCRYPTING FUNCTIONS\   *  * * * * */

    // returns NULL or the decrypted data
	public function decryptWithClearTextPass($sEncryptedData = '', $sPassword = '') {

		// no need to decrypt if too short data
		if (28 > strlen($sEncryptedData)) return $this->fail('not enough data to decrypt', -62);

		// create ome temporary files to use
		$sPathIn = $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathOut = $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);

		// dump content to temp file
		$mRes = @file_put_contents_safely($sPathIn, $sEncryptedData, 'wb');
		if (false === $mRes) {

			@unlink($sPathIn); @unlink($sPathOut);

			return $this->fail('could not write to file: ' . $sPathIn, -43);

		} //if failed to write contents to file

		// decrypt
		$mRes = $this->decryptFromFileToFile($sPassword, $sPathIn, $sPathOut);
		if (true !== $mRes) return null; // if failed to decrypt

        $sRes = @file_get_contents_safely($sPathOut);

        @unlink($sPathIn); @unlink($sPathOut);

        return (false === $sRes) ? $this->fail('could not read decrypted data from file: '
											   . $sPathOut, -44)
								: $sRes;

	} // decryptWithClearTextPass


    // returns NULL or the decrypted data
	public function decryptWithClearTextPassFromFile($sPassword = '',
													 $sPathIn = '') {

		// prepare a temporary file to use
		$sPathOut = $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);

		// decrypt
		$mRes = $this->decryptFromFileToFile($sPassword, $sPathIn, $sPathOut);
		if (true !== $mRes) {

			@unlink($sPathOut);

			return null;

		} // if failed to decrypt

		// grab decrypted content
        $sRes = @file_get_contents_safely($sPathOut);

        // remove temp file
		@unlink($sPathOut);

        return (false === $sRes) ? $this->fail('could not read from file: '
											   . $sPathOut, -45)
								: $sRes;

	} // decryptWithClearTextPassFromFile


    // returns NULL or TRUE
	public function decryptWithClearTextPassFromFileToFile($sPassword = '',
														   $sPathIn = '',
														   $sPathOut = '') {

		// decrypt
		$mRes = $this->decryptFromFileToFile($sPassword, $sPathIn, $sPathOut);

        return (true === $mRes) ? true : null;

	} // decryptWithClearTextPassFromFileToFile


    // returns NULL or the decrypted data which also is in $sPathOut
	public function decryptWithClearTextPassToFile($sEncryptedData = '',
												   $sPassword = '',
												   $sPathOut = '') {

		// no need to decrypt if too short data
		if (28 > strlen($sEncryptedData)) return $this->fail('not enough data to decrypt', -63);

		// make a temporary file to use
		$sPathIn = $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);

		// dump content to temp file
		$mRes = @file_put_contents_safely($sPathIn, $sEncryptedData, 'wb');
		if (false === $mRes) {

			@unlink($sPathIn);

			return $this->fail('could not write to file: '. $sPathIn, -46);

		} // if failed to write content to temp file

		// decrypt
		$mRes = $this->decryptFromFileToFile($sPassword, $sPathIn, $sPathOut);

        @unlink($sPathIn);

		if (true !== $mRes) return null;

		$sRes = @file_get_contents_safely($sPathOut);

        return (false === $sRes) ? $this->fail('could not read form file: '
											   . $sPathOut, -47)
								 : $sRes;

	} // decryptWithClearTextPassToFile


    // returns NULL or the decrypted data
	public function decryptWithFilePass($sEncryptedData = '', $sPassPath = '') {

		// no need to decrypt if too short data
		if (28 > strlen($sEncryptedData)) return $this->fail('not enough data to decrypt', -64);

		// prepare some temporary files to use
		$sPathIn = $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);
		$sPathOut = $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);

		// read password
		$sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword) return $this->fail('could not read password from file: '
													 . $sPassPath, -16);

		// dump data to temp file
		$mRes = @file_put_contents_safely($sPathIn, $sEncryptedData, 'wb');
		if (false === $mRes) return $this->fail('could not write to file: '
												. $sPathIn, -48);

		// decrypt
        $mRes = $this->decryptFromFileToFile($sPassword, $sPathIn, $sPathOut);
		if (true !== $mRes) {

			@unlink($sPathIn); @unlink($sPathOut);

			return null;

		} // if failed to decrypt

		// read decrypted content
        $sRes = @file_get_contents_safely($sPathOut);

        @unlink($sPathIn); @unlink($sPathOut);

        return (false === $sRes) ? $this->fail('could not read from file: '
											   . $sPathOut, -49)
								 : $sRes;

	} // decryptWithFilePass


    // returns NULL or decrypted data
	public function decryptWithFilePassFromFile($sPassPath = '', $sPathIn = '') {

		// prepare a temporary file to use
		$sPathOut = $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);

		// read the password
        $sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword) return $this->fail('could not read password from file: '
													 . $sPassPath, -50);

		// decrypt
		$mRes = $this->decryptFromFileToFile($sPassword, $sPathIn, $sPathOut);

        $sRes = @file_get_contents_safely($sPathOut);

        @unlink($sPathOut);

		if (true !== $mRes) return null;

        return (false === $sRes) ? $this->fail('could not read decrypted content from file: '
											   . $sPathOut, -51)
								 : $sRes;

	} // decryptWithFilePassFromFile


    // returns NULL or TRUE
	public function decryptWithFilePassFromFileToFile($sPassPath = '',
													  $sPathIn = '',
													  $sPathOut = '') {

		// read the password
		$sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword) return $this->fail('could not read password from file: '
													 . $sPassPath, -52);

		// decrypt
		$mRes = $this->decryptFromFileToFile($sPassword, $sPathIn, $sPathOut);

        return (true === $mRes) ? true : null;

	} // decryptWithFilePassFromFileToFile


    // returns NULL or the decrypted content which is also in $sPathOut
	public function decryptWithFilePassToFile($sEncryptedData = '',
											  $sPassPath = '',
											  $sPathOut = '') {

		// no need to decrypt if too short data
		if (28 > strlen($sEncryptedData)) return $this->fail('not enough data to decrypt', -65);

		// prepare a temporary file to use
		$sPathIn = $this->sTP . $this->fMakeSafeUniqueFileInPath($this->sTP);

		// read the password
		$sPassword = @file_get_contents_safely($sPassPath);
		if (false === $sPassword) return $this->fail('could not read password from file: '
													 . $sPassPath, -53);

		// dump encrypted data to temp file
		$mRes = @file_put_contents_safely($sPathIn, $sEncryptedData, 'wb');
		if (false === $mRes) return $this->fail('could not write to file: '
												. $sPathIn, -54);

		// decrypt
		$mRes = $this->decryptFromFileToFile($sPassword, $sPathIn, $sPathOut);

		// read decrypted content from file
        $sRes = @file_get_contents_safely($sPathOut);

        @unlink($sPathIn);

		if (true !== $mRes) return null;

        return (false === $sRes) ? $this->fail('could not read from file: '
											   . $sPathOut, -55)
								 : $sRes;

	} // decryptWithFilePassToFile

	/* * * * *  *   \PUBLIC deCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   PRIVATE FUNCTIONS   *  * * * * */

	/* * * * *  *   /PRIVATE enCRYPTING FUNCTIONS\   *  * * * * */

	// returns TRUE or NULL and the error message in results array eg ->sOut
	private function encryptFromFileToFile($sCipherCode = '', $sPassword = '', $sPathIn = '', $sPathOut = '') {

        // if invalid in path
		if (!is_file($sPathIn)) return $this->fail('invalid in path', 2);

        // if invalid cipher-code
        if (!isset($this->hCiphers[$sCipherCode]))
                return $this->fail('invalid cipher-code', -56);

		// if no password, no need to encrypt
		if (empty($sPassword)) return $this->fail('no password given', -57);

		$sCipher        = $this->hCiphers[$sCipherCode];
        $sCipherHMAC    = $this->hCipherHMAC[$sCipherCode];
        $iByteSizeIV    = $this->hCipherSizeIV[$sCipherCode];
		$iByteSizeKey   = $this->hCipherSizeKey[$sCipherCode];
        $iByteSizeBlock = $this->hCipherSizeBlock[$sCipherCode];
        $iByteSizeHMAC  = strlen(hash_hmac($sCipherHMAC, '', '', true));

        // generate the key for encryption
        $sKeyEnc = self::createSubkey($sPassword, self::subKeyDataEncryption(), $iByteSizeKey);
        if (false === $sKeyEnc) return $this->fail('error making subkey for encryption', -11);
        $sKeyEncEncoded = bin2hex($sKeyEnc);

        // generate a random initialization vector and convert to string of hex values
        try {

            $sIV = self::secureRandom($iByteSizeIV);
            $sIVencoded = bin2hex($sIV);

        } catch(Exception $e) {

            return $this->fail('error getting IV: ' . $e->getMessage(), -12);

        } // try catch IV creation


        // make two temporary files
        $sTmpFile1 = $this->fMakeSafeUniqueFileInPath($this->sTP);
        if (false == $sTmpFile1) return $this->fail('failed to create temp file', -4);

        $sTmpFile2 = $this->fMakeSafeUniqueFileInPath($this->sTP);
        if (false == $sTmpFile2) return $this->fail('failed to create temp file', -6);

        $sPathTmp1 = $this->sTP . $sTmpFile1;
        $sPathTmp2 = $this->sTP . $sTmpFile2;

		$sEscTmp1 = escapeshellarg($sPathTmp1);
        $sEscTmp2 = escapeshellarg($sPathTmp2);

        // pad file as needed
        $iByteSizePad = $iByteSizeBlock - (filesize($sPathIn) % $iByteSizeBlock);

		// add an extra block of padding for more compatibilty with eg Java
		$iByteSizePad += $iByteSizeBlock;

		// this is always true since we now add one extra block
        //if (0 < $iByteSizePad) {

            // prepend the padding
            $sPad = self::secureRandom($iByteSizePad);
            $mRes = @file_put_contents_safely($sPathTmp1, $sPad, 'wb');
            if (false === $mRes) return $this->fail('error prepending padding', -13);

        //} // if needs paddinng

        // append the original file
        $sScriptAppend = 'cat ' . escapeshellarg($sPathIn) . ' >> ' . $sEscTmp1;
        $this->doScript($sScriptAppend);
        if (0 !== $this->iRes()) return $this->fail('failed to append data', -5);

        // encrypt
 		$sScriptEncode = sprintf('openssl enc -%s -in %s -out %s -K %s -iv %s -nopad',
                                $sCipher, $sEscTmp1, $sEscTmp2, $sKeyEncEncoded, $sIVencoded);
		$this->doScript($sScriptEncode);
        if (0 !== $this->iRes()) return $this->fail('failed to encrypt step 1', -7);

        // prepend IV and pad-length
        file_put_contents_safely($sPathTmp1, $sIV . chr($iByteSizePad), 'wb');

        // append the encrypted data
        $sScriptAppend = 'cat ' . $sEscTmp2 . ' >> ' . $sEscTmp1;
        $this->doScript($sScriptAppend);
        if (0 !== $this->iRes()) return $this->fail('failed to append data', -23);

        // generate the key for authentication
        $sKeyAuth = self::createSubkey($sPassword, self::subKeyDataAuthentication(), $iByteSizeKey);

        // calculate the HMAC
        $sHMAC = hash_hmac_file($sCipherHMAC, $sPathTmp1, $sKeyAuth, true);
        if ($iByteSizeHMAC > strlen($sHMAC)) return $this->fail(
                'hmac is shorter than ' . $iByteSizeHMAC . ' bytes!', -8);
        $sHMAC = substr($sHMAC, 0, $iByteSizeHMAC);

        // write the final assortement to temp file 2
        // [___24_BYTE_CIPHER_MODE_LENGTH_CODE___][____HMAC____][[____IV____][___1_BYTE_PAD_LENGTH___][____CIPHERTEXT____]]
        $sHeader = $sCipherCode . $sHMAC;
        $mRes = @file_put_contents_safely($sPathTmp2, $sHeader, 'wb');
        if (false === $mRes) return $this->fail('error writing to final file', -9);

        // now append the encrypted data
        $sScriptAppend = 'cat ' . $sEscTmp1 . ' >> ' . $sEscTmp2;
        $this->doScript($sScriptAppend);
        if (0 !== $this->iRes()) return  $this->fail('failed to append encrypted data', -10);

        // remove temp file 1 - this is the encrypted file, so if it doesn't delete it's not so bad
        @unlink($sPathTmp1);

        // move the file from temp location to $sPathOut
        $mRes = @rename($sPathTmp2, $sPathOut);
        if (false === $mRes) return $this->fail('failed to move finalized file to ' . $sPathOut, -24);

        // we need to avoid loading file contents to memory, it may be huge!
		return true; //$this->aResults['fullOutput']; //@file_get_contents_safely($sPathOut);

	} // encryptFromFileToFile

	/* * * * *  *   \PRIVATE enCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PRIVATE deCRYPTING FUNCTIONS\   *  * * * * */

    // [___24_BYTE_CIPHER_MODE_LENGTH_CODE___][____HMAC____][[____IV____][___1_BYTE_PAD_LENGTH___][____CIPHERTEXT____]]
	// returns TRUE or NULL. A string describing the error can be found in the results array eg ->sOut()
	private function decryptFromFileToFile($sPassword = '', $sPathIn = '', $sPathOut = '') {

		// no need to decrypt if too short data
		if (empty($sPassword)) return $this->fail('no password given, abborting', -66);

        // if invalid in path
		if (!is_file($sPathIn)) return $this->fail('invalid in path', 2);

        // open the file and read the cipher-code portion
        $rF = @fopen($sPathIn, 'rb');
        if (false === $rF) return $this->fail('unable to open input file', -14);

        $sCipherCode = @fread($rF, 24);
        if (false === $sCipherCode) {

            @fclose($rF);
            return $this->fail('error reading from file', -15);

        } // if can not read from file

        // if invalid cipher-code
        if (!isset($this->hCiphers[$sCipherCode])) {

            @fclose($rF);
            return $this->fail('invalid cipher-code', -17);

        } // if invalid cipher-code

		$sCipher        = $this->hCiphers[$sCipherCode];
        $sCipherHMAC    = $this->hCipherHMAC[$sCipherCode];
        $iByteSizeIV    = $this->hCipherSizeIV[$sCipherCode];
		$iByteSizeKey   = $this->hCipherSizeKey[$sCipherCode];
        $iByteSizeBlock = $this->hCipherSizeBlock[$sCipherCode];
        $iByteSizeHMAC  = strlen(hash_hmac($sCipherHMAC, '', '', true));

        $sHMAC = fread($rF, $iByteSizeHMAC);

        // make two temporary files
        $sTmpFile1 = $this->fMakeSafeUniqueFileInPath($this->sTP);
        if (false == $sTmpFile1) {

            @fclose($rF);
            return $this->fail('failed to create temp file', -18);

        } // if error making temp file 1

        $sTmpFile2 = $this->fMakeSafeUniqueFileInPath($this->sTP);
        if (false == $sTmpFile2) {

            @fclose($rF);
            return $this->fail('failed to create temp file', -25);

        } // if error making temp file 1

        $sPathTmp1 = $this->sTP . $sTmpFile1;
        $sPathTmp2 = $this->sTP . $sTmpFile2;

		$sEscTmp1 = escapeshellarg($sPathTmp1);
        $sEscTmp2 = escapeshellarg($sPathTmp2);

        // transfer [___IV____][___1_BYTE_PAD_LENGTH___][___ENCRYPTED_DATA____] to temp file 1
        $rF1 = @fopen($sPathTmp1, 'wb');
        if (false === $rF1) {

            @fclose($rF);
            return $this->fail('failed to open temp file for writing', -19);

        } // if error opening temp file 1

         // loop rest data to temp file 1
        while (!feof($rF)) { fwrite($rF1, fread($rF, 1024)); }

        // close the files
        fclose($rF); fclose($rF1);

        // regenerate the authentication sub-key
        $sKeyAuth = self::createSubkey($sPassword, self::subKeyDataAuthentication(), $iByteSizeKey);

        // make sure the HMAC matches. If not the file has been tampered with!
        $sHMAC1 = hash_hmac_file($sCipherHMAC, $sPathTmp1, $sKeyAuth, true);
        if ($iByteSizeHMAC > strlen($sHMAC1)) return $this->fail(
                'hmac is shorter than ' . $iByteSizeHMAC . ' bytes!', -20);
        $sHMAC1 = substr($sHMAC1, 0, $iByteSizeHMAC);

        if (!self::slowEquals($sHMAC, $sHMAC1)) {

            return $this->fail('HMAC missmatch, aborting', -21);

        } // if HMACs do not match

        // now read the IV from temp file 1
        $rF1 = @fopen($sPathTmp1, 'rb');
        if (false === $rF1) return $this->fail('failed to open temp file for reading', -26);

        $sIV = fread($rF1, $iByteSizeIV);
        $sIVencoded = bin2hex($sIV);

        // read padding length
        $iByteSizePad = ord(fread($rF1, 1));

        // the rest of the file is the encrypted data, place it in tempfile 2
        $rF2 = @fopen($sPathTmp2, 'wb');
        if (false === $rF2) {

            @fclose($rF1);
            return $this->fail('failed to open temp file for writing', -27);

        } // if error opening temp file 2

        // loop rest data to temp file 2
        while (!feof($rF1)) { fwrite($rF2, fread($rF1, 1024)); }

        // close the files
        fclose($rF2); fclose($rF1);

        // regenerate the encryption sub-key
        $sKeyEnc = self::createSubkey($sPassword, self::subKeyDataEncryption(), $iByteSizeKey);
        if (false === $sKeyEnc) return $this->fail('error making subkey for decryption', -22);
        $sKeyEncEncoded = bin2hex($sKeyEnc);

        // decrypt the data in temp file 2 to temp file 1
        $sScriptDecode = sprintf('openssl enc -%s -d -in %s -out %s -K %s -iv %s -nopad',
                                $sCipher, $sEscTmp2, $sEscTmp1, $sKeyEncEncoded, $sIVencoded);
		$this->doScript($sScriptDecode);
        if (0 !== $this->iRes()) return $this->fail('failed to decrypt', -28);

        // remove padding, if needed
        if (0 < $iByteSizePad) {

            // remove padding

            $rF1 = @fopen($sPathTmp1, 'rb');
            if (false === $rF1) return
                    $this->fail('failed to open temp file for reading', -29);

            // drop padding
            fread($rF1, $iByteSizePad);

            $rF2 = @fopen($sPathTmp2, 'wb');
            if (false === $rF2) {

                @fclose($rF1);
                return $this->fail('failed to open temp file for writing', -30);

            } // if error opening temp file 2

            // loop rest data to temp file 2
            while (!feof($rF1)) { fwrite($rF2, fread($rF1, 1024)); }

            // close the files
            fclose($rF2); fclose($rF1);

            // remove temp 1
            @unlink($sPathTmp1);

            rename($sPathTmp2, $sPathOut);

        } else {

            // no padding

            // remove temp 2
            @unlink($sPathTmp2);

            rename($sPathTmp1, $sPathOut);

        } // if need to remove padding or not

        // don't load file to memory, just respond with TRUE
		return true; //@file_get_contents_safely($sPathOut); // if "" == $this->aResults['fullOutput'];

	} // decryptFromFileToFile

	/* * * * *  *   \PRIVATE deCRYPTING FUNCTIONS/   *  * * * * */

	/* * * * *  *   /PRIVATE FUNCTIONS 4 SUBCLASSING\   *  * * * * */

	// subclass this for more obfuscation
	protected function initCiphers() {

		$aCiphers = array(
            // AES-256 is MCRYPT_RIJNDAEL_128 NOT RIJNDAEL_256!!!
                'aes-128-cbc', 'aes-128-ecb',
                'aes-192-cbc', 'aes-192-ecb',
                'aes-256-cbc', 'aes-256-ecb',
                // MCRYPT_CAST_128
                'cast5-cbc',
                // cast6 is not supported by openssl on OS X 10.6.x
                'cast5-cbc', 'cast5-ecb',
                //'cast6-cbc', 'cast6-ecb',

                'des-cbc', 'des-ecb',
                //'des-ede-cbc', 'des-ede',
                'des-ede3-cbc', 'des-ede3' // des3 == des-ede3-cbc
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
				24, 24
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
		$this->hCiphers         = array_combine($aKeys, $aCiphers);

	} // initCiphers

	/* * * * *  *   \PRIVATE FUNCTIONS 4 SUBCLASSING/   *  * * * * */

} // SssS_Cypher3openSSLcli

/* * * *\ SssS_Cypher3openSSLcli.inc (CC)2013 Luke JZ aka SwissalpS /* * * */
?>
