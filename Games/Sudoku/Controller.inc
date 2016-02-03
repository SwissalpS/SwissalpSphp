<?php
/* * * *
 * * SwissalpS/Games/Sudoku/Controller.inc
 * * grid game solver (not only 3*3)
 * *
 * * not yet complete, can not yet do deductions and there are also some simpler rules that need to be implemented. Such as pairs...
 * *
 * * @version 20160203_202734 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\Games\Sudoku;

use Exception;

/* * *
 * * class Controller
 * *
 * * version 20160201_061345 (CC) Luke JZ aka SwissalpS
 * * */
class Controller {

    protected $oField;

    /* *
     * * function __construct()
     * *
     * * version 20160201_061425 (CC) Luke JZ aka SwissalpS
     * */
    public function __construct() {

        $a = [];

        //$this->resetFromArray($a);

    } // __construct
    /* *\ FUNCTION __construct END /* */


    /* *
     * * function __toString()
     * *
     * * version 20160201_105831 (CC) Luke JZ aka SwissalpS
     * */
    public function __toString() {

        if (!$this->oField) {

            return '';

        } // if not initialized

        return $this->oField->__toString();

    } // __toString
    /* *\ FUNCTION __toString END /* */


    /* *
     * * function __toStringFormated()
     * *
     * * version 20160203_014158 (CC) Luke JZ aka SwissalpS
     * */
    public function __toStringFormated() {

        if (!$this->oField) {

            return '';

        } // if not initialized

        return $this->oField->__toStringFormated();

    } // __toStringFormated
    /* *\ FUNCTION __toStringFormated END /* */


    /* *
     * * function eliminateByForcedAlleys()
     * *
     * * version 20160203_020229 (CC) Luke JZ aka SwissalpS
     * */
    public function eliminateByForcedAlleys() {

        if (!$this->oField) {

            return false;

        } // if not initialized

        $iCount = 0;
        while ($this->oField->eliminateByForcedAlleys()) {
            $iCount++;
        } // loop until no obvious found anymore

        return ++$iCount;

    } // eliminateByForcedAlleys
    /* *\ FUNCTION eliminateByForcedAlleys END /* */


    /* *
     * * function eliminateBySingleValueRule()
     * *
     * * version 20160202_215706 (CC) Luke JZ aka SwissalpS
     * */
    public function eliminateBySingleValueRule() {

        if (!$this->oField) {

            return false;

        } // if not initialized

        $iCount = 0;
        while ($this->oField->eliminateBySingleValueRule()) {
            $iCount++;
        } // loop until no obvious found anymore

        return ++$iCount;

    } // eliminateBySingleValueRule
    /* *\ FUNCTION eliminateBySingleValueRule END /* */


    /* *
     * * function eliminateObvious()
     * * remove obvious possibilities
     * *
     * * @return bool true if eliminated anything.
     * * version 20160201_122957 (CC) Luke JZ aka SwissalpS
     * */
    public function eliminateObvious() {

        if (!$this->oField) {

            return false;

        } // if not initialized

        $iCount = 0;
        while ($this->oField->eliminateObvious()) {
            $iCount++;
        } // loop until no obvious found anymore

        return ++$iCount;

    } // eliminateObvious
    /* *\ FUNCTION eliminateObvious END /* */


    /* *
     * * function isSolved()
     * *
     * * version 20160203_215828 (CC) Luke JZ aka SwissalpS
     * */
    public function isSolved() {

        if (!$this->oField) {

            return false;

        } // if not initialized

        return $this->oField->isSolved();

    } // isSolved
    /* *\ FUNCTION isSolved END /* */


    /* *
     * * function loadFile($sPathFile)
     * *
     * * version 20160201_062044 (CC) Luke JZ aka SwissalpS
     * */
    public function loadFile($sPathFile) {

        if (!is_readable($sPathFile)) {

            throw new Exception('File not readable: ' . $sPathFile);

        } // if not readable path given

        //$aLinesRaw = file($sPathFile, FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);

        $sString = file_get_contents($sPathFile);

        return $this->loadString($sString);

    } // loadFile
    /* *\ FUNCTION loadFile END /* */


    /* *
     * * function loadString()
     * *
     * * version 20160203_200131 (CC) Luke JZ aka SwissalpS
     * */
    public function loadString($sStringRaw) {

        // neutrialize line endings
        $sStringRaw = str_replace([chr(13), chr(10).chr(10)], chr(10), $sStringRaw);

        $aLinesRaw = explode(chr(10), $sStringRaw);

        if (1 == count($aLinesRaw)) {

            throw new Exception('Was unable to split given string into lines.');

        } // if unable to split into lines

        $aLines = [];
        $aColumnCounts = [];
        $sSplitter = null;

        foreach ($aLinesRaw as $sLine) {

            $sLine = trim($sLine);

            if ('' == $sLine) {

                continue;

            } // if skip because empty

            if (in_array($sLine{0}, ['#', '-', '/'])) {

                continue;

            } // if skip line

            if ($sSplitter) {

                $aLine = explode($sSplitter, $sLine);

            } else {

                // figure out what to use to split lines
                // space
                $aLine = explode(' ', $sLine);
                if (count($aLine)) {

                    $sSplitter = ' ';

                } else {

                    // tab
                    $aLine = explode(chr(9), $sLine);
                    if (count($aLine)) {

                        $sSplitter = chr(9);

                    } else {

                        // comma
                        $aLine = explode(',', $sLine);
                        if (count($aLine)) {

                            $sSplitter = ',';

                        } else {

                            // semi-colon
                            $aLine = explode(';', $sLine);
                            if (count($aLine)) {

                                $sSplitter = ';';

                            } else {

                                // pipe
                                $aLine = explode('|', $sLine);
                                if (count($aLine)) {

                                    $sSplitter = '|';

                                } else {

                                    // colon
                                    $aLine = explode(':', $sLine);
                                    if (count($aLine)) {

                                        $sSplitter = ':';

                                    } else {

                                        throw new Exception('Could not determine column seperator.');

                                    } // if not colon

                                } // if not pipe

                            } // if not semi-colon

                        } // if not comma

                    } // if not tab

                } // if not space

            } // if got splitter or not

            $aLines[] = $aLine;
            $aColumnCounts[] = count($aLine);

        } // loop all lines removing comments and splitting into columns

        $iLineCount = count($aLines);
        if (!$iLineCount) {

            throw new Exception('No lines remaining after removing commented lines.');

        } // if no lines found

        $aColumnCounts = array_unique($aColumnCounts);
        if (1 !== count($aColumnCounts)) {

            throw new Exception('Inconsistent column count.');

        } // if inconsistent count

        if ($aColumnCounts[0] != $iLineCount) {

            throw new Exception('Line count (' . $iLineCount . ') does not match column count (' . $aColumnCounts[0] . ').');

        } // if line and column count do not match

        return $this->resetFromArray($aLines);

    } // loadString
    /* *\ FUNCTION loadString END /* */


    /* *
     * * function possibleValues()
     * *
     * * version 20160201_103706 (CC) Luke JZ aka SwissalpS
     * */
    public function possibleValues() {

        if (!$this->oField) {

            return [];

        } // if not yet initialized

        return $this->oField->possibleValues();

    } // possibleValues
    /* *\ FUNCTION possibleValues END /* */


    /* *
     * * function resetFromArray()
     * *
     * * @param array $aLines a multidimensional array with values. Each entry is an array representing the values of the line starting top left.
     * * version 20160201_075546 (CC) Luke JZ aka SwissalpS
     * */
    protected function resetFromArray(&$aLines) {

        $this->oField = new Field($aLines);
        $this->oField->init($aLines);

        return $this;

    } // resetFromArray
    /* *\ FUNCTION resetFromArray END /* */


    /* * *
     * * function sharedInstance()
     * *
     * * version 20160201_061517 (CC) Luke JZ aka SwissalpS
     * * */
    public static function sharedInstance() {

        static $o = null;
        if (!$o) {

            $o = new self();

        } // first call

        return $o;

    } // sharedInstance
    /* * *\ FUNCTION sharedInstance END /* * */

} // Controller
/* * *\ CLASS Controller END /* * */

/* * * *\ SwissalpS/Games/Sudoku/Controller.inc (CC) Luke JZ aka SwissalpS /* * * */
