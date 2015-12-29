<?php
/* * * *
 * * SwissalpS/CLI/Tools.inc
 * *
 * * simple tools for basic cli apps
 * *
 * * more sophisticated options can be had with SssS_SocketController and
 * * SssS_ServerCommander classes using ANSI coloring etc.
 * *
 * * @used by _synergize/PHOCOA/SssSphocoaGenerator
 * * @version 20151228_171037 (CC) added namespace and moved to new location
 * * @version 20110609_214718 (CC) added checks for constants STDIN, STOUT, STDERR
 * * @version 20100501_083218 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\CLI;


if (!defined('STDIN')) { define('STDIN', fOpen('php://stdin', 'r')); }
if (!defined('STDOUT')) { define('STDOUT', fOpen('php://stdout', 'w')); }
if (!defined('STDERR')) { define('STDERR', fOpen('php://stderr', 'w')); }


class Tools {

    public static function getInputCLI($sPrompt, $sDefault) {

        if (77 < (strlen($sPrompt) + strlen($sDefault))) {

            fwrite(STDOUT, $sPrompt . chr(10) . '[' . $sDefault . '] ');

        } else {

            fwrite(STDOUT, $sPrompt . '[' . $sDefault . '] ');

        } // if long or short prompt

        $sReply = trim(fgets(STDIN));

        //fwrite(STDOUT, 'you said: ' . $sReply . chr(10));
        if (empty($sReply)) $sReply = $sDefault;

        fwrite(STDOUT,  'Using: ' . $sReply . chr(10));

        return $sReply;

    } // getInputCLI


/* this snippet shows the behaviour of getChoiceCLI()
    depending on the array format
// simple array
$a1 = array('one', 'two', 'three', 'four');

// caution! these can cause unexpected choices:
// mixed array with string, numerical and no indexs
$a2= array('one' => 'ONE longer', 'two', 5 => 'five', 'four' => 'four');

// string indexed array -> works great :-) returns the value not key as choice
$a3= array('one' => 'ONE longer', 'two' => 'too long', 'three' => 'tree', 'four' => 'four');
echo Tools::getChoiceCLI('prompt1>', $a1, 'four') . chr(10);
echo Tools::getChoiceCLI('prompt2>', $a2, 'four') . chr(10);
echo Tools::getChoiceCLI('prompt3>', $a3, 'four') . chr(10);
*/
    public static function getChoiceCLI($sPrompt, $aChoices, $sDefault = null) {

        if ('array' != gettype($aChoices)) return $sDefault;

        $iCount = count($aChoices);
        if (0 == $iCount) return $sDefault;

        $aChoicesValues = array_values($aChoices);
        $aChoicesKeys = array_keys($aChoices);
        if (null == $sDefault) $sDefault = $aChoicesValues[0];

        $sOut = '';
        for ($x = 0; $x < $iCount; $x++) {
            $sOut .= $x . ') ' . $aChoicesValues[$x] . ' (' . $aChoicesKeys[$x]
                    . chr(10);
        } //
        fwrite(STDOUT, $sOut);

        $sPrompt .= '[' . $sDefault . '] ';

        do {
            fwrite(STDOUT, $sPrompt);
            $sReply = trim(fgets(STDIN));

            // special case 0
            if ('0' == $sReply) { $sReply = $aChoicesValues[0]; break; }

            // just return hit -> default
            if (empty($sReply)) { $sReply = $sDefault; break; }
//echo '--is not empty---';
            // a choice spellt out
            if (in_array($sReply, $aChoicesValues)) break;
//echo '--is not spellt out---';
            // a key of a possibly associated array of choices...
            if (isset($aChoices[$sReply])) {
                        $sReply = $aChoices[$sReply]; break; }
//echo '--is not key---';
            // the index number of a choice -> defaults to 0 if no number
            $iReply = intval($sReply);
            if (isset($aChoicesValues[$iReply])) {
                    $sReply = $aChoicesValues[$iReply]; break; }
//echo '--is not in choices---';
        } while (true);

        fwrite(STDOUT, 'Using: ' . $sReply . chr(10));

        return $sReply;

    } // getChoiceCLI


    public static function removeTrailingSlash($sPath, $sSlash = '/') {

        if ($sSlash == substr($sPath, -1)) $sPath = substr($sPath, 0, -1);

        return $sPath;

    } // removeTrailingSlash


    public static function addTrailingSlash($sPath, $sSlash = '/') {

        $sPath .= ($sSlash != substr($sPath, -1)) ? $sSlash : ''; // add trailing /

        return $sPath;

    } // addTrailingSlash


    function __toString() { return print_r(self, true); } // __toString

} // Tools

/* * * *\ SwissalpS/CLI/Tools.inc (CC) Luke JZ aka SwissalpS /* * * */
