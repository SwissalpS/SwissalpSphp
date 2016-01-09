<?php
/* * * *
 * * SwissalpS/PHOCOA/Widgets/WFBlaWidget.inc
 * *
 * * Widget for localizeable strings
 * *
 * * @version 20101115_032022 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\Widgets;

use WFWidget;
use SwissalpS\PHOCOA\Localization\Bla as SssSBla;
use SwissalpS\PHOCOA\Localization\Localization as SssSLocalization;

class WFBlaWidget extends WFWidget {

	// string
	protected $table;
	// bool
	protected $noEdit;
	// bool
	protected $noDIV;
	// string
	protected $lang;
	// int
	protected $outputMode;

	function __construct($sId, $oPage) {

		parent::__construct($sId, $oPage);

		$this->table = null;
		$this->noEdit = false;
		$this->noDIV = false;
		$this->lang = SssSLocalization::currentLanguage();
		$this->outputMode = SssSBla::OUTPUT_MODE_SIMPLE;

	} // __construct

// TODO: test if this is still needed
	// register and set to current params
	function applyParams($aParams) {

		// set plist to use, if available
		if (isset($aParams['table'])) $this->table = $aParams['table'];
		else $this->table = null;

		if (isset($aParams['noDIV'])
				&& ('true' === $aParams['noDIV']
				|| true === $aParams['noDIV'])) {

			$aParams['noEdit'] = true;
			$this->noDIV = true;

		} // if simplest mode requested (js alerts etc.)

		// set noEdit flag
		$this->noEdit = (isset($aParams['noEdit'])
				&& ('true' === $aParams['noEdit']
				|| true === $aParams['noEdit'])) ? true : false;

		// set language
		$this->lang = isset($aParams['lang'])
			// if have param lang, use that
			? $aParams['lang']
			// otherwise ask the static function to determine
			: SssSLocalization::currentLanguage();

		$this->value = $aParams['value'];

		/*$sUID = $aParams['value'];
		// register and set to current params
		if (isset($this->aUIDs[$sUID])) {

			if (isset($this->aUIDs[$sUID]['params']))
				$this->aUIDs[$sUID]['params'] = array_merge($this->aUIDs[$sUID]['params'], $aParams);

			else
				$this->aUIDs[$sUID]['params'] = $aParams;

		} else
			$this->aUIDs[$sUID] = array('params' => $aParams);

		$this->sCurrentUID = $sUID;*/

	} // applyParams


	public static function exposedProperties() {

		$aItems = parent::exposedProperties();

		return array_merge($aItems,
			array('value', 'table', 'hidden', 'class', 'noEdit', 'lang', 'noDIV'));

	} // exposedProperties


	function canPushValueBinding() { return false; } // canPushValueBinding


	function render($sBlockContent = NULL) {

		if ($this->hidden) return null;

		$sOutPut = parent::render($sBlockContent);

		$aParams = array('noEdit' => $this->noEdit, 'noDIV' => $this->noDIV, 'id' => $this->id, 'table' => $this->table, 'lang' => $this->lang);

		$sOutPut .= SssSBla::bla($this->value, $aParams);

		return $sOutPut;

	} // render

} // SssSBlaWidget
/* * * *\ SwissalpS/PHOCOA/Widgets/WFLBlaWidget.inc (CC) Luke JZ aka SwissalpS /* * * */
