#!/usr/bin/php
<?php
/* * * *
 * * Sudoku
 * *
 * * grid game solver (not only 3*3)
 * *
 * * not yet complete, can not yet do deductions and there are also some simpler rules that need to be implemented. Such as pairs...
 * *
 * * @version 20160201_060856 (CC) Luke JZ aka SwissalpS
 * * * */
error_reporting(E_ALL); // 0); //
$sPathToLibrary = dirname(dirname(dirname(__FILE__)));
require_once($sPathToLibrary . '/SssS_bootstrap.inc');

$oS = SwissalpS\Games\Sudoku\Sudoku::sharedController();
$sExamplesPath = $sPathToLibrary . '/Games/Sudoku/examples/';
//$sFilePath = $sExamplesPath . 'SudokuExampleTestHard.txt';
//$sFilePath = $sExamplesPath . 'SudokuExampleTestHardest.txt';
$sFilePath = $sExamplesPath . 'SudokuExampleTestHard3.txt';
$oS->loadFile($sFilePath);
$iCountRoundsObvious = $oS->eliminateObvious();
$iCountRoundsSingleValue = $oS->eliminateBySingleValueRule();
$iCountRoundsAlley = $oS->eliminateByForcedAlleys();
$iCountRoundsObvious2 = $oS->eliminateObvious();
$iCountRoundsSingleValue2 = $oS->eliminateBySingleValueRule();
$iCountRoundsAlley2 = $oS->eliminateByForcedAlleys();
$iCountRoundsObvious3 = $oS->eliminateObvious();
$iCountRoundsSingleValue3 = $oS->eliminateBySingleValueRule();
$iCountRoundsAlley3 = $oS->eliminateByForcedAlleys();
$sState = $oS->__toStringFormated();
var_dump('obv1: '.$iCountRoundsObvious.' sing1: '.$iCountRoundsSingleValue.' alley1: '.$iCountRoundsAlley.' obv2: '.$iCountRoundsObvious2.' sing2: '.$iCountRoundsSingleValue2.' alley2: '.$iCountRoundsAlley2.' obv3: '.$iCountRoundsObvious3.' sing3: '.$iCountRoundsSingleValue3.' alley3: '.$iCountRoundsAlley3.' is solved: '.($oS->isSolved() ? 'Y' : 'N'), $sState);
exit(0);

/* * * *\ Sudoku (CC) Luke JZ aka SwissalpS /* * * */
