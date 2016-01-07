<?php
/* * * *
 * * SwissalpS/PHOCOA/Localization/globalFunctions.inc
 * *
 * * @version 20160107_000113 (CC) Luke JZ aka SwissalpS
 * * * */

use SwissalpS\PHOCOA\Localization\WFBundle;

if (!function_exists('WFLocalizedString')) {

    // feel free to use as WFLocalizedString($sKey, $sComment)
    function WFLocalizedString($sKey) {

        return WFBundle::mainBundle()->localizedStringForKey($sKey);

    } // WFLocalizedString

} // if function does not yet exist


if (!function_exists('WFLocalizedStringFromTable')) {

    // feel free to use as WFLocalizedStringFromTable($sKey, $sTable, $sComment)
    function WFLocalizedStringFromTable($sKey, $sTable) {

        return WFBundle::mainBundle()->localizedStringForKey($sKey, '', $sTable);

    } // WFLocalizedStringFromTable

} // if function does not yet exist


if (!function_exists('WFLocalizedStringFromTableInBundle')) {

    function WFLocalizedStringFromTableInBundle($sKey, $sTable, WFBundle $oBundle) {

        return $oBundle->localizedStringForKey($sKey, '', $sTable);

    } // WFLocalizedStringFromTableInBundle

} // if function does not yet exist


if (!function_exists('WFLocalizedStringWithDefaultValue')) {

    function WFLocalizedStringWithDefaultValue($sKey, $sTable = null, WFBundle $oBundle, $sValue) {

        return $oBundle->localizedStringForKey($sKey, $sValue, $sTable);

    } // WFLocalizedStringWithDefaultValue

} // if function does not yet exist

/* * * *\ SwissalpS/PHOCOA/Localization/globalFunctions.inc (CC) Luke JZ aka SwissalpS /* * * */
