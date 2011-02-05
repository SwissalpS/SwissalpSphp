<?php
/* * * *
 * * WFBundle
 * *
 * * sketched up brainstorm on WFBundle for localisation
 * * maybe we can move it to WFWebApplication
 * * I'm still using SssSLocalization for now with SssSBla as delegate
 * * because SssSBla allows editing quasi inline
 * *
 * * @version 20101116_204557 (CC) Luke JZ aka SwissalpS
 * * * */
// feel free to use as WFLocalizedString($sKey, $sComment)
function WFLocalizedString($sKey) {
	return WFBundle::mainBundle()->localizedStringForKey($sKey);
} // WFLocalizedString

// feel free to use as WFLocalizedStringFromTable($sKey, $sTable, $sComment)
function WFLocalizedStringFromTable($sKey, $sTable) {
	return WFBundle::mainBundle()->localizedStringForKey($sKey, '', $sTable);
} // WFLocalizedStringFromTable

function WFLocalizedStringFromTableInBundle($sKey, $sTable, WFBundle $oBundle) {
	return $oBundle->localizedStringForKey($sKey, '', $sTable);
} // WFLocalizedStringFromTableInBundle

function WFLocalizedStringWithDefaultValue($sKey, $sTable = null, WFBundle $oBundle, $sValue) {
	return $oBundle->localizedStringForKey($sKey, $sValue, $sTable);
} // WFLocalizedStringWithDefaultValue



class WFBundle extends WFObject {

	function localizedStringForKey($sKey, $sValue = '', $sTable = null) {

		$sResult = SssSLocalization::sharedInstance()->translate($sKey, null, $sTable);

		if (($sResult == $sKey) && (!empty($sValue)))
			$sResult = $sValue;

		return $sResult;

 	} // localizedStringForKey

 	static function mainBundle() {

 		static $o = null; if (!$o) $o = new WFBundle;
 		return $o;

 	} // mainBundle

 	/* these are just placeholders for now */

 	// return WFBundle
 	static function bundleWithPath($sPath) {} // bundleWithPath
 	// return id/WFBundle
 	function initWithPath($sPath) {} // initWithPath

 	// return WFBundle
 	static function bundleForClass($sClass) {} // bundleForClass
 	// return WFBundle
 	static function bundleWithIdentifier($sIdentifier) {} // bundleWithIdentifier

 	// return WFArray
 	static function allBundles() {} // allBundles
 	// return WFArray
 	static function allFrameworks() {} // allFrameworks

 	// return boolean
 	function load() { return true; } // load
 	function isLoaded() { return true; } // isLoaded
 	function unload() { return false; } // unload
 	function preflightAndReturnError(&$oError) {} // preflightAndReturnError
 	function loadAndReturnError(&$oError) {} // loadAndReturnError

 	// return string
 	function bundlePath() {} // bundlePath
 	function resourcePath() {} // resourcePath
 	function executablePath() {} // executablePath
 	function pathForAuxiliaryExecutable($sExecutableName) {} // pathForAuxiliaryExecutable

 	function privateFrameworksPath() {} // privateFrameworksPath
 	function sharedFrameworksPath() {} // sharedFrameworksPath
 	function sharedSupportPath() {} // sharedSupportPath
 	function builtInPlugInsPath() {} // builtInPlugInsPath

 	function bundleIdentifier() {} // bundleIdentifier

 	function classNamed($sClassName) {} // classNamed
 	function principalClass() {} // principalClass

 	// the pathForResource functions are dealt with in WFWebApplication

 	// return SssS_Plist object?
 	function infoDictionary() {} // infoDictionary
 	function localizedInfoDictionary() {} // localizedInfoDictionary
 	function objectForInfoDictionaryKey($sKey) {} // objectForInfoDictionaryKey

 	function localizations() {} // localizations
 	function preferredLocalizations() {} // preferredLocalizations
 	function developmentLocalization() {} // developmentLocalization

 	static function preferredLocalizationsFromArray($aLocalizationsArray) {} // preferredLocalizationsFromArray
 	static function preferredLocalizationsFromArray_ForPreferences($aLocalizationsArray, $aPreferencesArray) {} // preferredLocalizationsFromArray_ForPreferences

} // WFBundle
/* * * *\ WFBundle (CC) Luke JZ aka SwissalpS /* * * */
?>
