<?php
/* * * *
 * * SssSLocalizationDelegate
 * *
 * * @version 20101114_135930 (CC) Luke JZ aka SwissalpS
 * * * */

interface SssSLocalizationDelegate {

	// init whatever
	function initialize();

	// return array eg array('en', 'de', 'fr', 'it', 'rm')
	function languageOrder();

	// return string
	function translate($sKey, $sLang);

	// optional
	// helper for select widget
    //function selectLangValues();

	// optional
	// helper for select widget
    //function selectLangLabels();

} // SssSLocalizationDelegate

/* * * *\ SssSLocalizationDelegate (CC) Luke JZ aka SwissalpS /* * * */
?>
