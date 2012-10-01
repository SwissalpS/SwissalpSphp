<?php

// Created by PHOCOA WFModelCodeGen on Wed, 21 Jul 2010 15:19:57 +0200
class module_permissions extends WFModule {

	function checkSecurity(WFAuthorizationInfo $oAuthInfo) {

		if (-1 == $oAuthInfo->userid()) {

			$oRootInvocation = WFRequestController::sharedRequestController()->rootModuleInvocation();

			$sURLnow = (is_object($oRootInvocation))
					? $oRootInvocation->invocationPath() : 'admin/permissions';

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
        // if ($authInfo->userid() != $page->sharedOutlet('Permissions')->selection()->getUserId()) throw( new Exception("You don't have permission to edit Permissions.") );
    } // verifyEditingPermission

} // module_permissions

class module_permissions_list {
    function parameterList() {
        return array('paginatorState');
    }

    function parametersDidLoad($page, $params) {
        $page->sharedOutlet('paginator')->readPaginatorStateFromParams($params);
    }

    function noAction($page, $params) {
        $this->search($page, $params);
    } //

    function search($page, $params) {
        $query = $page->outlet('query')->value();
        $c = new Criteria();
        if (!empty($query))
        {
            $querySubStr = '%' . str_replace(' ', '%', trim($query)) . '%';

            $c->add(PermissionsPeer::DOMAIN, $querySubStr, Criteria::LIKE);
        }

        $page->sharedOutlet('paginator')->setDataDelegate(new WFPagedPropelQuery($c, 'PermissionsPeer'));
        $page->sharedOutlet('Permissions')->setContent($page->sharedOutlet('paginator')->currentItems());
    } //

    function clear($page, $params) {
        $page->outlet('query')->setValue(NULL);
        $this->search($page, $params);
    } //


    function setupSkin($page, $parameters, $skin) {
       // $skin->addHeadString('<link rel="stylesheet" type="text/css" href="' . $skin->getSkinDirShared() . '/form.css" />');
		$aND = array('noDIV' => true);
        $skin->setTitle(SssSBla::cleanForTitle(
        		SssSBla::bla('PermsPlur', $aND)
        		. ' ' . SssSBla::bla('SharedList', $aND)));
    } //

} // module_permissions_list



class module_permissions_edit {

    function parameterList() {
        return array('uid');
    } // parameterList

    function parametersDidLoad($page, $params) {
        if ($page->sharedOutlet('Permissions')->selection() === NULL) {
            if ($params['uid']) {
                $page->sharedOutlet('Permissions')->setContent(array(PermissionsPeer::retrieveByPK($params['uid'])));
                $page->module()->verifyEditingPermission($page);
            } else {
                // prepare content for new
                $page->sharedOutlet('Permissions')->setContent(array(new Permissions()));
            }
        }
    } // parametersDidLoad


    function save($page) {
        try {
            $page->sharedOutlet('Permissions')->selection()->save();
            $page->outlet('statusMessage')->setValue("Permissions saved successfully.");
        } catch (Exception $e) {
            $page->addError( new WFError($e->getMessage()) );
        }
    } // save


    function deleteObj($page) {
        $page->module()->verifyEditingPermission($page);
        $page->module()->setupResponsePage('confirmDelete');
    } // deleteObj


    function setupSkin($page, $parameters, $skin) {
       // $skin->addHeadString('<link rel="stylesheet" type="text/css" href="' . $skin->getSkinDirShared() . '/form.css" />');
		$aND = array('noDIV' => true);
        if ($page->sharedOutlet('Permissions')->selection()->isNew()) {
            $title = SssSBla::bla('SharedNew', $aND) . ' ' . SssSBla::bla('PermsSing', $aND);
        }
        else {
            $title = SssSBla::bla('SharedEdit', $aND) . ' ' . SssSBla::bla('PermsSing', $aND) . ':' . $page->sharedOutlet('Permissions')->selection()->valueForKeyPath('domain');
        }
        $skin->setTitle(SssSBla::cleanForTitle($title));
    } // setupSkin

} // module_permissions_edit



class module_permissions_confirmDelete
{
    function parameterList()
    {
        return array('uid');
    }
    function parametersDidLoad($page, $params)
    {
        // if we're a redirected action, then the Permissions object is already loaded. If there is no object loaded, try to load it from the object ID passed in the params.
        if ($page->sharedOutlet('Permissions')->selection() === NULL)
        {
            $objectToDelete = PermissionsPeer::retrieveByPK($params['uid']);
            if (!$objectToDelete) throw( new Exception("Could not load Permissions object to delete.") );
            $page->sharedOutlet('Permissions')->setContent(array($objectToDelete));
        }
        if ($page->sharedOutlet('Permissions')->selection() === NULL) throw( new Exception("Could not load Permissions object to delete.") );
    }
    function cancel($page)
    {
        $page->module()->setupResponsePage('edit');
    }
    function deleteObj($page)
    {
        $page->module()->verifyEditingPermission($page);
        $myObj = $page->sharedOutlet('Permissions')->selection();
        $myObj->delete();
        $page->sharedOutlet('Permissions')->removeObject($myObj);
        $page->module()->setupResponsePage('deleteSuccess');
    }
}
class module_permissions_detail
{
    function parameterList()
    {
        return array('uid');
    }
    function parametersDidLoad($page, $params)
    {
        $page->sharedOutlet('Permissions')->setContent(array(PermissionsPeer::retrieveByPK($params['uid'])));
    }
}
