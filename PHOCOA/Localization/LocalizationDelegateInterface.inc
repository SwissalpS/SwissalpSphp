<?php
/* * * *
 * * SwissalpS/PHOCOA/Localization/LocalizationDelegateInterface.inc
 * *
 * * @version 20160107_010413 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\Localization;

interface LocalizationDelegateInterface {

    // initialize tables etc.
    public function initialize();

    // return array eg array('en', 'de', 'fr', 'it', 'rm')
    public static function languageOrder();

    // return array for html select labels
    // currently optional
    //public function selectLangLabels();

    // return array for html select values
    // currently optional
    //public function selectLangValues();

    // optional
    //public function set($sValue, $sKey, $sLang, $sTable = '__main');

    // return string with translated sKey or unchanged sKey
    public function translate($sKey, $sLang = null, $sTable = '__main');

} // LocalizationDelegateInterface

/* * * *\ SwissalpS/PHOCOA/Localization/LocalizationDelegateInterface.inc (CC) Luke JZ aka SwissalpS /* * * */
