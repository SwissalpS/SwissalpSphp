<?php

class SRPNotesCriteria extends WFObject {

	var $a;

	function __construct() {

		$aND = array('noDIV' => true);

		$a = array(
			NotesPeer::NAME => SssSBla::bla('NotesCriteriaName', $aND),
			NotesPeer::NOTE => SssSBla::bla('NotesCriteriaNote', $aND),
			//NotesPeer::DATE => SssSBla::bla('NotesCriteriaDate', $aND),
			NotesPeer::BRIDGEUID => SssSBla::bla('NotesCriteriaBridge', $aND),
			//NotesPeer::KARMA => SssSBla::bla('NotesCriteriaKarma', $aND),
			//NotesPeer::PUBLISH => SssSBla::bla('NotesCriteriaPublish', $aND),
			NotesPeer::LANG => SssSBla::bla('NotesCriteriaLanguage', $aND)
			);

		$this->a['labels'] = array_values($a);
		$this->a['values'] = range(0, count($a)-1);
		$this->a['sqlValue'] = array_keys($a);

	} // __construct

	function selectCriteriaLabels() {

		return $this->a['labels'];

	} // selectCriteriaLabels

	function selectCriteriaValues() {

		return $this->a['values'];

	} // selectCriteriaValues

	function sqlForValue($i) {

		return $this->a['sqlValue'][$i];

	} // sqlForValue

	function editCriteriaLabelsHTML() {

		if (!WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo()->showBlaEditLinks()) return '';

		$aND = array('noDIV' => true);

		$sEditMeTextFull = SssSBla::bla('SharedEditLabel', $aND);

		$aBlas = array(
			array('sUID' => 'NotesCriteriaName',
				'sEditMeTextFull' => $sEditMeTextFull . ': NotesCriteriaName',
				'sEditMeTextShort' => 'eL'),
			array('sUID' => 'NotesCriteriaNote',
				'sEditMeTextFull' => $sEditMeTextFull . ': NotesCriteriaNote',
				'sEditMeTextShort' => 'eL'),
		/*	array('sUID' => 'NotesCriteriaDate',
				'sEditMeTextFull' => $sEditMeTextFull . ': NotesCriteriaDate',
				'sEditMeTextShort' => 'eL'),*/
			array('sUID' => 'NotesCriteriaBridge',
				'sEditMeTextFull' => $sEditMeTextFull . ': NotesCriteriaBridge',
				'sEditMeTextShort' => 'eL'),
		/*	array('sUID' => 'NotesCriteriaKarma',
				'sEditMeTextFull' => $sEditMeTextFull . ': NotesCriteriaKarma',
				'sEditMeTextShort' => 'eL'),
			array('sUID' => 'NotesCriteriaPublish',
				'sEditMeTextFull' => $sEditMeTextFull . ': NotesCriteriaPublish',
				'sEditMeTextShort' => 'eL'), */
			array('sUID' => 'NotesCriteriaLanguage',
				'sEditMeTextFull' => $sEditMeTextFull . ': NotesCriteriaLanguage',
				'sEditMeTextShort' => 'eL'),
			array('sUID' => 'NotesCriteriaEditLabels',
				'sEditMeTextFull' => $sEditMeTextFull . ': NotesCriteriaEditLabels',
				'sEditMeTextShort' => 'eL')
			);

		$sDone = SssSBla::bla('NotesCriteriaEditLabels', $aND);

		return SssSBla::wrapHTMLinEditDiv($aBlas, $sDone, false);

	} // editCriteriaLabelsHTML

} // SRPNotesCriteria


// Created by PHOCOA WFModelCodeGen on Wed, 21 Jul 2010 15:19:56 +0200
class module_notes extends WFModule {

	var $oNotesCriteria;

	function __construct($sInvocation) {

		parent::__construct($sInvocation);

		$this->oNotesCriteria = new SRPNotesCriteria();

	} // __construct

	function notesCriteria() { return $this->oNotesCriteria; } // notesCriteria

	function bridges() { return SssSBridges::sharedBridges(); } // bridges

	function countries() { return SssSCountries::sharedInstance(); } // countries

	function localization() { return SssSLocalization::sharedInstance(); } // localization

	function checkSecurity(WFAuthorizationInfo $oAuthInfo) {

		if (-1 == $oAuthInfo->userid()) {

			$oRootInvocation = WFRequestController::sharedRequestController()->rootModuleInvocation();

			$sURLnow = (is_object($oRootInvocation))
					? $oRootInvocation->invocationPath() : 'admin/notes';

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

	function authInfo() {

		return WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();

	} // authInfo


	function defaultPage() { return 'list'; } // defaultPage

	// this function should throw an exception if the user is not permitted to edit (add/edit/delete) in the current context
	function verifyEditingPermission($oPage) {
		// example
		// $authInfo = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();
		// if ($authInfo->userid() != $oPage->sharedOutlet('Notes')->selection()->getUserId()) throw( new Exception("You don't have permission to edit Notes.") );
	} // verifyEditingPermission

	function sharedInstancesDidLoad() {

		$oPaginator = $this->paginator;

		$sB = SssSBla::TRANS_PREFIX;

		$oPaginator->setSortOptions(array('+publish' => $sB . 'NotePublish', '-publish' => $sB . 'NotePublish', '+bridgeuid' => $sB . 'BridgesSing', '-bridgeuid' => $sB . 'BridgesSing', '+name' => $sB . 'SharedName', '-name' => $sB . 'SharedName', '+lang' => $sB . 'SharedLanguage', '-lang' => $sB . 'SharedLanguage', '+country' => $sB . 'BridgesCountry', '-country' => $sB . 'BridgesCountry', '+date' => $sB . 'NoteDate', '-date' => $sB . 'NoteDate', '+karma' => $sB . 'SharedKarma', '-karma' => $sB . 'SharedKarma', '+uid' => $sB . 'uid', '-uid' => $sB . 'uid', '+note' => $sB . 'NoteSing', '-note' => $sB . 'NoteSing', '+mediauid' => $sB . 'AdminNoteMediaUID', '-mediauid' => $sB . 'AdminNoteMediaUID'));

		$oPaginator->setDefaultSortKeys(array('-date', '+publish', '+karma', '+bridgeuid', '-lang', '+country', '+name', '+uid', '+note', '-mediauid'));

	} // sharedInstancesDidLoad

} // module_notes

class module_notes_list {

	function parameterList() {

		return array('paginatorState');

	} // parameterList

	function parametersDidLoad($oPage, $aParameters) {
		$oPage->sharedOutlet('paginator')->readPaginatorStateFromParams($aParameters);

	} // parametersDidLoad

	function noAction($oPage, $aParameters) {

		$this->search($oPage, $aParameters);

	} // noAction

	function search($oPage, $aParameters) {

		$query = $oPage->outlet('query')->value();
		$iSearchFor = intval($oPage->outlet('selectSearchCriteria')->value());
		$sSearchFor = $oPage->module()->oNotesCriteria->sqlForValue($iSearchFor);
		$oC = new Criteria();
		if (!empty($query)) {
			// query entered

			$querySubStr = '%' . str_replace(' ', '%', trim($query)) . '%';

			$oC->add($sSearchFor, $querySubStr, Criteria::LIKE);
		}

		$oPage->sharedOutlet('paginator')->setDataDelegate(new WFPagedPropelQuery($oC, 'NotesPeer'));
		$oPage->sharedOutlet('Notes')->setContent($oPage->sharedOutlet('paginator')->currentItems());

	} // search

	function clear($oPage, $aParameters) {

		$oPage->outlet('query')->setValue(NULL);

		$this->search($oPage, $aParameters);

	} // clear


	function setupSkin($oPage, $aParameters, $oSkin) {

		$aND = array('noDIV' => true);

		$oSkin->setTitle(SssSBla::cleanForTitle(
				SssSBla::bla('AdminNotesList', $aND)));

		$oSkin->addHeadString('<link rel="stylesheet" type="text/css" href="' . $oSkin->getSkinDir() . '/notesList.css" />');

		$oSkin->addHeadString('<link rel="stylesheet" type="text/css" href="' . $oSkin->getSkinDir() . '/notesEdit.css" />');

		$oSkin->addHeadString('<script src="' . $oSkin->getSkinDirShared() . '/SssSMediaBox.js" type="text/javascript"></script>');

	} // setupSkin

} // module_notes_list

class module_notes_edit {

	function parameterList() {
		return array('uid');
	}

	function parametersDidLoad($oPage, $aParameters) {

		$oAuthInfo = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();

		if (NULL === $oPage->sharedOutlet('Notes')->selection()) {

			if ($aParameters['uid']) {

                $oNote = NotesPeer::retrieveByPK($aParameters['uid']);
                $oPage->sharedOutlet('Notes')->setContent(array($oNote));

				$oPage->module()->verifyEditingPermission($oPage);

			} else {

				// prepare content for new
				$oNote = new Notes();

				$sCountry = (isset($_POST['country'])
						&& in_array($_POST['country'], SssSCountries::sharedCountries()->selectCountriesValues()))
						? $_POST['country'] : 'CH';
				$oNote->setCountry($sCountry);

				// language of note
				$aLangs = SssSCountries::sharedCountries()->selectLangValues();

				$sLang = (isset($_POST['lang'])
						&& in_array($_POST['lang'], $aLangs))
						? $_POST['lang'] : $aLangs[0];
				$oNote->setLang($sLang);

				$oUser = UsersPeer::retrieveByPK($oAuthInfo->userid());

				$sName = $oUser->getRealname();
				if (!empty($sName)) $oNote->setName($sName);

				$sURL = $oUser->getUrl();
				if (!empty($sURL)) $oNote->setUrl0($sURL);

				$sRegion = $oUser->getRegion();
				if (!empty($sRegion)) $oNote->setRegion($sRegion);

				if (!isset($_POST['country'])) {

					$sCountryMy = $oUser->getCountry();
					if (!empty($sCountryMy)) $oNote->setCountry($sCountryMy);

				} // if country not set by post

				$iKarma = $oUser->getKarma();
				$oNote->setKarma($iKarma);
				$oNote->setHandle($oAuthInfo->userid());

				$aBridgesUIDs = SssSBridges::selectBridgesValues();

				if (0 < count($aBridgesUIDs))
					$iBridgeUID = $aBridgesUIDs[0];

				$oBridge = BridgesPeer::retrieveByPK($iBridgeUID);

				$oNote->setBridgeuid($iBridgeUID);
				$oPage->sharedOutlet('Notes')->setContent(array($oNote));

				// fill our shared bridge instance
				$oPage->sharedOutlet('Bridge')->setContent($oBridge);

			} // if existing or new
			$oPage->outlet('selectCountry')->setValue($oNote->getCountry());
			$oPage->outlet('selectBridge')->setValue($oNote->getBridgeuid());

		} // if none selected

	} // parametersDidLoad

	function save($oPage) {

		$oNote = $oPage->sharedOutlet('Notes')->selection();

		//
		$oNote->setBridgeuid($oPage->outlet('bridgeuid')->value());

		// handle uploaded file
		$this->handleUpload($oPage, $oNote);

		$this->handleEmbededLinks($oPage, $oNote);

		// count the postings, just in case... we also check B4 serving
		//$this->handlePostCount($oPage);

		$bOK = false;

		try {

			$oNote->save();
			$oPage->outlet('statusMessage')->setValue(SssSBla::bla('NoteGuestSavedSuccessfully') . '.');

			$bOK = true;

		} catch (Exception $e) {

			$oPage->addError(new WFError($e->getMessage()));

		}

		if ($bOK) {

			// redirect to info about posting
			$sURL = WFRequestController::WFURL('/notes/thanks/' . $oNote->getUid());
			if (WFRequestController::isAjax())
				throw (new WFRequestController_InternalRedirectException($sURL));
			else
				throw (new WFRedirectRequestException($sURL));

		} // if success

	} // save


	function handleEmbededLinks($oPage, &$oNote) {

		// only deal with them if not already got a mediauid
		if ($oNote->getMediauid()) return;

		// only if is you tube url
	/*	$aNoteLines = explode(chr(10), trim($oNote->getNote()));
		$aFirstLineBits = explode(' ', trim($aNoteLines[0]));
		$sUTubeID = SssSgdImageBasic::parseYouTubeURL4id($aFirstLineBits[0]);*/
		$sUTubeID = SssSgdImageBasic::parseYouTubeURL4id($oNote->getNote(), false);
//throw (new WFException('UTid:' . $sUTubeID . ': note:' . $oNote->getNote() . ':'));
		if (!$sUTubeID) return;

		$oMediaMap = new Mediamap();

		$oAuthInfo = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();

		if ($oAuthInfo->isSuperUser()) $oMediaMap->setKarma(200);
		else $oMediaMap->setKarma(100);

		$oMediaMap->setMime('application/x-shockwave-flash');
		$oMediaMap->setWidth(0); // TODO:
		$oMediaMap->setHeight(0); // TODO:
		$oMediaMap->setType(0); // TODO:

		$oMediaMap->setUrl('http://www.youtube.com/watch?v=' . $sUTubeID);
		$oMediaMap->save();
		$oNote->setMediauid($oMediaMap->getMediauid());

	} // handleEmbededLinks


	function handlePostCount($oPage, $bIncCount = true) {

		$oAuthInfo = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();

		// registered users that are logged in don't have restriction
		if ($oAuthInfo->isLoggedIn()) return true;

		$aPostTimes = $oAuthInfo->getPostTimes();

		// not yet over critical post amount?
		if (4 > count($aPostTimes)) {

			if ($bIncCount) $oAuthInfo->addPostTime();

			return true;

		} //

		// when was last?
		$iDiff = time() - array_pop($aPostTimes);
		if (3600 < $iDiff) {
			// over an hour since last post, ok go ahead

			if ($bIncCount) $oAuthInfo->addPostTime();

			return true;

		} //

		// redirect to info about posting
		$sURL = WFRequestController::WFURL('/notes/overposted');
		throw (new WFRedirectRequestException($sURL));

		// TODO: check the count before serving the page and include a human checker
		// or do we simply redirect to some info page?
		// TODO: remove any uploads?
		/*
		$oPage->addError(new WFError(SssSBla::bla('NoteUnregisteredPostedTooMany')));
		$oPage->outlet('statusMessage')->setValue(SssSBla::bla('NoteGuestSavedSuccessfully') . '.');
		*/
		return null;

	} // handlePostCount


	function handleUpload($oPage, &$oNote) {

		$sTmpFile = $oPage->outlet('uploadMedia')->tmpFileName();
		if (!$sTmpFile) return;

		// file was uploaded

		$oAuthInfo = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();

		$sBridgeUID = strval($oNote->getBridgeuid());

		$sOrigName = $oPage->outlet('uploadMedia')->originalFileName();

		// check integrity
		$aInfo = getimagesize($sTmpFile);

		$bImage = true;

		if (empty($aInfo)) {
			// probably not an image
			$bImage = false;

			// define mime by extension -> not safe!
			$sMime = E::mime_content_type($sOrigName);
//WFLog::log('#############tmpfile: ' . $sTmpFile . ' : mime: ' . $sMime . ' :');
			if ('application/octet-stream' == $sMime) {

				@unlink($sTmpFile);
				$oPage->addError(
						new WFError(SssSBla::bla('MediaUploadNotAccepted')
								. ' ' . $sOrigName));
				return;

			} // unknown/app

		} // if gd compatible image

		// finally it will be bridgeUID/size/SWISSROPE_bUID_date.ext
		$sURL = $this->moveToUploadDir($sOrigName, $sTmpFile, $sBridgeUID);

		if (false === $sURL) {
			$oPage->addError(
					new	WFError(SssSBla::bla('MediaUploadFailed')));

			return;

		} // if move failed

		$oMediaMap = new Mediamap();

		if ($oAuthInfo->isSuperUser()) $oMediaMap->setKarma(200);
		else $oMediaMap->setKarma(100);

		if ($bImage) {
			// seems to be an image

			$oMediaMap->setMime($aInfo['mime']);
			$oMediaMap->setWidth($aInfo[0]);
			$oMediaMap->setHeight($aInfo[1]);
			$oMediaMap->setType($aInfo[2]);

		} else {

			$oMediaMap->setMime($sMime);
			$oMediaMap->setWidth(0); // TODO:
			$oMediaMap->setHeight(0); // TODO:
			$oMediaMap->setType(0); // TODO:

		} // if image or other filetype

		$oMediaMap->setUrl($sURL);
		$oMediaMap->save();
		$oNote->setMediauid($oMediaMap->getMediauid());

		@unlink($sTmpFile);

	} // handleUpload


	function moveToUploadDir($sOrigName, $sTmpFile, $sBridgeUID) {

		$sSuffix = strrchr($sOrigName, '.');
		$sDate = gmdate('Ymd_His');
		$sURL = sprintf('SWISSROPE_%1$s_%2$s%3$s',
				$sBridgeUID, $sDate, $sSuffix); // TODO: what if...multiple submissions in the same second for same bridge do we check for existance first or use uniqid?

		$sPath = MEDIA_UPLOAD_DIR; // . DIR_SEP . $sBridgeUID;

		$oRunner = new SssS_ShellScriptRunnerForDummies();
		$oRunner->makePath($sPath);

		// catch path creation error
		if (0 !== $oRunner->iRes()) {
			WFLog::log('oops can not create MEDIA_UPLOAD_DIR:' . $sPath, WFLog::WARN_LOG);
			return false;
		} // if create dir error

		// make sure it isn't executable
		$oRunner->doScript('chmod a-x ' . $sTmpFile, IS_PRODUCTION);
		// catch error
		if (0 !== $oRunner->iRes()) {
			WFLog::log('oops can not chmod a-x:' . $sTmpFile, WFLog::WARN_LOG);
			return false;
		} // if chmod -x error

		// make sure cron user can read and write
		$oRunner->doScript('chmod a+rw ' . $sTmpFile, IS_PRODUCTION);
		// catch error
		if (0 !== $oRunner->iRes()) {
			WFLog::log('oops can not chmod a+rw:' . $sTmpFile, WFLog::WARN_LOG);
			return false;
		} // if chmod -x error

		$sDownloadPath = MEDIA_DOWNLOAD_DIR . DIR_SEP . 'bridge'
				. DIR_SEP . $sBridgeUID . DIR_SEP . 'original' . DIR_SEP;

		$bExistsInDownload = is_file($sDownloadPath . $sURL);

		$bExistsInUpload = is_file($sPath . DIR_SEP . $sURL);

		$iCount = 0;
		while($bExistsInUpload || $bExistsInDownload) {

			$sURL = sprintf('SWISSROPE_%1$s_%2$s%4$s%3$s',
				$sBridgeUID, $sDate, $sSuffix, $iCount);

			$bExistsInDownload = is_file($sDownloadPath . $sURL);

			$bExistsInUpload = is_file($sPath . DIR_SEP . $sURL);

			$iCount++;

		} // loop until valid filename

		// now move to upload dir // TODO: make sure not yet existing!!!!
		$bOK = rename($sTmpFile, $sPath . DIR_SEP . $sURL);

		if ($bOK) return $sURL;

		WFLog::log('oops can not move:' . $sTmpFile . ' to: ' . $sPath
				. DIR_SEP . $sURL, WFLog::WARN_LOG);

		return false;

	} // moveToUploadDir


	function deleteObj($oPage) {

		$oPage->module()->verifyEditingPermission($oPage);

		$oPage->module()->setupResponsePage('confirmDelete');

	} // deleteObj


	function setupSkin($oPage, $aParameters, $oSkin) {

		$oSkin->addHeadString('<link rel="stylesheet" type="text/css" href="' . $oSkin->getSkinDir() . '/notesEdit.css" />');

		$oSkin->addHeadString('<script src="' . $oSkin->getSkinDirShared() . '/SssSMediaBox.js" type="text/javascript"></script>');

		$aND = array('noDIV' => true);

		if ($oPage->sharedOutlet('Notes')->selection()->isNew()) {

			$title = SssSBla::bla('NoteNew', $aND);

		} else {

			$title = SssSBla::bla('NoteEdit', $aND);

		} //

		$oSkin->setTitle($title);

	} // setupSkin

} // module_notes_edit

class module_notes_confirmDelete {

	function parameterList() {

		return array('uid');

	} // parameterList


	function parametersDidLoad($oPage, $aParameters) {

		// if we're a redirected action, then the Notes object is already loaded. If there is no object loaded, try to load it from the object ID passed in the params.
		if ($oPage->sharedOutlet('Notes')->selection() === NULL) {

			$objectToDelete = NotesPeer::retrieveByPK($aParameters['uid']);

			if (!$objectToDelete) {

				$sURL = WFRequestController::WFURL('/notes/list');

				if (WFRequestController::isAjax())
					throw (new WFRequestController_InternalRedirectException($sURL));
				else
					throw (new WFRedirectRequestException($sURL));

			} // if no object (possibly already deleted)
			$oPage->sharedOutlet('Notes')->setContent(array($objectToDelete));

		} // if Notes not yet populated

	} // parametersDidLoad

	function cancel($oPage) {

		//$oPage->module()->setupResponsePage('detail');

		$oNote = $oPage->sharedOutlet('Notes')->selection();

		$sURL = WFRequestController::WFURL((($oNote) ? '/notes/detail/' . $oNote->getUid() : '/notes/list'));

		if (WFRequestController::isAjax())
			throw (new WFRequestController_InternalRedirectException($sURL));
		else
			throw (new WFRedirectRequestException($sURL));

	} // cancel

	function deleteObj($oPage) {

		$oPage->module()->verifyEditingPermission($oPage);

		$oNote = $oPage->sharedOutlet('Notes')->selection();

		Notes::deleteNoteWithAttachmentAndChildren($oNote);

		$oPage->sharedOutlet('Notes')->removeObject($oNote);

		$oPage->module()->setupResponsePage('deleteSuccess');

	} // deleteObj

} // module_notes_confirmDelete

class module_notes_detail {

	function parameterList() {

		return array('uid');

	}

	function parametersDidLoad($oPage, $aParameters) {
		$oPage->sharedOutlet('Notes')->setContent(array(NotesPeer::retrieveByPK($aParameters['uid'])));

	} // parametersDidLoad

} // module_notes_detail
