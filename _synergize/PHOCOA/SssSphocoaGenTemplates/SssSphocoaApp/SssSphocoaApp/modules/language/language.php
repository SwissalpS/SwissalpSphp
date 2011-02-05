<?php
/* vim: set expandtab tabstop=4 shiftwidth=4: */

class module_language extends WFModule {
	function sharedInstancesDidLoad() {}

	function defaultPage() { return 'switchto'; }

} // module_language


class module_language_switchto {

	public function parameterList() {

		return array('lang', 'continueURL');

	} // parameterList


	function parametersDidLoad($oPage, $aParams) {

		if (isset($aParams['lang'])) {
			$oAuth = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();
			if (method_exists($oAuth, 'languageOrder')) {
				$sLang = strtolower($aParams['lang']);

				if (in_array($sLang, $oAuth->languageOrder())) {

						$oAuth->switchToLanguage($sLang);
						WFAuthorizationManager::sharedAuthorizationManager()->valueForKey('authorizationDelegate')->save($oAuth);

				} // if valid lang

			} // if got method languageOrder

		} // if got lang param

		if (isset($aParams['continueURL'])) {
			$this->gotoURL(WFWebApplication::unserializeURL($aParams['continueURL']));
		} // if got return to url

		// else??? ever happen?

	} // parametersDidLoad


	function gotoURL($url) {
		// use an internal redirect for ajax requests (otherwise might not work), but use 302 for normal logins. This ensures that the URL in the address bar is correct.
		if (WFRequestController::isAjax())
			throw(new WFRequestController_InternalRedirectException($url));

		else
			throw(new WFRequestController_RedirectException($url));

	} // gotoURL

} // module_language_switchto

?>
