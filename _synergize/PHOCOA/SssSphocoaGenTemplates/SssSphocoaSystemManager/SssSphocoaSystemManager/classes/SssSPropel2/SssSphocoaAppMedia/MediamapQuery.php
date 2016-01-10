<?php

namespace SssSPropel2\SssSphocoaAppMedia;

use SssSPropel2\SssSphocoaAppMedia\Base\MediamapQuery as BaseMediamapQuery;

use SwissalpS\Graphics\GDimageBasic as SssSgdImageBasic;

/**
 * Skeleton subclass for performing query and update operations on the 'mediaMap' table.
 *
 *
 *
 * You should add additional methods to this class to meet the
 * application requirements.  This class will only be generated as
 * long as it does not already exist in the output directory.
 *
 */
class MediamapQuery extends BaseMediamapQuery {


	static function findImageFile($sBridgeUID, $s, $sSize = 'original') {

		$sPath = MEDIA_DOWNLOAD_DIR . DIR_SEP . 'bridge' . DIR_SEP . $sBridgeUID . DIR_SEP . $sSize . DIR_SEP . $s;

//WFLog::log($sPath);

		if (is_file($sPath)) {

            return $sPath;

        } // if path found

		$sPath = SssSgdImageBasic::removeImageSuffix($sPath);

		$aExtensions = SssSgdImageBasic::imageExtensions();
		foreach ($aExtensions as $sExt) {
//WFLog::log($sPath . '.' . $sExt);

			if (is_file($sPath . '.' . $sExt)) {

                return $sPath . '.' . $sExt;

            } // if found an extension that exists

		} // foreach extension

return null;

		// if not found by filepath, try by url
		$sPath = 'http://' . MEDIA_DOWNLOAD_HOST . '/bridge/' . $sBridgeUID
				. '/' . $sSize . '/' . SssSgdImageBasic::removeImageSuffix($s);

		foreach ($aExtensions as $sExt) {

			if (is_file($sPath . '.' . $sExt)) {

                return $sPath . '.' . $sExt;

            } // if found an extension that exists

//WFLog::log($sPath . '.' . $sExt);
		} // foreach extension

		return null;

	} // findImageFile


	static function findMediaFile($sBridgeUID, $s, $sSize = 'original') {

		$sPath = MEDIA_DOWNLOAD_DIR . DIR_SEP . 'bridge' . DIR_SEP . $sBridgeUID . DIR_SEP . $sSize . DIR_SEP . $s;

		if (is_file($sPath)) {

            return $sPath;

        } // if path found

		$sPath = SssSgdImageBasic::removeMediaSuffix($sPath);

		$aExtensions = SssSgdImageBasic::mediaExtensions();
		foreach ($aExtensions as $sExt) {

			if (is_file($sPath . '.' . $sExt)) {

                return $sPath . '.' . $sExt;

            } // if path found

		} // foreach extension

return null;
		// if not found by filepath, try by url
		$sPath = 'http://' . MEDIA_DOWNLOAD_HOST . '/bridge/' . $sBridgeUID
				. '/' . $sSize . '/' . SssSgdImageBasic::removeMediaSuffix($s);

		foreach ($aExtensions as $sExt) {

			if (is_file($sPath . '.' . $sExt)) {

                return $sPath . '.' . $sExt;

            } // if path found

//WFLog::log($sPath . '.' . $sExt);
		} // foreach extension

		return null;

	} // findMediaFile


	// return array('path' => ..., 'url' => ...) or empty array
	static function imgBigOrBiggerForBridgeUID($iUID) {

		$aNotes = NotesQuery::notesPopularThumbnailsForBridgeUID($iUID, 1);

		if (empty($aNotes)) {

            return array();

        } // if no thumbnails found for bridge

		$oMediaMap = self::create()->findPk($aNotes[0]->getMediauid());

		return $oMediaMap->imgBigOrBigger();

	} // imgBigOrBiggerForBridgeUID


	// return array('path' => ..., 'url' => ...) or empty array
	static function imgBigOrSmallerForBridgeUID($iUID) {

		$aNotes = NotesQuery::notesPopularThumbnailsForBridgeUID($iUID, 1);

		if (empty($aNotes)) {

            return array();

        } // if no notes

		$oMediaMap = self::create()->findPk($aNotes[0]->getMediauid());

		return $oMediaMap->imgBigOrSmaller();

	} // imgBigOrSmallerForBridgeUID


	// return array('path' => ..., 'url' => ...) or empty array
	static function imgThumbOrBiggerForBridgeUID($iUID) {

		$aNotes = NotesQuery::notesPopularThumbnailsForBridgeUID($iUID, 1);

		if (empty($aNotes)) {

            return array();

        } // if no notes found

		$oMediaMap = self::create()->findPk($aNotes[0]->getMediauid());

		return $oMediaMap->imgThumbOrBigger();

	} // imgThumbOrBiggerForBridgeUID


	static function parseIdentifier($value) {

		if (!$value) {

            return NULL;

        } // if no proper value given

		$sPattern = '/^swissrope_([0-9]+)_[0-9]+_[0-9]+(\.[a-z0-9]+)?$/i';
		$aMatches = array();
		$iFound = preg_match($sPattern, $value, $aMatches);
		if (1 === $iFound) {
			// is an identifier
			// for this bridge uid
			$sBridgeUID = $aMatches[1];
			$sIdentifier = SssSgdImageBasic::removeImageSuffix($value);

			return array($sBridgeUID, $sIdentifier);

		} // if

		return array(null, null);

	} // parseIdentifier


	static function replacementThumbPathAndUrl($sMime) {

		$sFileName = 'fileTypeUnknown.png';
// http://www.w3schools.com/media/media_mimeref.asp
		switch ($sMime) {

			case 'application/pdf' :
				$sFileName = 'fileTypeThumbPDF.png';
				break;

			case 'application/octet-stream' :
				$sFileName = 'fileTypeThumbApp.png';
				break;

			case 'application/futuresplash' :
				$sFileName = 'fileTypeThumbSPL.png';
				break;

			case 'application/x-shockwave-flash' :
				$sFileName = 'fileTypeThumbSWF.png';
				break;

			case 'audio/mpeg' :
				$sFileName = 'fileTypeThumbMP3.png';
			//	break;
			case 'audio/x-mpegurl' :
				$sFileName = 'fileTypeThumbM3U.png';
			//	break;
			case 'audio/x-ms-wma' :
				$sFileName = 'fileTypeThumbWMA.png';
			//	break;
			case 'audio/x-ms-wax' :
				$sFileName = 'fileTypeThumbWAX.png';
			//	break;
			case 'application/ogg' :
				$sFileName = 'fileTypeThumbOGG.png';
			//	break;
			case 'audio/x-wav' :
				$sFileName = 'fileTypeThumbWAV.png';
			//	break;
			case 'audio/x-aiff' :
				$sFileName = 'fileTypeThumbAIFF.png';
				$sFileName = 'fileTypeThumbAudio.png';
				break;

			case 'image/gif' :
				$sFileName = 'fileTypeThumbGIF.png';
			//	break;
			case 'image/jpeg' :
				$sFileName = 'fileTypeThumbJPG.png';
			//	break;
			case 'image/png' :
				$sFileName = 'fileTypeThumbPNG.png';
			//	break;
			case 'image/x-xbitmap' :
				$sFileName = 'fileTypeThumbXBM.png';
			//	break;
			case 'image/x-xpixmap' :
				$sFileName = 'fileTypeThumbXPM.png';
			//	break;
			case 'image/x-xwindowdump' :
				$sFileName = 'fileTypeThumbXWD.png';
			//	break;
			case 'image/x-icon' :
				$sFileName = 'fileTypeThumbICO.png';
				$sFileName = 'fileTypeThumbImage.png';
				break;

			case 'application/postscript' :
				$sFileName = 'fileTypeThumbPS.png';
			//	break;
			case 'application/x-bittorrent' :
				$sFileName = 'fileTypeThumbTORRENT.png';
			//	break;
			case 'application/x-ns-proxy-autoconfig' :
				$sFileName = 'fileTypeThumbPAC.png';
			//	break;

			case 'text/css' :
				$sFileName = 'fileTypeThumbCSS.png';
			//	break;
			case 'text/html' :
				$sFileName = 'fileTypeThumbHTML.png';
			//	break;
			case 'text/javascript' :
				$sFileName = 'fileTypeThumbJS.png';
			//	break;
			case 'text/xml' :
				$sFileName = 'fileTypeThumbXML.png';
			//	break;
			case 'text/plain' :
				$sFileName = 'fileTypeThumbTXT.png';
				$sFileName = 'fileTypeThumbText.png';
				break;

			case 'application/x-dvi' :
				$sFileName = 'fileTypeThumbDVI.png';
			//	break;

			case 'video/mpeg' :
				$sFileName = 'fileTypeThumbMPG.png';
			//	break;
			case 'video/quicktime' :
				$sFileName = 'fileTypeThumbQT.png';
			//	break;
			case 'video/x-msvideo' :
				$sFileName = 'fileTypeThumbAVI.png';
			//	break;
			case 'video/x-ms-asf' :
				$sFileName = 'fileTypeThumbASF.png';
			//	break;
			case 'video/x-ms-wmv' :
				$sFileName = 'fileTypeThumbWMV.png';
				$sFileName = 'fileTypeThumbVideo.png';
				break;

			case 'application/pgp-signature' :
				$sFileName = 'fileTypeThumbPGP.png';
			//	break;
			case 'application/x-gzip' :
				$sFileName = 'fileTypeThumbGZ.png';
			//	break;
			case 'application/x-tgz' :
				$sFileName = 'fileTypeThumbTGZ.png';
			//	break;
			case 'application/x-tar' :
				$sFileName = 'fileTypeThumbTAR.png';
			//	break;
			case 'application/zip' :
				$sFileName = 'fileTypeThumbZIP.png';
			//	break;
			case 'application/x-bzip' :
				$sFileName = 'fileTypeThumbBZ2.png';
			//	break;
			case 'application/x-bzip-compressed-tar' :
				$sFileName = 'fileTypeThumbTBZ.png';
				$sFileName = 'fileTypeThumbArchive.png';
				break;

			default :
				$sFileName = 'fileTypeUnknown.png';

		} // switch

		$sPathThumb = MEDIA_DOWNLOAD_DIR . DIR_SEP . 'shared' . DIR_SEP . $sFileName;

		if (!is_file($sPathThumb)) {

			$sFileName = 'fileTypeUnknown.png';
			$sPathThumb = MEDIA_DOWNLOAD_DIR . DIR_SEP . 'shared' . DIR_SEP . $sFileName;

		} // fallback to something

		$sURLThumb = 'http://' . MEDIA_DOWNLOAD_HOST . '/shared/' . $sFileName;

		return array($sPathThumb, $sURLThumb);

	} // replacementThumbPathAndUrl


	// @depricated use imgThumbOrBiggerForBridgeUID
	// return array('path' => ..., 'url' => ...) or empty array
	static function thumbnailForBridgeUID($iUID) {

		return self::imgThumbOrBiggerForBridgeUID($iUID);

	} // thumbnailForBridgeUID

} // MediamapQuery
