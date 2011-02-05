<?php
class module_admin extends WFModule {

	function checkSecurity(WFAuthorizationInfo $oAuthInfo) {

		if (-1 == $oAuthInfo->userid()) {
			$oRootInvocation = WFRequestController::sharedRequestController()->rootModuleInvocation();
			$sURL = 'login/promptLogin/' . ((!$oRootInvocation) ? '' : WFWebApplication::serializeURL(WFRequestController::WFURL($oRootInvocation->invocationPath())));

			$sURL = WFRequestController::WFURL($sURL);

			throw (new WFRedirectRequestException($sURL));

		} // if not loged in

    	if (!$oAuthInfo->isSuperUser())
    		return WFAuthorizationManager::DENY;
    		//throw(new WFException("You don't have permission to access admin."));

    	return WFAuthorizationManager::ALLOW;

	} // checkSecurity



    function defaultPage() {

    	return 'users/list';

    } // defaultPage

} // module_admin
?>
