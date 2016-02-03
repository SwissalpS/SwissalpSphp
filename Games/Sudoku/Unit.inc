<?php
/* * * *
 * * SwissalpS/Games/Sudoku/Unit.inc
 * * grid game solver (not only 3*3)
 * *
 * * not yet complete, can not yet do deductions and there are also some simpler rules that need to be implemented. Such as pairs...
 * *
 * * @version 20160203_202734 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\Games\Sudoku;

/* * *
 * * class Unit
 * *
 * * version 20160201_061037 (CC) Luke JZ aka SwissalpS
 * * */
class Unit {

    protected $aPossibleValues;
    protected $bIsPreset;
    protected $iPosition;

    /* *
     * * function __construct()
     * *
     * * version 20160201_062938 (CC) Luke JZ aka SwissalpS
     * */
    public function __construct($iPosition, $iValue = 0) {

        $this->iPosition = $iPosition;

        if (0 < $iValue) {

            $this->aPossibleValues = [$iValue];
            $this->bIsPreset = true;

        } else {

            $this->aPossibleValues = Sudoku::possibleValues();
            $this->bIsPreset = false;

        } // if value given or not

    } // __construct
    /* *\ FUNCTION __construct END /* */


    /* *
     * * function addValue()
     * *
     * * version 20160201_070223 (CC) Luke JZ aka SwissalpS
     * */
    public function addValue($iValue) {

        $bAdded = false;

        if ($this->bIsPreset) {

            return $bAdded;

        } // if preset value


        if (!in_array($iValue, $this->aPossibleValues)) {

            $this->aPossibleValues[] = $iValue;
            $bAdded = true;

        } // if not already in array

        return $bAdded;

    } // addValue
    /* *\ FUNCTION addValue END /* */


    /* *
     * * function isPreset()
     * *
     * * version 20160201_063618 (CC) Luke JZ aka SwissalpS
     * */
    public function isPreset() {

        return $this->bIsPreset;

    } // isPreset
    /* *\ FUNCTION isPreset END /* */


    /* *
     * * function possibleValues()
     * *
     * * version 20160201_114136 (CC) Luke JZ aka SwissalpS
     * */
    public function possibleValues() {

        return $this->aPossibleValues;

    } // possibleValues
    /* *\ FUNCTION possibleValues END /* */


    /* *
     * * function removeValue()
     * *
     * * version 20160201_070716 (CC) Luke JZ aka SwissalpS
     * */
    public function removeValue($iValue) {

        $bRemoved = false;

        if ($this->bIsPreset) {

            return $bRemoved;

        } // if preset value

        $bRemoved = in_array($iValue, $this->aPossibleValues);

        if ($bRemoved) {

            $this->aPossibleValues = array_values(array_diff($this->aPossibleValues, [$iValue]));

        } // if need to remove at all

        return $bRemoved;

    } // removeValue
    /* *\ FUNCTION removeValue END /* */


    /* *
     * * function setValue()
     * *
     * * @return bool true if anything changed
     * * version 20160202_225955 (CC) Luke JZ aka SwissalpS
     * */
    public function setValue($iValue) {

        $bChanged = false;

        if ($this->bIsPreset) {

            return $bChanged;

        } // if preset value

        if (1 == count($this->aPossibleValues)
            && ($iValue == $this->aPossibleValues[0])) {

            return $bChanged;

        } // if already set to this single value

        $bChanged = true;

        $this->aPossibleValues = [$iValue];

        return $bChanged;

    } // setValue
    /* *\ FUNCTION setValue END /* */

} // Unit
/* * *\ CLASS Unit END /* * */

/* * * *\ SwissalpS/Games/Sudoku/Unit.inc (CC) Luke JZ aka SwissalpS /* * * */
