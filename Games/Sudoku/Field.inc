<?php
/* * * *
 * * SwissalpS/Games/Sudoku/Field.inc
 * * grid game solver (not only 3*3)
 * *
 * * not yet complete, can not yet do deductions and there are also some simpler rules that need to be implemented. Such as pairs...
 * *
 * * @version 20160203_202734 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\Games\Sudoku;

use Exception;

/* * *
 * * class Field
 * *
 * * version 20160201_061143 (CC) Luke JZ aka SwissalpS
 * * */
class Field {

    protected $aSquares;
    protected $iValueMax;

    /* *
     * * function __construct()
     * *
     * * @param array $aLines a multidimensional array with values. Each entry is an array representing the values of the line starting top left.
     * * version 20160201_075934 (CC) Luke JZ aka SwissalpS
     * */
    public function __construct(&$aLines) {

        $iCountLines = count($aLines);
        if (!isset($aLines[0])) {
            return null;
        }
        $iCountColumns = count($aLines[0]);

        if ($iCountLines != $iCountColumns) {

            throw new Exception('Line and column count do not match. (' . $iCountLines . ' | ' . $iCountColumns . ')');

        } // if line and column count does not match

        $this->iValueMax = $iCountColumns;
        $this->aSquares = [];
        $fSide = sqrt($this->iValueMax);
        $iSide = intval($fSide);
        if ($fSide != $iSide) {

            throw new Exception('Column count is not a square number.');

        } // if not square

        unset($fSide);

    } // __construct
    /* *\ FUNCTION __construct END /* */


    /* *
     * * function __toString()
     * *
     * * version 20160201_105831 (CC) Luke JZ aka SwissalpS
     * */
    public function __toString() {

        if (!$this->iValueMax) {

            return '';

        } // if not initialized

        $sOut = '';
        $sSplitter = ' '; //chr(9);
        $sSplitterPVs = ',';
        $sSplitterLines = chr(10);

        $iCountColumns = 0;
        $iCountSquareColumns = 0;
        $iCountSquares = 0;
        $iCountLines = 0;
        $iCountUnits = 0;
        $iSide = sqrt($this->iValueMax);

        $iTotal = $this->iValueMax * $this->iValueMax;

        $oSquare = $this->aSquares[$iCountSquares];

        for ($iCount = 0; $iCount < $iTotal; $iCount++) {

            $aPVs = $oSquare->possibleValues($iCountUnits);
            $sOut .= implode($sSplitterPVs, $aPVs);

            $iCountColumns++;
            if ($iCountColumns == $iSide) {

                // next square

                $iCountColumns = 0;
                $iCountSquareColumns++;
                if ($iCountSquareColumns == $iSide) {

                    // end of line

                    $sOut .= $sSplitterLines;

                    $iCountSquareColumns = 0;
                    $iCountLines++;
                    if ($iCountLines == $iSide) {

                        // next square is next increment
                        $iCountLines = 0;
                        $iCountUnits = 0;
                        $iCountSquares++;

                    } else {

                        // next square is at beginning of line
                        $iCountSquares -= $iSide -1;
                        $iCountUnits++;

                    } // if new line is in which square


                } else {

                    // not end of line -> next square is next increment

                    $iCountSquares++;
                    $iCountUnits -= $iSide -1;

                    $sOut .= $sSplitter;

                } // if end of line or not

                if (isset($this->aSquares[$iCountSquares])) {

                    $oSquare = $this->aSquares[$iCountSquares];

                } // if got one there at all, last one won't

            } else {

                // not next square

                $iCountUnits++;

                $sOut .= $sSplitter;

            } // if next square or not

        } // loop each unit

        return $sOut;

    } // __toString
    /* *\ FUNCTION __toString END /* */


    /* *
     * * function __toStringFormated()
     * *
     * * version 20160203_012711 (CC) Luke JZ aka SwissalpS
     * */
    public function __toStringFormated() {

        if (!$this->iValueMax) {

            return '';

        } // if not initialized

        $sOut = chr(10);
        $sIn = $this->__toString();

        $aLinesRaw = explode(chr(10), $sIn);
        $aLines = [];
        foreach ($aLinesRaw as $sLine) {

            $aLines[] = explode(' ', $sLine);

        } // loop each line splitting

        $aColumnWidths = [];
        for ($iColumn = 0; $iColumn < $this->iValueMax; $iColumn++) {

            $aColumn = array_column($aLines, $iColumn);
            $iBiggestWidth = 0;
            foreach ($aColumn as $sValue) {

                $iWidth = strlen($sValue);
                if ($iBiggestWidth < $iWidth) {
                    $iBiggestWidth = $iWidth;
                } // if wider value found

            } // loop each value in column to find widest

            $aColumnWidths[] = $iBiggestWidth;

        } // loop each column finding widest

        for ($iRow = 0; $iRow < $this->iValueMax; $iRow++) {

            for ($iColumn = 0; $iColumn < $this->iValueMax; $iColumn++) {

                $sValue = $aLines[$iRow][$iColumn];
                $iWidthMax = $aColumnWidths[$iColumn];
                $iWidth = strlen($sValue);

                $sOut .= $sValue . (str_repeat(' ', 2 + $iWidthMax - $iWidth));

            } // loop each column adding output and spacer

            $sOut .= chr(10);

        } // loop each row adding to output

        return $sOut;

    } // __toStringFormated
    /* *\ FUNCTION __toStringFormated END /* */


    /* *
     * * function convertLinearToRectangular()
     * *
     * * @param int $iSquare the linear square-count. 0 is top-left.
     * * @param int $iUnit the linear unit-count inside square. 0 is top-left.
     * * @return array ['column' => x, 'row' => y]
     * * version 20160201_235208 (CC) Luke JZ aka SwissalpS
     * */
    public function convertLinearToRectangular($iSquare, $iUnit) {

        if (($iSquare >= $this->iValueMax)
           || ($iUnit >= $this->iValueMax)
           || (0 > $iSquare)
           || (0 > $iUnit)) {

            debug_print_backtrace();

            return ['column' => -1, 'row' => -1];

        } // if invalid coordinates or value passed

        $iSide = intval(sqrt($this->iValueMax));

        $iColumnInSquare = $iUnit % $iSide;
        $iSquareColumn = $iSquare % $iSide;

        $iRowInSquare = floor($iUnit / $iSide);
        $iSquareRow = floor($iSquare / $iSide);

        $iRow = ($iSquareRow * $iSide) + $iRowInSquare;
        $iColumn = ($iSquareColumn * $iSide) + $iColumnInSquare;

        return ['column' => intval($iColumn), 'row' => intval($iRow)];

    } // convertLinearToRectangular
    /* *\ FUNCTION convertLinearToRectangular END /* */


    /* *
     * * function convertRectangularToLinear()
     * *
     * * @param int $iRow the cartesian row. 0 is top-most.
     * * @param int $iColumn the cartesian column. 0 is left-most.
     * * @return array ['unit' => i, 'square' => j]
     * * version 20160201_235132 (CC) Luke JZ aka SwissalpS
     * */
    public function convertRectangularToLinear($iRow, $iColumn) {

        if (($iColumn >= $this->iValueMax)
           || ($iRow >= $this->iValueMax)
           || (0 > $iColumn)
           || (0 > $iRow)) {

            debug_print_backtrace();

            return ['square' => -1, 'unit' => -1];

        } // if invalid coordinates or value passed

        $iSide = intval(sqrt($this->iValueMax));

        $iColumnInSquare = $iColumn % $iSide;
        $iSquareColumn = floor($iColumn / $iSide);

        $iRowInSquare = $iRow % $iSide;
        $iSquareRow = floor($iRow / $iSide);

        $iUnit = ($iRowInSquare * $iSide) + $iColumnInSquare;
        $iSquare = ($iSquareRow * $iSide) + $iSquareColumn;

        return ['unit' => intval($iUnit), 'square' => intval($iSquare)];

    } // convertRectangularToLinear
    /* *\ FUNCTION convertRectangularToLinear END /* */


    /* *
     * * function eliminateByForcedAlleys()
     * *
     * * version 20160203_020459 (CC) Luke JZ aka SwissalpS
     * */
    public function eliminateByForcedAlleys() {

        if (!$this->iValueMax) {

            debug_print_backtrace();

            return false;

        } // if not initialized

        $bRemoved = false;

        $iSide = intval(sqrt($this->iValueMax));
        for ($iCountSquares = 0; $iCountSquares < $this->iValueMax; $iCountSquares++) {

            $oSquare = $this->aSquares[$iCountSquares];

            for ($iCountUnits = 0; $iCountUnits < $this->iValueMax; $iCountUnits++) {

                $aPVs = $oSquare->possibleValues($iCountUnits);
                if (1 >= count($aPVs)) {

                    // only one value or less -> skip
                    // these are dealt with the simpler rules
                    continue;

                } // if skip

                $aPosition = $this->convertLinearToRectangular($iCountSquares, $iCountUnits);
                $iColumnInit = $aPosition['column'];
                $iRowInit = $aPosition['row'];

                foreach ($aPVs as $iValue) {
//var_dump(' -- S: '.$iCountSquares.' U: '.$iCountUnits.' V: '.$iValue);

                    // find horizontal alleys
                    $aUnitsToSkip = $this->unitsInSquareRow($iCountUnits);
//var_dump(' -- S: '.$iCountSquares.' U: '.$iCountUnits.' V: '.$iValue,'uisr:',$aUnitsToSkip);

                    $bIsAlley = true;
                    for ($iCountUnits2 = 0; $iCountUnits2 < $this->iValueMax; $iCountUnits2++) {

                        if (in_array($iCountUnits2, $aUnitsToSkip)) {

                            continue;

                        } // if skip

                        $aPVs2 = $oSquare->possibleValues($iCountUnits2);
                        if (in_array($iValue, $aPVs2)) {

                            // not unique to row in square
                            $bIsAlley = false;
                            break;

                        } // if value found

                    } // loop units checking if value exists somewhere

                    if ($bIsAlley) {
//var_dump(' rowAlley found -- S: '.$iCountSquares.' U: '.$iCountUnits.' V: '.$iValue.' R: '.$iRowInit.' C: '.$iColumnInit);

                        // clear out value in row except this square
                        if ($this->removeValueFromRowExcept($iValue, $iRowInit, $iColumnInit)) {

                            $bRemoved = true;

                        } // if removed something

                    } // if alley found

                    // find vertical alleys
                    $aUnitsToSkip = $this->unitsInSquareColumn($iCountUnits);
//var_dump('uisc:',$aUnitsToSkip);
                    $bIsAlley = true;
                    for ($iCountUnits2 = 0; $iCountUnits2 < $this->iValueMax; $iCountUnits2++) {

                        if (in_array($iCountUnits2, $aUnitsToSkip)) {

                            continue;

                        } // if skip

                        $aPVs2 = $oSquare->possibleValues($iCountUnits2);
                        if (in_array($iValue, $aPVs2)) {
//var_dump('not v-alley');
                            // not unique to row in square
                            $bIsAlley = false;
                            break;

                        } // if value found

                    } // loop units checking if value exists somewhere

                    if ($bIsAlley) {
//var_dump(' colAlley found -- S: '.$iCountSquares.' U: '.$iCountUnits.' V: '.$iValue.' R: '.$iRowInit.' C: '.$iColumnInit);

                        // clear out value in column except this square
                        if ($this->removeValueFromColumnExcept($iValue, $iRowInit, $iColumnInit)) {

                            $bRemoved = true;

                        } // if removed something

                    } // if alley found

                } // loop all possible values

            } // loop units

        } // loop squares

        return $bRemoved;

    } // eliminateByForcedAlleys
    /* *\ FUNCTION eliminateByForcedAlleys END /* */


    /* *
     * * function eliminateBySingleValueRule()
     * *
     * * version 20160202_220042 (CC) Luke JZ aka SwissalpS
     * */
    public function eliminateBySingleValueRule() {

        if (!$this->iValueMax) {

            debug_print_backtrace();

            return false;

        } // if not initialized

        $bRemoved = false;

        $iSide = intval(sqrt($this->iValueMax));

        // look in each square for units with unique value to other units of square
        for ($iCountSquares = 0; $iCountSquares < $this->iValueMax; $iCountSquares++) {
//var_dump('Square: '.$iCountSquares);
            $oSquare = $this->aSquares[$iCountSquares];

            for ($iValue = 1; $iValue <= $this->iValueMax; $iValue++) {
//var_dump('Value: '.$iValue);
                $iUnitWithValue = -1;
                for ($iCountUnits = 0; $iCountUnits < $this->iValueMax; $iCountUnits++) {
//var_dump('Unit: '.$iCountUnits);
                    $aPVs = $oSquare->possibleValues($iCountUnits);
                    if (in_array($iValue, $aPVs)) {

                        if (-1 === $iUnitWithValue) {

                            // first unit found with this value
                            $iUnitWithValue = $iCountUnits;

                        } else {

                            // already had another unit with this value, skip value
                            $iUnitWithValue = -2;
//var_dump('breaking');
                            break;

                        } // if first unit with value or not
//var_dump('should not show after breaking');
                    } // if unit contains value
//var_dump('should also not show after breaking');
                } // loop each unit in square for this value
//var_dump('should be first after breaking');
                // has one been found?
                if (0 <= $iUnitWithValue) {

//var_dump('  ---   S: '.$iCountSquares.' U: '.$iUnitWithValue.' V: '.$iValue);

                    // set value as single possibility
                    if ($oSquare->setValue($iValue, $iUnitWithValue)) {

                        $bRemoved = true;

                    } // if removed something

                    // no need to eliminate from square as is unique

                    // convert coordinates
                    $aPosition = $this->convertLinearToRectangular($iCountSquares, $iUnitWithValue);
                    $iColumn = $aPosition['column'];
                    $iRow = $aPosition['row'];

                    // eliminate all in row
                    if ($this->removeValueFromRowExcept($iValue, $iRow, $iColumn)) {

                        $bRemoved = true;

                    } // if removed something

                    // eliminate all in column
                    if ($this->removeValueFromColumnExcept($iValue, $iRow, $iColumn)) {

                        $bRemoved = true;

                    } // if removed something

                } // if a unique possibility was found

            } // loop each possible value to find uniques

        } // loop all squares

        // look at each row for units with unique value to other units of row
        for ($iRow = 0; $iRow < $this->iValueMax; $iRow++) {

            $iColumnWithValue = -1;
            for ($iValue = 1; $iValue <= $this->iValueMax; $iValue++) {

                for ($iColumn = 0; $iColumn < $this->iValueMax; $iColumn++) {

                    $aPosition = $this->convertRectangularToLinear($iRow, $iColumn);
                    $iSquare = $aPosition['square'];
                    $iUnit = $aPosition['unit'];

                    $oSquare = $this->aSquares[$iSquare];
                    $aPVs = $oSquare->possibleValues($iUnit);

                    if (in_array($iValue, $aPVs)) {

                        if (-1 === $iColumnWithValue) {

                            // first unit found with this value
                            $iColumnWithValue = $iColumn;

                        } else {

                            // already had another unit with this value, skip value
                            $iColumnWithValue = -2;
                            break;

                        } // if first unit with value or not

                    } // if unit has this value as option

                } // loop each unit in row for this value

                // has one been found?
                if (0 <= $iColumnWithValue) {

                    // convert coordinates
                    $aPosition = $this->convertRectangularToLinear($iRow, $iColumnWithValue);
                    $iSquare = $aPosition['square'];
                    $iUnit = $aPosition['unit'];

                    $oSquare = $this->aSquares[$iSquare];

                    // set value as single possibility
                    if ($oSquare->setValue($iValue, $iUnit)) {

                        $bRemoved = true;

                    } // if removed something

                    // eliminate from square
                    if ($oSquare->removeValueExcept($iValue, $iUnit)) {

                        $bRemoved = true;

                    } // if removed something

                    // no need to eliminate all in row as is unique

                    // eliminate all in column
                    if ($this->removeValueFromColumnExcept($iValue, $iRow, $iColumnWithValue)) {

                        $bRemoved = true;

                    } // if removed something

                } // if a unique possibility was found

            } // loop each possible value to find uniques

        } // loop each row

        // look at each column for units with unique value to other units of column
        for ($iColumn = 0; $iColumn < $this->iValueMax; $iColumn++) {

            $iRowWithValue = -1;
            for ($iValue = 1; $iValue <= $this->iValueMax; $iValue++) {

                for ($iRow = 0; $iRow < $this->iValueMax; $iRow++) {

                    $aPosition = $this->convertRectangularToLinear($iRow, $iColumn);
                    $iSquare = $aPosition['square'];
                    $iUnit = $aPosition['unit'];

                    $oSquare = $this->aSquares[$iSquare];
                    $aPVs = $oSquare->possibleValues($iUnit);

                    if (in_array($iValue, $aPVs)) {

                        if (-1 === $iRowWithValue) {

                            // first unit found with this value
                            $iRowWithValue = $iRow;

                        } else {

                            // already had another unit with this value, skip value
                            $iRowWithValue = -2;
                            break;

                        } // if first unit with value or not

                    } // if unit has this value as option

                } // loop each unit in column for this value

                // has one been found?
                if (0 <= $iRowWithValue) {

                    // convert coordinates
                    $aPosition = $this->convertRectangularToLinear($iRowWithValue, $iColumn);
                    $iSquare = $aPosition['square'];
                    $iUnit = $aPosition['unit'];

                    $oSquare = $this->aSquares[$iSquare];

                    // set value as single possibility
                    if ($oSquare->setValue($iValue, $iUnit)) {

                        $bRemoved = true;

                    } // if removed something

                    // eliminate from square
                    if ($oSquare->removeValueExcept($iValue, $iUnit)) {

                        $bRemoved = true;

                    } // if removed something

                    // eliminate all in row
                    if ($this->removeValueFromRowExcept($iValue, $iRowWithValue, $iColumn)) {

                        $bRemoved = true;

                    } // if removed something

                    // no need to eliminate all in column as is unique

                } // if a unique possibility was found

            } // loop each possible value to find uniques

        } // loop each column

        return $bRemoved;

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

        if (!$this->iValueMax) {

            debug_print_backtrace();

            return false;

        } // if not initialized

        $bHaveEliminated = false;

        $iSide = intval(sqrt($this->iValueMax));

        // find solved units
        // loop each square
        for ($iCountSquares = 0; $iCountSquares < $this->iValueMax; $iCountSquares++) {

            $oSquare = $this->aSquares[$iCountSquares];

            // loop each unit in square
            for ($iCountUnits = 0; $iCountUnits < $this->iValueMax; $iCountUnits++) {

                $aPVs = $oSquare->possibleValues($iCountUnits);
                if (1 == count($aPVs)) {

                    // found one with only one possible value
                    $iValue = $aPVs[0];

                    // eliminate from square
                    if ($oSquare->removeValueExcept($iValue, $iCountUnits)) {

                        $bHaveEliminated = true;

                    } // if any got removed

                    // convert coordinates
                    $aPosition = $this->convertLinearToRectangular($iCountSquares, $iCountUnits);
                    $iColumn = $aPosition['column'];
                    $iRow = $aPosition['row'];

                    // eliminate from row neighbours
                    if ($this->removeValueFromRowExcept($iValue, $iRow, $iColumn)) {

                        $bHaveEliminated = true;

                    } // if any removed

                    // eliminate from column neighbours
                    if ($this->removeValueFromColumnExcept($iValue, $iRow, $iColumn)) {

                        $bHaveEliminated = true;

                    } // if any removed

                } // if found one

            } // loop all units of square

            //$oUnit = $this->getUnit($iSquare, $iCountUnits);

        } // loop all squares

        return $bHaveEliminated;

    } // eliminateObvious
    /* *\ FUNCTION eliminateObvious END /* */


    /* *
     * * function init()
     * *
     * * version 20160201_121844 (CC) Luke JZ aka SwissalpS
     * */
    public function init(&$aLines) {

        $this->aSquares = [];
        $iSide = intval(sqrt($this->iValueMax));

        // convert given array into a single dimensional array of values
        $aData = [];
        foreach ($aLines as $aLine) {
            foreach ($aLine as $iValue) {
                if (false !== strpos($iValue, ',')) {
                    //$aData[] = explode(',', $iValue);
                    $iValue = 0;
                }
                $aData[] = intval($iValue);
            } // loop values
        } // loop lines

        // prepare square array (multidimensional[maxSquares])
        $aSquareValues = array_fill(0, $this->iValueMax, []);
        // count squares
        $iCountSquares = 0;
        // count squares horizontally
        $iCountSquareColumns = 0;
        // count columns in square
        $iCountColumns = 0;
        // count lines in square
        $iCountLines = 0;
        // convert given array into chunks for squares
        for ($iCount = 0; $iCount < count($aData); $iCount++) {

            // extract next value
            $iValue = $aData[$iCount];
            // assign value
            $aSquareValues[$iCountSquares][] = $iValue;

            // increment column counter
            $iCountColumns++;
            if ($iCountColumns == $iSide) {

                // move to next square

                // reset counter
                $iCountColumns = 0;

                // increment horizontal counter
                $iCountSquareColumns++;
                if ($iCountSquareColumns == $iSide) {

                    // also move to next line

                    // reset counter
                    $iCountSquareColumns = 0;

                    // increment line counter
                    $iCountLines++;
                    if ($iCountLines == $iSide) {

                        // move to next row of squares

                        // reset counter
                        $iCountLines = 0;

                        // increment square counter
                        $iCountSquares++;

                    } else {

                        // move to beginning of this row of squares
                        $iCountSquares -= $iSide -1;

                    } // if next square row or not

                } else {

                    // increment square counter
                    $iCountSquares++;

                } // if move to next line or not

            } // if move to next square or not

        } // loop all values

        unset($aData);

        for ($iCountSquares = 0; $iCountSquares < $this->iValueMax; $iCountSquares++) {

            $aSquare = $aSquareValues[$iCountSquares];

            $oSquare = new Square($aSquare);

            $this->aSquares[] = $oSquare;

        } // loop all lines constructing squares

        $oSquare = null;
        unset($aSquareValues, $aSquare, $oSquare);

        return $this;

    } // init
    /* *\ FUNCTION init END /* */


    /* *
     * * function isSolved()
     * *
     * * @return bool true if all fields are filled
     * * version 20160203_212655 (CC) Luke JZ aka SwissalpS
     * */
    public function isSolved() {

        if (!$this->iValueMax) {

            debug_print_backtrace();

            return false;

        } // if not initialized

        foreach ($this->aSquares as $oSquare) {

            if (!$oSquare->isSolved()) {

                return false;

            } // if not solved

        } // loop each square

        // TODO: check if correctly solved!

        return true;

    } // isSolved
    /* *\ FUNCTION isSolved END /* */


    /* *
     * * function possibleValues()
     * *
     * * version 20160201_103706 (CC) Luke JZ aka SwissalpS
     * */
    public function possibleValues() {

        if (!$this->iValueMax) {

            debug_print_backtrace();

            return [];

        } // if not initialized

        static $a = null;
        if (!$a) {

            $a = [];
            for ($iCount = 1; $iCount <= $this->iValueMax; $iCount++) {

                $a[] = $iCount;

            } // loop all possible values

        } // if first call

        return $a;

    } // possibleValues
    /* *\ FUNCTION possibleValues END /* */


    /* *
     * * function removeValueFromColumnExcept()
     * *
     * * version 20160201_131759 (CC) Luke JZ aka SwissalpS
     * */
    public function removeValueFromColumnExcept($iValue, $iRow, $iColumn) {

        if (!$this->iValueMax) {

            debug_print_backtrace();

            return false;

        } // if not initialized

        if (($iRow >= $this->iValueMax)
           || ($iColumn >= $this->iValueMax)
           || (0 > $iRow)
           || (0 > $iColumn)
           || (1 > $iValue)
           || ($iValue > $this->iValueMax)) {

            debug_print_backtrace();

            return false;

        } // if invalid coordinates or value passed

        $bRemoved = false;
        $iSide = intval(sqrt($this->iValueMax));

        // first square in column of squares
        $aPosition = $this->convertRectangularToLinear(0, $iColumn);
        $iSquareFirst = $aPosition['square'];
        $iUnitFirst = $aPosition['unit'];

        // square to skip
        $aPosition = $this->convertRectangularToLinear($iRow, $iColumn);
        $iSquareSkip = $aPosition['square'];

        for ($iCountSquares = $iSquareFirst; $iCountSquares < $this->iValueMax; $iCountSquares += $iSide) {

            // need to skip?
            if ($iCountSquares == $iSquareSkip) {

                continue;

            } // if skip

            $oSquare = $this->aSquares[$iCountSquares];

            for ($iCountUnits = $iUnitFirst; $iCountUnits < $this->iValueMax; $iCountUnits += $iSide) {

                if ($oSquare->removeValue($iValue, $iCountUnits)) {

                    $bRemoved = true;

                } // if removed something

            } // loop units

        } // loop all squares in row

        return $bRemoved;

    } // removeValueFromColumnExcept
    /* *\ FUNCTION removeValueFromColumnExcept END /* */


    /* *
     * * function removeValueFromRowExcept()
     * *
     * * version 20160201_131759 (CC) Luke JZ aka SwissalpS
     * */
    public function removeValueFromRowExcept($iValue, $iRow, $iColumn) {

        if (!$this->iValueMax) {

            debug_print_backtrace();

            return false;

        } // if not initialized

        if (($iRow >= $this->iValueMax)
           || ($iColumn >= $this->iValueMax)
           || (0 > $iRow)
           || (0 > $iColumn)
           || (1 > $iValue)
           || ($iValue > $this->iValueMax)) {

            debug_print_backtrace();

            return false;

        } // if invalid coordinates or value passed

        $bRemoved = false;
        $iSide = intval(sqrt($this->iValueMax));

        // first square in row of squares
        $aPosition = $this->convertRectangularToLinear($iRow, 0);
        $iSquareFirst = $aPosition['square'];
        $iUnitFirst = $aPosition['unit'];

        // square to skip
        $aPosition = $this->convertRectangularToLinear($iRow, $iColumn);
        $iSquareSkip = $aPosition['square'];

        for ($iCountSquares = $iSquareFirst; $iCountSquares < $iSquareFirst + $iSide; $iCountSquares++) {

            // need to skip?
            if ($iCountSquares == $iSquareSkip) {

                continue;

            } // if skip

            $oSquare = $this->aSquares[$iCountSquares];

            for ($iCountUnits = $iUnitFirst; $iCountUnits < $iUnitFirst + $iSide; $iCountUnits++) {

                if ($oSquare->removeValue($iValue, $iCountUnits)) {

                    $bRemoved = true;

                } // if removed something

            } // loop units

        } // loop all squares in row

        return $bRemoved;

    } // removeValueFromRowExcept
    /* *\ FUNCTION removeValueFromRowExcept END /* */


    /* *
     * * function unitsInSquareColumn()
     * *
     * * @param int $iUnit a unit index of the column to extract
     * * version 20160203_151457 (CC) Luke JZ aka SwissalpS
     * */
    public function unitsInSquareColumn($iUnit) {

        $aOut = [];

        if (!$this->iValueMax) {

            debug_print_backtrace();

            return $aOut;

        } // if not initialized

        if ((0 > $iUnit) || ($iUnit >= $this->iValueMax)) {

            debug_print_backtrace();

            return $aOut;

        } // if invalid index given

        $iSide = intval(sqrt($this->iValueMax));

        $iFirstInColumn = $iUnit % $iSide;

        for ($iCount = 0; $iCount < $this->iValueMax; $iCount += $iSide) {

            $aOut[] = $iCount + $iFirstInColumn;

        } // collect ids of column

        return $aOut;

    } // unitsInSquareColumn
    /* *\ FUNCTION unitsInSquareColumn END /* */


    /* *
     * * function unitsInSquareRow()
     * *
     * * @param int $iUnit a unit index of the row to extract
     * * version 20160203_150430 (CC) Luke JZ aka SwissalpS
     * */
    public function unitsInSquareRow($iUnit) {

        $aOut = [];

        if (!$this->iValueMax) {

            debug_print_backtrace();

            return $aOut;

        } // if not initialized

        if ((0 > $iUnit) || ($iUnit >= $this->iValueMax)) {

            debug_print_backtrace();

            return $aOut;

        } // if invalid index given

        $iSide = intval(sqrt($this->iValueMax));

        $iFirstInRow = intval(floor($iUnit / $iSide));

        for ($iCount = 0; $iCount < $iSide; $iCount++) {

            $aOut[] = $iCount + $iFirstInRow;

        } // collect ids of row

        return $aOut;

    } // unitsInSquareRow
    /* *\ FUNCTION unitsInSquareRow END /* */

} // Field
/* * *\ CLASS Field END /* * */

/* * * *\ SwissalpS/Games/Sudoku/Field.inc (CC) Luke JZ aka SwissalpS /* * * */
