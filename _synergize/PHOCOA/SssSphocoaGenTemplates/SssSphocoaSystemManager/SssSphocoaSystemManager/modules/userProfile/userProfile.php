<?php

// Created by PHOCOA WFModelCodeGen on Wed, 21 Jul 2010 15:19:57 +0200
class module_userProfile extends WFModule {

	var $oCountries;

	function __construct($sInvocation) {

		parent::__construct($sInvocation);

		$this->oCountries = new SssSCountries();

	} // __construct

	function countries() { return $this->oCountries; } // countries

	function checkSecurity(WFAuthorizationInfo $oAuthInfo) {

		if (-1 == $oAuthInfo->userid()) {

			$oRootInvocation = WFRequestController::sharedRequestController()->rootModuleInvocation();

			$sURLnow = (is_object($oRootInvocation))
					? $oRootInvocation->invocationPath() : $_SERVER['REQUEST_URI'];

			$sURL = WFRequestController::WFURL('/login/promptLogin/'
					. WFWebApplication::serializeURL(
							WFRequestController::WFURL($sURLnow)));

			throw (new WFRedirectRequestException($sURL));

		} // if not loged in
// TODO: redirect to detail page if request is to edit a page not belonging to currently logged in user and user doesn't have higher authority
		if (strToLower($oAuthInfo->userid()) == strToLower(substr(strrchr($_SERVER['REQUEST_URI'], '/'), 1)))
			return WFAuthorizationManager::ALLOW;

		if (!$oAuthInfo->isSuperUser())
			return WFAuthorizationManager::DENY;
			//throw(new WFException("You don't have permission to access admin."));

		return WFAuthorizationManager::ALLOW;

	} // checkSecurity


	function defaultPage() { return 'edit'; } // defaultPage

	// this function should throw an exception if the user is not permitted to edit (add/edit/delete) in the current context
	function verifyEditingPermission($oPage) {
		// example
		$oAuthInfo = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();

		if ($oAuthInfo->isSuperUser()) return;

		if ($oAuthInfo->userid() != $oPage->sharedOutlet('Users')->selection()->getHandle())
			throw( new Exception(SssSBla::bla('UserProfileYouMayNotEdit')));

	} // verifyEditingPermission

} // module_userProfile


class module_userProfile_edit {

	function parameterList() { return array('handle'); } //


	function parametersDidLoad($oPage, $aParams) {

		// populate multiSelectPPerms
		$a = PermpresetsPeer::doSelect(new Criteria());
		$oPage->sharedOutlet('Permpresets')->setContent($a);

		if ($oPage->sharedOutlet('Users')->selection() === NULL) {

			if (!isset($aParams['handle']))
				$aParams['handle'] = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo()->userid();

			// test if handle already exists
			$oUser = UsersPeer::retrieveByPK($aParams['handle']);

			if (!$oUser)
				throw (new WFRedirectRequestException('/admin/users/edit/' . $aParams['handle']));

			$oPage->sharedOutlet('Users')->setContent(array($oUser));
			$oPage->module()->verifyEditingPermission($oPage);

		} // if no entry selected

	} // parametersDidLoad


	function save($oPage) {

		$oPage->module()->verifyEditingPermission($oPage);

		try {

			$oUser = $oPage->sharedOutlet('Users')->selection();

// looks like fields set by js are not updated correctly
//throw (new WFException($oPage->outlet('country')->value() . ' ' . $oUser->getCountry() . ' ' . $oPage->outlet('region')->value() . ' ' . $oUser->getRegion()));
			// need to update value from selector
			$oUser->setCountry($oPage->outlet('country')->value());

			// if new password request
			$sPass1 = trim($oPage->outlet('pass1')->value());
			$sPass2 = trim($oPage->outlet('pass2')->value());

			if (!empty($sPass1) || !empty($sPass2)) {

				if ($sPass1 === $sPass2) {
					$oUser->setPasshash(MyAuthorizationDelegate::passHashForPass($sPass1));

					MyAuthorizationDelegate::mailUserNewPass($oUser, $sPass1);

				} else {

					throw (new WFException(SssSBla::bla('AdminUsersBothPasswordsMustMatch') . ' 1:' . $sPass1 . ':2:' . $sPass2 . ':'));

				} // if passwords match or not

			} // if any password has been entered

			$oUser->save();
			$oPage->outlet('statusMessage')->setValue(SssSBla::bla('AdminUsersSavedSuccess'));

		} catch (Exception $e) {

			$oPage->addError(new WFError($e->getMessage()));

		} //

	} // save


	function deleteObj($oPage) {
		$oPage->module()->verifyEditingPermission($oPage);
		$oPage->module()->setupResponsePage('confirmDelete');
	} // deleteObj


	function setupSkin($oPage, $aParams, $oSkin) {

		$aND = array('noDIV' => true);
		$oUser = $oPage->sharedOutlet('Users')->selection();

		if ($oUser->isNew()) {
			$sTitle = SssSBla::bla('SharedNew', $aND) . ' ' . SssSBla::bla('UsersSing', $aND);

		} else {
			$sTitle = SssSBla::bla('SharedEdit', $aND) . ' ' . SssSBla::bla('UsersSing', $aND) . ':' . $oUser->valueForKeyPath('handle');

		} // setupSkin

		$oSkin->setTitle(SssSBla::cleanForTitle($sTitle));

	} //

} // module_userProfile_edit



class module_userProfile_confirmDelete
{
	function parameterList()
	{
		return array('handle');
	}
	function parametersDidLoad($oPage, $aParams)
	{
		// if we're a redirected action, then the Users object is already loaded. If there is no object loaded, try to load it from the object ID passed in the params.
		if ($oPage->sharedOutlet('Users')->selection() === NULL)
		{
			$objectToDelete = UsersPeer::retrieveByPK($aParams['handle']);
			if (!$objectToDelete) throw( new Exception("Could not load Users object to delete.") );
			$oPage->sharedOutlet('Users')->setContent(array($objectToDelete));
		}
		if ($oPage->sharedOutlet('Users')->selection() === NULL) throw( new Exception("Could not load Users object to delete.") );
	}
	function cancel($oPage)
	{
		$oPage->module()->setupResponsePage('edit');
	}
	function deleteObj($oPage)
	{
		$oPage->module()->verifyEditingPermission($oPage);
		$myObj = $oPage->sharedOutlet('Users')->selection();
		$myObj->delete();
		$oPage->sharedOutlet('Users')->removeObject($myObj);
		$oPage->module()->setupResponsePage('deleteSuccess');
	}
}
