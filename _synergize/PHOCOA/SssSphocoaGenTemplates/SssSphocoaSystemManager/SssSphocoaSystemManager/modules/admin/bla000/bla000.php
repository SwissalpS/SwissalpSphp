<?php
use SssSPropel2\phocoa\Bla000;
use SssSPropel2\phocoa\Bla000Query;
use SwissalpS\PHOCOA\Localization\Bla as SssSBla;

// Created by PHOCOA WFModelCodeGen on Wed, 21 Jul 2010 15:19:56 +0200
class module_bla000 extends WFModule {

	function checkSecurity(WFAuthorizationInfo $oAuthInfo) {

		if (-1 == $oAuthInfo->userid()) {

			$oRootInvocation = WFRequestController::sharedRequestController()->rootModuleInvocation();

			$sURLnow = (is_object($oRootInvocation))
					? $oRootInvocation->invocationPath() : 'admin/bla000';

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


    function defaultPage() {

		return 'list';

	} // defaultPage


    // this function should throw an exception if the user is not permitted to edit (add/edit/delete) in the current context
    function verifyEditingPermission($oPage) {
        // example
        // $authInfo = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();
        // if ($authInfo->userid() != $oPage->sharedOutlet('Bla000')->selection()->getUserId()) throw( new Exception("You don't have permission to edit Bla000.") );
    } // verifyEditingPermission

} // module_bla000

class module_bla000_list {

    function parameterList() {

        return array('paginatorState');

    } // parameterList


    function parametersDidLoad($oPage, $params) {

        $oPage->sharedOutlet('paginator')->readPaginatorStateFromParams($params);

    } // parametersDidLoad


    function noAction($oPage, $params) {

        $this->search($oPage, $params);

    } // noAction


    function search($oPage, $params) {

		$sQuery = trim($oPage->outlet('query')->value());

		$oC = Bla000Query::create();

		if (strlen($sQuery)) {

			$sQuerySubStr = '%' . str_replace(' ', '%', $sQuery) . '%';

			$oC->filterByUid($sQuerySubStr);

		} // if got query string

		$oPaginator = $oPage->sharedOutlet('paginator');
		$oSharedEntity = $oPage->sharedOutlet('SssSPropel2_phocoa_Bla000');
		$oPagedQuery = new WFPagedPropelQuery($oC);

		$oPaginator->setDataDelegate($oPagedQuery);
		$oSharedEntity->setContent($oPaginator->currentItems());

    } // search


    function clear($oPage, $params) {

        $oPage->outlet('query')->setValue(NULL);
        $this->search($oPage, $params);

    } // clear


    function setupSkin($oPage, $parameters, $skin) {

       // $skin->addHeadString('<link rel="stylesheet" type="text/css" href="' . $skin->getSkinDirShared() . '/form.css" />');
		$aND = array('noDIV' => true);
        $skin->setTitle(SssSBla::cleanForTitle(
        		SssSBla::bla('BlaPlur', $aND)
        		. ' ' . SssSBla::bla('SharedList', $aND)));

    } // setupSkin

} // module_bla000_list



class module_bla000_edit {

    function parameterList() {

        return array('uid');

    } // parameterList


    function parametersDidLoad($oPage, $params) {

        if ($oPage->sharedOutlet('Bla000')->selection() === NULL) {

            if ($params['uid']) {

            	// test if uid already exists
            	$oTemp = Bla000Query::create()->findPk($params['uid']);

                if ($oTemp) {

                	$oPage->sharedOutlet('Bla000')->setContent(array($oTemp));

                } else {

                	$oBla = new Bla000();
                	$oBla->setUid($params['uid']);
                	$oPage->sharedOutlet('Bla000')->setContent(array($oBla));

                } // if found entry or not

                $oPage->module()->verifyEditingPermission($oPage);

            } else {
                // prepare content for new
                $oPage->sharedOutlet('Bla000')->setContent(array(new Bla000()));
            } //

        } //

/* doesn't work this way
        $oEditor = $oPage->outlet('scratchHTML');
        $oEditor->DefaultLanguage = SssSBla::defaultLanguage();
        $oEditor->Value = SssSBla::defaultLanguage(); */

    } // parametersDidLoad


    function save($oPage) {
        try {
            $oPage->sharedOutlet('Bla000')->selection()->save();
            $oPage->outlet('statusMessage')->setValue(SssSBla::bla('BlaSing') . ': ('
            		. $oPage->sharedOutlet('Bla000')->selection()->valueForKeyPath('uid')
            		. ') ' . SssSBla::bla('SysSavedSuccessfully') . '.');

        } catch (Exception $e) {
            $oPage->addError( new WFError($e->getMessage()) );
        } //

    } // save


    function deleteObj($oPage) {
        $oPage->module()->verifyEditingPermission($oPage);
        $oPage->module()->setupResponsePage('confirmDelete');
    } // deleteObj


    function setupSkin($oPage, $parameters, $skin) {

       // $skin->addHeadString('<link rel="stylesheet" type="text/css" href="' . $skin->getSkinDirShared() . '/form.css" />');
		$aND = array('noDIV' => true);
        if ($oPage->sharedOutlet('Bla000')->selection()->isNew()) {
            $title = SssSBla::bla('SharedNew', $aND) . ' ' . SssSBla::bla('BlaSing', $aND);
        } else {
            $title = SssSBla::bla('SharedNew', $aND) . ' ' . SssSBla::bla('BlaSing', $aND)
            	. ': ' . $oPage->sharedOutlet('Bla000')->selection()->valueForKeyPath('uid');
        } //

        $skin->setTitle(SssSBla::cleanForTitle($title));

    } //

} // module_bla000_edit


class module_bla000_confirmDelete {

    function parameterList() { return array('uid'); } // parameterList


    function parametersDidLoad($oPage, $params) {
        // if we're a redirected action, then the Bla000 object is already loaded. If there is no object loaded, try to load it from the object ID passed in the params.
        if ($oPage->sharedOutlet('Bla000')->selection() === NULL)
        {
            $objectToDelete = Bla000Peer::retrieveByPK($params['uid']);
            if (!$objectToDelete) throw( new Exception("Could not load Bla000 object to delete.") );
            $oPage->sharedOutlet('Bla000')->setContent(array($objectToDelete));
        }
        if ($oPage->sharedOutlet('Bla000')->selection() === NULL) throw( new Exception("Could not load Bla000 object to delete.") );
    } //


    function cancel($oPage) { $oPage->module()->setupResponsePage('edit'); } //


    function deleteObj($oPage) {

        $oPage->module()->verifyEditingPermission($oPage);
        $myObj = $oPage->sharedOutlet('Bla000')->selection();
        $myObj->delete();
        $oPage->sharedOutlet('Bla000')->removeObject($myObj);
        $oPage->module()->setupResponsePage('deleteSuccess');

    } //

} // module_bla000_confirmDelete



class module_bla000_detail {

    function parameterList() { return array('uid'); }


    function parametersDidLoad($oPage, $params) {
        $oPage->sharedOutlet('Bla000')->setContent(array(Bla000Peer::retrieveByPK($params['uid'])));

    } // parametersDidLoad

} // module_bla000_detail
