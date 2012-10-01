<?php

// Created by PHOCOA WFModelCodeGen on Wed, 21 Jul 2010 15:19:57 +0200
class module_permpresets extends WFModule {

	function checkSecurity(WFAuthorizationInfo $oAuthInfo) {

		if (-1 == $oAuthInfo->userid()) {

			$oRootInvocation = WFRequestController::sharedRequestController()->rootModuleInvocation();

			$sURLnow = (is_object($oRootInvocation))
					? $oRootInvocation->invocationPath() : 'admin/permpresets';

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
	function verifyEditingPermission($page) {

		// example
		// $authInfo = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();
		// if ($authInfo->userid() != $page->sharedOutlet('Permpresets')->selection()->getUserId()) throw( new Exception("You don't have permission to edit Permpresets.") );

	} //

} //

class module_permpresets_list {

	function parameterList() {
		return array('paginatorState');
	}

	function parametersDidLoad($page, $params) {
		$page->sharedOutlet('paginator')->readPaginatorStateFromParams($params);
	}

	function noAction($page, $params) {
		$this->search($page, $params);
	}

	function search($page, $params) {
		$query = $page->outlet('query')->value();
		$c = new Criteria();
		if (!empty($query)) {
			$querySubStr = '%' . str_replace(' ', '%', trim($query)) . '%';

			$c->add(PermpresetsPeer::NAME, $querySubStr, Criteria::LIKE);
		}

		$page->sharedOutlet('paginator')->setDataDelegate(new WFPagedPropelQuery($c, 'PermpresetsPeer'));
		$page->sharedOutlet('Permpresets')->setContent($page->sharedOutlet('paginator')->currentItems());
	}

	function clear($page, $params) {
		$page->outlet('query')->setValue(NULL);
		$this->search($page, $params);
	}


	function setupSkin($page, $parameters, $skin) {
		//$skin->addHeadString('<link rel="stylesheet" type="text/css" href="' . $skin->getSkinDirShared() . '/form.css" />');
		$aND = array('noDIV' => true);
		$skin->setTitle(SssSBla::cleanForTitle(
				SssSBla::bla('PermsPlur', $aND)
				. ' ' . SssSBla::bla('SharedList', $aND)));

	} // setupSkin

} // module_permpresets_list

class module_permpresets_edit {

	function parameterList() {
		return array('uid');
	}


	function parametersDidLoad($page, $params) {

		//$a = range(1, PermissionsPeer::doCount(new Criteria()));
		//$page->sharedOutlet('Permissions')->setContent(PermissionsPeer::retrieveByPKs($a));
		$page->sharedOutlet('Permissions')->setContent(PermissionsPeer::doSelect(new Criteria()));

		if ($page->sharedOutlet('Permpresets')->selection() === NULL) {

			if ($params['uid']) {
				$page->sharedOutlet('Permpresets')->setContent(array(PermpresetsPeer::retrieveByPK($params['uid'])));
				$page->module()->verifyEditingPermission($page);
			} else {
				// prepare content for new
				$page->sharedOutlet('Permpresets')->setContent(array(new Permpresets()));
			}
		}
	}
	function save($page)
	{
		try {
			$page->sharedOutlet('Permpresets')->selection()->save();
			$page->outlet('statusMessage')->setValue("Permpresets saved successfully.");
		} catch (Exception $e) {
			$page->addError( new WFError($e->getMessage()) );
		}
	}
	function deleteObj($page)
	{
		$page->module()->verifyEditingPermission($page);
		$page->module()->setupResponsePage('confirmDelete');
	}

	function setupSkin($page, $parameters, $skin) {
	   // $skin->addHeadString('<link rel="stylesheet" type="text/css" href="' . $skin->getSkinDirShared() . '/form.css" />');
		$aND = array('noDIV' => true);
		if ($page->sharedOutlet('Permpresets')->selection()->isNew())
		{
			$title = SssSBla::bla('SharedNew', $aND) . ' ' . SssSBla::bla('PPermsSing', $aND);
		}
		else
		{
			$title = SssSBla::bla('SharedEdit', $aND) . ' '
					. SssSBla::bla('PPermsSing', $aND) . ':'
					. $page->sharedOutlet('Permpresets')->selection()->valueForKeyPath('name');
		}
		$skin->setTitle(SssSBla::cleanForTitle($title));
	} // setupSkin

} // module_permpresets_edit

class module_permpresets_confirmDelete
{
	function parameterList()
	{
		return array('uid');
	}
	function parametersDidLoad($page, $params)
	{
		// if we're a redirected action, then the Permpresets object is already loaded. If there is no object loaded, try to load it from the object ID passed in the params.
		if ($page->sharedOutlet('Permpresets')->selection() === NULL)
		{
			$objectToDelete = PermpresetsPeer::retrieveByPK($params['uid']);
			if (!$objectToDelete) throw( new Exception("Could not load Permpresets object to delete.") );
			$page->sharedOutlet('Permpresets')->setContent(array($objectToDelete));
		}
		if ($page->sharedOutlet('Permpresets')->selection() === NULL) throw( new Exception("Could not load Permpresets object to delete.") );
	}
	function cancel($page)
	{
		$page->module()->setupResponsePage('edit');
	}
	function deleteObj($page)
	{
		$page->module()->verifyEditingPermission($page);
		$myObj = $page->sharedOutlet('Permpresets')->selection();
		$myObj->delete();
		$page->sharedOutlet('Permpresets')->removeObject($myObj);
		$page->module()->setupResponsePage('deleteSuccess');
	}
}
class module_permpresets_detail
{
	function parameterList()
	{
		return array('uid');
	}
	function parametersDidLoad($page, $params)
	{
		$page->sharedOutlet('Permpresets')->setContent(array(PermpresetsPeer::retrieveByPK($params['uid'])));
	}
}
