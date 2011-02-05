<?php
/* * * *
 * * AppPlistSettings.inc
 * *
 * * @version 20101012_225447 (CC) Luke JZ aka SwissalpS
 * * * */
class AppPlistSettings extends WFObject {

	static function sharedInstance() {

		static $oMe = null;

		if (!$oMe) {

			$oMe = new SssS_Plist(APP_ROOT . '/conf/AppSettings.plist');

		}

		return $oMe;

	} // sharedInstance

} // AppPlistSettings
/* * * *\ AppPlistSettings (CC) Luke JZ aka SwissalpS /* * * */
?>
