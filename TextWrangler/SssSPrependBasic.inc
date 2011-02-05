<?php
/* * * *
 * * SssSPrependBasic.inc
 * *
 * * @version 20110201_134530 (CC) Luke JZ aka SwissalpS
 * * * */
if (!defined('AUTO_INCLUDING')) {
	require_once('SssSTWFilter.inc');
} // if not autoincluding but on php's include path

class SssSPrependBasic extends SssSTWFilter {

	protected $sPrependString;

	function __construct($sPrependString = '#') {

		parent::__construct();

		$this->sPrependString = $sPrependString;

	} // __construct


	function prepend() {

		forEach ($this->payLoadLines() as $sLine) {

			$this->sOut  .= $this->sPrependString . $sLine;

		} // loop each line

	} // prepend


	function preremove() {

		$iLen = strLen($this->sPrependString);

		forEach ($this->payLoadLines() as $sLine) {

			if (subStr($sLine, 0, $iLen) == $this->sPrependString) {

				$this->sOut .= subStr($sLine, $iLen);

			} else $this->sOut  .= $sLine;

		} // loop each line

	} // preremove


} // SssSPrependBasic

/* * * *\ SssSPrependBasic.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
