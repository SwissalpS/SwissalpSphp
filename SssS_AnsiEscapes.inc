<?php
/* * * *
 * * SssS_AnsiEscapes.inc
 * *
 * * thanks to sites like http://www.understudy.net/custom.html
 * * http://www.bluesock.org/~willg/dev/ansi.html
 * * @version 20091102_232817 (CC) Luke JZ aka SwissalpS
 * * * */
class SssS_AnsiEscapes {

	public static $aColours = array(
			'neutral'			=> '[0m',
			'bold'				=> '[1m',
			'lightup'			=> '[2m',
			'italic'			=> '[3m',
			'underline'			=> '[4m',
			'blinking'			=> '[5m',
			'blackoutline'		=> '[6m',
			'inverse'			=> '[7m',
			'invisible'			=> '[8m',
			'strikethrough'		=> '[9m',
			'boldoff'			=> '[22m',
			'italicsoff'		=> '[23m',
			'underlineoff'		=> '[24m',
			'blinkingoff'		=> '[25m',
			'inverseoff'		=> '[27m',
			'strikethroughoff'	=> '[29m',
			'black'				=> '[30m',
			'red'				=> '[31m',
			'green'				=> '[32m',
			'blackongray'		=> '[0;47m',
			'yellow'			=> '[33m',
			'blue'				=> '[34m',
			'magenta'			=> '[35m',
			'cyan'				=> '[36m',
			'white'				=> '[37m',
			'underlineblack'	=> '[30;4m',
			'underlinered'		=> '[31;4m',
			'underlinegreen'	=> '[32;4m',
			'underlineyellow'	=> '[33;4m',
			'underlineblue'		=> '[34;4m',
			'underlinemagenta'	=> '[35;4m',
			'underlinecyan'		=> '[36;4m',
			'underlinewhite'	=> '[37;4m',
			'boldblack'			=> '[30;1m',
			'boldred'			=> '[31;1m',
			'boldgreen'			=> '[32;1m',
			'boldyellow'		=> '[33;1m',
			'boldblue'			=> '[34;1m',
			'boldmagenta'		=> '[35;1m',
			'boldcyan'			=> '[36;1m',
			'boldwhite'			=> '[37;1m',
			'blackonred'		=> '[30;41m',
			'blackongreen'		=> '[30;42m',
			'blackonyellow'		=> '[30;43m',
			'blackonblue'		=> '[30;44m',
			'blackonmagenta'	=> '[30;45m',
			'blackoncyan'		=> '[30;46m',
			'blackonwhite'		=> '[30;47m',
			'redonblack'		=> '[31;40m',
			'redongreen'		=> '[31;42m',
			'redonyellow'		=> '[31;43m',
			'redonblue'			=> '[31;44m',
			'redonmagenta'		=> '[31;45m',
			'redoncyan'			=> '[31;46m',
			'redonwhite'		=> '[31;47m',
			'greenonblack'		=> '[32;40m',
			'greenonred'		=> '[32;41m',
			'greenonyellow'		=> '[32;43m',
			'greenonblue'		=> '[32;44m',
			'greenonmagenta'	=> '[32;45m',
			'greenoncyan'		=> '[32;46m',
			'greenonwhite'		=> '[32;47m',
			'yellowonblack'		=> '[33;40m',
			'yellowonred'		=> '[33;41m',
			'yellowongreen'		=> '[33;42m',
			'yellowonblue'		=> '[33;44m',
			'yellowonmagenta'	=> '[33;45m',
			'yellowoncyan'		=> '[33;46m',
			'yellowonwhite'		=> '[33;47m',
			'blueonblack'		=> '[34;40m',
			'blueonred'			=> '[34;41m',
			'blueongreen'		=> '[34;42m',
			'blueonyellow'		=> '[34;43m',
			'blueonmagenta'		=> '[34;45m',
			'blueoncyan'		=> '[34;46m',
			'blueonwhite'		=> '[34;47m',
			'magentaonblack'	=> '[35;40m',
			'magentaonred'		=> '[35;41m',
			'magentaongreen'	=> '[35;42m',
			'magentaonyellow'	=> '[35;43m',
			'magentaonblue'		=> '[35;44m',
			'magentaoncyan'		=> '[35;46m',
			'magentaonwhite'	=> '[35;47m',
			'cyanonblack'		=> '[36;40m',
			'cyanonred'			=> '[36;41m',
			'cyanongreen'		=> '[36;42m',
			'cyanonyellow'		=> '[36;43m',
			'cyanonblue'		=> '[36;44m',
			'cyanonmagenta'		=> '[36;45m',
			'cyanonwhite'		=> '[36;47m',
			'whiteonblack'		=> '[37;40m',
			'whiteonred'		=> '[37;41m',
			'whiteongreen'		=> '[37;42m',
			'whiteonyellow'		=> '[37;43m',
			'whiteonblue'		=> '[37;44m',
			'whiteonmagenta'	=> '[37;45m',
			'whiteoncyan'		=> '[37;46m'
			);

	public static $aMovements = array(
			// cursor movements
			'cursortoh'			=> '[%1$d;%2$dH', // might work for some (line;column)
			'cursortof'			=> '[%1$d;%2$df', // might work for others
			'cursorup'			=> '[%1$dA',	// move up x lines
			'cursordown'		=> '[%1$dB',	// move down x lines
			'cursorright'		=> '[%1$dC',	// move right x spaces
			'cursorleft'		=> '[%1$dD',	// move left x spaces
			'cursorpos'			=> '[%1$d;%2$dR', // report current pos line;col
			'cursorsave'		=> '[s',		// save cursor pos
			'cursorreturn'		=> '[u',		// return to saved cursor pos
			'cursorcleara'		=> '[2J', 		// works for some
			'cursorclearb'		=> 'c',			// works for others
			'cursorcleareol'	=> '[K'
			);


	/**
	 * flatten($aBits)
	 * __Function_description_here__
	 *
	 * @param array $aBits __parameter_description_here__
	 *
	 * @return public static __return_description_here__
	 *
	 * @uses __something__ - __uses_desciption_here__
	 *
	 * @package __SssS__ - __package_desciption_here__
	 * @author Luke JZ aka SwissalpS <SwissalpS@LukeZimmerman.com>
	 * @license __license_url_here__
	 * @copyright 2007 Luke JZ aka SwissalpS
	 * @version 20091106_005806 (CC)2007 Luke JZ aka SwissalpS
	 */
	public static function flatten($aBits) {

		$sFlat = '';

		foreach ($aBits as $sBit) {

			$sRes = SssS_AnsiEscapes::gimme($sBit);

			$sFlat .= ('' !== $sRes) ? $sRes : $sBit;

		} // foreach bit

		return $sFlat;

	} // flatten



	public static function gimme($sName = 'neutral') {
		$sName = strtolower($sName);
		if (isset(SssS_AnsiEscapes::$aColours[$sName]))
				return chr(27) . SssS_AnsiEscapes::$aColours[$sName];
		if (isset(SssS_AnsiEscapes::$aMovements[$sName]))
				return chr(27) . SssS_AnsiEscapes::$aMovements[$sName];
		if ('escape' == $sName) return chr(27);
		if ('bell' == $sName) return chr(7);
		return '';
	} // gimme

	static function __toString() { return print_r(self, true); } // __toString

} // SssS_AnsiEscapes
/* * * *\ SssS_AnsiEscapes.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
