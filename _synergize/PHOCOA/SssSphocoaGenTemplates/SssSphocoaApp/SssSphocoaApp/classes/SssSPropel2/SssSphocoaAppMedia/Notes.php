<?php

namespace SssSPropel2\SssSphocoaAppMedia;

use SssSPropel2\SssSphocoaAppMedia\Base\Notes as BaseNotes;

use Propel\Runtime\Connection\ConnectionInterface;
use SwissalpS\PHOCOA\Localization\Bla as SssSBla;
use SwissalpS\PHOCOA\Localization\Countries as SssSCountries;
use WFAuthorizationInfo;
use WFAuthorizationManager;
use WFError;

/**
 * Skeleton subclass for representing a row from the 'Notes' table.
 *
 *
 *
 * You should add additional methods to this class to meet the
 * application requirements.  This class will only be generated as
 * long as it does not already exist in the output directory.
 *
 */
class Notes extends BaseNotes {


    static function deleteNoteWithAttachmentAndChildren($oNote) {

		if (!$oNote) {

            return null;

        } // if no note given

		// first delete media if has
		$iMediaUID = $oNote->getMediauid();
		if (!empty($iMediaUID)) {

			$oMediaMap = MediamapQuery::create()->findByPk($iMediaUID);
			if ($oMediaMap) {

				$sFile = RUNTIME_DIR . '/deleteTheseMediaFiles';
				$sDeleteThese = @file_get_contents($sFile);// _safely

				$iBridgeUID = $oNote->getBridgeuid();
				// delete original file
				$sDeleteThese .= chr(10) . MediamapQuery::findMediaFile($iBridgeUID, $oMediaMap->getUrl());
				//unlink(MediamapPeer::findMediaFile($iBridgeUID, $oMediaMap->getUrl()));

				// delete big file
				$sDeleteThese .= chr(10) . MediamapQuery::findMediaFile($iBridgeUID, $oMediaMap->getBigurl(), 'big');
				//unlink(MediamapPeer::findMediaFile($iBridgeUID, $oMediaMap->getBigurl(), 'big'));

				// delete thumb file
				$sDeleteThese .= chr(10) . MediamapQuery::findMediaFile($iBridgeUID, $oMediaMap->getThumburl(), 'thumbs');
				//unlink(MediamapPeer::findMediaFile($iBridgeUID, $oMediaMap->getThumburl(), 'thumbs'));

				@file_put_contents($sFile, $sDeleteThese); // _safely

			} // if has mediaMap

		} // if has mediaUID

		// now children if has
		$aChildren = explode(',', $oNote->getChildren());
		foreach ($aChildren as $iChild) {

			$iChild = intval($iChild);
			Notes::deleteNoteWithAttachmentAndChildrenByPK($iChild);

		} // foreach check each child

		// last delete the note
		$oNote->delete();

    } // deleteNoteWithAttachmentAndChildren


    static function deleteNoteWithAttachmentAndChildrenByPK($iUID) {

    	Notes::deleteNoteWithAttachmentAndChildren(
    			NotesQuery::create()->findByPk($iUID));

    } // deleteNoteWithAttachmentAndChildrenByPK


	function save(ConnectionInterface $oConnection = null) {

		if ($this->isNew()) {

			$this->setDate(time());

			$oAuthInfo = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();

			// if super user is posting/saveing instantly publish
			if ($oAuthInfo->isSuperUser()) {
				$this->setPublish(1);
				$this->setKarma(200);
			} else {
				$this->setPublish(0);
				$this->setKarma(100);
			} // if super user or any other

		} // if new note

		parent::save($oConnection);

	} // save


	function validateChildren(&$sValue, &$bEdited, &$aErrors) {

        $sValueCached = $sValue;
		$aNew = array();
		$a = explode(',', $sValue);
		foreach ($a as $iChild) {

			$iChild = intval($iChild);
			// TODO: check if children actually exist
			//if (exists in Notes)
				$aNew[] = $iChild;

		} // foreach check each child

		$sValue = implode(',', $aNew);

		$bEdited = ($sValueCached !== $sValue);

		return true;

	} // validateChildren


	function validateCountry(&$sValue, &$bEdited, &$aErrors) {

        $sValueCached = $sValue;
		$sValue = strtoupper(trim($sValue));
    	if (empty($sValue)) {

    		$sValue = 'CH';

    	} else {

    		if (!in_array($sValue, SssSCountries::countryIDs())) {

    			$sValue = 'CH';

    		} // if not a valid country

    	} // if MT

		$bEdited = ($sValueCached !== $sValue);

    	return true;

	} // validateCountry


	function validateEmail(&$sValue, &$bEdited, &$aErrors) {

		$bOk = true;
        $sValueCached = $sValue;
		$sValue = trim($sValue);

		if (!empty($sValue)) {

			if (0 == preg_match('/^[^@]+@[a-z0-9._-]+\.[a-z]+$/i', $sValue)) {

				$bOk = false;
				$aErrors[] = new WFError(SssSBla::bla('NoteInvalidEmail'));

			} // if not valid email

		} // if not empty email

    	$bEdited = ($sValueCached !== $sValue);

		return $bOk;

	} // validateEmail


	function validateHandle(&$sValue, &$bEdited, &$aErrors) {

        $sValueCached = $sValue;
		$sValue = trim($sValue);
    	if (empty($sValue)) {

    		$sValue = strval(WFAuthorizationInfo::NO_USER);

    	} else {

    		$sValue = htmlspecialchars($sValue, ENT_QUOTES);

    	} // if MT

    	$bEdited = ($sValueCached !== $sValue);

    	return true;

	} // validateHandle


	function validateLang(&$sValue, &$bEdited, &$aErrors) {

        $sValueCached = $sValue;
		$sValue = strtolower(trim($sValue));
    	if (empty($sValue)) {

    		$sValue = SssSBla::defaultLanguage();

    	} else {

    		if (!in_array($sValue, SssSBla::defaultLanguages())) {

    			$sValue = SssSBla::defaultLanguage();

    		} // if not a valid language

    	} // if MT

		$bEdited = ($sValueCached !== $sValue);

    	return true;

	} // validateLang


	function validateName(&$sValue, &$bEdited, &$aErrors) {

        $sValueCached = $sValue;
		$sValue = trim($sValue);
    	if (empty($sValue)) {

    		$sValue = SssSBla::bla('NoteNoName', array('noEdit' => true));

    	} else {

    		$sValue = htmlspecialchars($sValue, ENT_QUOTES);

    	} // if MT

    	$bEdited = ($sValueCached !== $sValue);

    	return true;

	} // validateName


	function validateNote(&$sValue, &$bEdited, &$aErrors) {

		$bOk = true;
        $sValueCached = $sValue;
		$sValue = trim($sValue);
    	if (empty($sValue)) {

    		$aErrors[] = new WFError(SssSBla::bla('NoteNoteMT'));

    		$bOk = false;

    	} else {

    		$sValue = htmlspecialchars($sValue, ENT_QUOTES);

    	} // if MT

    	$bEdited = ($sValueCached !== $sValue);

    	return $bOk;

	} // validateNote


	function validateRegion(&$sValue, &$bEdited, &$aErrors) {

        $sValueCached = $sValue;
    	if (empty($sValue)) {

    		$sValue = 'unknown';

    	} else {

    		$sValue = htmlspecialchars($sValue, ENT_QUOTES);

    	} // if MT or not

    	$bEdited = ($sValueCached !== $sValue);

    	return true;

    } // validateRegion


    function validateUrl0(&$sValue, &$bEdited, &$aErrors) {

		$bOk = true;
        $sValueCached = $sValue;
		$sValue = trim($sValue);

    	if (!empty($sValue)) {

    		if ('http' != substr($sValue, 0, 4)) {

                $sValue = 'http://' . $sValue;

            } // if no leading http

			if (0 == preg_match('|^http(s)?://[a-z0-9._-]+\.[a-z]+(:[0-9]+)?(/.*)?$|i', $sValue)) {

				$bOk = false;
				$aErrors[] = new WFError( SssSBla::bla('NoteInvalidUrl0'));

			} // if not valid email

    	} // if MT

    	$bEdited = ($sValueCached !== $sValue);

    	return $bOk;

    } // validateUrl0

} // Notes
