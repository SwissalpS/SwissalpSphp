<?php
/* * * *
 * * SssS_TextDecorator.inc
 * *
 * * @version 20091105_113317 (CC) Luke JZ aka SwissalpS
 * * * */
//error_reporting(E_ALL); // 0); //

class SssS_TextDecorator {

	function SssS_TextDecorator() {

		// init if needed

	} // SssS_TextDecorator

	/**
	 * indentLines($mLines, $iAmount = iAmount = 1, $sFiller = ' ', $sLineBreak = "\n")
	 * indent a block of text prepending iAmount sFillers to each line in mLines
	 *
	 * @param array|string $mLines a string returns a string, an arry of lines returns an array
	 * @param int $iAmount how many indentations to make (defaults to: 1)
	 * @param string $sFiller the indenter (defaults to: ' ')
	 *
	 * @return string|array prepended lines in format as given
	 *
	 * @uses __something__ - __uses_desciption_here__
	 *
	 * @package SssS_TextDecorator
	 * @author Luke JZ aka SwissalpS <SwissalpS@LukeZimmerman.com>
	 * @license __license_url_here__
	 * @copyright 2007 Luke JZ aka SwissalpS
	 * @version 20091105_224707 (CC)2007 Luke JZ aka SwissalpS
	 */
	public static function indentLines($mLines, $iAmount = 1,
			$sFiller = ' ', $sLineBreak = "\n") {

		$sPadding = str_repeat($sFiller, abs($iAmount));

		$bReturnArray = false;

		switch (gettype($mLines)) {

			case 'array' : $aLines = $mLines; $bReturnArray = true;
			break;

			case 'string' : $aLines = SssS_TextDecorator::splitIntoLines($mLines);
			break;

			default : return 'wrong argument type given in SssS_TextDecorator::indentLines';

		} // switch type

		$aPaddedLines = array();
		foreach ($aLines as $sLine) {

			$aPaddedLines[] = $sPadding . $sLine;

		} // foreach line, prefix padding

		return ($bReturnArray) ? $aPaddedLines : implode($sLineBreak, $aPaddedLines);
	} // indentLines



	/**
	 * centeredTextPages($s = null, $sFiller = null, $iTotalWidth = 80, $iTotalHeight = 24, $iXoffset = 0, $iYoffset = 0, $bWrap = true)
	 * center text for console
	 *
	 * @param string $s __parameter_description_here__ (defaults to: 'null')
	 * @param int $iTotalWidth __parameter_description_here__ (defaults to: iTotalWidth=80)
	 * @param string $sFiller __parameter_description_here__ (defaults to: 'null')
	 * @param string $bWrap __parameter_description_here__ (defaults to: '')
	 * @param mixed $true __parameter_description_here__
	 *
	 * @return array of pages with the text centered at offsets
	 *
	 * @uses __something__ - __uses_desciption_here__
	 *
	 * @package SssS_TextDecorator
	 * @author Luke JZ aka SwissalpS <SwissalpS@LukeZimmerman.com>
	 * @license __license_url_here__
	 * @copyright 2007 Luke JZ aka SwissalpS
	 * @version 20091105_133223 (CC)2007 Luke JZ aka SwissalpS
	 */
	public static function centeredTextPages($s = null, $sFiller = null,
			$iTotalWidth = 80, $iTotalHeight = 24, $iXoffset = 0, $iYoffset = 0,
			$sLineBreak = "\n", $bWrap = true) {
			// TODO: $bWrap = true

		$aRet = array('');

		if (0 == strlen($s)) return $aRet; // no need to doodle

		$iFl = strlen($sFiller);
		if (0 == $iFl) { $sFiller = ' '; $iFl = 1; }

		if (0 == $iTotalWidth) return $s; // can't fit into no space
		if (0 == $iTotalHeight) return $s; // can't fit into no space

		//
		$iMaxW = $iTotalWidth - abs($iXoffset);
		$iMaxH = $iTotalHeight - abs($iYoffset);

		// split into lines
		$aLines = SssS_TextDecorator::splitIntoLines($s);

		$aCutLines = array();
		foreach($aLines as $sLine) {

			$s = trim($sLine);
			$iL = strlen($s);

			if ($iMaxW < $iL) {

				// wrap without cutting
				$aSubLines = explode($sLineBreak, wordwrap($s, $iMaxW, $sLineBreak));

				foreach ($aSubLines as $sSubLine) {

					$s2 = trim($sSubLine);

					if ($iMaxW < strlen($s2)) {

						// wrap cutting words
						$aSubSubLines = explode($sLineBreak, wordwrap($s2,
								$iMaxW, $sLineBreak, true));

						foreach ($aSubSubLines as $sSubSubLine) {
							$aCutLines[] = $sSubSubLine;
						} // foreach sub-sub-line

					} // if still longer than allowed
					else {

						$aCutLines[] = $s2;

					} // if fits in our box
				} // foreach subline
			} // if longer than allowed

			else {
				$aCutLines[] = $s;

			} // if fits in our box
		} // foreach line

		$aCenteredLines = array();
		foreach ($aCutLines as $sLine) {

			$iL = strlen($sLine);

			$iPaddingPreCalc = (0.5 * ($iMaxW - $iL) / $iFl);

			$iPadding = max(0, $iPaddingPreCalc + $iXoffset);

			$aCenteredLines[] = str_repeat($sFiller, $iPadding) . $sLine;

		} // foreach line, center it

		$aPages = array_chunk($aCenteredLines, $iMaxH);

		$aRet = array();
		foreach ($aPages as $aPage) {

			$sBlob = implode($sLineBreak, $aPage);

			$sFillerStream = str_repeat($sLineBreak, $iTotalHeight - $iMaxH);

			$sPage = (0 <= $iYoffset)
					? $sFillerStream . $sBlob
					: $sBlob . $sFillerStream;

			$aRet[] = $sPage;

		} // foreach page, make string and prepend

		return $aRet;

	} // centeredTextPages



	/**
	 * splitIntoLines($s)
	 * splits input string into an array of lines
	 *
	 * @param string $s the string to split up
	 *
	 * @return array containing the seperate lines
	 *
	 * @version 20091105_120820 (CC)2007 Luke JZ aka SwissalpS
	 */
	public static function splitIntoLines($s) {

		$aRet = explode("\r\n", $s);

		if (1 == count($aRet)) $aRet = explode("\r", $aRet[0]);

		if (1 == count($aRet)) $aRet = explode("\n", $aRet[0]);

		return $aRet;

	} // splitIntoLines

	function __toString() { return print_r($this, true); } // __toString

} // SssS_TextDecorator

/* * * *\ SssS_TextDecorator.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
