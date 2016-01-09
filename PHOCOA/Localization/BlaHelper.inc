<?php
/* * * *
 * * SwissalpS/PHOCOA/Localization/BlaHelper.inc
 * * bind translatable values in yaml bindings
 * *
 * * discussion: if we agree not to request anything but translatables via valueForKey infrastructure, we could ommit asking parent for value. That would speed up.
 * *
 * * @version 20101112_184931 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\Localization;

class BlaHelper extends Helper {

	function __construct() {

        parent::__construct();

	} // __construct

	function ___translate($sKey) {

		return Bla::bla($sKey);

	} // ___translate

	static function sharedTranslator() {

		static $o = null;
		if (!$o) $o = new BlaHelper();

		return $o;

	} // sharedTranslator

} // BlaHelper

/* * * *\
 * * SwissalpS/PHOCOA/Localization/BlaHelper.inc (CC) Luke JZ aka SwissalpS /* * * */
