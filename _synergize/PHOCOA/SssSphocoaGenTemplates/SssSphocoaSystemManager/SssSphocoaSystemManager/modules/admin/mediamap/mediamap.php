<?php

// Created by PHOCOA WFModelCodeGen on Wed, 21 Jul 2010 15:19:56 +0200
class module_mediamap extends WFModule {

	function checkSecurity(WFAuthorizationInfo $oAuthInfo) {

		if (-1 == $oAuthInfo->userid()) {

			$oRootInvocation = WFRequestController::sharedRequestController()->rootModuleInvocation();

			$sURLnow = (is_object($oRootInvocation))
					? $oRootInvocation->invocationPath() : 'admin/mediamap';

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
    function verifyEditingPermission($page)
    {
        // example
        // $authInfo = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();
        // if ($authInfo->userid() != $page->sharedOutlet('Mediamap')->selection()->getUserId()) throw( new Exception("You don't have permission to edit Mediamap.") );
    }
}

class module_mediamap_list
{
    function parameterList()
    {
        return array('paginatorState');
    }

    function parametersDidLoad($page, $params)
    {
        $page->sharedOutlet('paginator')->readPaginatorStateFromParams($params);
    }

    function noAction($page, $params)
    {
        $this->search($page, $params);
    }

    function search($page, $params)
    {
        $query = $page->outlet('query')->value();
        $c = new Criteria();
        if (!empty($query))
        {
            $querySubStr = '%' . str_replace(' ', '%', trim($query)) . '%';

            $c->add(MediamapPeer::URL, $querySubStr, Criteria::LIKE);
        }

        $page->sharedOutlet('paginator')->setDataDelegate(new WFPagedPropelQuery($c, 'MediamapPeer'));
        $page->sharedOutlet('Mediamap')->setContent($page->sharedOutlet('paginator')->currentItems());
    }

    function clear($page, $params)
    {
        $page->outlet('query')->setValue(NULL);
        $this->search($page, $params);
    }


    function setupSkin($page, $parameters, $skin)
    {
		$aND = array('noDIV' => true);
        $skin->addHeadString('<link rel="stylesheet" type="text/css" href="' . $skin->getSkinDirShared() . '/form.css" />');
        $skin->setTitle('Mediamap List');
    }
}

class module_mediamap_edit
{
    function parameterList()
    {
        return array('mediauid');
    }
    function parametersDidLoad($page, $params)
    {
        if ($page->sharedOutlet('Mediamap')->selection() === NULL)
        {
            if ($params['mediauid'])
            {
                $page->sharedOutlet('Mediamap')->setContent(array(MediamapPeer::retrieveByPK($params['mediauid'])));
                $page->module()->verifyEditingPermission($page);
            }
            else
            {
                // prepare content for new
                $page->sharedOutlet('Mediamap')->setContent(array(new Mediamap()));
            }
        }
    }
    function save($page)
    {
        try {
            $page->sharedOutlet('Mediamap')->selection()->save();
            $page->outlet('statusMessage')->setValue("Mediamap saved successfully.");
        } catch (Exception $e) {
            $page->addError( new WFError($e->getMessage()) );
        }
    }
    function deleteObj($page)
    {
        $page->module()->verifyEditingPermission($page);
        $page->module()->setupResponsePage('confirmDelete');
    }

    function setupSkin($page, $parameters, $skin)
    {
		$aND = array('noDIV' => true);
        $skin->addHeadString('<link rel="stylesheet" type="text/css" href="' . $skin->getSkinDirShared() . '/form.css" />');
        if ($page->sharedOutlet('Mediamap')->selection()->isNew())
        {
            $title = 'New Mediamap';
        }
        else
        {
            $title = 'Edit Mediamap:' . $page->sharedOutlet('Mediamap')->selection()->valueForKeyPath('url');
        }
        $skin->setTitle($title);
    }
}

class module_mediamap_confirmDelete
{
    function parameterList()
    {
        return array('mediauid');
    }
    function parametersDidLoad($page, $params)
    {
        // if we're a redirected action, then the Mediamap object is already loaded. If there is no object loaded, try to load it from the object ID passed in the params.
        if ($page->sharedOutlet('Mediamap')->selection() === NULL)
        {
            $objectToDelete = MediamapPeer::retrieveByPK($params['mediauid']);
            if (!$objectToDelete) throw( new Exception("Could not load Mediamap object to delete.") );
            $page->sharedOutlet('Mediamap')->setContent(array($objectToDelete));
        }
        if ($page->sharedOutlet('Mediamap')->selection() === NULL) throw( new Exception("Could not load Mediamap object to delete.") );
    }
    function cancel($page)
    {
        $page->module()->setupResponsePage('edit');
    }
    function deleteObj($page)
    {
        $page->module()->verifyEditingPermission($page);
        $myObj = $page->sharedOutlet('Mediamap')->selection();
        $myObj->delete();
        $page->sharedOutlet('Mediamap')->removeObject($myObj);
        $page->module()->setupResponsePage('deleteSuccess');
    }
}
class module_mediamap_detail
{
    function parameterList()
    {
        return array('mediauid');
    }
    function parametersDidLoad($page, $params)
    {
        $page->sharedOutlet('Mediamap')->setContent(array(MediamapPeer::retrieveByPK($params['mediauid'])));
    }
}
