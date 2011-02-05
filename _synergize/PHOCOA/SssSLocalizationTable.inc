<?php
/* * * *
 * * SssSLocalizationTable
 * *
 * * @version 20101114_135806 (CC) Luke JZ aka SwissalpS
 * * * */
// a source of localizable strings can represent
interface SssSLocalizationTable {

	// return translated value or the given key
	// fall back to next language in list of SssSLocalization::languageOrder()
	// if sLang isn't given use the first in the order
	function translate($sKey, $sLang = null);

	// set a string for a key in this table, writable sources also save
	function set($sValue, $sKey, $sLang);

	// return the tables name, this will probably be the same as the name of the class
	function name();

} // SssSLocalizationTable

/* * * *\ SssSLocalizationTable (CC) Luke JZ aka SwissalpS /* * * */
?>
