<?php

// Created by PHOCOA WFModelCodeGen on Wed, 21 Jul 2010 15:19:57 +0200
class module_users extends WFModule {

	function checkSecurity(WFAuthorizationInfo $oAuthInfo) {

		if (-1 == $oAuthInfo->userid()) {

			$oRootInvocation = WFRequestController::sharedRequestController()->rootModuleInvocation();

			$sURLnow = (is_object($oRootInvocation))
					? $oRootInvocation->invocationPath() : 'admin/users';

			$sURL = WFRequestController::WFURL('login/promptLogin/'
					. WFWebApplication::serializeURL(
							WFRequestController::WFURL($sURLnow)));

			throw (new WFRedirectRequestException($sURL));

		} // if not loged in

		if (!$oAuthInfo->isSuperUser())
			return WFAuthorizationManager::DENY;
			//throw(new WFException("You don't have permission to access admin."));

		return WFAuthorizationManager::ALLOW;

	} // checkSecurity


	function defaultPage() { return 'list'; }

	// this function should throw an exception if the user is not permitted to edit (add/edit/delete) in the current context
	function verifyEditingPermission($oPage)
	{
		// example
		// $authInfo = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();
		// if ($authInfo->userid() != $oPage->sharedOutlet('Users')->selection()->getUserId()) throw( new Exception("You don't have permission to edit Users.") );
	}
}

class module_users_list
{
	function parameterList()
	{
		return array('paginatorState');
	}

	function parametersDidLoad($oPage, $aParams)
	{
		$oPage->sharedOutlet('paginator')->readPaginatorStateFromParams($aParams);
	}

	function noAction($oPage, $aParams)
	{
		$this->search($oPage, $aParams);
	}

	function search($oPage, $aParams)
	{
		$query = $oPage->outlet('query')->value();
		$c = new Criteria();
		if (!empty($query))
		{
			$querySubStr = '%' . str_replace(' ', '%', trim($query)) . '%';

			$c->add(UsersPeer::HANDLE, $querySubStr, Criteria::LIKE);
		}

		$oPage->sharedOutlet('paginator')->setDataDelegate(new WFPagedPropelQuery($c, 'UsersPeer'));
		$oPage->sharedOutlet('Users')->setContent($oPage->sharedOutlet('paginator')->currentItems());
	}

	function clear($oPage, $aParams) {
		$oPage->outlet('query')->setValue(NULL);
		$this->search($oPage, $aParams);
	} //


	function setupSkin($oPage, $aParams, $skin) {

	   // $skin->addHeadString('<link rel="stylesheet" type="text/css" href="' . $skin->getSkinDirShared() . '/form.css" />');
		$aND = array('noDIV' => true);
		$skin->setTitle(SssSBla::cleanForTitle(
				SssSBla::bla('UsersPlur', $aND)
				. ' ' . SssSBla::bla('SharedList', $aND)));

	} //

} //


class module_users_edit {

	function parameterList() { return array('handle'); } //


	function parametersDidLoad($oPage, $aParams) {

	// populate multiSelectPPerms
	$a = PermpresetsPeer::doSelect(new Criteria());
	$oPage->sharedOutlet('Permpresets')->setContent($a);
//var_dump($_REQUEST, $a);
		if ($oPage->sharedOutlet('Users')->selection() === NULL) {

			if ($aParams['handle']) {

				// test if handle already exists
				$oUser = UsersPeer::retrieveByPK($aParams['handle']);

				if (!$oUser) {
					// new entry
					$oUser = new Users();
					$oUser->setHandle($aParams['handle']);

				} // if existing or new entry

				if (isset($_REQUEST['pass1'], $_REQUEST['pass2'])) {
					if ($_REQUEST['pass1'] == $_REQUEST['pass2']) {
						$oUser->setPasshash(sha1('SkyProm' . $_REQUEST['pass1']));
					} else throw (new WFException(SssSBla::bla('AdminUsersPassMissmatch')));
				} // if password change/set

				$oPage->sharedOutlet('Users')->setContent(array($oUser));
				$oPage->module()->verifyEditingPermission($oPage);

			} else {
				// prepare content for new
				$oPage->sharedOutlet('Users')->setContent(array(new Users()));

			} // if existing or new

		} // if no entry selected

	} // parametersDidLoad


	function save($oPage) {

		//$oPage->module()->verifyEditingPermission($oPage);

		try {
			$oUser = $oPage->sharedOutlet('Users')->selection();

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


	function setupSkin($oPage, $aParams, $skin) {
	   // $skin->addHeadString('<link rel="stylesheet" type="text/css" href="' . $skin->getSkinDirShared() . '/form.css" />');
		$aND = array('noDIV' => true);
		if ($oPage->sharedOutlet('Users')->selection()->isNew()) {
			$title = SssSBla::bla('SharedNew', $aND) . ' ' . SssSBla::bla('UsersSing', $aND);

		} else {
			$title = SssSBla::bla('SharedEdit', $aND) . ' ' . SssSBla::bla('UsersSing', $aND) . ':' . $oPage->sharedOutlet('Users')->selection()->valueForKeyPath('handle');

		} //

		$skin->setTitle(SssSBla::cleanForTitle($title));

	} //

} //



class module_users_confirmDelete
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
class module_users_detail
{
	function parameterList()
	{
		return array('handle');
	}
	function parametersDidLoad($oPage, $aParams)
	{
		$oPage->sharedOutlet('Users')->setContent(array(UsersPeer::retrieveByPK($aParams['handle'])));
	}
}
