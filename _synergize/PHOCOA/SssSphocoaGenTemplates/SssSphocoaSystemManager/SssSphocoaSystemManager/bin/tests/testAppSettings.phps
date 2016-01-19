#!/usr/bin/php
<?php
/* * * *
 * * testAppSettings.phps
 * * test for SwissalpS\PHOCOA\Settings\ApplicationSettings
 * *
 * * CAUTION: this will reset/delete any existing ApplicationSettings.plist
 * *
 * * @version 20160118_022258 (CC) Luke JZ aka SwissalpS
 * * * */
error_reporting(E_ALL); // 0); //
define('NL', chr(10));

// this is not sufficient, we need to bootstrap into an app
//require_once('/gitSwissalpS/SwissalpSphp/SssS_bootstrap.inc');
require_once(dirname(dirname(dirname(__FILE__))) . DIRECTORY_SEPARATOR . 'conf' . DIRECTORY_SEPARATOR . 'SssSphocoaApp.conf');

use SwissalpS\PHOCOA\Settings\ApplicationSettings;
use SwissalpS\XML\Plist;

$sPathConfig = APP_ROOT . DIR_SEP . 'conf' . DIR_SEP . 'ApplicationSettings.plist';
echo 'Searching for settings file in ' . $sPathConfig . NL;
if (file_exists($sPathConfig)) {
    echo '+ config found' . NL;
} else {
    echo '- no config found' . NL;
} //
$oS = ApplicationSettings::sharedInstance();
$oS->reset();

testBool($oS);
testData($oS);
testDate($oS);
testDouble($oS);
testFloat($oS);
testInteger($oS);
testString($oS);
testSet($oS);
testToArray($oS);
testToJson($oS);
testToPlist($oS);
testToString($oS);
testToYaml($oS);
$oS->reset();
exit();
function testBool(ApplicationSettings $oS) {

    $sKeyPath = 'bool/tests/A';
    $aTests = ['y', 'ye', 'Yes', true, 'true', 'TrUe', '1', '3', '1.1', 1.2, 1,
               0.13, false, null, 'hun', chr(0), '', '0',
               chr(254) . chr(0) . chr(127) . chr(0), 1/PHP_INT_MAX, [], ['uh'=>'ote', 'htn'=>'the'], new SwissalpS\CLI\ChmodHelper()];
    $aResultsGoal = [true, false, true, true, true, true, true, true, true, true, true,
                     true, false, false, false, false, false, false,
                     false, true, false, false, false];
    $aErrors = [];
    $iCount = 0;
    foreach ($aTests as $mTest) {

        $sKeyPathTest = $sKeyPath . $iCount;

        $oS->setBool($sKeyPathTest, $mTest);
        $sTestBoolReturn = $oS->getBool($sKeyPathTest, 'FAIL');

        $mTarget = $aResultsGoal[$iCount];

        if ($sTestBoolReturn !== $mTarget) {

            $aErrors[] = ['#' => $iCount, 'input' => $mTest, 'result' => $sTestBoolReturn, 'target' => $mTarget];

        } // if unexpected value

        $iCount++;

    } // loop all kinds

    echo __FUNCTION__ . ' ';

    if (count($aErrors)) {

        echo 'FAIL';
        var_dump($aErrors);

    } else {

        echo 'PASS';

    } // if got errors or not

    echo NL;

    return $aErrors;

} // testBool


function testDate(ApplicationSettings $oS) {

    $sKeyPath = 'date/tests/B';
    $aTests = ['y', 'now', 'next week', true, 'true', 'TrUe',
               '1', '3', '1.1', 1.2, 1, 0.13,
               false, null, 'hun', chr(0), '', '0',
               chr(254) . chr(0) . chr(127) . chr(0), 1/PHP_INT_MAX, [], ['uh'=>'ote', 'htn'=>'the'], new SwissalpS\CLI\ChmodHelper(),
               '20151231_123411', '31.12.2015 12:34:11', '31/12/2015 12:34:11', '12-31-2015 12:34:11',
               new WFDateTime()];
    // TODO: more standard date formats to test
    $sNow = (string)time();
    $sThen = (string)gmmktime(12, 34, 11, 12, 31, 2015);
    $aResultsGoal = ['FAIL', $sNow, (string)(time()+7*24*3600), 'FAIL', 'FAIL', 'FAIL',
                     '943920000', '943920000', '943920000', 'FAIL', '1', 'FAIL',
                     'FAIL', 'FAIL', 'FAIL', 'FAIL', 'FAIL', '943920000',
                     'FAIL', 'FAIL', 'FAIL', 'FAIL', 'FAIL',
                     $sThen, $sThen, $sThen, $sThen,
                     $sNow, ];
    $aErrors = [];
    $iCount = 0;

    foreach ($aTests as $mTest) {

        $sKeyPathTest = $sKeyPath . $iCount;
        $oS->setDate($sKeyPathTest, $mTest);
        $mReturnA = $oS->getDate($sKeyPathTest, 'FAIL');

        if ('FAIL' == $mReturnA) {

            $mReturn = $mReturnA;
            $sRFC = '';
            $sPlist = '';

        } else {

            $mReturn = $mReturnA->format('U');
            $sRFC = $mReturnA->format('r');
            $sPlist = $mReturnA->format('Y-m-d\TH:i:s\Z');

        } // if 'FAIL' returned or not

        $mTarget = $aResultsGoal[$iCount];

        if ($mReturn !== $mTarget) {

            $aErrors[] = ['#' => $iCount, 'input' => $mTest, 'result' => $mReturn, 'target' => $mTarget, 'rfc' => $sRFC, 'plist' => $sPlist];

        } // if unexpected value

        $iCount++;

    } // loop all tests

    echo __FUNCTION__ . ' ';

    if (count($aErrors)) {

        echo 'FAIL';
        var_dump($aErrors);

    } else {

        echo 'PASS';

    } // if got errors or not

    echo NL;

    return $aErrors;

} // testDate


function testData(ApplicationSettings $oS) {

    $sKeyPath = 'data/tests/C';
    $aTests = ['next week', true, 'true', 'TrUe',
               '1', '3', '1.1', 1.2, 1, 0.13,
               false, null, chr(0), '', '0',
               chr(254) . chr(0) . chr(127) . chr(0), 1/PHP_INT_MAX,
               [], ['uh'=>'ote', 'htn'=>'the'],
               new SwissalpS\CLI\ChmodHelper(), new WFDateTime()];
    $aResultsGoal = ['next week', '1', 'true', 'TrUe',
                     '1', '3', '1.1', '1.2', '1', '0.13',
                     'FAIL', 'FAIL', chr(0), 'FAIL', '0',
                     chr(254) . chr(0) . chr(127) . chr(0), (string)(1/PHP_INT_MAX),
                     'FAIL', 'FAIL',
                     'FAIL', 'FAIL'];
    $aErrors = [];
    $iCount = 0;

    foreach ($aTests as $mTest) {

        $sKeyPathTest = $sKeyPath . $iCount;

        $oS->setData($sKeyPathTest, $mTest);
        $mReturn = $oS->getData($sKeyPathTest, 'FAIL');

        $mTarget = $aResultsGoal[$iCount];

        if ($mReturn !== $mTarget) {

            $sCode = '';
            for ($iCount = 0; $iCount < strlen($mReturn); $iCount++) {
                $sCode .= ord($mReturn[$iCount]) . ' ';
            }

            $aErrors[] = ['#' => $iCount, 'input' => $mTest, 'result' => $mReturn, 'resCode' => $sCode, 'target' => $mTarget];

        } // if unexpected value

        $iCount++;

    } // loop all tests

    echo __FUNCTION__ . ' ';

    if (count($aErrors)) {

        echo 'FAIL';
        var_dump($aErrors);

    } else {

        echo 'PASS';

    } // if got errors or not

    echo NL;

    return $aErrors;

} // testData


function testDouble(ApplicationSettings $oS) {

    $sKeyPath = 'double/tests/D';
    $aTests = [null, chr(0), '', '0', '1', chr(254) . chr(0) . chr(127) . chr(0), 1.11111, 1/PHP_INT_MAX, [], ['uh'=>'ote', 'htn'=>'the'], new SwissalpS\CLI\ShellScriptRunner(), 0, 5, false, true];
    $aResultsGoal = [0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.11111, 1/PHP_INT_MAX, 0.0, 0.0, 0.0, 0.0, 5.0, 0.0, 1.0];
    $aErrors = [];
    $iCount = 0;

    foreach ($aTests as $mTest) {

        $sKeyPathTest = $sKeyPath . $iCount;

        $oS->setDouble($sKeyPathTest, $mTest);
        $mReturn = $oS->getDouble($sKeyPathTest, 'FAIL');

        $mTarget = $aResultsGoal[$iCount];

        if ($mReturn !== $mTarget) {

            $aErrors[] = ['#' => $iCount, 'input' => $mTest, 'result' => $mReturn, 'target' => $mTarget];

        } // if unexpected value

        $iCount++;

    } // loop all tests

    echo __FUNCTION__ . ' ';

    if (count($aErrors)) {

        echo 'FAIL';
        var_dump($aErrors);

    } else {

        echo 'PASS';

    } // if got errors or not

    echo NL;

    return $aErrors;

} // testDouble


function testFloat(ApplicationSettings $oS) {

    $sKeyPath = 'float/tests/E';
    $aTests = [null, chr(0), '', '0', '1', chr(254) . chr(0) . chr(127) . chr(0), 1.11111, 1/PHP_INT_MAX, [], ['uh'=>'ote', 'htn'=>'the'], new SwissalpS\CLI\ShellScriptRunner(), 0, 5, false, true];
    $aResultsGoal = [0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.11111, 1/PHP_INT_MAX, 0.0, 0.0, 0.0, 0.0, 5.0, 0.0, 1.0];
    $aErrors = [];
    $iCount = 0;

    foreach ($aTests as $mTest) {

        $sKeyPathTest = $sKeyPath . $iCount;

        $oS->setFloat($sKeyPathTest, $mTest);
        $mReturn = $oS->getFloat($sKeyPathTest, 'FAIL');

        $mTarget = $aResultsGoal[$iCount];

        if ($mReturn !== $mTarget) {

            $aErrors[] = ['#' => $iCount, 'input' => $mTest, 'result' => $mReturn, 'target' => $mTarget];

        } // if unexpected value

        $iCount++;

    } // loop all tests

    echo __FUNCTION__ . ' ';

    if (count($aErrors)) {

        echo 'FAIL';
        var_dump($aErrors);

    } else {

        echo 'PASS';

    } // if got errors or not

    echo NL;

    return $aErrors;

} // testFloat


function testInteger(ApplicationSettings $oS) {

    $sKeyPath = 'integer/tests/F';
    $aTests = [null, chr(0), '', '0', '1', chr(254) . chr(0) . chr(127) . chr(0), 1.11111, 1/PHP_INT_MAX, [], ['uh'=>'ote', 'htn'=>'the'], new SwissalpS\CLI\ChmodHelper()];
    $aResultsGoal = [0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0];
    $aErrors = [];
    $iCount = 0;

    foreach ($aTests as $mTest) {

        $sKeyPathTest = $sKeyPath . $iCount;

        $oS->setInteger($sKeyPathTest, $mTest);
        $mReturn = $oS->getInteger($sKeyPathTest, 'FAIL');

        $mTarget = $aResultsGoal[$iCount];

        if ($mReturn !== $mTarget) {

            $aErrors[] = ['#' => $iCount, 'input' => $mTest, 'result' => $mReturn, 'target' => $mTarget];

        } // if unexpected value

        $iCount++;

    } // loop all tests

    echo __FUNCTION__ . ' ';

    if (count($aErrors)) {

        echo 'FAIL';
        var_dump($aErrors);

    } else {

        echo 'PASS';

    } // if got errors or not

    echo NL;

    return $aErrors;

} // testInteger


function testString(ApplicationSettings $oS) {

    $sKeyPath = 'string/tests/G';
    $aTests = [null, chr(0), '', '0', '1', false, true, TRUE, chr(254) . chr(0) . chr(127) . chr(0), 1.11111, 1/PHP_INT_MAX, [], ['uh'=>'ote', 'htn'=>'the'], new SwissalpS\CLI\ChmodHelper()];
    $aResultsGoal = ['', '', '', '0', '1', '', '1', '1', chr(254) . chr(0) . chr(127) . chr(0), '1.11111', (string)(1/PHP_INT_MAX), '', '', ''];
    $aErrors = [];
    $iCount = 0;

    foreach ($aTests as $mTest) {

        $sKeyPathTest = $sKeyPath . $iCount;

        $oS->setString($sKeyPathTest, $mTest);
        $mReturn = $oS->getString($sKeyPathTest, 'FAIL');

        $mTarget = $aResultsGoal[$iCount];

        if ($mReturn !== $mTarget) {

            $aErrors[] = ['#' => $iCount, 'input' => $mTest, 'result' => $mReturn, 'target' => $mTarget];

        } // if unexpected value

        $iCount++;

    } // loop all tests

    echo __FUNCTION__ . ' ';

    if (count($aErrors)) {

        echo 'FAIL';
        var_dump($aErrors);

    } else {

        echo 'PASS';

    } // if got errors or not

    echo NL;

    return $aErrors;

} // testString


function testSet(ApplicationSettings $oS) {

    $sKeyPath = 'set/tests/H';
    $aTests = [null, chr(0), '', '0', '1', chr(254) . chr(0) . chr(127) . chr(0),
               1.11111, 1/PHP_INT_MAX, [], ['uh'=>'ote', 'htn'=>'the'],
               new SwissalpS\CLI\ChmodHelper()];/*];*/
    $aResultsGoal = ['FAIL', '', '', '0', '1', chr(254) . chr(0) . chr(127) . chr(0),
                     1.11111, 1/PHP_INT_MAX, [], ['uh'=>'ote', 'htn'=>'the'],
                     '[[SssS_Plist_data]]VHpveU5Ub2lVM2RwYzNOaGJIQlRYRU5NU1Z4RGFHMXZaRWhsYkhCbGNpSTZNRHA3ZlE9PT09==', ''];
    $aErrors = [];
    $iCount = 0;

    foreach ($aTests as $mTest) {

        $sKeyPathTest = $sKeyPath . $iCount;

        $oS->set($sKeyPathTest, $mTest);
        $mReturn = $oS->get($sKeyPathTest, 'FAIL');

        $mTarget = $aResultsGoal[$iCount];

        if ($mReturn !== $mTarget) {

            $aErrors[] = ['#' => $iCount, 'input' => $mTest, 'result' => $mReturn, 'target' => $mTarget];

        } // if unexpected value

        $iCount++;

    } // loop all tests

    echo __FUNCTION__ . ' ';

    if (count($aErrors)) {

        echo 'FAIL';
        var_dump($aErrors);

    } else {

        echo 'PASS';

    } // if got errors or not

    echo NL;

    return $aErrors;

} // testSet


function testToArray(ApplicationSettings $oS) {

    $oS->reset();

    $sKeyPath = 'toArray/tests/I';

    $oS->setBool($sKeyPath . '/boolTestTrue', true);
    $oS->setBool($sKeyPath . '/boolTestFalse', false);

    $oS->setData($sKeyPath . '/dataTestA', chr(254) . chr(0) . chr(127) . chr(0));

    $oS->setDate($sKeyPath . '/dateTestA', gmmktime(12, 34, 11, 12, 31, 2015));

    $oS->setDouble($sKeyPath . '/doubleTestA', 1/PHP_INT_MAX);

    $oS->setFloat($sKeyPath . '/floatTestA', 1/PHP_INT_MAX);

    $oS->setInteger($sKeyPath . '/intTestA', PHP_INT_MAX);

    $oS->setString($sKeyPath . '/stringTest', 'testing one, two and three');

    $oS->save();
    $oS->reload();

    $aWFno = ['toArray' => ['tests' => ['I' => [
        'boolTestTrue' => true,
        'boolTestFalse' => false,
        'dataTestA' => '[[CDATA]]/gB/AA',
        'dateTestA' => '[[IDATE]]1451565251',
        'doubleTestA' => 1.0842021724855E-19,
        'floatTestA' => 1.0842021724855E-19,
        'intTestA' => 9223372036854775807,
        'stringTest' => 'testing one, two and three']]]];

    $aWFyes = [];

    $aWFnoTest = $oS->toArray(false);
    $aWFyesTest = $oS->toArray(true);

    echo __FUNCTION__ . ' ';

    if ($aWFno !== $aWFnoTest) { //count($aErrors)) {

        echo 'FAIL';
        var_dump($aWFnoTest);

    } else {

        echo 'PASS';

    } // if got errors or not

    echo NL;

    return [];

} // testToArray


function testToJson(ApplicationSettings $oS) {

    $oS->reset();

    $sKeyPath = 'toJson/tests/J';

    $oS->setBool($sKeyPath . '/boolTestTrue', true);
    $oS->setBool($sKeyPath . '/boolTestFalse', false);

    $oS->setData($sKeyPath . '/dataTestA', chr(254) . chr(0) . chr(127) . chr(0));

    $oS->setDate($sKeyPath . '/dateTestA', gmmktime(12, 34, 11, 12, 31, 2015));

    $oS->setDouble($sKeyPath . '/doubleTestA', 1/PHP_INT_MAX);

    $oS->setFloat($sKeyPath . '/floatTestA', 1/PHP_INT_MAX);

    $oS->setInteger($sKeyPath . '/intTestA', PHP_INT_MAX);

    $oS->setString($sKeyPath . '/stringTest', 'testing one, two and three');

    $oS->save();
    $oS->reload();

    $sJson = 'TODO{"toJson":{"tests":{"J":{"boolTestTrue":true,"boolTestFalse":false,"dataTestA":"[[SssS_Plist_data]]\/gB\/AA==","dateTestA":"[[SssS_Plist_date]]2015-12-31T12:34:11Z","doubleTestA":1.0842021724855e-19,"floatTestA":1.0842021724855e-19,"intTestA":9223372036854775807,"stringTest":"testing one, two and three"}}}}';

    $sJsonTest = $oS->toJson();

    echo __FUNCTION__ . ' ';

    if ($sJson !== $sJsonTest) {

        echo 'FAIL';
        var_dump($sJsonTest);

    } else {

        echo 'PASS';

    } // if got errors or not

    echo NL;

    return [];

} // testToJson


function testToPlist(ApplicationSettings $oS) {

    $oS->reset();

    $sKeyPath = 'toPlist/tests/K';

    $oS->setBool($sKeyPath . '/boolTestTrue', true);
    $oS->setBool($sKeyPath . '/boolTestFalse', false);

    $oS->setData($sKeyPath . '/dataTestA', chr(254) . chr(0) . chr(127) . chr(0));

    $oS->setDate($sKeyPath . '/dateTestA', gmmktime(12, 34, 11, 12, 31, 2015));

    $oS->setDouble($sKeyPath . '/doubleTestA', 1/PHP_INT_MAX);

    $oS->setFloat($sKeyPath . '/floatTestA', 1/PHP_INT_MAX);

    $oS->setInteger($sKeyPath . '/intTestA', PHP_INT_MAX);

    $oS->setString($sKeyPath . '/stringTest', 'testing one, two and three');

    $oS->save();
    $oS->reload();

    $sPlist = '<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist>
    <dict>
        <key>toPlist</key><dict>
            <key>tests</key><dict>
                <key>K</key><dict>
                    <key>boolTestTrue</key><true/>
                    <key>boolTestFalse</key><false/>
                    <key>dataTestA</key><data>/gB/AA==</data>
                    <key>dateTestA</key><date>2015-12-31T12:34:11Z</date>
                    <key>doubleTestA</key><real>1.0842021724855E-19</real>
                    <key>floatTestA</key><real>1.0842021724855E-19</real>
                    <key>intTestA</key><integer>9223372036854775807</integer>
                    <key>stringTest</key><string>testing one, two and three</string>
                </dict>
            </dict>
        </dict>
    </dict>
</plist>
';

    $sPlistTest = $oS->toPlist();

    echo __FUNCTION__ . ' ';

    if ($sPlist !== $sPlistTest) {

        echo 'FAIL';
        var_dump($sPlistTest);

    } else {

        echo 'PASS';

    } // if got errors or not

    echo NL;

    return [];

} // testToPlist


function testToString(ApplicationSettings $oS) {

    $oS->reset();

    $sKeyPath = 'toString/tests/L';

    $oS->setBool($sKeyPath . '/boolTestTrue', true);
    $oS->setBool($sKeyPath . '/boolTestFalse', false);

    $oS->setData($sKeyPath . '/dataTestA', chr(254) . chr(0) . chr(127) . chr(0));

    $oS->setDate($sKeyPath . '/dateTestA', gmmktime(12, 34, 11, 12, 31, 2015));

    $oS->setDouble($sKeyPath . '/doubleTestA', 1/PHP_INT_MAX);

    $oS->setFloat($sKeyPath . '/floatTestA', 1/PHP_INT_MAX);

    $oS->setInteger($sKeyPath . '/intTestA', PHP_INT_MAX);

    $oS->setString($sKeyPath . '/stringTest', 'testing one, two and three');

    $oS->save();
    $oS->reload();

    $sString = '';

    $sStringTest = $oS->toString();

    echo __FUNCTION__ . ' ';

    if ($sString !== $sStringTest) {

        echo 'FAIL';
        var_dump($sStringTest);

    } else {

        echo 'PASS';

    } // if got errors or not

    echo NL;

    return [];

} // testToString


function testToYaml(ApplicationSettings $oS) {

    $oS->reset();

    $sKeyPath = 'toYaml/tests/M';

    $oS->setBool($sKeyPath . '/boolTestTrue', true);
    $oS->setBool($sKeyPath . '/boolTestFalse', false);

    $oS->setData($sKeyPath . '/dataTestA', chr(254) . chr(0) . chr(127) . chr(0));

    $oS->setDate($sKeyPath . '/dateTestA', gmmktime(12, 34, 11, 12, 31, 2015));

    $oS->setDouble($sKeyPath . '/doubleTestA', 1/PHP_INT_MAX);

    $oS->setFloat($sKeyPath . '/floatTestA', 1/PHP_INT_MAX);

    $oS->setInteger($sKeyPath . '/intTestA', PHP_INT_MAX);

    $oS->setString($sKeyPath . '/stringTest', 'testing one, two and three');

    $oS->save();
    $oS->reload();

    $sYaml = "TODOtoYaml:
    tests: { M: { boolTestTrue: true, boolTestFalse: false, dataTestA: '[[SssS_Plist_data]]/gB/AA==', dateTestA: '[[SssS_Plist_date]]2015-12-31T12:34:11Z', doubleTestA: 1.0842021724855E-19, floatTestA: 1.0842021724855E-19, intTestA: 9223372036854775807, stringTest: 'testing one, two and three' } }
";

    $sYamlTest = $oS->toYaml();

    echo __FUNCTION__ . ' ';

    if ($sYaml !== $sYamlTest) {

        echo 'FAIL';
        var_dump($sYamlTest);

    } else {

        echo 'PASS';

    } // if got errors or not

    echo NL;

    return [];

} // testToYaml

/* * * *\ testAppSettings.phps (CC) Luke JZ aka SwissalpS /* * * */
