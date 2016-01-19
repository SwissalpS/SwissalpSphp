<?php
/* * * *
 * * SwissalpS/PHOCOA/Settings/ApplicationSettingsInterface.inc
 * * interface for application settings
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

interface ApplicationSettingsInterface {

    // clear cache, if has. Return true on success, null on failure.
    public function flushCache();

    // get mixed value at key-path. $mDefault is what should be returned if there is nothing at key-path.
    public function get($sKeyPath, $mDefault = null);

    // get boolean value at key-path. $bDefault is what should be returned if there is nothing at key-path.
    public function getBool($sKeyPath, $bDefault = false);

    // get WFDateTime at key-path. $oDefault is what should be returned if there is nothing at key-path.
    public function getDate($sKeyPath, $oDefault = 0);

    // get data as char-string at key-path. $cDefault is what should be returned if there is nothing at key-path.
    public function getData($sKeyPath, $cDefault = '');

    // get double value at key-path. $dDefault is what should be returned if there is nothing at key-path.
    public function getDouble($sKeyPath, $dDefault = 0.0);

    // get float value at key-path. $fDefault is what should be returned if there is nothing at key-path.
    public function getFloat($sKeyPath, $fDefault = 0.0);

    // get integer value at key-path. $iDefault is what should be returned if there is nothing at key-path.
    public function getInteger($sKeyPath, $iDefault = 0);

    // get mixed value at key-path. If there is nothing at key-path, then set value to $mDefaultValue and return $mDefaultValue. If $mDefaultValue is null, then only get, do not set inexistant path to null.
    public function getOrSet($sKeyPath, $mDefaultValue = null);

    // get string value at key-path. $sDefault is what should be returned if there is nothing at key-path.
    public function getString($sKeyPath, $sDefault = '');

    // reload from persistant storage, not cache. Flush and rebuild cache, if has feature. Return true on success, null on failure.
    public function reload();

    // reset to default values. Clear persistant storage and cache. Return true on success, null on failure.
    public function reset();

    // save the settings to persistant storage. Return a positive number greater than 0 to signal positive.
    public function save();

    // set mixed value at key-path. $mValue may be of any type, as long as your class can deal with it. Arrays are typical here.
    public function set($sKeyPath, $mValue);

    // set boolean value at key-path. TRUE == TRUE, v > 0, '1', 'y', 'Y', 'yes', 'YES', 'true', 'TRUE', 'tRuE', ..., everything else is FALSE
    public function setBool($sKeyPath, $bValue);

    // set Date at key-path. Only the date value needs to be stored, so a WFDateTime object can be constructed and returned.
    public function setDate($sKeyPath, $oValue);

    // set Data at key-path. $cValue is a char-string.
    public function setData($sKeyPath, $cValue);

    // set double value at key-path.
    public function setDouble($sKeyPath, $dValue);

    // set float value at key-path.
    public function setFloat($sKeyPath, $fValue);

    // set integer value at key-path.
    public function setInteger($sKeyPath, $iValue);

    // set string value at key-path.
    public function setString($sKeyPath, $sValue);

    // return shared settings instance aka singelton.
    public static function sharedInstance();

    // return a WFArray representation of all settings.
    // Data objects convert to base64 encoded strings wrapped as such: [[CDATA]]data_encoded
    // Date objects are returned as WFDateTime objects.
    // Arrays are returned as WFArray objects.
    // with bWF = false
    // Data objects convert to base64 encoded strings wrapped as such: [[CDATA]]data_encoded
    // Date objects convert to unix time-stamp and wrapped as such: [[IDATE]]time_stamp
    // Arrays are returned as standard arrays.
    public function toArray($bWF = true);

    // TODO: check Json standards for types
    // return a JSON-string representation of all settings.
    // Boolean entries convert to '0' or '1'?
    // Data objects convert to base64 encoded strings wrapped as such: [[CDATA]data_encoded]
    // Date objects convert to JSON time-stamp?
    public function toJson();

    // return a plist-string representation of all settings.
    // Data objects convert to base64 encoded strings wrapped as such: [[CDATA]]data_encoded
    // Date objects convert to 'Y-m-d\TH:i:s\Z'.
    public function toPlist();

    // return a string representation of all settings.
    // Boolean entries convert to 'NO' or 'YES'.
    // Data objects convert to base64 encoded strings wrapped as such: [[CDATA]]data_encoded
    // DateTime objects convert to 'Y-m-d\TH:i:s\Z'.
    // This format is mainly for human readers, debugging etc. You are free to chose the format.
    public function toString();

    // same as toArray(true)
    public function toWFArray();

    // TODO: check yaml standards for types
    // return a YAML-string representation of all settings.
    // Boolean entries convert to '0' or '1'?
    // Data objects convert to base64 encoded strings wrapped as such: [[CDATA]data_encoded]
    // Date objects convert to unix time-stamp? // TODO: check yaml standard for all types
    public function toYaml();

} // ApplicationSettingsInterface

/* * * *\ SwissalpS/PHOCOA/Settings/ApplicationSettingsInterface.inc (CC) Luke JZ aka SwissalpS /* * * */
