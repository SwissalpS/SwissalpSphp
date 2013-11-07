<?php
/* * * *
 * * SssS_PBKDF2.inc
 * * Password hashing with PBKDF2 as defined by RSA's PKCS #5:
 * *                                https://www.ietf.org/rfc/rfc2898.txt
 * * Password-Based Key Derivation Function 2
 * *
 * * Based on havoc's version:
 * * havoc AT defuse.ca
 * * https://defuse.ca/php-pbkdf2.htm
 * *
 * * remodeled to class structure by Luke SwissalpS
 * * also added compatibility for machines without mcrypt and
 * * possibility to set the order of hash portions
 * *
 * * @version 20131027_131313 (CC) Luke JZ aka SwissalpS
 * * * */
 /*
  * USAGE (default settings):
    // get hash for database
    $sHash = SssS_PBKDF2::defaultInstance()->createHash($sPassword);
    // validate password against hash from database
    $bIsValid = SssS_PBKDF2::defaultInstance()->validatePassword($sPassword, $sHash);

  * USAGE (with custom parameters):
    // make instance with custom parameters (fictional and not all valid to rfc)
    $o = new SssS_PBKDF2(
            array(
                'sHASH_ALGORITHM' => 'sha256',
                'iITERATIONS' => 5051,
                'iSALT_BYTES' => 128,
                'iHASH_BYTES' => 256),
            array(
                'iSECTIONS' => 4,          // don't change this
                'iALGORITHM_INDEX' => 0,   // you may change the positions with care!
                'iITERATION_INDEX' => 1,
                'iSALT_INDEX' => 2,
                'iPBKDF2_INDEX' => 3));

    // create hash for database
    $sHash = $o->createHash($sPassword);
    // validate password against hash from database
    $bIsValid = $o->validatePassword($sPassword, $sHash);
 *
 */
class SssS_PBKDF2 {

    public $aPBKDF2 = array();
    public $aHASH = array();

    function __construct($aPBKDF2 = array(), $aHash = array()) {

        // These constants may be changed without breaking existing hashes.
        $this->aPBKDF2 = array_merge(array(
                'sHASH_ALGORITHM' => 'sha256',
                'iITERATIONS' => 3000,
                'iSALT_BYTES' => 24,
                'iHASH_BYTES' => 24), $aPBKDF2);

        $this->aHASH = array_merge(array(
                'iSECTIONS' => 4,          // don't change this
                'iALGORITHM_INDEX' => 0,   // you may change the positions with care!
                'iITERATION_INDEX' => 1,
                'iSALT_INDEX' => 2,
                'iPBKDF2_INDEX' => 3), $aHash);

    } // __construct


    // returns hash to save in db as the password or throws an Exception
    function createHash($sPassword) {

        $sSalt = base64_encode(self::secureRandom($this->aPBKDF2['iSALT_BYTES']));

        $sOut = '';
        aSort($this->aHASH, SORT_NUMERIC);
        $aSortedKeys = array_keys($this->aHASH);

        for ($i = 0; $this->aHASH['iSECTIONS'] > $i; $i++) {

            switch (array_shift($aSortedKeys)) {

                case 'iALGORITHM_INDEX' :
                    $sOut .= $this->aPBKDF2['sHASH_ALGORITHM'] . ':';
                break;

                case 'iITERATION_INDEX' :
                    $sOut .= $this->aPBKDF2['iITERATIONS'] . ':';
                break;

                case 'iSALT_INDEX' :
                    $sOut .= $sSalt . ':';
                break;

                case 'iPBKDF2_INDEX' :
                    $sOut .= base64_encode(self::pbkdf2(
                            $this->aPBKDF2['sHASH_ALGORITHM'],
                            $sPassword,
                            $sSalt,
                            $this->aPBKDF2['iITERATIONS'],
                            $this->aPBKDF2['iHASH_BYTES'],
                            true)) . ':';
                break;

                default : throw new Exception('unknown index', -2);
                break;

            } // switch key

        } // loop all sections in order

        return trim($sOut, ':');

    } // createHash


    // returns TRUE or FALSE
    function validatePassword($sPassword, $sGoodHash) {

        $aParams = explode(':', $sGoodHash);

        if ($this->aHASH['iSECTIONS'] > count($aParams)) return false;

        $sPBKDF2 = base64_decode($aParams[$this->aHASH['iPBKDF2_INDEX']]);

        return self::slowEquals($sPBKDF2, self::pbkdf2(
                $aParams[$this->aHASH['iALGORITHM_INDEX']],
                $sPassword,
                $aParams[$this->aHASH['iSALT_INDEX']],
                (int)$aParams[$this->aHASH['iITERATION_INDEX']],
                strLen($sPBKDF2),
                true));

    } // validatePassword


    // Compares two strings $a and $b in length-constant time.
    // returns TRUE or FALSE
    static function slowEquals($sA, $sB) {

        $iDiff = strlen($sA) ^ strlen($sB);

        for ($i = 0; $i < strlen($sA) && $i < strlen($sB); $i++) {

            $iDiff |= ord($sA[$i]) ^ ord($sB[$i]);

        } // loop until shortest end reached

        return $iDiff === 0;

    } // slowEquals


    /*
     * PBKDF2 key derivation function as defined by RSA's PKCS #5: https://www.ietf.org/rfc/rfc2898.txt
     * $sAlgorithm - The hash algorithm to use. Recommended: SHA256
     * $sPassword - The password.
     * $sSalt - A salt that is unique to the password.
     * $iCount - Iteration count. Higher is better, but slower. Recommended: At least 1000.
     * $iKeyLength - The length of the derived key in bytes.
     * $bRawOutput - If true, the key is returned in raw binary format. Hex encoded otherwise.
     * Returns: A $iKeyLength-byte key derived from the password and salt.
     *
     * Test vectors can be found here: https://www.ietf.org/rfc/rfc6070.txt
     *
     * This implementation of PBKDF2 was originally created by https://defuse.ca
     * With improvements by http://www.variations-of-shadow.com
     */
    static function pbkdf2($sAlgorithm, $sPassword, $sSalt, $iCount, $iKeyLength, $bRawOutput = false) {

        $sAlgorithm = strToLower($sAlgorithm);

        if (!in_array($sAlgorithm, hash_algos(), true)) {

            throw new Exception('PBKDF2 ERROR: Invalid hash algorithm.', -1);

        } // if invalid hash algorithm

        if (0 >= $iCount || 0 >= $iKeyLength) {

            throw new Exception('PBKDF2 ERROR: Invalid parameters.', -1);

        } // if invalid parameters

        $iHashLength = strLen(hash($sAlgorithm, '', true));

        $iBlockCount = ceil($iKeyLength / $iHashLength);

        $sOutput = '';
        for ($i = 1; $i <= $iBlockCount; $i++) {

            // $i encoded as 4 bytes, big endian.
            $sLast = $sSalt . pack('N', $i);

            // first iteration
            $sLast = $sXorSum = hash_hmac($sAlgorithm, $sLast, $sPassword, true);

            // perform the other $count - 1 iterations
            for ($j = 1; $j < $iCount; $j++) {

                $sXorSum ^= ($sLast = hash_hmac($sAlgorithm, $sLast, $sPassword, true));

            } // loop j

            $sOutput .= $sXorSum;

        } // loop i


        if ($bRawOutput) {

            return subStr($sOutput, 0, $iKeyLength);

        } else {

            return bin2hex(subStr($sOutput, 0, $iKeyLength));

        } // if raw output or not

    } // pbkdf2


    // returns $iByteLength-long random 'string' or throws an Exception
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

                throw new Exception('failed to open /dev/urandom', -3);

            } // if able to read from /dev/urandom

        } // if got mcrypt, openssl or none of those

    } // secureRandom


    // returns an instance with default settings
    static function defaultInstance() {

        static $o = null;

        if (null === $o) {

            $o = new SssS_PBKDF2();

        } // first call

        return $o;

    } // defaultInstance

} // SssS_PBKDF2

/* * * *\ SssS_PBKDF2.inc (CC)2013 Luke JZ aka SwissalpS /* * * */
?>
