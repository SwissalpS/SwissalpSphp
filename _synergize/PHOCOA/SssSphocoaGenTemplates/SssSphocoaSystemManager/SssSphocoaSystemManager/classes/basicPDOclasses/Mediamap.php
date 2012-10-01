<?php

require 'basicPDOclasses/om/BaseMediamap.php';


/**
 * Skeleton subclass for representing a row from the 'mediaMap' table.
 *
 *
 *
 * You should add additional methods to this class to meet the
 * application requirements.  This class will only be generated as
 * long as it does not already exist in the output directory.
 *
 * @package    basicPDOclasses
 */
class Mediamap extends BaseMediamap {

	/**
	 * Initializes internal state of Mediamap object.
	 * @see        parent::__construct()
	 */
	public function __construct() {
		// Make sure that parent constructor is always invoked, since that
		// is where any default values for this object are set.
		parent::__construct();

	} // __construct()

	function amYouTube() {

		return (null !== SssSgdImageBasic::parseYouTubeURL4id($this->url)) ? true : false;

	} // amYouTube

	function amUploadedMedia() {

		list($sBridgeUID, $sIdentifier) = MediamapPeer::parseIdentifier($this->url);
		return (null !== $sIdentifier) ? true : false;

	} // amUploadedMedia

	protected function _imgDealWithYouTubeThumb() {

		$aReturn = array('path' => false, 'url' => false);

		if ($sYTid = SssSgdImageBasic::parseYouTubeURL4id($this->url)) { //$this->amYouTube()) {

			$aReturn['url'] = 'http://img.youtube.com/vi/' . $sYTid . '/' . rand(0, 3) . '.jpg';

			return $aReturn;

		} // if youtube

		return null;

	} // _imgDealWithYouTubeThumb

	function imgThumbOrBigger() {

		if ($mRes = $this->_imgDealWithYouTubeThumb()) return $mRes;

		$aInfo = $this->getInfoArray();

		if (isset($aInfo['thumbPath'], $aInfo['thumbURL'])) return array('path' => $aInfo['thumbPath'], 'url' => $aInfo['thumbURL']);

		// image?
		if (isset($aInfo['bigPath'], $aInfo['bigURL'])) {

			$aPinfo = pathinfo($aInfo['bigPath']);
			$sExt = strToLower($aPinfo['extension']);

			if (in_array($sExt, SssSgdImageBasic::imageExtensions()))
				return array('path' => $aInfo['bigPath'], 'url' => $aInfo['bigURL']);

		} // if got big

		// image?
		if (isset($aInfo['origPath'], $aInfo['origURL'])) {

			$aPinfo = pathinfo($aInfo['origPath']);
			$sExt = strToLower($aPinfo['extension']);

			if (in_array($sExt, SssSgdImageBasic::imageExtensions()))
				return array('path' => $aInfo['origPath'], 'url' => $aInfo['origURL']);

		} // if got big

		// otherwise return replacement thumb
		$aR = array();
		list($aR['path'], $aR['url']) = MediamapPeer::replacementThumbPathAndUrl($this->mime);
		return $aR;

	} // imgThumbOrBigger

	function imgBigOrBigger() {

		if ($mRes = $this->_imgDealWithYouTubeThumb()) return $mRes;

		$aInfo = $this->getInfoArray();

		// image?
		if (isset($aInfo['bigPath'], $aInfo['bigURL'])) {

			$aPinfo = pathinfo($aInfo['bigPath']);
			$sExt = strToLower($aPinfo['extension']);

			if (in_array($sExt, SssSgdImageBasic::imageExtensions()))
				return array('path' => $aInfo['bigPath'], 'url' => $aInfo['bigURL']);

		} // if got big

		// image?
		if (isset($aInfo['origPath'], $aInfo['origURL'])) {

			$aPinfo = pathinfo($aInfo['origPath']);
			$sExt = strToLower($aPinfo['extension']);

			if (in_array($sExt, SssSgdImageBasic::imageExtensions()))
				return array('path' => $aInfo['origPath'], 'url' => $aInfo['origURL']);

		} // if got big

		// otherwise return replacement thumb
		$aR = array();
		list($aR['path'], $aR['url']) = MediamapPeer::replacementThumbPathAndUrl($this->mime);
		return $aR;} // imgBigOrBigger

	function imgBigOrSmaller() {

		if ($mRes = $this->_imgDealWithYouTubeThumb()) return $mRes;

		$aInfo = $this->getInfoArray();

		// image?
		if (isset($aInfo['bigPath'], $aInfo['bigURL'])) {

			$aPinfo = pathinfo($aInfo['bigPath']);
			$sExt = strToLower($aPinfo['extension']);

			if (in_array($sExt, SssSgdImageBasic::imageExtensions()))
				return array('path' => $aInfo['bigPath'], 'url' => $aInfo['bigURL']);

		} // if got big

		if (isset($aInfo['thumbPath'], $aInfo['thumbURL'])) return array('path' => $aInfo['thumbPath'], 'url' => $aInfo['thumbURL']);

		// otherwise return replacement thumb
		$aR = array();
		list($aR['path'], $aR['url']) = MediamapPeer::replacementThumbPathAndUrl($this->mime);

		return $aR;

	} // imgBigOrSmaller

	protected function _amUploadedMediaAndExistAt($sIdentifierRaw, $sSize = null) {

		list($sBridgeUID, $sIdentifier) = MediamapPeer::parseIdentifier($sIdentifierRaw);

		if (null === $sIdentifier) return false;

		return MediamapPeer::findMediaFile($sBridgeUID, $sIdentifier, $sSize);

	} // amUploadedMediaAndExistAt

	function amUploadedMediaAndExistAt() {

		return $this->_amUploadedMediaAndExistAt($this->url);

	} // amUploadedMediaAndExistAt

	function amUploadedMediaAndThumbExistAt() {

		return $this->_amUploadedMediaAndExistAt($this->thumburl, 'thumbs');

	} // amUploadedMediaAndThumbExistAt

	function amUploadedMediaAndBigExistAt() {

		return $this->_amUploadedMediaAndExistAt($this->bigurl, 'big');

	} // amUploadedMediaAndThumbExistAt

/*
	function getOrigURL() {

	} // getOrigURL

	function getThumbURL() {

	} // getThumbURL

	function getOrigPath() {

	} // getOrigPath

	function getThumbPath() {

	} // getThumbPath

	function getThumbName() {

	} // getThumbName

	function getOrigName() {

	} // getOrigName
*/
	function getInfoArray() {

		/*static $aInfo = null;
		if ($aInfo) return $aInfo;*/

		$aInfo = array(
			'origURL' => null,
			'thumbURL' => null,
			'bigURL' => null,
			'origPath' => null,
			'thumbPath' => null,
			'bigPath' => null,
			'identifier' => null,
			'bridgeUID' => null,
			'origWidth' => null,
			'origHeight' => null,
			'origMime' => null,
			'bigWidth' => null,
			'bigHeight' => null,
			'bigMime' => null,
			'thumbWidth' => null,
			'thumbHeight' => null,
			'thumbMime' => null
		);

		list($sBridgeUID, $sIdentifier) = MediamapPeer::parseIdentifier($this->url);
		if (null === $sIdentifier) {
			// not an identifier, treat as url
			// if it has sheme, it probably also has domain etc
			// otherwise it's local/on media server
			$aURL = parse_url($this->url);
			// TODO

		} else {

			$aInfo['identifier'] = $sIdentifier;
			$aInfo['bridgeUID'] = $sBridgeUID;

			$sPath = MediamapPeer::findMediaFile($sBridgeUID, $this->url);

			if ($sPath) {

				$aInfo['origPath'] = $sPath;
				$sFileName = basename($sPath);
				$aInfo['origURL'] = 'http://' . MEDIA_DOWNLOAD_HOST . '/bridge/' . $sBridgeUID . '/original/' . $sFileName;
				if (0 < $this->width) $aInfo['origWidth'] = $this->width;
				if (0 < $this->height) $aInfo['origHeight'] = $this->width;
				$aInfo['origMime'] = $this->mime;

				$sPathBig = MediamapPeer::findMediaFile($sBridgeUID, $this->bigurl, 'big');
				if ($sPathBig) {

					$aInfo['bigPath'] = $sPathBig;
					$sFileNameBig = basename($sPathBig);
					$aInfo['bigURL'] = 'http://' . MEDIA_DOWNLOAD_HOST . '/bridge/' . $sBridgeUID . '/big/' . $sFileNameBig;
//WFLog::logToFile('mediamap.log', 'sPathBig: ' . $sPathBig . ' bigURL: ' . $aInfo['bigURL']);

				} // if found big version

				$sPathThumb = MediamapPeer::findImageFile($sBridgeUID, $this->thumburl, 'thumbs');

				if ($sPathThumb) {

					$aInfo['thumbPath'] = $sPathThumb;
					$sFileNameThumb = basename($sPathThumb);
					$aInfo['thumbURL'] = 'http://' . MEDIA_DOWNLOAD_HOST . '/bridge/' . $sBridgeUID . '/thumbs/' . $sFileNameThumb;

				} else {

					// get a replacement thumbnail showing the file type
					list($aInfo['thumbPath'], $aInfo['thumbURL']) = MediamapPeer::replacementThumbPathAndUrl($this->mime);

				} // if found thumbnail version

			} // if file found

		} // if url is identifier

		return $aInfo;

	} // getInfoArray

} // Mediamap
