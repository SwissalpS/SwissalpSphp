<?php
/* * * *
 * * SwissalpS/Games/Rubik/Scrambler.inc
 * * Rubik's scrambler for NxNxN cubes
 * *
 * * Sides are denoted as follows:
 * * F = front, facing user
 * * B = back, facing away from user
 * * U = up, facing the ceiling/sky
 * * D = down, facing feet/lap
 * * L = left
 * * R = right
 * *
 * * Always turn as though looking at the face in clockwise direction
 * * unless there is a "'" denoting counter-clockwise direction.
 * *
 * * example output for an 8x8x8 cube:
 * * R₁₊₂' F₄2 R₃2 U₄2 F₄2 R₃2 U₄2 F₂' U₂₋₄' R₂₋₄ F₂₊₃ U₃₊₄2 F₁₊₂' U₁₊₂2 F₁₋₃' U₁₊₂ R₂2 U₁₊₂2 R2
 * *
 * * explanation of used syntax:
 * * R₁₊₂' => rotate the 2 rightmost slices counter-clockwise by 90 degrees
 * * F₄2 => rotate the fourth slice from the front by 180 degrees
 * * R2 => rotate the rightmost slice ie the right face clockwise by 90 degrees
 * * R₂₋₄ => rotate the second, third and forth slices from the right clockwise by 90 degrees
 * *
 * * usage example:
 * * require_once('Scrambler.inc');
 * * use \SwissalpS\Games\Rubik\Scrambler as Scrambler;
 * *
 * * // outputs 44 moves for a 3x3x3 (standard Rubik's cube)
 * * // only using F, R and U moves
 * * echo Scrambler::scramble();
 * *
 * * // outputs 22 moves for a 4x4x4 (Rubik's revenge cube)
 * * // including B, L and D moves
 * * echo Scrambler::scramble(4, 22, false);
 * *
 * * // outputs 55 moves for a 22x22x22 (current world record https://youtu.be/lzzD1aNsLVU)
 * * // only using F, R and U moves and including moves past the middle slice eg R₄₋₁₇
 * * echo Scrambler::scramble(22, 55, true, false);
 * *
 * * Output is for UTF-8, so if you want to output in other encoding such as iso-xxx
 * * you need to use multibyte-encoding-conversion functions
 * *
 * * @version 20170808_110427 (CC) Luke JZ aka SwissalpS idiot-proofing
 * * @version 20170806_141732 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\Games\Rubik;

class Scrambler {

	static $aSides = array('F', 'R', 'U', 'B', 'L', 'D');

	/* *
	 * * function scramble()
	 * *
	 * * $iSideCount: number of segments per side
	 * * $iMoveCount: number of moves to output
	 * * $bFRUonly: true => only use F, R and U moves
	 * * $bAvoidSlicingOverCenter: true => eg 3x3x3, avoid moves like R₁₊₂
	 * * 							which is equal to L
	 * * $bNoInnerSlices: true => no R₂₋₄ instead make it R₁₋₄ and express as R₄
	 * *
	 * * version 20170808_110427 (CC) Luke JZ aka SwissalpS
	 * */
	static function scramble($iSideCount = 3, $iMoveCount = 44,
							 $bFRUonly = true, $bAvoidSlicingOverCenter = true,
							 $bNoInnerSlices = false) {

		// clean input
		$iSideCount = floor(abs($iSideCount));
		$iMoveCount = floor(abs($iMoveCount));

		// prepare output
		$sOut = '';

		// cache multibyte numbers
		$aNumberStrings = array(self::subscriptNumber(0));
		for ($iCount = 1; $iCount < $iSideCount; $iCount++) {

			$aNumberStrings[] = self::subscriptNumber($iCount);

		} // loop building multibyte cache

		// cache subscript minus and plus signs
		$sMinus = self::subscriptMinus();
		$sPlus = self::subscriptPlus();

		// cache max slice
		$iSideCountMax = $bAvoidSlicingOverCenter ? floor($iSideCount * 0.5) - 1 : $iSideCount - 2;

		$sSideLast = 'D';

		// loop until requested number of moves are collected
		while (0 < $iMoveCount) {

			$bChooseAnotherSide = true;

			while ($bChooseAnotherSide) {

				$sSideNext = self::randomSide($bFRUonly);

				$bChooseAnotherSide = ($sSideLast == $sSideNext);

			} // choose a side other than what was chosen last move

			// remember last side
			$sSideLast = $sSideNext;

			// append side to algo
			$sOut .= $sSideNext;

			// choose first slice

			if ($bNoInnerSlices) {

				$iSliceFirst = 0;

			} else {

				$iSliceFirst = rand(0, $iSideCountMax);

			} // if no inner slices

			// choose last slice
			$iSliceLast = rand($iSliceFirst, $iSideCountMax);

			if ($iSliceFirst == $iSliceLast) {

				if (0 < $iSliceFirst) {

					$sOut .= $aNumberStrings[$iSliceFirst + 1];

				} // if not outermost slice

			} else {

				if (!$bNoInnerSlices) {

					$sOut .= $aNumberStrings[$iSliceFirst + 1];
					$sOut .= ($iSliceFirst + 1 == $iSliceLast) ? $sPlus : $sMinus;

				} // if no inner slices

				$sOut .= $aNumberStrings[$iSliceLast + 1];

			} // append slice(s)

			$b180 = rand(0, 1);

			if ($b180) {

				$sOut .= '2';

			} else {

				// append direction
				$sOut .= self::randomDirection();

			} // if 180 or 90 degree turn

			// append space
			$sOut .= ' ';

			// decrement counter
			$iMoveCount--;

		} // loop moves

		return $sOut;

	} // scramble


	static function randomDirection() {

		$bBackward = rand(0, 1);

		return $bBackward ? "'" : "";

	} // randomDirection


	static function randomSide($bFRUonly = true) {

		$iMax = $bFRUonly ? 2 : 5;

		$iSide = rand(0, $iMax);

		return self::$aSides[$iSide];

	} // randomSide


	static function subscriptMinus() {

		return json_decode('"\u208B"');

	} // subscriptMinus


	static function subscriptNumber($iVal) {

		$sOut = '';

		// convert number to string
		$sIn = '' . $iVal . '';

		// loop each digit
		$iTotal = strlen($sIn);
		for ($iCount = 0; $iCount < $iTotal; $iCount++) {

			$sOut .= self::unichr(2080 + $sIn{$iCount});

		} // loop digits

		return $sOut;

	} // subscriptNumber


	static function subscriptPlus() {

		return json_decode('"\u208A"');

	} // subscriptPlus


	static function unichr($iVal) {

		$sVal = '\u' . $iVal;
		return json_decode('"' . $sVal . '"');

	} // unichr

} // Scrambler

/* * * *\ Rubik's scrambler (CC) Luke JZ aka SwissalpS /* * * */
?>