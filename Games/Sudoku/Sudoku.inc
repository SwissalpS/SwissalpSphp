<?php
/* * * *
 * * SwissalpS/Games/Sudoku/Sudoku.inc
 * * grid game solver (not only 3*3)
 * *
 * * not yet complete, can not yet do deductions and there are also some simpler rules that need to be implemented. Such as pairs...
 * *
 * * @version 20160203_202734 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\Games\Sudoku;


/* * *
 * * class Sudoku
 * *
 * * version 20160201_062552 (CC) Luke JZ aka SwissalpS
 * * */
class Sudoku {

    const NW = 1;
    const N = 2;
    const NE = 4;
    const W = 8;
    const E = 16;
    const SW = 32;
    const S = 64;
    const SE = 128;

    /* *
     * * function possibleValues()
     * *
     * * version 20160201_071206 (CC) Luke JZ aka SwissalpS
     * */
    public static function possibleValues() {

        return Controller::sharedInstance()->possibleValues();

    } // possibleValues
    /* *\ FUNCTION possibleValues END /* */


    /* *
     * * function sharedController()
     * *
     * * version 20160201_122805 (CC) Luke JZ aka SwissalpS
     * */
    public static function sharedController() {

        return Controller::sharedInstance();

    } // sharedController
    /* *\ FUNCTION sharedController END /* */


    /* *
     * * function size()
     * *
     * * version 20160201_103528 (CC) Luke JZ aka SwissalpS
     * */
    public static function size() {

        return count(Controller::sharedInstance()->possibleValues());

    } // size
    /* *\ FUNCTION size END /* */

} // Sudoku
/* * *\ CLASS Sudoku END /* * */

/* * * *\ SwissalpS/Games/Sudoku/Sudoku.inc (CC) Luke JZ aka SwissalpS /* * * */
