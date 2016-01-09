<?php
/* * * *
 * * SwissalpS/PHOCOA/Localization/BlaHelperND.inc
 * * bind translatable values in yaml bindings
 * *
 * * discussion: if we agree not to request anything but translatables via valueForKey infrastructure, we could ommit asking parent for value. That would speed up.
 * *
 * * @version 20101112_184931 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\Localization;

// depricated, same result is achieved by using SssSLocalizationHelper with SssSBla as it's delegate
class BlaHelperND extends BlaHelper {

	function ___translate($sKey) {

		return Bla::bla($sKey, array('noDIV' => true));

	} // ___translate

	static function sharedTranslator() {

		static $o = null;
		if (!$o) $o = new BlaHelperND();

		return $o;

	} // sharedTranslator

} // BlaHelperND

/* * * *\
 * * SwissalpS/PHOCOA/Localization/BlaHelperND.inc (CC) Luke JZ aka SwissalpS /* * * */
