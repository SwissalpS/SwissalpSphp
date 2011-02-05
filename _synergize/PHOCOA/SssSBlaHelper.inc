<?php
/* * * *
 * * SssSBlaHelper
 * * bind translatable values in yaml bindings
 * *
 * * discussion: if we agree not to request anything but translatables via valueForKey infrastructure, we could ommit asking parent for value. That would speed up.
 * *
 * * @version 20101112_184931 (CC) Luke JZ aka SwissalpS
 * * * */

class SssSBlaHelper extends SssSLocalizationHelper {

	function __construct() {

        parent::__construct();

	} // __construct

	function ___translate($sKey) {

		return SssSBla::bla($sKey);

	} // ___translate

	static function sharedTranslator() {

		static $o = null;
		if (!$o) $o = new SssSBlaHelper();

		return $o;

	} // sharedTranslator

} // SssSBlaHelper


// depricated, same result is achieved by using SssSLocalizationHelper with SssSBla as it's delegate
class SssSBlaHelperND extends SssSBlaHelper {

	function ___translate($sKey) {

		return SssSBla::bla($sKey, array('noDIV' => true));

	} // ___translate

	static function sharedTranslator() {

		static $o = null;
		if (!$o) $o = new SssSBlaHelperND();

		return $o;

	} // sharedTranslator

} // SssSBlaHelperND

class SssSBlaHelperNDhtmlEntities extends SssSBlaHelperND {

	function ___translate($sKey) {

		return htmlentities(parent::___translate($sKey));

	} // ___translate

	static function sharedTranslator() {

		static $o = null;
		if (!$o) $o = new SssSBlaHelperNDhtmlEntities();

		return $o;

	} // sharedTranslator

} // SssSBlaHelperNDhtmlEntities

/* * * *\ SssSBlaHelper (CC) Luke JZ aka SwissalpS /* * * */
?>
