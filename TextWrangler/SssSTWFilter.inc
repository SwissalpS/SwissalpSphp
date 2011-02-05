<?php
/* * * *
 * * SssSTWFilter.inc
 * *
 * * @version 20110201_131904 (CC) Luke JZ aka SwissalpS
 * * * */

class SssSTWFilter {

	protected $iSelEnd;
	protected $iSelStart;
	protected $sFileName;
	protected $sPathTmpFile;
	protected $sOut;

	function __construct() {

		$this->iSelEnd = intVal($_SERVER['BB_DOC_SELEND']);
		$this->iSelStart = intVal($_SERVER['BB_DOC_SELSTART']);
		$this->sFileName = $_SERVER['BB_DOC_NAME'];
		$this->sPathTmpFile = $_SERVER['argv'][1];

		$this->sOut = '';

	} // __construct


	function isSelection() {

		return (bool)($this->iSelStart != $this->iSelEnd);

	} // isSelection


	function payLoad() {

		static $sAll = null;
		if (!$sAll) $sAll = file_get_contents($this->sPathTmpFile);
		return $sAll;

	} // payLoad


	function payLoadLines() {

		static $aLines = null;
		if (!$aLines) $aLines = file($this->sPathTmpFile);
		return $aLines;

	} // payLoadLines


	function apply() {

		echo $this->sOut;

		exit(0);

	} // apply


	function getOutputBuffer() {

		return $this->sOut;

	} // getOutputBuffer


	function setOutputBuffer($sOutNew = '') {

		$this->sOut = (string)$sOutNew;

	} // getOutputBuffer

} // SssSTWFilter

/* * * *\ SssSTWFilter.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
