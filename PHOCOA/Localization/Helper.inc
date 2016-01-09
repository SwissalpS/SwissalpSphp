<?php
/* * * *
 * * SwissalpS/PHOCOA/Localization/Helper.inc
 * * bind translatable values in yaml bindings
 * *
 * * discussion: if we agree not to request anything but translatables via valueForKey infrastructure, we could ommit asking parent for value. That would speed up.
 * *
 * * @version 20101116_231138 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\Localization;

use WFObject;
use WFUndefinedKeyException;

class Helper extends WFObject {

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

		return Localization::sharedInstance()->translate($sKey);

	} // ___translate


	function currentLangUC() {

		return strToUpper(Localization::currentLanguage());

	} // currentLangUC


	function currentLangLC() {

		return strToLower(Localization::currentLanguage());

	} // currentLangLC


	static function sharedTranslator() {

		static $o = null;

		if (!$o) $o = new Helper();

		return $o;

	} // sharedTranslator

} // Helper

/* * * *\ SwissalpS/PHOCOA/Localization/Helper.inc (CC) Luke JZ aka SwissalpS /* * * */
