#!/usr/bin/php
<?php
/* * * *
 * * SssS_BaseConverter.php
 * * convert base ten integers to any base from (dec) 2 to 62
 * *
 * * for bases > 36 case matters: lower case -> (dec) 10 to 35
 * *							  upper case -> (dec) 36 to 61
 * *
 * * TODO: conversions from string to dec for those types not well supported
 * * by pack and unpack
 * *
 * * @version 20100421_122214 (CC) Luke JZ aka SwissalpS
 * * * */
//error_reporting(E_ALL); // 0); //

if (!function_exists('div')) {
// return the int part of a division without rounding
// both inputs will have any digits after . cut off before division takes place
function div($x, $y) {
	// for 0 instead of error, uncomment next line
	//if (0 == $y) return 0;

	return intval(intval($x) / intval($y));
} // div
} // if not declared yet

if (!function_exists('itemAtIndex')) {
// return the item of an array at specified index or throw error
// allows negative indexes to access from the back
function itemAtIndex($a, $iIndex) {
	if ('array' != gettype($a))
		trigger_error('itemAtIndex: first argument must be an array!'
		, E_USER_ERROR);

	$iCount = count($a);
	$idx = intval($iIndex);
	$idxAbs = abs($idx);

	// is positive?
	if (-1 < $idx) {
		// check range
		if ($idxAbs >= $iCount)
			trigger_error('itemAtIndex: index (' . $idx
										. ') is out of bounds!', E_USER_ERROR);
		// in range & positive -> normal
	 	return $a[$idx];
	} // if positive

	// negative
	// check range
	if ($idxAbs > $iCount)
		trigger_error('itemAtIndex: index (' . $idx
										. ') is out of bounds!', E_USER_ERROR);

	return $a[$iCount - $idxAbs];
} // itemAtIndex
} // if not declared yet



class SssS_BaseConverter {
	// theoretically you could fill up all unicode characters
	private $iBaseMax = 62;
	// smallest possible base
	private $iBaseMin = 2;
	// all available symbols (you can easilly extend them)
	private $aAllSymbols = array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z");

	private $aCurrentSymbolsPositive;
	private $aCurrentSymbolsNegative;
	private $aCurrentSymbolsNegativePadders;

	private $aProps;

/*	// default base to convert to
	var iBase = 16;
	// for padding and justification. "how many chars make a value?" (bytes * 2)
	property iWordLength = 8;
	property bRightJustified = true;
	// hex values are case insensitive.
	// This flag applies only to bases > 36 with values > 35 for reading
	// For output of bases > 10 and bases < 36 this defines output: true => lowercase
	bUseCapsWhereCaseIsInsignificant = true;
	property bBigEndianInput = true;
	property bBigEndianOutput = true;

	// constructor
	function SssS_BaseConverter(iBase = 16, iWordLength = 8, bRightJustified = true, bUseCapsWhereCaseIsInsignificant = true, bBigEndianInput = true, bBigEndianOutput = true) {
*/
	// constructor
	public function SssS_BaseConverter($aArgs = false) {

		$this->aProps = array(
			'iBase'=> 16,
			'iWordLength'=> 8,
			'bRightJustified'=> true,
			'bUseCapsWhereCaseIsInsignificant'=> true,
			'bBigEndianInput'=> true,
			'bBigEndianOutput'=> true);

		if ((false != $aArgs) && ('array' == gettype($aArgs))) {
			$this->setBase(((isset($aArgs['iBase']))
								? $aArgs['iBase'] : $this->aProps['iBase']));

			if (isset($aArgs['iWordLength']))
									$this->setWordLength($aArgs['iWordLength']);

			if (isset($aArgs['bRightJustified']))
					$this->setBoolForKey($aArgs['bRightJustified'],
							'bRightJustified');

			if (isset($aArgs['bUseCapsWhereCaseIsInsignificant']))
					$this->setBoolForKey(
							$aArgs['bUseCapsWhereCaseIsInsignificant'],
							'bUseCapsWhereCaseIsInsignificant');

			if (isset($aArgs['bBigEndianInput']))
					$this->setBoolForKey($aArgs['bBigEndianInput'],
							'bBigEndianInput');

			if (isset($aArgs['bBigEndianOutput']))
					$this->setBoolForKey($aArgs['bBigEndianOutput'],
							'bBigEndianOutput');

//			if (isset($aArgs[])) $this->setBoolForKey(, );
		} // if valid args array
		else $this->setBase($this->aProps['iBase']);

	} // SssS_BaseConverter

	/* public setters and getters BEGIN */

	// sets the base to use for output
	// returns false if iNewBase is not in range of allowed bases
	// negative and floats are converted to absolute integers without rounding
	public function setBase($iNewBase) {
		if (($iNewBase < $this->iBaseMin)
			|| ($iNewBase > $this->iBaseMax)) return false;

		$iB = intval(abs($iNewBase));
		$this->aProps['iBase'] = $iB;

		$this->aCurrentSymbolsPositive = array_slice($this->aAllSymbols, 0, $iB);
		$this->aCurrentSymbolsNegative = array_slice($this->aAllSymbols, 1, $iB -1);
		$this->aCurrentSymbolsNegative []= $this->aAllSymbols[0];
		$iB2 = ceil($iB * 0.5);
		$this->aCurrentSymbolsNegativePadders =
									array_slice($this->aAllSymbols, $iB2, $iB2);

		return true;
	} // setBase

	// returns current base
	public function base() { return $this->aProps['iBase']; } // base

	// sets the WordLength for padding - defaults to 8 ('4 bytes')
	// raises error if iNewLength is 0
	// negative and floats are converted to absolute integers without rounding
	public function setWordLength($iNewLength) {
		$iWL = intval(abs($iNewLength));
		if (0 == $iWL) trigger_error('SssS_BaseConverter::setWordLength:'
				. ' argument must NOT be 0!', E_USER_ERROR);

		$this->aProps['iWordLength'] = $iWL;

		return true;
	} // setWordLength

	public function wordLength($iNewLength) {
		return $this->aProps['iWordLength'];
	} // wordLength

	// sets true for key sKey only if true === bValue
	// any other value for bValue sets sKeys value to false!
	public function setBoolForKey($bValue, $sKey) {
		$this->aProps[$sKey] = (true === $bValue) ? true : false;
	} // setBoolForKey

	// returns true only if sKeys value is true else false
	public function boolForKey($sKey) {
		if (!isset($this->aProps[$sKey]))
			trigger_error('SssS_BaseConverter::boolForKey: unknown key ('
					. $sKey . ')', E_USER_ERROR);

		return (true === $this->aProps[$sKey]) ? true : flase;
	} // boolForKey

	// set arbitrary symbols to be used
	// updates current symbol arrays with new set of symbols
	// also if base was higher than the new symbol array allows to go then
	// changes base to highest possible base in new symbol array.
	public function setAllSymbols($a) {
		if ('array' != gettype($a))
			trigger_error('SssS_BaseConverter::setAllSymbols: argument must be'
				. ' an array!', E_USER_ERROR);

		$iCount = count($a);

		if (2 > $iCount) trigger_error('SssS_BaseConverter::setAllSymbols:'
				. ' need at least two symbols', E_USER_ERROR);

		// seems ok
		$this->aAllSymbols = $a;
		$this->iBaseMax = $iCount;
		$iB = $this->base();
		if ($iB > $iCount) $iB = $iCount;
		$this->setBase($iB);
	} // setAllSymbols

	public function allSymbols() { return $this->aAllSymbols; } // allSymbols

	public function currentSymbolsPositive() {
		return $this->aCurrentSymbolsPositive;
	} // currentSymbolsPositive

	public function currentSymbolsNegative() {
		return $this->aCurrentSymbolsNegative;
	} // currentSymbolsNegative

	public function currentSymbolsNegativePadders() {
		return $this->aCurrentSymbolsNegativePadders;
	} // currentSymbolsNegativePadders

	public function baseMax() { return $this->iBaseMax; } // baseMax

	public function baseMin() { return $this->iBaseMin; } // baseMin

	/* public setters and getters END */

	/* public conversion to string functions BEGIN */

	public function dec2str($mDec, $iWL = false) {
		if (!$iWL) { $iWL = $this->aProps['iWordLength']; }
		else { $iWL = intval(abs($iWL)); }

		$sType = gettype($mDec);
		if ('integer' != $sType) trigger_error('SssS_BaseConverter::dec2str:'
				. ' invalid type (' . $sType . ') for argument. currently I'
				. ' only handle (signed) integers. I\'ve chopped off without'
				. ' rounding.', E_USER_NOTICE);

		$iDec = intval($mDec);

		$sOut = ''; $iWidth = 0; $iB = $this->base();

		if (-1 < $iDec) {
			// positive number
			do {
				$idx = $iDec % $iB;
				$sDigit = itemAtIndex($this->aCurrentSymbolsPositive, $idx);
//echo "idec:$iDec  idx: $idx     s: $sDigit\n";
				$sOut = $sDigit . $sOut; // prepend string with translated digit
				$iDec = div($iDec, $iB);
				$iWidth++;
			} while (! ((0 == $iDec) && (0 == $iWidth % $iWL)));
		} else {
			// negative number
			do {
				$idx = ($iDec % $iB) -1;
				$sDigit = itemAtIndex($this->aCurrentSymbolsNegative, $idx);
//echo "idec:$iDec  idx: $idx     s: $sDigit\n";
				$sOut = $sDigit . $sOut; // prepend string with translated digit
				$iDec = div($iDec +1, $iB) -1;
				$iWidth++;
			} while (! ((-1 == $iDec) && (0 == $iWidth % $iWL) && (in_array($sDigit, $this->aCurrentSymbolsNegativePadders))));
		} // if pos or neg

		if (($this->aProps['bUseCapsWhereCaseIsInsignificant']) && (37 > $iB))
			$sOut = strtoupper($sOut);

		return $sOut;
	} // dec2str

	public function dec2array($mDec, $iWL = false) {
		if (!$iWL) { $iWL = $this->aProps['iWordLength']; }
		else { $iWL = intval(abs($iWL)); }

		$sType = gettype($mDec);
		if ('integer' != $sType) trigger_error('SssS_BaseConverter::dec2str:'
				. ' invalid type (' . $sType . ') for argument. currently I'
				. ' only handle (signed) integers. I\'ve chopped off without'
				. ' rounding.', E_USER_NOTICE);

		$iDec = intval($mDec);

		$aOut = array(); $iWidth = 0; $iB = $this->base();

		if (-1 < $iDec) {
			// positive number
			do {
				$idx = $iDec % $iB;
				$sDigit = itemAtIndex($this->aCurrentSymbolsPositive, $idx);
				echo "idec:$iDec  idx: $idx     s: $sDigit\n";
				array_unshift($aOut, $sDigit); // prepend array with translated digit
				$iDec = div($iDec, $iB);
				$iWidth++;
			} while (! ((0 == $iDec) && (0 == $iWidth % $iWL)));
		} else {
			// negative number
			do {
				$idx = ($iDec % $iB) -1;
				$sDigit = itemAtIndex($this->aCurrentSymbolsNegative, $idx);
				$sDigitOut = (($this->aProps['bUseCapsWhereCaseIsInsignificant'])
					&& (37 > $iB)) ? strtoupper($sDigit) : $sDigit;
				echo "idec:$iDec  idx: $idx     s: $sDigit\n";
				array_unshift($aOut, $sDigitOut); // prepend array with translated digit
				$iDec = div($iDec +1, $iB) -1;
				$iWidth++;
			} while (! ((-1 == $iDec) && (0 == $iWidth % $iWL) && (in_array($sDigit, $this->aCurrentSymbolsNegativePadders)))); //(14 > $iWidth); //
		} // if pos or neg

		return $aOut;
	} // dec2array

	/* public conversion to string functions END */

	function __toString() { return print_r($this, true); } // __toString

} // SssS_BaseConverter


/* * * *\ SssS_BaseConverter.php (CC) Luke JZ aka SwissalpS /* * * */
?>
