<?php
/* * * *
 * * SwissalpS/Games/Sudoku/Square.inc
 * * grid game solver (not only 3*3)
 * *
 * * not yet complete, can not yet do deductions and there are also some simpler rules that need to be implemented. Such as pairs...
 * *
 * * @version 20160203_202734 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\Games\Sudoku;

/* * *
 * * class Square
 * *
 * * version 20160201_061045 (CC) Luke JZ aka SwissalpS
 * * */
class Square {

    protected $aUnits;

    /* *
     * * function __construct()
     * *
     * * @param array $aValues a single-dimensional array with values for this square. Each entry is a value starting top left.
     * * version 20160201_103105 (CC) Luke JZ aka SwissalpS
     * */
    public function __construct(&$aValues) {

        $this->aUnits = [];
        $iCountPosition = 0;
        foreach ($aValues as $iValue) {

            $this->aUnits[] = new Unit($iCountPosition, $iValue);
            $iCountPosition++;

        } // loop each value

    } // __construct
    /* *\ FUNCTION __construct END /* */


    /* *
     * * function isSolved()
     * *
     * * version 20160203_213413 (CC) Luke JZ aka SwissalpS
     * */
    public function isSolved() {

        $aValuesFound = [];
        $iSize = Sudoku::size();

        foreach ($this->aUnits as $oUnit) {

            $aPVs = $oUnit->possibleValues();
            $iCount = count($aPVs);
            if (1 < $iCount) {

                return false;

            } // if multiple possible values found -> not solved

            if (1 == $iCount) {

                $iValue = $aPVs[0];

                if ((1 > $iValue)
                    || ($iSize < $iValue)){

                    return false;

                } // if invalid value

                $aValuesFound[] = $iValue;

            } // if has only one value

        } // loop each unit checking if has only one value

        array_unique($aValuesFound);

        if ($iSize != count($aValuesFound)) {

            return false;

        } // if has duplicate values in square

        return true;

    } // isSolved
    /* *\ FUNCTION isSolved END /* */


    /* *
     * * function possibleValues()
     * *
     * * version 20160201_113640 (CC) Luke JZ aka SwissalpS
     * */
    public function possibleValues($iUnit) {

        if (isset($this->aUnits[$iUnit])) {

            $oUnit = $this->aUnits[$iUnit];

            return $oUnit->possibleValues();

        } else {

            debug_print_backtrace();

            return Sudoku::possibleValues();

        } // if got a unit at iPos or not

    } // possibleValues
    /* *\ FUNCTION possibleValues END /* */


    /* *
     * * function removeValue()
     * *
     * * version 20160201_130219 (CC) Luke JZ aka SwissalpS
     * */
    public function removeValue($iValue, $iUnit) {

        $bRemoved = false;

        if (!isset($this->aUnits[$iUnit])) {

            debug_print_backtrace();

            return $bRemoved;

        } // if invalid position given

        $oUnit = $this->aUnits[$iUnit];

        if ($oUnit->removeValue($iValue)) {

            $bRemoved = true;

        } // if removed something

        return $bRemoved;

    } // removeValue
    /* *\ FUNCTION removeValue END /* */


    /* *
     * * function removeValueExcept()
     * *
     * * version 20160201_130219 (CC) Luke JZ aka SwissalpS
     * */
    public function removeValueExcept($iValue, $iUnit) {

        $bRemoved = false;

        if (!isset($this->aUnits[$iUnit])) {

            debug_print_backtrace();

            return $bRemoved;

        } // if invalid position given

        $iTotal = count($this->aUnits);

        for ($iCountUnits = 0; $iCountUnits < $iTotal; $iCountUnits++) {

            if ($iUnit == $iCountUnits) {

                continue;

            } // if skip

            $oUnit = $this->aUnits[$iCountUnits];
            if ($oUnit->removeValue($iValue)) {

                $bRemoved = true;

            } // if removed something

        } // loop all units except the one mentioned

        return $bRemoved;

    } // removeValueExcept
    /* *\ FUNCTION removeValueExcept END /* */


    /* *
     * * function setValue()
     * *
     * * @return bool true if anything changed
     * * version 20160202_225630 (CC) Luke JZ aka SwissalpS
     * */
    public function setValue($iValue, $iUnit) {

        $bChanged = false;

        if (!isset($this->aUnits[$iUnit])) {

            debug_print_backtrace();

            return $bChanged;

        } // if invalid position given

        $oUnit = $this->aUnits[$iUnit];

        if ($oUnit->setValue($iValue)) {

            $bChanged = true;

        } // if removed something

        return $bChanged;

    } // setValue
    /* *\ FUNCTION setValue END /* */

} // Square
/* * *\ CLASS Square END /* * */

/* * * *\ SwissalpS/Games/Sudoku/Square.inc (CC) Luke JZ aka SwissalpS /* * * */
