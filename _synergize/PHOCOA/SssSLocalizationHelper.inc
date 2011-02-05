<?php
/* * * *
 * * SssSLocalizationHelper
 * * bind translatable values in yaml bindings
 * *
 * * discussion: if we agree not to request anything but translatables via valueForKey infrastructure, we could ommit asking parent for value. That would speed up.
 * *
 * * @version 20101116_231138 (CC) Luke JZ aka SwissalpS
 * * * */

class SssSLocalizationHelper extends WFObject {

	function __construct() {

        parent::__construct();

	} // __construct


	function valueForKey($sKey) {

		try {

			$mResult = parent::valueForKey($sKey);

		} catch (WFUndefinedKeyException $e) {

			$mResult = $this->___translate($sKey);

		} // catch

		return $mResult;

	} // valueForKey


	function ___translate($sKey) {

		return SssSLocalization::sharedInstance()->translate($sKey);

	} // ___translate


	function currentLangUC() { return strToUpper(SssSLocalization::currentLanguage()); } // currentLangUC


	function currentLangLC() { return strToLower(SssSLocalization::currentLanguage()); } // currentLangLC


	static function sharedTranslator() {

		static $o = null;

		if (!$o) $o = new SssSLocalizationHelper();

		return $o;

	} // sharedTranslator

} // SssSLocalizationHelper

/* * * *\ SssSLocalizationHelper (CC) Luke JZ aka SwissalpS /* * * */
?>
