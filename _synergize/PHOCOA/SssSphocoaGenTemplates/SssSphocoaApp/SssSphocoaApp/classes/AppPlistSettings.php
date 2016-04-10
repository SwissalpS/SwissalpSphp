<?php
/* * * *
 * * AppPlistSettings.inc
 * *
 * * @version 20160410_112532 (CC) Luke JZ aka SwissalpS
 * * @version 20101012_225447 (CC) Luke JZ aka SwissalpS
 * * * */
use SwissalpS\XML\Plist;

class AppPlistSettings extends WFObject {

	static function sharedInstance() {

		static $oMe = null;

		if (!$oMe) {

			$oMe = new Plist(APP_ROOT . DIR_SEP . 'conf' . DIR_SEP
							 . 'AppSettings.plist');

		} // if first call

		return $oMe;

	} // sharedInstance

} // AppPlistSettings
/* * * *\ AppPlistSettings (CC) Luke JZ aka SwissalpS /* * * */
