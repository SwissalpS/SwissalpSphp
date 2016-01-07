<?php
/*
 * SwissalpS/PHOCOA/Localization/Bla.inc
 *
 * @version 20110314_140553 added php_utf8 functions to improve detection on certain systems
*/
/* * * *
 * * SssSBla
 * *
 * * pre-project design for what will later be the NSLocalizedString pendent {(SssS|WF)LocalizedString()}
 * *
 * * first written as a WFWidget (render) then realized I needed to use in other context too (bla), so now there are some static functions and some widget functions doing basically the same thing. -> TODO: refactor to make obvious which are which and find out if the caching helps at all.
 * *
 * * @version 20100806_165223 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\Localization;

use WFAuthorizationManager;

\SwissalpS\Strings\Encoding\Utf8Loader::load();

class Bla implements LocalizationDelegateInterface {

	const DEFAULT_ID = 'SssSBla'; // default instance id

	const TRANS_PREFIX = 'SssSBla'; // prefix for labels etc that need translating

	const CLASS_QUERY = 'SssSPropel2\\phocoa\\Bla000Query';
	//const CLASS_QUERY = 'SssSPropel2\\phocoaBla\\BlaQuery';

	const OUTPUT_MODE_SIMPLE = 1; // just text
	const OUTPUT_MODE_DIV	 = 2; // wrapped in div (default)
	const OUTPUT_MODE_ADMINL = 4; // with admin link as popup
	const OUTPUT_MODE_SWITCH = 8; // switchable to other language (not yet implemented) TODO:

 	// helper array for select widgets assoc. array with the language names translated to their names in their respective language
	protected $aLangs;

/* extending functions */

	function  __construct() {

	//	parent::__construct();

	} // __construct

	// helper for select widget
    function selectLangValues() {

    	return array_values($this->aLangs);

    } // selectLangValues

	// helper for select widget
    function selectLangLabels() {

    	return array_keys($this->aLangs);

    } // selectLangLabels

	static function bla($sKey, $aParams = null) {

		static $oSharedLocalization = null;
		if (!$oSharedLocalization) {

			$oSharedLocalization = Localization::sharedInstance();

		} // first call

		if (!is_array($aParams)) {

			$aParams = array('id' => self::DEFAULT_ID);

		} // if not array

		if (!isset($aParams['id'])) {

			$aParams['id'] = self::DEFAULT_ID;

		} // if id not set

		if (!isset($aParams['langOrder'])) {

			$aParams['langOrder'] = Localization::languageOrder();

		} // if language order not set

		$sTranslated = (isset($aParams['lang']))
			? $oSharedLocalization->translate($sKey, $aParams['lang'])
			: $oSharedLocalization->translate($sKey);

		// now check if we need to add edit link
		$iOutputMode = (self::showBlaEditLinks())
			? self::OUTPUT_MODE_ADMINL
			: self::OUTPUT_MODE_DIV;

		if (isset($aParams['outputMode'])) {

			$iOutputMode = intVal($aParams['outputMode']);

		} // if output mode set

		$bNoEdit = (isset($aParams['noEdit'])
				&& ('false' === $aParams['noEdit']
				|| false === $aParams['noEdit']));

		if (isset($aParams['noDIV'])
				&& ('true' === $aParams['noDIV']
				|| true === $aParams['noDIV'])) {

			$bNoEdit = true;

		} // if no DIV

		if ($bNoEdit) {

			$iOutputMode = self::OUTPUT_MODE_SIMPLE;

		} // if no edit

		switch ($iOutputMode) {

			case self::OUTPUT_MODE_SWITCH :
				$sOutput = '';
				break;

			case self::OUTPUT_MODE_ADMINL :
				$sOutput = self::wrapHTMLinEditDiv(
					array(
						array(
							'sUID' => $sKey,
							'sEditMeTextShort' => 'e',
							'sEditMeTextFull' => $oSharedLocalization->translate('SssSBlaInternalEditText'))
						),
					$sTranslated);
				break;

			case self::OUTPUT_MODE_SIMPLE :
				$sOutput = $sTranslated;
				break;

			case self::OUTPUT_MODE_DIV :
			default :
				// just the text in div no js
				$sOutput = self::wrapInDiv($sKey, $aParams['id'],
					$sTranslated);
				break;

		} // switch outputMode

		return $sOutput;

	} // bla


	// title, and some other html tags, don't display right with html entities
	static function cleanForTitle($sText) {

		static $sMatchNum = null;
		static $sMatchAlpha = null;
		if (!$sMatchNum) {
			$sMatchNum = "|&#([0-9]+);|i";
			$sMatchAlpha = "|&([a-z]+);|i";
		} // first call

		// strip double encoded numericals, too risky for alphas
	//	$sText = preg_replace("|(&amp;)#([0-9]+);|i", '&#$2;', $sText);

		$iRes = preg_match_all($sMatchNum, $sText, $aFound, PREG_SET_ORDER);

		if (0 < $iRes) {

			foreach ($aFound as $aMatches) {
				if (127 < $aMatches[1]) {

					$sText = str_replace($aMatches[0], html_entity_decode($aMatches[0]), $sText); //, ENT_NOQUOTES, 'UTF8')


				} // if not 'dangerous'

			} // foreach found match

		} // if found matches

		$aFound = array(); $aIllegal = array('lt', 'gt', 'quot');
		$iRes = preg_match_all($sMatchAlpha, $sText, $aFound, PREG_SET_ORDER);

		if (0 < $iRes) {

			foreach ($aFound as $aMatches) {
				if (!in_array($aMatches[1], $aIllegal)) {

					$sText = str_replace($aMatches[0], html_entity_decode($aMatches[0]), $sText); //, ENT_NOQUOTES

				} // if not 'dangerous'

			} // foreach found match

		} // if found matches

		return $sText;

	} // cleanForTitle


	// convert special chars to html escapes (not all!)
	static function cleanSpecialChars(&$sText) {

		$sNew = '';
		$sText = str_replace(array('\"', "\'", '\\\\'), array('"', "'", '\\'), $sText);
		$iLen = mb_strlen($sText); //, 'UTF-8');
		for ($j = 0; $j < $iLen; $j++) {

			$s = mb_substr($sText, $j, 1); //, 'UTF-8');
			$a = utf8ToUnicode($s);
			$i = (isset($a[0])) ? $a[0] : ord($s);

			switch ($i) {
				// tab, nl, cr
				case 9 :
				case 10 :
				case 13 :
				// other ascii chars
				case ($i < 128 && $i > 31) :
					// they are fine, just append
					$sNew .= $s;
					break;

				default:
					// any others need to be 'escaped'
					$sNew .= '&#' . $i . ';';

			} // switch char value

		} // foreach char

		$sText = $sNew;

	} // cleanSpecialChars


	static function removeBlaPrefixIfHas($sText) {

        $iLenS = strlen($sText);

		static $sPrefix = null;
		static $iLenP = null;

		if (!$sPrefix) {

			$sPrefix = self::TRANS_PREFIX;
			$iLenP = strlen($sPrefix);

		} // first call

		if (($iLenP < $iLenS)
				&& ($sPrefix == substr($sText, 0, $iLenP))) {

			return substr($sText, $iLenP);

		} // if got prefix, remove it

		return false;

	} // removeBlaPrefixIfHas


	static function showBlaEditLinks() {

		$oAuthInfo = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();

		if (is_object($oAuthInfo) && method_exists($oAuthInfo, 'showBlaEditLinks')) {

			return $oAuthInfo->showBlaEditLinks();

		} else {

			return false;

		} // if authinfo responds to showBlaEditLinks

	} // showBlaEditLinks

	// translateAndFormatIfNeeded
	static function translateIfNeeded($s, $aParams = null) {

		$iLenS = strlen($s);

		static $iLenP = null;
		if (!$iLenP) {

			$iLenP = strlen(self::TRANS_PREFIX);

		} // if first call

		if (($iLenP < $iLenS)
				&& (self::TRANS_PREFIX == substr($s, 0, $iLenP))) {

			$s = self::bla(substr($s, $iLenP), $aParams);

		} // if long enough to and has prefix

		return $s;

	} // translateIfNeeded


	static function wrapInDiv($sUID, $sID, $sTranslated, $sClass = '') {

		return sprintf(
				'<div id="SssSBla%1$s" name="%2$s" class="SssSBlaVisitor%4$s">%3$s'
				. '</div>', $sUID, $sID, $sTranslated, $sClass);

	} // wrapInDiv


	static function wrapHTMLinEditDiv($aBlas, $sInnerHTML = 'whatever html like <a href="#">some link</a>', $bUseMouseUp = true, $sJSActions = '', $sListenerJSInScriptTag = '', $sBlaToolTip = -1) {
		/*
		aParams = array(
			'sUID' => 'BLAUID',
			'sEditMeTextFull' => 'title description',
			'sEditMeTextShort' => 'e',
			'sJSActions' => 'onMouseDown="alert(\'ouch\');"',
			'sListenerJSInScriptTag' => '<script>code</script>'
			);
		*/
		$sButtons = '';
		$sUrlEdit = null;
		$iCount = 0;
		if (-1 == $sBlaToolTip) $sBlaToolTip = SssSLocalization::sharedInstance()->translate('SharedEditLocalized');

		foreach ($aBlas as $aParams) {

			if (!is_array($aParams)) continue;
			if (!isset($aParams['sUID'])) continue;

			$iCount++;
			$sUID = $aParams['sUID'];

			$sEditMeTextShort = (isset($aParams['sEditMeTextShort']))
				? $aParams['sEditMeTextShort']
				: 'e';

			$sEditMeTextFull = (isset($aParams['sEditMeTextFull']))
				? $aParams['sEditMeTextFull']
				: 'edit ' . $sUID;

			// TODO: make path setable
			$sUrlEdit = WFRequestController::WFURL('admin/bla000/edit/' . $sUID);

			$sJSActions = (isset($aParams['sJSActions']))
				? ' ' . trim($aParams['sJSActions'])
				: '';

			$sListenerJSInScriptTag = (isset($aParams['sListenerJSInScriptTag']))
				? $aParams['sListenerJSInScriptTag']
				: '';

			$sButtons .= sprintf('<a href="%1$s" target="_blank" class="SssSBlaEditLink" title="%2$s"%4$s>%3$s</a>%5$s ',
				$sUrlEdit, $sEditMeTextFull, $sEditMeTextShort, $sJSActions, $sListenerJSInScriptTag);

		} // loop all to be inserted

		$sOver = "this.children(0).className='SssSBlaVisible';";

		$sOut = "this.children(0).className='SssSBlaHidden';";

		$sUp = '';

		if ($bUseMouseUp && (1 == $iCount)) {

			$sUp = ' onMouseUp="window.open(\''
			. $sUrlEdit . '\');"';

			$sButtons = '';

			$sBlaToolTip .= ': ' . $sUID;

		} // if only one, we can use the whole div as a button

		return sprintf(
				'<div class="SssSBla" title="%8$s" onMouseOver="%1$s" onMouseOut="%2$s"%3$s%7$s><div class="SssSBlaHidden">%4$s</div>%5$s</div>%6$s',
					$sOver, $sOut, $sUp, $sButtons, $sInnerHTML, $sListenerJSInScriptTag, $sJSActions, $sBlaToolTip);

	} // wrapHTMLinEditDiv

/* 	SssSLocalizationDelegate methods */

	public function initialize() {

		Localization::sharedInstance()->addKeys(array('SssSBlaInternalEditText' => array(
				'en' => 'edit this text',
				'de' => 'diesen Text bearbeiten',
				'it' => 'modificare il testo',
				'fr' => 'modifier ce texte',
				'rm' => 'adattar quest text')));


		// build the language helper array for select widgets
    	$aLangsRaw = self::languageOrder();

		$aLang = array();
		foreach ($aLangsRaw as $sLang) {

			$aLang[self::bla(
					'Bla000Lang' . strToUpper($sLang),
							array('lang' => $sLang, 'noDIV' => true))] = $sLang;

		} // foreach lang build array

		$this->aLangs = $aLang;

	} // initialize

	public static function languageOrder() {

		static $aLangs = null;

		if (!$aLangs) {

			$oAuthInfo = WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo();

			if (is_object($oAuthInfo)) {

				if (method_exists($oAuthInfo, 'languageOrder'))
					$aLangs = $oAuthInfo->languageOrder();
				elseif (isset($oAuthInfo->languageOrder) && is_array($oAuthInfo->languageOrder))
					$aLangs = $oAuthInfo->languageOrder;
				else $aLangs = null;

			} // if got authinfo

			if (empty($aLangs)) $aLangs = array('de', 'en', 'fr', 'it', 'rm');

		} // first run

		return $aLangs;

	} // languageOrder

	// return string
	public function translate($sKey, $sLang = NULL, $sTable = '__main') {

		// prepend the requested language
		$aLangs = array_merge(array($sLang), $this->languageOrder());

		$sClassName = self::CLASS_QUERY;
		if (class_exists($sClassName)) {

			$oCriteria = $sClassName::create();

			if ($oBla = $oCriteria->findPk($sKey)) {

				// return the translated value, falling back to next language in list
				foreach ($aLangs as $sLang) {

					$mVal = $oBla->valueForKey(strToLower($sLang));

					if (!empty($mVal)) {

						return $mVal;

					} // if got value

				} // loop languages

			} // if got entry in db

		} // if Bla000Query class exists

		return $sKey;

	} // translate

} // Bla

/* * * *\ SwissalpS/PHOCOA/Localization/Bla.inc (CC) Luke JZ aka SwissalpS /* * * */
