<?php
/* * * *
 * * SssSgdImageBasic.inc
 * *
 * * @version 20100907_124717 (CC) Luke JZ aka SwissalpS
 * * * */
//error_reporting(E_ALL); // 0); //

class SssSgdImageBasic extends WFObject {

	// the gdimage resource holder
	protected $rImage;

	protected $sPathFile;

	// getimagesize info
	protected $aInfo;

	function __construct($sPathFile = null) {

		parent::__construct();

		$this->rImage = false;
		$this->sPathFile = null;
		$this->aInfo = false;

		if ($sPathFile) $this->load($sPathFile);

	} // __construct

	function __destruct() {

		if (is_resource($this->rImage)) imageDestroy($this->rImage);

	} // __destruct

	static function bestAvailableSaveType($iType) {

		do {

			switch($iType) {

				// TODO: GD and GD2 format
				case IMAGETYPE_BMP :
				case IMAGETYPE_GIF :
				case IMAGETYPE_WBMP :
					$iType = $iType;
					break;
				//case IMAGETYPE_XPM :
				case IMAGETYPE_XBM :
					$iType = IMAGETYPE_XBM;
					break;
				case IMAGETYPE_JB2 :
				case IMAGETYPE_JP2 :
				case IMAGETYPE_JPC :
				case IMAGETYPE_JPX :
				case IMAGETYPE_SWC :
				case IMAGETYPE_SWF :
				case IMAGETYPE_JPEG :
					$iType = IMAGETYPE_JPEG;
					break;
				case IMAGETYPE_TIFF_II :
				case IMAGETYPE_TIFF_MM :
				case IMAGETYPE_IFF :
				case IMAGETYPE_PSD :
				case IMAGETYPE_PNG :
				default :
					$iType = IMAGETYPE_PNG;
					break;

			} // switch image type

		} while(!(imagetypes() & $iType));

		return $iType;

	} // bestAvailableSaveType

	function defaultImageType() { return IMAGETYPE_JPEG; } // defaultImageType

	function dup($rImage = null) {

		if (!is_resource($rImage) && is_resource($this->rImage))
			$rImage = $this->rImage;
		else {
			// TODO: check if file path is given
			throw(new WFError('no image resource passed'));
			return null;
		}

		return SssSgdImageBasic::duplicate($rImage);

	} // dup

	static function mergeImageFilesToAs($aPaths, $sTargetPath, $iTargetType = null) {

		$aImages = array();
		foreach($aPaths as $sPathFile) {

			$oImage = new SssSgdImageBasic($sPathFile);
			if (is_resource($oImage->getImage())) {

				$aImages []= $oImage;

			} // skip invalid

		} // foreach

		$iCount = count($aImages);
		if (0 == $iCount) return null;

		$oFirstImage = array_shift($aImages);

		if (1 == $iCount)
			return $oFirstImage->saveTo($sTargetPath, $iTargetType);

		$iW = $oFirstImage->width();
		$iH = $oFirstImage->height();

		$fRatio = $iW / $iH;

		$rBackground = $oFirstImage->getImage();// imageCreateTrueColor($iW, $iH);
		//imageAlphaBlending($rBackground, true);
		//imageCopy($rBackground, $oFirstImage->getImage(), 0, 0, 0, 0, $iW, $iH);


		$aSettings = array(
				'resize' => 'center', // 'skew' 'cut' 'center'
				'opacity' => 100, // 0-100
				);

		foreach ($aImages as $oImage) {

			$fRatioLayer = $oImage->width() / $oImage->height();

			if ($fRatioLayer > $fRatio) {
				// height will be smaller than background
				switch ($aSettings['resize']) {

					case 'skew' : // either skew
						$oImage->resize($iW, $iH);
						$iXd = $iYd = $iXs = $iYs = 0;
						$iWs = $iW; $iHs = $iH;
					break;

					case 'cut' : // or cut off some width
						// fit to height
						$oImage->resize(null, $iH);
						$iXd = $iYd = $iYs = 0;
						$iXs = intval(($oImage->width() - $iW) * 0.5);
						$iWs = $iW; $iHs = $iH;
					break;

					case 'center' : // or have some background look thru top & bottom
					default :
						// fit to width
				 		$oImage->resize($iW);
						$iYd = intval(($oImage->height() - $iH) * 0.5);
						$iXd = $iXs = $iYs = 0;
						$iWs = $iW; $iHs = $oImage->height();
					break;

				} // switch resize setting

			} else {
				// either same ratio or
				// width will be smaller than background
				switch ($aSettings['resize']) {

					case 'skew' : // either skew
						$oImage->resize($iW, $iH);
						$iXd = $iYd = $iXs = $iYs = 0;
						$iWs = $iW; $iHs = $iH;
					break;

					case 'cut' : // or cut off some height
						// fit to width
				 		$oImage->resize($iW);
						$iXd = $iYd = $iXs = 0;
						$iYs = intval(($oImage->height() - $iH) * 0.5);
						$iWs = $iW; $iHs = $iH;
					break;

					case 'center' : // or have some background look thru left & right
					default :
						// fit to height
						$oImage->resize(null, $iH);
						$iXd = intval(($oImage->width() - $iW) * 0.5);
						$iYd = $iXs = $iYs = 0;
						$iWs = $oImage->width(); $iHs = $iH;
					break;

				} // switch resize setting

			} // if mismatched size

			// TODO: test png opacity!!
/*			if (IMAGETYPE_GIF == $oImage->imageType()
					|| IMAGETYPE_PNG == $oImage->imageType()) {

				$iTransparencyIndex = imageColorTransparent($oImage->getImage());

				// If we have a specific transparent color
				if (0 <= $iTransparencyIndex) {

					// Get the original image's transparent color's RGB values
					$aTransColour = imageColorsForIndex($oImage->getImage(), $iTransparencyIndex);

					// Allocate the same color in the new image resource
					$iTransparencyIndex = imageColorAllocate($oFirstImage->getImage(), $aTransColour['red'], $aTransColour['green'], $aTransColour['blue']);

					// Completely fill the background of the new image with allocated color.
					//imageFill($rImageNew, 0, 0, $iTransparencyIndex);

					// Set the background color for new image to transparent
					imageColorTransparent($oFirstImage->getImage(), $iTransparencyIndex);

				} elseif (IMAGETYPE_PNG == $oImage->imageType()) {
					// Always make a transparent background color for PNGs that don't have one allocated already

					// Turn off transparency blending (temporarily)
					imageAlphaBlending($oFirstImage->getImage(), false);

					// Create a new transparent color for image
					$iTransparencyIndex = imageColorAllocateAlpha($oFirstImage->getImage(), 0, 0, 0, 127);

					// Completely fill the background of the new image with allocated color.
					//imageFill($rImageNew, 0, 0, $iTransparencyIndex);

					// Restore transparency blending
					imageSaveAlpha($oFirstImage->getImage(), true);

				} // if transparency given

			} // if need to take care of transparency (gif or png)
*****/
			//imageSaveAlpha($oFirstImage->getImage(), false);
			//imageSaveAlpha($oFirstImage->getImage(), true);
			imageAlphaBlending($oImage->getImage(), true);
			imageCopyMerge($rBackground, $oImage->getImage(), $iXd, $iYd, $iXs, $iYs, $iWs, $iHs, $aSettings['opacity']);

			//imageAlphaBlending($oFirstImage->getImage(), false);
			//imageSaveAlpha($oFirstImage->getImage(), true);
			unset($oImage);

		} // foreach

		//imageAlphaBlending($oFirstImage->getImage(), false);
		//imageSaveAlpha($oFirstImage->getImage(), true);

		//$oFirstImage->setImage($rBackground);
		return $oFirstImage->saveTo($sTargetPath, $iTargetType);

	} // mergeImageFilesToAs

	static function duplicate($rImage) {

		if (!is_resource($rImage)) {
			// TODO: check if file path is given
			throw(new WFError('no image resource passed'));
			return null;
		}

		$iW = imagesx($rImage);
		$iH = imagesy($rImage);

		$rImageClone = imageCreateTrueColor($iW, $iH);
		// TODO: analize transparency and alpha settings and clone those too

		$bRes = imageCopy($rImageClone, $rImage, 0, 0, 0, 0, $iW, $iH);

		if ($bRes) return $rImageClone;

		imageDestroy($rImageClone);

		throw(new WFError('image duplication failed'));
		return null;

	} // duplicate

	function getPathFile() {

		return $this->sPathFile;

	} // getPathFile

	function getImage() {

		return $this->rImage;

	} // getImage

	function height() {

		if (!$this->rImage) return 0;

		return imagesy($this->rImage);

	} // height

	function imageCreateFunctionForImageType($iType) {

		switch($iType) {

			case IMAGETYPE_BMP :
				return (function_exists('imageCreateFromBMP'))
						? 'imageCreateFromBMP' : null;
			case IMAGETYPE_GIF :
				return (function_exists('imageCreateFromGIF'))
						? 'imageCreateFromGIF' : null;
			case IMAGETYPE_IFF :
				return (function_exists('imageCreateFromIFF'))
						? 'imageCreateFromIFF' : null;
			case IMAGETYPE_JB2 :
				return (function_exists('imageCreateFromJB2'))
						? 'imageCreateFromJB2' : null;
			case IMAGETYPE_JP2 :
				return (function_exists('imageCreateFromJP2'))
						? 'imageCreateFromJP2' : null;
			case IMAGETYPE_JPC :
				return (function_exists('imageCreateFromJPC'))
						? 'imageCreateFromJPC' : null;
			case IMAGETYPE_JPEG :
				return (function_exists('imageCreateFromJPEG'))
						? 'imageCreateFromJPEG' : null;
			case IMAGETYPE_JPX :
				return (function_exists('imageCreateFromJPX'))
						? 'imageCreateFromJPX' : null;
			case IMAGETYPE_PNG :
				return (function_exists('imageCreateFromPNG'))
						? 'imageCreateFromPNG' : null;
			case IMAGETYPE_PSD :
				return (function_exists('imageCreateFromPSD'))
						? 'imageCreateFromPSD' : null;
			case IMAGETYPE_SWC :
				return (function_exists('imageCreateFromSWC'))
						? 'imageCreateFromSWC' : null;
			case IMAGETYPE_SWF :
				return (function_exists('imageCreateFromSWF'))
						? 'imageCreateFromSWF' : null;
			case IMAGETYPE_TIFF_II :
			case IMAGETYPE_TIFF_MM :
				return (function_exists('imageCreateFromTIFF'))
						? 'imageCreateFromTIFF' : null;
			case IMAGETYPE_WBMP :
				return (function_exists('imageCreateFromWBMP'))
						? 'imageCreateFromWBMP' : null;
			case IMAGETYPE_XBM :
				return (function_exists('imageCreateFromXBM'))
						? 'imageCreateFromXBM' : null;
			case IMAGETYPE_XPM :
				return (function_exists('imageCreateFromXPM'))
						? 'imageCreateFromXPM' : null;
			// TODO: GD and GD2 format
			default : return $this->imageCreateFunctionForImageType($this->defaultImageType());

		} // switch image type

	} // imageCreateFunctionForImageType

	static function audioExtensions() {

		static $a = null; if (!$a) $a = array(
			'au', 'snd', 'mid', 'rmi', 'mp3', 'aiff',
			'aif', 'aifc', 'm3u', 'ra', 'ram', 'wav'
			);

		return $a;

	} // audioExtensions

	static function imageExtensions() {

		static $a = null; if (!$a) $a = array(
			'gif', 'jpeg', 'jpg', 'png', 'swf', 'psd', 'bmp', 'tiff', 'jpc',
			'jp2', 'jpf', 'jb2', 'swc', 'wbmp', 'xbm'
			);

		return $a;

	} // imageExtensions

	static function mediaExtensions() {

		static $a = null; if (!$a) $a = array_merge(
				SssSgdImageBasic::imageExtensions(),
				SssSgdImageBasic::videoExtensions(),
				SssSgdImageBasic::audioExtensions());

		return $a;

	} // mediaExtensions

	static function videoExtensions() {

		static $a = null; if (!$a) $a = array(
			'dvi', 'mvb', 'wmv', 'swf', 'mp2', 'mpa', 'mpe', 'mpeg', 'mpg', 'mpv2', 'mov', 'qt', 'lsf', 'lsx', 'asf', 'asr', 'asx', 'avi', 'movie'
			);

		return $a;

	} // audioExtensions

	function imageType() {

		if (empty($this->aInfo) || !isset($this->aInfo[2]))
				return $this->defaultImageType();

		return $this->aInfo[2];

	} // imageType

	function isLandscape() {

		return ($this->width() > $this->height());

	} // isLandscape

	function isPortrait() { return !$this->isLandscape(); } // isPortrait

	function load($sPathFile) {

		$aInfo = @getimagesize($sPathFile);

		if (false === $aInfo || empty($aInfo)) {

			//throw(new WFError('Either not an image or file does not exist: ' . $sPathFile . ' :'));
			return null;

		} // if error reading or not an image (GIF, JPG, PNG, SWF, SWC, PSD, TIFF, BMP, IFF, JP2, JPX, JB2, JPC, XBM, or WBMP)

		$this->aInfo = $aInfo;
		$this->sPathFile = $sPathFile;

		$oImage = @imageCreateFromString(file_get_contents($sPathFile));
		if ($oImage) $this->setImage($oImage);

/*
		$sCreateFunction = $this->imageCreateFunctionForImageType($this->imageType());
		$rImage = $sCreateFunction($sPathFile);
		if ($sCreateFunction) $this->setImage($rImage);
*/

	} // load

	function mime() {

		if (!$this->aInfo || !isset($this->aInfo['mime'])) return null;

		return $this->aInfo['mime'];

	} // mime

	static function parseYouTubeURL4id($sUrl, $bMustBeginWithURL = true) {

		if (empty($sUrl)) return null;

		$sPatternShare = ($bMustBeginWithURL)
			? ';^(http://)?(www.)?youtube.com/watch\?v=([^&/[:space:]]+)(.*)$;i'
			: ';(http://)?(www.)?youtube.com/watch\?v=([^&/[:space:]]+)(.*)$;i';
		if (1 === preg_match($sPatternShare, $sUrl, $aMatches)) return $aMatches[3];

		$aMatches = array();
		$sPatternEmbed = ($bMustBeginWithURL)
			? ';^(http://)?(www.)?youtube.com/v/([^\?/[:space:]]+)(.*)$;i'
			: ';(http://)?(www.)?youtube.com/v/([^\?/[:space:]]+)(.*)$;i';
		if (1 === preg_match($sPatternEmbed, $sUrl, $aMatches)) return $aMatches[3];

		$aMatches = array();
		$sPatternShort = ($bMustBeginWithURL)
			? ';^(http://)?(www.)?youtu.be/([^\?/[:space:]]+)(.*)$;i'
			: ';(http://)?(www.)?youtu.be/([^\?/[:space:]]+)(.*)$;i';
		if (1 === preg_match($sPatternShort, $sUrl, $aMatches)) return $aMatches[3];

		return null;

	} // parseYouTubeURL4id

	static function removeImageSuffix($sPath) {

		$sPath = trim($sPath);

		$sLastSuffix = strrchr($sPath, '.');

		if (false !== $sLastSuffix
				&& in_array(strToLower(substr($sLastSuffix, 1)),
						SssSgdImageBasic::imageExtensions())) {

			$sPath = substr($sPath, 0, -strlen($sLastSuffix));

		} // if an image extension, remove it

		return $sPath;

	} // removeImageSuffix

	static function removeMediaSuffix($sPath) {

		$sPath = trim($sPath);

		$sLastSuffix = strrchr($sPath, '.');

		if (false !== $sLastSuffix
				&& in_array(strToLower(substr($sLastSuffix, 1)),
						SssSgdImageBasic::mediaExtensions())) {

			$sPath = substr($sPath, 0, -strlen($sLastSuffix));

		} // if an image extension, remove it

		return $sPath;

	} // removeMediaSuffix

	function resize($iNewWidth = null, $iNewHeight = null) {

		$iWo = $this->width();
		$iHo = $this->height();

		if (0 >= $iWo || 0 >= $iHo) return null;

		if (0 < $iNewWidth && 0 < $iNewHeight) {
			// both set
			$fFactorX = $iNewWidth / (float)$iWo;
			$fFactorY = $iNewHeight / (float)$iHo;

		} elseif (0 < $iNewWidth) {
			// only width given
			$fFactorX = $fFactorY = $iNewWidth / (float)$iWo;

		} elseif (0 < $iNewHeight) {
			// only height given
			$fFactorX = $fFactorY = $iNewHeight / (float)$iHo;

		} else {
			// none given
			return null;

		} //

		return $this->scale($fFactorX, $fFactorY);

	} // resize

	function save() {

		return $this->saveTo($this->sPathFile);

	} // save

	static function saveImageAsType($rImage, $iType, $sPathFile) {

		if (!is_resource($rImage)) {
			throw(new WFError('needs to be a resource: arg 1'));
			return null;
		}

		$iType = self::bestAvailableSaveType($iType);

		if (!(imagetypes() & $iType)) {

			throw(new WFError('type not supported on this build'));
			return null;

		} // if not supported type on this build

		$sExt = image_type_to_extension($iType);

		switch($iType) {

			case IMAGETYPE_BMP : $bRes = imageBMP($rImage, $sPathFile . $sExt);
				break;

			case IMAGETYPE_GIF :
				$bRes = imageGIF($rImage, $sPathFile . $sExt);
				// imageColorTransparent();
				break;

			case IMAGETYPE_JPEG :
				$iQuality = 75; // 0-100
				$bRes = imageJPEG($rImage, $sPathFile . $sExt, $iQuality);
				break;

			case IMAGETYPE_PNG :
				$iQuality = 0; // (no compression) 0-9 (high compression)
				$iFilters = PNG_NO_FILTER; // PNG_ALL_FILTERS or any other PNG_FILTER_xxx combination
				//$rImage2 = SssSgdImageBasic::duplicate($rImage);
				//imageAlphaBlending($rImage2, false);
				//imageSaveAlpha($rImage2, true);
				$bRes = imagePNG($rImage, $sPathFile . $sExt, $iQuality);//, $iFilters);
				//imageDestroy($rImage2);
				break;

			case IMAGETYPE_WBMP :
				$iThreshold = 125; // 0-255
				$bRes = image2wbmp($rImage, $sPathFile . $sExt, $iThreshold);
				break;
				/*
				$iForeground = //imagecolorallocate();
				$bRes = imagewbmp($rImage, $sPathFile . $sExt, $iForeground);
				*/

			case IMAGETYPE_XBM :
				//$iForeground = //imagecolorallocate();
				$bRes = imageXBM($rImage, $sPathFile . $sExt); //, $iForeground);
				break;

			// TODO: GD and GD2 format

		} // switch image type

		return ($bRes) ? $sPathFile . $sExt : null;

	} // saveImageAsType

	function saveTo($sPathFile, $iType = null) {

		if (!$this->rImage) return null;

		// remove possible given suffix (if an image suffix)
		$sPathFile = SssSgdImageBasic::removeImageSuffix($sPathFile);

		// is a type requested
		$iType = ($iType) ? $iType : $this->imageType();

		return SssSgdImageBasic::saveImageAsType($this->rImage, $iType, $sPathFile);

	} // saveTo

	function scale($fFactor, $fFactorY = null) {

		if (!is_resource($this->rImage)) return null;

		$fFactorY = (0 < $fFactorY) ? $fFactorY : $fFactor;

		$iWo = $this->width();
		$iHo = $this->height();

		$iWn = (float)$iWo * (float)$fFactor;
		$iHn = (float)$iHo * (float)$fFactorY;

		$rImageNew = imageCreateTrueColor($iWn, $iHn);

		if (IMAGETYPE_GIF == $this->aInfo[2]
				|| IMAGETYPE_PNG == $this->aInfo[2]) {

			$iTransparencyIndex = imageColorTransparent($this->rImage);

			// If we have a specific transparent color
			if (0 <= $iTransparencyIndex) {

				// Get the original image's transparent color's RGB values
				$aTransColour = imageColorsForIndex($this->rImage, $iTransparencyIndex);

				// Allocate the same color in the new image resource
				$iTransparencyIndex = imageColorAllocate($rImageNew, $aTransColour['red'], $aTransColour['green'], $aTransColour['blue']);

				// Completely fill the background of the new image with allocated color.
				imageFill($rImageNew, 0, 0, $iTransparencyIndex);

				// Set the background color for new image to transparent
				imageColorTransparent($rImageNew, $iTransparencyIndex);

			} elseif (IMAGETYPE_PNG == $this->aInfo[2]) {
				// Always make a transparent background color for PNGs that don't have one allocated already

				// Turn off transparency blending (temporarily)
				imageAlphaBlending($rImageNew, false);

				// Create a new transparent color for image
				$iTransparencyIndex = imageColorAllocateAlpha($rImageNew, 0, 0, 0, 127);

				// Completely fill the background of the new image with allocated color.
				imageFill($rImageNew, 0, 0, $iTransparencyIndex);

				// Restore transparency blending
				imageSaveAlpha($rImageNew, true);

			} // if transparency given

		} // if need to take care of transparency (gif or png)
	//	imageAlphaBlending($rImageNew, false);
		//imageSaveAlpha($rImageNew, true);

		//imageAlphaBlending($this->rImage, true);

		imageCopyResampled($rImageNew, $this->rImage, 0, 0, 0, 0, $iWn, $iHn, $iWo, $iHo);

		$this->setImage($rImageNew);

		return true;

	} // scale

	// set the resource identifier for open gd image
	function setImage($rImage = false) {

		if (is_resource($this->rImage)) imageDestroy($this->rImage);
		$this->rImage = $rImage;

	} // setImage

	function width() {

		if (!$this->rImage) return 0;

		return imagesx($this->rImage);

	} // width

} // SssSgdImageBasic

/* * * *\ SssSgdImageBasic.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
