<?php
/* vim: set expandtab tabstop=4 shiftwidth=4: */
/* * * *
 * * smarty_function_SssSBla
 * *
 * * @version 20100727_145411 (CC) Luke JZ aka SwissalpS
 * * * */
/**
 * @package UI
 * @subpackage Widgets
 */

/**
 *  Smarty plugin to include a SssSBla.
 *
 *  Smarty Params:
 *  value - The uid of the snippet to translate.
 *
 *  optional:
 *  table - The name of the table to take the translation from
 *  lang - 2 letter language code to override the currently used language
 *  noDIV - boolean if true, equivalent to WFLocalizedString($value);
 *
 *  @param array The params from smarty tag.
 *  @param object WFSmarty object of the current tpl.
 *  @return string The rendered HTML.
 */

function smarty_function_SssSBla($aParams, &$oSmarty) {

	// must have a bla uid set
	if (!isset($aParams['value'])) throw new Exception('value required.');

	return SssSBla::bla($aParams['value'], $aParams);
/*
	// if id not set (default) or seperate instance
	$sID = (isset($aParams['id'])) ? $aParams['id'] : SssSBla::DEFAULT_ID;
	$aParams['id'] = $sID;

	$oPage = $oSmarty->getPage();

	// is this going to the default bla object or a seperate instance
	if ($oPage->hasOutlet($sID)) $oBla = $oPage->outlet($sID);
	else {
		// no bla outlet yet, create and add one
		$oBla = new SssSBlaWidget($sID, $oPage);

	} // if no bla outlet yet

	// set current params
	$oBla->applyParams($aParams);

	// and output
	return $oBla->render();
*/
} // smarty_function_SssSBla

/* * * *\ smarty_function_SssSBla (CC) Luke JZ aka SwissalpS /* * * */
?>
