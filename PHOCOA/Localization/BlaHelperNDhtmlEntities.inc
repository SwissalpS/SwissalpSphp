<?php
/* * * *
 * * SwissalpS/PHOCOA/Localization/BlaHelperNDhtmlEntities.inc
 * * bind translatable values in yaml bindings
 * *
 * * discussion: if we agree not to request anything but translatables via valueForKey infrastructure, we could ommit asking parent for value. That would speed up.
 * *
 * * @version 20101112_184931 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\Localization;

class BlaHelperNDhtmlEntities extends BlaHelperND {

	function ___translate($sKey) {

		return htmlentities(parent::___translate($sKey));

	} // ___translate

	static function sharedTranslator() {

		static $o = null;
		if (!$o) $o = new BlaHelperNDhtmlEntities();

		return $o;

	} // sharedTranslator

} // BlaHelperNDhtmlEntities

/* * * *\
 * * SwissalpS/PHOCOA/Localization/BlaHelperNDhtmlEntities.inc (CC) Luke JZ aka SwissalpS /* * * */
