<?php
class module_panel extends WFModule {

	function __construct($sInvocation) {

		parent::__construct($sInvocation);

	} // __construct

	function checkSecurity(WFAuthorizationInfo $oAuthInfo) {

		if (!$oAuthInfo->isSuperUser())
			return WFAuthorizationManager::DENY;
			//throw(new WFException("You don't have permission to access admin."));

		return WFAuthorizationManager::ALLOW;

	} // checkSecurity

	function authInfo() {

		return WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();

	} // authInfo

	function defaultPage() { return 'show'; } // defaultPage

	// this function should throw an exception if the user is not permitted to edit (add/edit/delete) in the current context
	function verifyEditingPermission($oPage) {

		if (!$this->authInfo()->isSuperUser())
			$oPage->module()->setupResponsePage('hide');

	} // verifyEditingPermission

	function currentURLserialized() {

		$oRootInvocation = WFRequestController::sharedRequestController()->rootModuleInvocation();
		$sUrlNow = (is_object($oRootInvocation) ? $oRootInvocation->invocationPath() : '/notes/listall');
		$sCurrentSerialized = WFWebApplication::serializeURL(WFRequestController::WFURL($sUrlNow));

		return $sCurrentSerialized;

	} // currentURLserialized

} // module_panel

class module_panel_show {

	function parameterList() {

		return array('switch', 'continueURL');

	} // parameterList

	function parametersDidLoad($oPage, $aParams) {

		$oPage->module()->verifyEditingPermission($oPage);

		if (isset($aParams['switch'])) {

			 WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo()->setShowBlaEditLinks(0 < intval($aParams['switch']));

		} // if got switch

		if (isset($aParams['continueURL'])) {
			$this->gotoURL(WFWebApplication::unserializeURL($aParams['continueURL']));

		} // if got return to url

		// else we need to setup the links


	} // parametersDidLoad

	function gotoURL($url) {
		// use an internal redirect for ajax requests (otherwise might not work), but use 302 for normal logins. This ensures that the URL in the address bar is correct.
		if (WFRequestController::isAjax())
			throw(new WFRequestController_InternalRedirectException($url));

		else
			throw(new WFRequestController_RedirectException($url));

	} // gotoURL

} // module_panel_show
