<?php
/* * * *
 * * SssSLocalization
 * *
 * * @version 20101116_024450 (CC) Luke JZ aka SwissalpS
 * * * */
if (!defined('LOCALIZATION_DELEGATE'))
	define('LOCALIZATION_DELEGATE', 'SssSBla');

class SssSLocalization extends WFObject {

	// array holding on to the translated keys
	protected $aUIDs;
	protected $aStringsCache;

	// apple has tables, we might somehow imitate that where apropriate
	var $aTables;

	protected $oDelegate;

	function __construct() {

		$this->aTables = array();

		$this->aUIDs = array();

		// load delegate if any
		if (defined('LOCALIZATION_DELEGATE')) {

			$sDelegateClass = LOCALIZATION_DELEGATE;
			$sDelegatePath = APP_ROOT . '/classes/' . $sDelegateClass . '.php';

			if  (!class_exists($sDelegateClass)) {

				// load the file containing the class
				if (!file_exists($sDelegatePath))
					WFLog::log('Localization delegate class file could not be found, skipping: ' . $sDelegatePath);
				else require($sDelegatePath);

			} // if class not yet loaded

			if  (!class_exists($sDelegateClass))
				WFLog::log('Localization delegate class could not be found, skipping: ' . $sDelegateClass);
			else $this->oDelegate = new $sDelegateClass;

		} // if got a delegate class defined

	} // __construct


	function initialize() {

		if (is_object($this->oDelegate)
			&& method_exists($this->oDelegate, 'initialize')) {

				$this->oDelegate->initialize();

		} // if delegate exists and responds to initialize()

		return $this;

	} // initialize

	function assertTableExistsInCache($sTable = '__main') {

		if (!isset($this->aUIDs[$sTable])) {

			$this->aUIDs[$sTable] = array();

		} // if table does not exist in cache, prepare it

		return $this;

	} // assertTableExistsInCache

	function assertKeyExistsInCache($sKey, $sTable = '__main') {

		$this->assertTableExistsInCache($sTable);

		if (!isset($this->aUIDs[$sTable][$sKey])) {

			$this->aUIDs[$sTable][$sKey] = array();

		} // if key does not exist, prepare it

		return $this;

	} // assertKeyExistsInCache

	//function addKey() {} // addKey

	// method for other classes to add their strings to the cache hash
	function addKeys(Array $aNewKeys) {

		$sTable = '__main';

		// I would prefer not to need to load all languages...hmmm

		// do we need to check the incomming array structure and case?
		//$this->aUIDs = array_merge_recursive($this->aUIDs, $aNewKeys);
		// seems so, above does not satisfy
		foreach ($aNewKeys as $sKey => $aNewKey) {

			$this->assertKeyExistsInCache($sKey, $sTable);

			foreach ($aNewKey as $sLang => $sValue) {

				$this->aUIDs[$sTable][$sKey][strToLower($sLang)] = $sValue;

			} // loop languages

		} // loop keys

		return $this;

	} // addKeys


	function addTable(SssSLocalizationTable $oSssSLocalizationTable) {

		if ($oSssSLocalizationTable instanceOf SssSLocalizationTable) {

			$this->aTables[$oSssSLocalizationTable->name()] = $oSssSLocalizationTable;

		} // if instance of SssSLocalizationTable

		return $this;

	} // addTable


	function delegate() { return $this->oDelegate; } // delegate


	// helper for select widget
	function selectLangValues() {

		if (is_object($this->oDelegate) && method_exists($this->oDelegate, 'selectLangValues')) {

			return $this->oDelegate->selectLangValues();

		} // if delegate responds to selectLangValues

		return self::languageOrder();

	} // selectLangValues


	// helper for select widget
	function selectLangLabels() {

		if (is_object($this->oDelegate) && method_exists($this->oDelegate, 'selectLangLabels')) {

			return $this->oDelegate->selectLangLabels();

		} // if delegate responds to selectLangLabels

		return self::languageOrder();

	} // selectLangLabels


	function set($sValue, $sKey, $sLang, $sTable = '__main') {

		// no. lang needs to be given
		//if (!$sLang) $sLang = self::currentLanguage();

		// first update cache
		$this->assertKeyExistsInCache($sKey, $sTable);
		$this->aUIDs[$sTable][$sKey][$sLang] = $sValue;

		// can we set straight away?
		if (isset($this->aTables[$sTable])) {

			return $this->aTables[$sTable]->set($sValue, $sKey, $sLang);

		} // if got one registered

		// well then we need to find out which table responds to the key
		foreach ($this->aTables as $oTable) {

			$sResult = $oTable->translate($sKey, $sLang);

			if ($sKey != $sResult) {

				return $oTable->set($sValue, $sKey, $sLang);

			} // if got a translation

		} //

		// last fallback is the delegate
		if (is_object($this->oDelegate) && method_exists($this->oDelegate, 'set')) {

			return $this->oDelegate->set($sValue, $sKey, $sLang, $sTable);

		} // if

	} // set

	function translate($sKey, $sLang = null, $sTable = '__main') {

		if (!$sLang) $sLang = self::currentLanguage();

		if (isset($this->aUIDs[$sTable][$sKey][$sLang]))
			return $this->aUIDs[$sTable][$sKey][$sLang];

		// not yet in cache, try delegate
		if (is_object($this->oDelegate) && method_exists($this->oDelegate, 'translate')) {

			$sResult = $this->oDelegate->translate($sKey, $sLang, $sTable);

			if ($sKey != $sResult) {

				$this->assertKeyExistsInCache($sKey, $sTable);

				$this->aUIDs[$sTable][$sKey][$sLang] = $sResult;

				return $sResult;

			} // if got a translation

		} // if delegate -> let it find the translation

		// try the table object, if exists
		if (isset($this->aTables[$sTable])) {

			$sResult = $this->aTables[$sTable]->translate($sKey, $sLang, $sTable);

			if ($sKey != $sResult) {

				$this->assertKeyExistsInCache($sKey, $sTable);

				$this->aUIDs[$sTable][$sKey][$sLang] = $sResult;

				return $sResult;

			} // if got a translation

		} // if got a table with this name

		// try desperately to find a translation
		// now we loop the tables
		foreach ($this->aTables as $oTable) {

			$sResult = $oTable->translate($sKey, $sLang, $sTable);

			if ($sKey != $sResult) {

				$this->assertKeyExistsInCache($sKey, $sTable);

				$this->aUIDs[$sTable][$sKey][$sLang] = $sResult;

				return $sResult;

			} // if got a translation

		} // loop all tables

		return $sKey;

	} // translate


	static function currentLanguage() {

		static $sLang = null;

		if (!$sLang) {

			$aLangs = self::languageOrder();

			$sLang = (empty($aLangs)) ? 'de' : $aLangs[0];

		} // first run

		return $sLang;

	} // currentLanguage


	static function languageOrder() {

		static $aLangs = null;

		if (!$aLangs) {

			$oDelegate = self::sharedInstance()->delegate();

			if (is_object($oDelegate) && method_exists($oDelegate, 'languageOrder'))
				$aLangs = $oDelegate->languageOrder();

			if (empty($aLangs)) $aLangs = self::defaultLanguages();

		} // first run

		return $aLangs;

	} // languageOrder


	static function defaultLanguages() {

		// TODO: move/add/sort according to the browsers request
		return array('de', 'en', 'fr', 'it', 'rm');

	} // defaultLanguages


	static function sharedInstance() {

		static $o = null;
		if (!$o) { $o = new SssSLocalization(); $o->initialize(); }

		return $o;

	} // sharedInstance

} // SssSLocalization
/* * * *\ SssSLocalization (CC) Luke JZ aka SwissalpS /* * * */
?>
