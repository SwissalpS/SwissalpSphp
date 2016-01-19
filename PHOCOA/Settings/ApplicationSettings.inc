<?php
/* * * *
 * * SwissalpS/PHOCOA/Settings/ApplicationSettings.inc
 * * application settings
 * *
 * * key-paths use '/' as seperator. i.e '/some/setting/keypath'
 * * '' and '/' both mean the same -> root
 * * also '/someSetting' and 'someSetting' are equal
 * * numerically indexed arrays can be accessed as such:
 * * 'arraySetting/4' -> gets the 5th item from array 'arraySetting'
 * *
 * * Only store small pieces of information. Although data is supported,
 * * prefer to store a file path than the contents of the file.
 * *
 * * @version 20160116_233228 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\Settings;

use SwissalpS\XML\Plist;
use WFArray;
use WFDateTime;
use WFJSON;
use WFYaml;

class ApplicationSettings implements ApplicationSettingsInterface {

    protected $oP;
    protected $sPathPlist;

    public function __construct() {

        $this->sPathPlist = APP_ROOT . DIR_SEP . 'conf' . DIR_SEP . 'ApplicationSettings.plist';

        $this->oP = new Plist($this->sPathPlist, 'utf-8', false, true, true);

    } // __construct

    // clear cache, if has. Return true on success, null on failure.
    public function flushCache() {

        return true;

    } // flushCache


    // get mixed value at key-path. $mDefault is what should be returned if there is nothing at key-path.
    public function get($sKeyPath, $mDefault = null) {

        return $this->oP->get($sKeyPath, $mDefault);

    } // get


    // get boolean value at key-path. $bDefault is what should be returned if there is nothing at key-path.
    public function getBool($sKeyPath, $bDefault = false) {

        return (bool)$this->oP->getBool($sKeyPath, $bDefault);

    } // getBool


    // get WFDateTime at key-path. $oDefault is what should be returned if there is nothing at key-path.
    public function getDate($sKeyPath, $oDefault = 0) {

        $sDate = $this->oP->getDateForKeyPath($sKeyPath, 'U');

        if ('' == $sDate) {

            return $oDefault;

        } // if no date for key-path

        $oDate = new WFDateTime();
        $oDate->setTimestamp($sDate);

        return $oDate;

    } // getDate


    // get data as char-string at key-path. $cDefault is what should be returned if there is nothing at key-path.
    public function getData($sKeyPath, $cDefault = '') {

        $mData = $this->oP->getData($sKeyPath, $cDefault);

        if ($cDefault === $mData) {

            return $cDefault;

        } // if default returned

        return base64_decode($mData);

    } // getData


    // get double value at key-path. $dDefault is what should be returned if there is nothing at key-path.
    public function getDouble($sKeyPath, $dDefault = 0.0) {

        return (double)$this->oP->getDouble($sKeyPath, $dDefault);

    } // getDouble


    // get float value at key-path. $fDefault is what should be returned if there is nothing at key-path.
    public function getFloat($sKeyPath, $fDefault = 0.0) {

        return (float)$this->oP->getFloat($sKeyPath, $fDefault);

    } // getFloat


    // get integer value at key-path. $iDefault is what should be returned if there is nothing at key-path.
    public function getInteger($sKeyPath, $iDefault = 0) {

        return (int)$this->oP->getInteger($sKeyPath, $iDefault);

    } // getInteger


    // get mixed value at key-path. If there is nothing at key-path, then set value to $mDefaultValue and return $mDefaultValue. If $mDefaultValue is null, then only get, do not set inexistant path to null.
    public function getOrSet($sKeyPath, $mDefaultValue = NULL) {

        return $this->oP->getOrSet($sKeyPath, $mDefaultValue);

    } // getOrSet


    // get string value at key-path. $sDefault is what should be returned if there is nothing at key-path.
    public function getString($sKeyPath, $sDefault = '') {

        return (string)$this->oP->getString($sKeyPath, $sDefault);

    } // getString


    // reload from persistant storage, not cache. Flush and rebuild cache, if has feature. Return true on success, null on failure.
    public function reload() {

        $this->flushCache();

        unset($this->oP);

        $this->oP = new Plist($this->sPathPlist, 'utf-8', false, true, true);

        return true;

    } // reload


    // reset to default values. Clear persistant storage and cache. Return true on success, null on failure.
    public function reset() {

        $this->flushCache();

        unset($this->oP);

        @unlink($this->sPathPlist);

        $this->oP = new Plist($this->sPathPlist, 'utf-8', false, true, true);

        return true;

    } // reset


    // save the settings to persistant storage. Return a positive number greater than 0 to signal positive.
    public function save() {

        return $this->oP->save();

    } // save


    // set mixed value at key-path. $mValue may be of any type, as long as your class can deal with it. Arrays are typical here.
    public function set($sKeyPath, $mValue) {

        if (is_object($mValue)) {
            if (method_exists($mValue, '__toPlistArray')) {

                $mValue = $mValue->__toPlistArray();

            } else if (method_exists($mValue, '__toDataBase64')) {

                $mValue = $mValue->__toDataBase64();
                return $this->setData($sKeyPath, $mValue);

            } else if (method_exists($mValue, '__toString')) {

                $mValue = $mValue->__toString();
                return $this->setString($sKeyPath, $mValue);

            } else {

                $mValue = Plist::serializeAndEncodeObject($mValue);
                return $this->setData($sKeyPath, $mValue);

            } // if responds to methods

        } // if object

        return $this->oP->set($sKeyPath, $mValue);

    } // set


    // set boolean value at key-path. TRUE == TRUE, v > 0, '1', 'y', 'Y', 'yes', 'YES', 'true', 'TRUE', 'tRuE', ..., everything else is FALSE
    public function setBool($sKeyPath, $bValue) {

        if (!is_bool($bValue)) {

            if (is_null($bValue)) {

                $bValue = false;

            } elseif (is_numeric($bValue)) {

                $bValue = (0 < $bValue);

            } elseif (is_string($bValue)) {

                $sValue = strtolower($bValue);

                $bValue = in_array($sValue, array('1', 'y', 'yes', 'true'));

            } else {

                $bValue = false;

            } // if is null, string or numeric

        } // if isn't a boolean given

        return $this->oP->setBool($sKeyPath, $bValue);

    } // setBool


    // set Date at key-path. Only the date value needs to be stored, so a WFDateTime object can be constructed and returned.
    public function setDate($sKeyPath, $oValue) {

        return $this->oP->setDate($sKeyPath, $oValue);

    } // setDate


    // set Data at key-path. $cValue is a char-string.
    public function setData($sKeyPath, $cValue) {

        if (is_array($cValue) || is_object($cValue) || is_null($cValue)) {

            $cValue = '';

        } // if array, object or null given

        return $this->oP->setData($sKeyPath, base64_encode($cValue));

    } // setData


    // set double value at key-path.
    public function setDouble($sKeyPath, $dValue) {

        return $this->oP->setDouble($sKeyPath, $dValue);

    } // setDouble


    // set float value at key-path.
    public function setFloat($sKeyPath, $fValue) {

        return $this->oP->setFloat($sKeyPath, $fValue);

    } // setFloat


    // set integer value at key-path.
    public function setInteger($sKeyPath, $iValue) {

        return $this->oP->setInteger($sKeyPath, $iValue);

    } // setInteger


    // set string value at key-path.
    public function setString($sKeyPath, $sValue) {

        return $this->oP->setString($sKeyPath, $sValue);

    } // setString


    // return shared settings instance aka singelton.
    public static function sharedInstance() {

        static $o = null;
        if (!$o) {

            $o = new self();

        } // if first call

        return $o;

    } // sharedInstance


    // return a WFArray representation of all settings.
    // Data objects convert to base64 encoded strings wrapped as such: [[CDATA]]data_encoded
    // Date objects are returned as WFDateTime objects.
    // Arrays are returned as WFArray objects.
    // with bWF = false
    // Data objects convert to base64 encoded strings wrapped as such: [[CDATA]]data_encoded
    // Date objects convert to unix time-stamp and wrapped as such: [[IDATE]]time_stamp
    // Arrays are returned as standard arrays.
    public function toArray($bWF = true) {

        $aPlist = $this->oP->__toPlistArray();

        $this->toArrayRecursion($aPlist, $bWF);

        if ($bWF) {

            return new WFArray($aPlist);

        } // if use WF-objects where possible

        return $aPlist;

    } // toArray


    protected function toArrayRecursion(&$a, $bWF = false) {

        $iTotal = count($a);
        $aKeys = array_keys($a);
//var_dump($iTotal, $a);
        for ($iCount = 0; $iCount < $iTotal; $iCount++) {
//var_dump($iCount);
            $sKey = $aKeys[$iCount];
            $mItem = $a[$sKey];
            $sType = gettype($mItem);

            if (is_string($mItem)) {

                // check for data
                if (0 === strpos($mItem, '[[SssS_Plist_data]]')) {

                    $mItem = rtrim($mItem, '=');

                    $a[$sKey] = str_replace('[[SssS_Plist_data]]', '[[CDATA]]', $mItem);

                } elseif (0 === strpos($mItem, '[[SssS_Plist_date]]')) {

                    $mItem = str_replace('[[SssS_Plist_date]]', '', $mItem);

                    $mItem = $this->oP->formatDate($mItem, 'U');

                    if ($bWF) {

                        $oDate = new WFDateTime();
                        $oDate->setTimestamp($mItem);
                        $a[$sKey] = $oDate;

                    } else {

                        $a[$sKey] = '[[IDATE]]' . $mItem;

                    } // if return WF-objects where possible

                } // if is data or date

            } elseif (is_array($mItem)) {

                // recurse
                $this->toArrayRecursion($mItem, $bWF);
                if ($bWF) {

                    $a[$sKey] = new WFArray($mItem);

                } else {

                    $a[$sKey] = $mItem;

                } // if use WF-objects where possible

            } // if string or array

        } // loop all elements in array

    } // toArrayRecursion


    // return a JSON-string representation of all settings.
    // Boolean entries convert to '0' or '1'?
    // Data objects convert to base64 encoded strings wrapped as such: [[CDATA]data_encoded]
    // Date objects convert to JSON time-stamp.
    public function toJson() {

        return WFJSON::encode($this->oP->__toPlistArray());

    } // toJson


    // return a plist-string representation of all settings.
    // Data objects convert to base64 encoded strings wrapped as such: [[CDATA]data_encoded]
    // Date objects convert to 'Y-m-d\TH:i:s\Z'.
    public function toPlist() {

        return $this->oP->XML();

    } // toPlist


    // return a string representation of all settings.
    // Boolean entries convert to 'NO' or 'YES'.
    // Data objects convert to base64 encoded strings wrapped as such: [[CDATA]]data_encoded
    // DateTime objects convert to 'Y-m-d\TH:i:s\Z'.
    // This format is mainly for human readers, debugging etc. You are free to chose the format.
    public function toString() {

        return $this->oP->outputPlist();

    } // toString


    // same as toArray(true)
    public function toWFArray() {

        return $this->toArray(true);

    } // toWFArray


    // return a YAML-string representation of all settings.
    // Boolean entries convert to '0' or '1'?
    // Data objects convert to base64 encoded strings wrapped as such: [[CDATA]data_encoded]
    // Date objects convert to unix time-stamp? // TODO: check yaml standard for all types
    public function toYaml() {

        return  WFYaml::dump($this->oP->__toPlistArray());

    } // toYaml

} // ApplicationSettings

/* * * *\ SwissalpS/PHOCOA/Settings/ApplicationSettings.inc (CC) Luke JZ aka SwissalpS /* * * */
