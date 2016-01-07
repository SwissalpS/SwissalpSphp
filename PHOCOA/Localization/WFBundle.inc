<?php
/* * * *
 * * SwissalpS/PHOCOA/Localization/WFBundle.inc
 * *
 * * sketched up brainstorm on WFBundle for localisation
 * * maybe we can move it to WFWebApplication
 * * I'm still using SssSLocalization for now with SssSBla as delegate
 * * because SssSBla allows editing quasi inline
 * *
 * * @version 20160106_233952 (CC) Luke JZ aka SwissalpS + namespace
 * * @version 20101116_204557 (CC) Luke JZ aka SwissalpS
 * * * */
//TODO: actually Bundle belongs into SssS\PHOCOA\Application
namespace SwissalpS\PHOCOA\Localization;

use WFObject;

require_once(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'globalFunctions.inc');

class WFBundle extends WFObject {

	function localizedStringForKey($sKey, $sValue = '', $sTable = null) {

		$sResult = Localization::sharedInstance()->translate($sKey, null, $sTable);

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
