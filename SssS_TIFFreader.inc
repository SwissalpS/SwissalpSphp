<?php
/* * * *
 * * SssS_TIFFreader.inc
 * *
 * * TODO: lots
 * * CAUTION alpha stage, will crash on medium sized images!!! PHP is not
 * * very good at this -> if you can do it with gdlib or similar use that!
 * *
 * * @version 20100422_022730 (CC) Luke JZ aka SwissalpS
 * * * */
//error_reporting(E_ALL); // 0); //

if (!defined('AUTO_INCLUDING')) {
	require_once('/Volumes/UNIX/ImgSrach/public/SssS/SssS_BaseConverter.php');
	require_once('/Volumes/UNIX/ImgSrach/public/SssS/SssS_FileGetPutContents.inc');
	require_once('/Volumes/UNIX/ImgSrach/public/SssS/_external/CustomException.inc');
} // if not autoincluding but on php's include path

class TIFFreaderException extends CustomException {} // TIFFreaderException

define("SssS_LITTLE_ENDIAN", 0);
define("SssS_BIG_ENDIAN", 1);

define("SssS_ERROR_ENDIAN_MARKER_INVALID", 0x01);
define("SssS_ERROR_TIFF_MARKER_INVALID", 0x02);
define("SssS_ERROR_TIFF_TOO_SHORT", 0x04);
define("SssS_ERROR_TIFF_IFD_POINTER_OOB", 0x08); // points over eof
define('SssS_ERROR_TIFF_IFD_NO_ENTRIES', 0x10);
define('SssS_ERROR_TIFF_IFD_UNKNOWN_TYPE', 0x20);
define('SssS_ERROR_FILE_NOT_READABLE', 0x40);
define('SssS_ERROR_FILE_NOT_WRITEABLE', 0x80);
define("SssS_ERROR_TIFF_TOO_LONG", 0x0100);

/*   Junk while building */
//$o = new SssS_BaseConverter();

//$o->setAllSymbols(array("h", "H"));
//var_dump($o->dec2array(-255, 1));
//var_dump($o->dec2str(255, 1));
//var_dump($o->currentSymbolsNegative());
//var_dump($o->currentSymbolsPositive());
//var_dump($o->currentSymbolsNegativePadders());
/*var_dump($o);

echo '
' . gettype(0);//itemAtIndex($o->aCurrentSymbolsNegativePadders, 8);
echo strtolower("   aeuTAOEH");
echo strtoupper("   aeuTAOEH");
echo ceil(3.0);exit;
echo intval(-34.32);exit;
echo div(45.8, 10.2);
$x = 0;
do {
	echo $x++ . chr(10);
	if (12<$x) break;
} while (true);
*/

/*	DEMO 	DEMO 	DEMO 	DEMO 	DEMO 	DEMO 	DEMO 	DEMO 	DEMO * /
$sPath2Tiff = '/Volumes/Users/luke/Desktop/captchaImage.tiff';

// catch exceptions for smooth experience
try {
	// make a new reader and read
	$oTR = new SssS_TIFFreader($sPath2Tiff);

	// above can also be done this way:
	//$oTR = new SssS_TIFFreader();
	//$oTR->readTIFF($sPath2Tiff); // this is where the exceptions occur (if)

} catch (TIFFreaderException $e) {
	// analyze and deal with exception (this demo doesn't, it just outputs some info)
	die($e->__toString());
} catch (Exception $e) {
	// other Exception
	die();
} // try - catch exceptions: SssS_TIFFreader throws only on fatal, other trips trigger error E_USER_NOTICE

// ask for an array (hash) of IFD entries that this reader can deal with.
// first trying if IFD0 is useable. 0 is not so meaningfull -> can be ommited
// if you were interested in the fourth for instance, you would pass a 3
// returns an array with the first useable IFD array (hash) and its index
// if there are none, will return array(false, 0);
list($hIFD, $idx) = $oTR->anyUseableIFD(0);

// you can access existing entries using its tag as decimal or hex
$iWidth = $hIFD['aIFDentries'][0x100]['aValues'][0];

// or use the simpler accessor
// returns false if tag/IFD doesn't exist, a value if tag holds 1,
// an array of values if tag holds more than 1. 'a value' may also be an array!
// if $idx is ommited, anyUsebaleIFD() is used
$iHeight = $oTR->valueForTagOfIFD(0x101, $idx);

// now you can ask for pixel info and loop
//for ($y = 0; $y < $iHeight; $y++) {
//	for ($x = 0; $x < $iWidth; $x++) {
//		// will use anyUsebaleIFD() if $idx is ommited
//		$aPixelValues = $oTR->pixelValuesOfIFD($x, $y, $idx);
//		// do something...
//	} // for x
//} // for y

// or ask for the 2 dimensional array with all pixel values
// if idx is ommited, anyUsebaleIFD() is used
$aPixels = $oTR->pixelValuesOfIFDasArray($idx);

var_dump($aPixels);

echo "done";

/*   DEMO END    DEMO END    DEMO END    DEMO END    DEMO END    DEMO END */

class SssS_TIFFreader {

	protected $aPixelValuesPerIFD = array();

	// BOOL true if machine uses big endian
	protected $bMachineByteOrderIsBigEndian;

	// string containing raw tiff data
	protected $sDataTIFF;

	// the length in bytes of the inputfile (filesize())
	protected $iDataLen;

	// constructor
	function SssS_TIFFreader($sPath2tiff = false) {
		$this->bMachineByteOrderIsBigEndian = machineByteOrderIsBigEndian();

		if (false != $sPath2tiff) $this->readTIFF($sPath2tiff);

	} // SssS_TIFFreader

	// either throws exception, triggers error or returns false if can't read
	// returns a 0 based index of the first usable IFD
	// if you check against false, be sure to use === strict comparison as
	// 0 is a good reply
	public function readTIFF($sPath2tiff) {
		// 'release' old data
		unset($this->sDataTIFF, $this->hTIFF);

		// compare free memory and filesize
		$this->iDataLen = filesize($sPath2tiff);
		if (false === $this->iDataLen) {
			throw new TIFFreaderException('can not read file: '	. $sPath2tiff,
												SssS_ERROR_FILE_NOT_READABLE);
		} else if (60 > $this->iDataLen) {
			throw new TIFFreaderException('filesize to small, no reasonable'
					. ' data possible', SssS_ERROR_TIFF_TOO_SHORT);
		// reserving a quarter of total size for overhead ->fMemUseFactor()
		} else if (maxMemInBytes() < ($this->fMemUseFactor() * $this->iDataLen)) {
			throw new TIFFreaderException('file is to large for the memory'
					. ' available to PHP (depending on your setup you may be'
					. ' able to increase this to at least: '
					. ($this->fMemUseFactor() * $this->iDataLen),
					SssS_ERROR_TIFF_TOO_LONG);
		} // TODO: routine/class that splits file and maps locations for simple 'readFromTo' access

		// php doesn't support reading from, to so we need to read all at once,
		// then parse out what we want
		$this->sDataTIFF = file_get_contents_safely($sPath2tiff, 'rb');
		if (false === $this->sDataTIFF)
			throw new TIFFreaderException('can not read file: ' . $sPath2tiff,
												SssS_ERROR_FILE_NOT_READABLE);

		$sData = &$this->sDataTIFF;

		$this->hTIFF = $this->defaultTIFFdict();

//		-- TIFF header (8-bytes)long ; offset 0
//		--http://partners.adobe.com/public/developer/en/tiff/TIFF6.pdf
//		-- bytes
//		-- 0 + 1 => big/little endian (MM/II) x4D4D / x4949
//		-- 2 + 3 => 42 -- constant tiff id x2A
//		-- 4, 5, 6 + 7 => offset of first IFD = A
//		-- A + A+1 => number of entries = B
//		-- A+2+B*12 => offset of next IFD = C

		// get the endian marker (2 bytes)
		$a = unpack('C2EM', substr($sData, 0, 2)); //byteOrder
//var_dump($a);
		if (2 > count($a)) throw new TIFFreaderException('Missing the endian'
				. ' marker in file: ' . $sPath2tiff,
				SssS_ERROR_ENDIAN_MARKER_INVALID);

		$iEM1 = $a['EM1']; $iEM2 = $a['EM2'];
		if ($iEM1 != $iEM2) throw new TIFFreaderException('Endian marker bytes'
				. ' mismatch in file: ' . $sPath2tiff,
				SssS_ERROR_ENDIAN_MARKER_INVALID);

		if ((77 != $iEM1) && (73 != $iEM1)) throw new TIFFreaderException(
				'Endian marker is neither big nor little endian in file: '
				. $sPath2tiff, SssS_ERROR_ENDIAN_MARKER_INVALID);

		$iEndian = (77 == $iEM1) ? SssS_BIG_ENDIAN : SssS_LITTLE_ENDIAN;
		$bBigEndian = (SssS_BIG_ENDIAN == $iEndian);
		$this->hTIFF['iEndian'] = $iEM1;

		// get the tiff marker (2 bytes)
		unset($a);
		$sFormat = ($bBigEndian) ? 'n' : 'v'; // unsigned short (2 bytes)
		$a = unpack($sFormat, substr($sData, 2, 2));
		if (42 != $a[1]) trigger_error(SssS_ERROR_TIFF_MARKER_INVALID,
																E_USER_NOTICE);

		// read IFDs
		$iIFDpos = 4;
		do {
			$iIFDstart = unsignedLongFromString(substr($sData, $iIFDpos, 4),
					$this->bMachineByteOrderIsBigEndian, $bBigEndian);
			if (0 == $iIFDstart) break; // 00 00 00 00 marks no more IFDs (4 bytes of 0)

			if ($this->iDataLen < $iIFDstart) {
				trigger_error(SssS_ERROR_TIFF_IFD_POINTER_OOB, E_USER_NOTICE);
				break;
			} // if next IFD is out of bounds

			$hIFD = $this->defaultIFDdict();
			$hIFD['iIFDstartPos'] = $iIFDstart;

//			-- TIFF IFD (12-bytes * B + 2)long ; offset A
//			-- bytes in IFD offset
//			-- 0 + 1 => Number of entries = n
//			-- 2 - 13 => Entry 0 = X1
//			-- 2+(n-1)*12 - (2+(n-1)*12)+11 = Entry n = Xn
//			-- 2+n*12 - 3+2+n*12 => next IFD's offset = An

			// check n -> how many entries in this IFD (2 bytes)
			unset($a);
			$sFormat = ($bBigEndian) ? 'n' : 'v'; // unsigned short (2 bytes)
			$a = unpack($sFormat, substr($sData, $iIFDstart, 2));
			$n = $a[1];
			if (0 == $n) {
				trigger_error(SssS_ERROR_TIFF_IFD_NO_ENTRIES, E_USER_NOTICE);
				break;
			} // if no IFD entries -> hope there was at least one IFD

			$hIFD['iEntries'] = $n;

			// set next IFDs pointer position
			$iIFDpos = $iIFDstart + 2 + $n * 12;

			// loop directory entries
			$iEntryPos = $iIFDstart + 2;
			for ($iCountEntries = 0; $iCountEntries < $n; $iCountEntries++) {
				$hIFDentry = array();

//-- TIFF IFD directory entry
//-- bytes in Directory entry offset (12-bytes)long ; offset A + 2 + (n-1) * 12
//-- 0 + 1 => Tag = X (according to specs tags must be ordered ascending but not all must be given)
//-- 2 + 3 => Type (1 = byte 8-bit, 2 = ascii 8-bit [7ascii+1null],
//--				  3 = short 16-bit unsigned int, 4 = long 32-bit unsigned int,
//--				  5 = rational two longs [8-bytes] numerator and denominator
//-- 			TIFF6 also defines signed types:
//--				  6 = sByte  8-bit, 7 = undefined 8-bit, 8 = sShort 16-bit, 9 = sLong 32-bit,
//--				 10 = sRational two sLongs [4+4-bytes] numerator and denominator,
//--				 11 = float 4-byte IEEE, 12 = double 8-byte IEEE)
//-- 4, 5, 6 + 7 => Count (number of values) multiply with typelength to get actual byte-size
//-- 8, 9, 10 + 11 => Value or file-offset to Value if greater than 4 bytes (smaller are left justified)

				// read Tag -> X (2 bytes)
				unset($a); $sFormat = ($bBigEndian) ? 'n' : 'v'; // unsigned short (2 bytes)
				$a = unpack($sFormat, substr($sData, $iEntryPos, 2));
				$iTag = $a[1]; $hIFDentry['iTag'] = $iTag;

				// read Type
				unset($a); $sFormat = ($bBigEndian) ? 'n' : 'v'; // unsigned short (2 bytes)
				$a = unpack($sFormat, substr($sData, $iEntryPos + 2, 2));
				$iType = $a[1]; $hIFDentry['iType'] = $iType;

				// read count (4 bytes)
				unset($a);// $sFormat = ($bBigEndian) ? 'N' : 'V'; // unsigned long (4 bytes)
				// PHP has a bug with this type, so we need to do some checking
				// and use machine's byte ordering
				$iCountValues = unsignedLongFromString(
						substr($sData, $iEntryPos + 4, 4),
						$this->bMachineByteOrderIsBigEndian, $bBigEndian);
				$hIFDentry['iCountValues'] = $iCountValues;

				// determine byte length per value
				$bSkipThisEntry = false;
				switch ($iType) {
					case 1: // BYTE 8-bit unsigned integer
					case 6: // SBYTE An 8-bit signed (twos-complement) integer
					case 7: // UNDEFINED An 8-bit byte that may contain anything, depending on the definition of the field
						$iByteLengthPerValue = 1; break;
					case 2: // ASCII 8-bit byte that contains a 7-bit ASCII code; the last byte must be NUL (binary zero) iCountValues is total length (including NULs)
						$iByteLengthPerValue = 1 / $iCountValues; break;
					case 3: // SHORT 16-bit (2-byte) unsigned integer
					case 8: // SSHORT A 16-bit (2-byte) signed (twos-complement) integer
							// for now ok, at next inspection we'll change this for SSHORT
						$iByteLengthPerValue = 2; break;
					case 4: // LONG 32-bit (4-byte) unsigned integer
					case 9: // SLONG A 32-bit (4-byte) signed (twos-complement) integer
					case 11: // FLOAT Single precision (4-byte) IEEE format
						$iByteLengthPerValue = 4; break;
					case 5: // RATIONAL Two LONGs:  the first represents the numerator of a fraction; the second, the denominator.
					case 10: // SRATIONAL Two SLONG’s:  the first represents the numerator of a fraction, the second the denominator
					case 12: // DOUBLE Double precision (8-byte) IEEE format
						$iByteLengthPerValue = 8; break;
					default: // specs tell us to skip unknown types
						trigger_error(SssS_ERROR_TIFF_IFD_UNKNOWN_TYPE, E_USER_NOTICE);
						$bSkipThisEntry = true;
						$hIFDentry['aSkippedEntries'][] = array(
								'sReason' => 'Unknown value type',
								'iType' => $iType, 'iTag' => $iTag);
				} // switch type for byte lengths

				if (!$bSkipThisEntry) {
					$hIFDentry['iByteLengthPerValue'] = $iByteLengthPerValue;

					$iByteLength = $iByteLengthPerValue * $iCountValues;
					// if false -> value portion is a file offset pointer somewhere and right justified
					// if true -> value is in value portion of directory entry and left justified
					$bValuesFitInEntry = ($iByteLength < 5);
					$hIFDentry['bValuesFitInEntry'] = $bValuesFitInEntry;

					// read value(s)
					$aValues = array();
					if ($bValuesFitInEntry) {
						$iValueOffset = $iEntryPos + 8;
					} else {
						// values at offset
						$iValueOffset = unsignedLongFromString(
								substr($sData, $iEntryPos + 8, 4),
								$this->bMachineByteOrderIsBigEndian, $bBigEndian);
					} // if values fit in entry or not
					$sChunk = substr($sData, $iValueOffset, $iByteLength);

					// values in entry
					unset($a);
					switch ($iType) {
						case 1: // BYTE 8-bit unsigned integer
						case 7: // UNDEFINED An 8-bit byte that may contain anything, depending on the definition of the field
							$sFormat = 'C*'; // unsigned char (1 byte)
							$aValues = array_values(unpack($sFormat, $sChunk));
							break;
						case 2: // ASCII 8-bit byte that contains a 7-bit ASCII code; the last byte must be NUL (binary zero) iCountValues is total length (including NULs)
							$sFormat = 'a*'; // NUL-padded string
							$aValues = array_values(unpack($sFormat, $sChunk));
							break;
						case 3: // SHORT 16-bit (2-byte) unsigned integer
							$sFormat = ($bBigEndian) ? 'n*' : 'v*'; // unsigned short (2 bytes)
							$aValues = array_values(unpack($sFormat, $sChunk));
							break;
						case 4: // LONG 32-bit (4-byte) unsigned integer
							$aValues = arrayOfUnsignedLongFromString($sChunk,
									$this->bMachineByteOrderIsBigEndian,
									$bBigEndian);
							break;
						case 5: // RATIONAL Two LONGs:  the first represents the numerator of a fraction; the second, the denominator.
							$a = arrayOfUnsignedLongFromString($sChunk,
									$this->bMachineByteOrderIsBigEndian,
									$bBigEndian);
							$aValues = array_chunk($a, 2);
							break;
						case 6: // SBYTE An 8-bit signed (twos-complement) integer
							$sFormat = 'c*'; // signed char (1 byte)
							$aValues = array_values(unpack($sFormat, $sChunk));
							break;
						case 8: // SSHORT A 16-bit (2-byte) signed (twos-complement) integer
							// PHP doesn't support this, so we'll read raw data and remember to reorder
							// when the time comes for us to read it
							$aValues = arrayOfSignedShortsFromString($sChunk,
											$this->bMachineByteOrderIsBigEndian,
											$bBigEndian);
							break;
						case 9: // SLONG A 32-bit (4-byte) signed (twos-complement) integer
							$aValues = arrayOfSignedLongFromString($sChunk,
											$this->bMachineByteOrderIsBigEndian,
											$bBigEndian);
							break;
						case 10: // SRATIONAL Two SLONG’s:  the first represents the numerator of a fraction, the second the denominator
							$a = arrayOfSignedLongFromString($sChunk,
											$this->bMachineByteOrderIsBigEndian,
											$bBigEndian);
							$aValues = array_chunk($a, 2);
							break;
						case 11: // FLOAT Single precision (4-byte) IEEE format
							// PHP uses machine dependent size and byte order, so this
							// may give strange results
							trigger_error('SssS_TIFFreader::: PHP may not intepret'
									.' Float correctly...', E_USER_NOTICE);
							$aValues = arrayOfFloatFromString($sChunk,
									$this->bMachineByteOrderIsBigEndian,
									$bBigEndian);
							break;
						case 12: // DOUBLE Double precision (8-byte) IEEE format
							// PHP uses machine dependent size and byte order, so this
							// may give strange results
							trigger_error('SssS_TIFFreader::: PHP may not intepret'
									.' Double correctly...', E_USER_NOTICE);
							$aValues = arrayOfDoubleFromString($sChunk,
									$this->bMachineByteOrderIsBigEndian,
									$bBigEndian);
							break;
					} // switch type to read values
					$hIFDentry['aValues'] = $aValues;

					// check for known tags
					switch ($iTag) {
						case 0xFE: // 254 NewSubfileType LONG 1 value(s)
								   // replaces 0xFF
								   // This field is made up of a set of 32 flag bits. Unused bits are expected to be 0. Bit 0 is the low-order bit
								   // default is 0 for all bits (0, 1 and 2 are defined atm)
							$hIFDentry['sTag'] = 'NewSubfileType';
							break;
						case 0xFF: // 255 SubfileType SHORT 1 value(s)
								   // depricated in favor of 0xFE
								   // 1 = full res, 2 = reduced res, 3 = single page
							$hIFDentry['sTag'] = 'SubfileType';
							break;
						case 0x100: // 256 ImageWidth SHORT or LONG 1 value(s)
									// no default
							$hIFDentry['sTag'] = 'ImageWidth';
							break;
						case 0x101: // 257 ImageLength SHORT or LONG 1 value(s)
									// no default
							$hIFDentry['sTag'] = 'ImageLength';
							break;
						case 0x102: // 258 BitsPerSample SHORT SamplesPerPixel value(s)
									// defaults to 1
							$hIFDentry['sTag'] = 'BitsPerSample';
							break;
						case 0x103: // 259 Compression SHORT 1 value(s)
									//  1 Uncompressed (default)
									//  2 CCITT 1D
									//  3 Group 3 Fax
									//  4 Group 4 Fax
									//  5 LZW
									//  6 JPEG
									//  32773 PackBits
							$hIFDentry['sTag'] = 'Compression';
							break;
						case 0x106: // 262 PhotometricInterpretation SHORT 1 value(s)
									//  0 WhiteIsZero
									//  1 BlackIsZero
									//  2 RGB
									//  3 RGB Palette
									//  4 Transparency mask
									//  5 CMYK
									//  6 YCbCr
									//  8 CIELab
									// required!
							$hIFDentry['sTag'] = 'PhotometricInterpretation';
							break;
						case 0x107: // 263 Threshholding SHORT 1 value(s)
									// 1 = nothing (default), 2 or 3
							$hIFDentry['sTag'] = 'Threshholding';
							break;
						case 0x108: // 264 CellWidth SHORT 1 value(s)
									// see Threshholding
							$hIFDentry['sTag'] = 'CellWidth';
							break;
						case 0x109: // 265 CellLength SHORT 1 value(s)
									// should only be present if Threshholding = 2
							$hIFDentry['sTag'] = 'CellLength';
							break;
						case 0x10A: // 266 FillOrder SHORT 1 value(s)
									// 1 (default) or 2 (not required for baseline reader)
							$hIFDentry['sTag'] = 'FillOrder';
							break;
						case 0x10D: // 269 DocumentName ASCII value(s)
							$hIFDentry['sTag'] = 'DocumentName';
							break;
						case 0x10E: // 270 ImageDescription ASCII value(s)
							$hIFDentry['sTag'] = 'ImageDescription';
							break;
						case 0x10F: // 271 Make ASCII value(s)
									// Manufacturer of the scanner, video digitizer, or other type of equipment used to generate the image. Synthetic images should not include this field.
							$hIFDentry['sTag'] = 'Make';
							break;
						case 0x110: // 272 Model ASCII value(s)
									// The model name or number of the scanner, video digitizer, or other type of equipment used to generate the image.
							$hIFDentry['sTag'] = 'Model';
							break;
						case 0x111: // 273 StripOffsets SHORT or LONG (recomended) StripsPerImage value(s)
									// byte offset to each strip
									// required! although TileOffsets can replace this
							$hIFDentry['sTag'] = 'StripOffsets';
							break;
						case 0x112: // 274 Orientation SHORT 1 value(s)
									// 1 (default), 2, 3, 4, 5, 6, 7 or 8
							$hIFDentry['sTag'] = 'Orientation';
							break;
						case 0x115: // 277 SamplesPerPixel SHORT 1 value(s)
									// default = 1
							$hIFDentry['sTag'] = 'SamplesPerPixel';
							break;
						case 0x116: // 278 RowsPerStrip SHORT or LONG (more compatible is long) 1 value(s)
									// keep so that each strip is ~8Kbytes (writers)
									// defaults to  2**32 - 1, which is effectively infinity. That is, the entire image is one strip
							$hIFDentry['sTag'] = 'RowsPerStrip';
							break;
						case 0x117: // 279 StripByteCounts LONG or SHORT StripsPerImage value(s)
									// required!
									// For each strip, the number of bytes in the strip after compression
							$hIFDentry['sTag'] = 'StripByteCounts';
							break;
						case 0x118: // 280 MinSampleValue SHORT SamplesPerPixel value(s)
									// only for statistics. default is 0
							$hIFDentry['sTag'] = 'MinSampleValue';
							break;
						case 0x119: // 281 MaxSampleValue SHORT SamplesPerPixel value(s)
									// only for statistics. default is 2**(BitsPerSample) - 1
							$hIFDentry['sTag'] = 'MaxSampleValue';
							break;
						case 0x11A: // 282 XResolution RATIONAL 1 value(s)
							$hIFDentry['sTag'] = 'XResolution';
							break;
						case 0x11B: // 283 YResolution RATIONAL 1 value(s)
							$hIFDentry['sTag'] = 'YResolution';
							break;
						case 0x11C: // 284 PlanarConfiguration SHORT 1 value(s)
									// readers only need to support 1 = Chunky (default)
									// 2 = Planar and is not recomended
									// only required when SamplesPerPixel > 1
							$hIFDentry['sTag'] = 'PlanarConfiguration';
							break;
						case 0x11D: // 285 PageName ASCII value(s)
							$hIFDentry['sTag'] = 'PageName';
							break;
						case 0x11E: // 286 XPosition RATIONAL value(s)
							$hIFDentry['sTag'] = 'XPosition';
							break;
						case 0x11F: // 287 YPosition RATIONAL value(s)
							$hIFDentry['sTag'] = 'YPosition';
							break;
						case 0x120: // 288 FreeOffsets LONG value(s)
									// not recomended, see FreeByteCounts
							$hIFDentry['sTag'] = 'FreeOffsets';
							break;
						case 0x121: // 289 FreeByteCounts LONG value(s)
									// not recomended, see FreeOffsets
							$hIFDentry['sTag'] = 'FreeByteCounts';
							break;
						case 0x122: // 290 GrayResponseUnit SHORT 1value(s)
									// 1, 2 (default), 3 (recomended), 4 or 5
							$hIFDentry['sTag'] = 'GrayResponseUnit';
							break;
						case 0x123: // 291 GrayResponseCurve SHORT 2**BitsPerSample value(s)
									// may be ignored by readers
							$hIFDentry['sTag'] = 'GrayResponseCurve';
							break;
						case 0x124: // 292 T4Options LONG 1 value(s)
							$hIFDentry['sTag'] = 'T4Options';
							break;
						case 0x125: // 293 T6Options LONG 1 value(s)
							$hIFDentry['sTag'] = 'T6Options';
							break;
						case 0x128: // 296 ResolutionUnit SHORT 1 value(s)
									// 1 none, 2 inch (default) or 3 cm
							$hIFDentry['sTag'] = 'ResolutionUnit';
							break;
						case 0x129: // 297 PageNumber SHORT 2 value(s)
							$hIFDentry['sTag'] = 'PageNumber';
							break;
						case 0x12D: // 301 TransferFunction SHORT {1 or SamplesPerPixel}*2** BitsPerSample
							$hIFDentry['sTag'] = 'TransferFunction';
							break;
						case 0x131: // 305 Software ASCII value(s)
									// Name and version number of the software package(s) used to create the image.
							$hIFDentry['sTag'] = 'Software';
							break;
						case 0x132: // 306 DateTime ASCII 20 value(s)
									// “YYYY:MM:DD HH:MM:SS” The length of the string, including the terminating NUL, is 20 bytes.
							$hIFDentry['sTag'] = 'DateTime';
							break;
						case 0x13B: // 315 Artist ASCII value(s)
							$hIFDentry['sTag'] = 'Artist';
							break;
						case 0x13C: // 316 HostComputer ASCII value(s)
									// machine on which image was created
							$hIFDentry['sTag'] = 'HostComputer';
							break;
						case 0x13D: // 317 Predictor SHORT 1 value(s)
							$hIFDentry['sTag'] = 'Predictor';
							break;
						case 0x13E: // 318 WhitePoint RATIONAL 2 value(s)
							$hIFDentry['sTag'] = 'WhitePoint';
							break;
						case 0x13F: // 319 PrimaryChromaticities RATIONAL 6 value(s)
							$hIFDentry['sTag'] = 'PrimaryChromaticities';
							break;
						case 0x140: // 320 ColorMap SHORT 3 * (2**BitsPerSample) value(s)
									// see PhotometricInterpretation - palette color
							$hIFDentry['sTag'] = 'ColorMap';
							break;
						case 0x141: // 321 HalftoneHints SHORT 2 value(s)
							$hIFDentry['sTag'] = 'HalftoneHints';
							break;
						case 0x142: // 322 TileWidth SHORT or LONG 1 value(s)
							$hIFDentry['sTag'] = 'TileWidth';
							break;
						case 0x143: // 323 TileLength SHORT or LONG 1 value(s)
							$hIFDentry['sTag'] = 'TileLength';
							break;
						case 0x144: // 324 TileOffsets LONG TilesPerImage value(s)
							$hIFDentry['sTag'] = 'TileOffsets';
							break;
						case 0x145: // 325 TileByteCounts SHORT or LONG TilesPerImage value(s)
							$hIFDentry['sTag'] = 'TileByteCounts';
							break;
						case 0x14C: // 332 InkSet SHORT 1 value(s)
							$hIFDentry['sTag'] = 'InkSet';
							break;
						case 0x14D: // 333 InkNames ASCII total number of characters in all ink name strings, including zeros
							$hIFDentry['sTag'] = 'InkNames';
							break;
						case 0x14E: // 334 NumberOfInks SHORT 1 value(s)
							$hIFDentry['sTag'] = 'NumberOfInks';
							break;
						case 0x150: // 336 DotRange BYTE or SHORT 2, or 2* NumberOfInks
							$hIFDentry['sTag'] = 'DotRange';
							break;
						case 0x151: // 337 TargetPrinter ASCII any value(s)
							$hIFDentry['sTag'] = 'TargetPrinter';
							break;
						case 0x152: // 338 ExtraSamples BYTE number of extra components per pixel
									// 0 (default), 1 or 2 for each extra sample
							$hIFDentry['sTag'] = 'ExtraSamples';
							break;
						case 0x153: // 339 SampleFormat SHORT SamplesPerPixel value(s)
									// 1 (default uint), 2 (sint), 3 (IEEE float) or 4 (undefined -> uint)
							$hIFDentry['sTag'] = 'SampleFormat';
							break;
						case 0x154: // 340 SMinSampleValue Any SamplesPerPixel value(s)
							$hIFDentry['sTag'] = 'SMinSampleValue';
							break;
						case 0x155: // 341 SMaxSampleValue Any SamplesPerPixel value(s)
							$hIFDentry['sTag'] = 'SMaxSampleValue';
							break;
						case 0x156: // 342 TransferRange SHORT 6 value(s)
							$hIFDentry['sTag'] = 'TransferRange';
							break;
						case 0x200: // 512 JPEGProc SHORT 1 value(s)
							$hIFDentry['sTag'] = 'JPEGProc';
							break;
						case 0x201: // 513 JPEGInterchangeFormat LONG 1 value(s)
							$hIFDentry['sTag'] = 'JPEGInterchangeFormat';
							break;
						case 0x202: // 514 JPEGInterchangeFormatLngth LONG 1 value(s)
							$hIFDentry['sTag'] = 'JPEGInterchangeFormatLngth';
							break;
						case 0x203: // 515 JPEGRestartInterval SHORT 1 value(s)
							$hIFDentry['sTag'] = 'JPEGRestartInterval';
							break;
						case 0x205: // 517 JPEGLosslessPredictors SHORT SamplesPerPixel value(s)
							$hIFDentry['sTag'] = 'JPEGLosslessPredictors';
							break;
						case 0x206: // 518 JPEGPointTransforms SHORT SamplesPerPixel value(s)
							$hIFDentry['sTag'] = 'JPEGPointTransforms';
							break;
						case 0x207: // 519 JPEGQTables LONG SamplesPerPixel value(s)
							$hIFDentry['sTag'] = 'JPEGQTables';
							break;
						case 0x208: // 520 JPEGDCTables LONG SamplesPerPixel value(s)
							$hIFDentry['sTag'] = 'JPEGDCTables';
							break;
						case 0x209: // 521 JPEGACTables LONG SamplesPerPixel value(s)
							$hIFDentry['sTag'] = 'JPEGACTables';
							break;
						case 0x211: // 529 YCbCrCoefficients RATIONAL 3 value(s)
							$hIFDentry['sTag'] = 'YCbCrCoefficients';
							break;
						case 0x212: // 530 YCbCrSubSampling SHORT 2 value(s)
							$hIFDentry['sTag'] = 'YCbCrSubSampling';
							break;
						case 0x213: // 531 YCbCrPositioning SHORT 1 value(s)
							$hIFDentry['sTag'] = 'YCbCrPositioning';
							break;
						case 0x214: // 532 ReferenceBlackWhite LONG 2*SamplesPerPixel value(s)
							$hIFDentry['sTag'] = 'ReferenceBlackWhite';
							break;
						case 0x8298: // 33432 Copyright ASCII Anyvalue(s)
							$hIFDentry['sTag'] = 'Copyright';
							break;
						case (($iTag >= 32768) && ($iTag < 65000)): // 'private' fields anyone may use and request registration
							$hIFDentry['sTag'] = 'private'; break;
						case (($iTag >= 65000) && ($iTag <= 65535)): // 'reusable' range anyone may use no registration possible
							$hIFDentry['sTag'] = 'reusable'; break;
						default:
							$hIFD['aNonSpecTags'][] = $iTag;
							$hIFDentry['sTag'] = 'unknown';
					} // switch for known tags
				} // if not skip
				$hIFDentry['xTag'] = '0x' . dechex($iTag);
				$hIFD['aIFDentries'][$iTag] = $hIFDentry;
				$iEntryPos += 12;
			} // for loop directory entries

			$this->hTIFF['aIFDs'][] = $hIFD;
		} while(true); // loop IFD's

		unset($hIFDentry, $hIFD, $iTag, $aValues, $a, $sFormat, $sChunk, $iEM1,
				$iEM2, $a, $iEndian, $bBigEndian, $iIFDpos, $iIFDstart,
				$iEntryPos, $iCountValues, $bSkipThisEntry,
				$iByteLengthPerValue, $bValuesFitInEntry, $iValueOffset);

		// check that at least IFD0 is correct and useable
		list($hIFD, $iDx) = $this->anyUseableIFD(0);
		if (false == $hIFD) return false;
		return $iDx;
	} // readTIFF

	function anyUseableIFD($iPreferedIdx = 0) {
		$aResult = array(false, 0);
		$aIFDs = &$this->hTIFF['aIFDs'];
		$iCount = count($aIFDs);

		// no IFDs?
		if (0 == $iCount) return $aResult;

		// is prefered one good?
		if (isset($aIFDs[$iPreferedIdx]) && $this->isUseableIFD($iPreferedIdx))
							return array($aIFDs[$iPreferedIdx], $iPreferedIdx);

		for ($idx = 0; $idx < $iCount; $idx++) {
			if ($idx != $iPreferedIdx) {
				if ($this->isUseableIFD($idx)) return array($aIFDs[$idx], $idx);
			} // if not the one already checked
		}

		// if none was found, return the false array
		return $aResult;
	} // anyUseableIFD

	function isUseableIFD($idx) {
		$aIFDs = &$this->hTIFF['aIFDs'];
		$iCount = count($aIFDs);

		if (0 == $iCount) return false;
		if ($idx >= $iCount) return false;
		if (0 > $idx) return false;

		$hIFD = &$aIFDs[$idx];

		// check width, height
		if (isset($hIFD['aIFDentries'][0x100])
			&& isset($hIFD['aIFDentries'][0x101])) {
			if ((1 > $hIFD['aIFDentries'][0x100]['aValues'][0])
				|| (1 > $hIFD['aIFDentries'][0x101]['aValues'][0])) return false;
		} else return false;

		// check PhotometricInterpretation
		if (isset($hIFD['aIFDentries'][0x106])) {
			// we only deal with RGB in this version
			if (2 != $hIFD['aIFDentries'][0x106]['aValues'][0]) return false;
		} else return false;

		// check SamplesPerPixel
		if (isset($hIFD['aIFDentries'][0x115])) {
			if (3 > $hIFD['aIFDentries'][0x115]['aValues'][0]) return false;
			$iSamplesPerPixel = $hIFD['aIFDentries'][0x115]['aValues'][0];
		} else return false;

		// check PlanarConfiguration (only required if BitsPerSample > 1)
		if (isset($hIFD['aIFDentries'][0x11C])) {
			if (1 != $hIFD['aIFDentries'][0x11C]['aValues'][0]) return false;
		} else $hIFD['aIFDentries'][0x11C] = array('iTag' => 0x11C,
				'iType' => 3, 'iCountValues' => 1, 'aValues' => array(1));

		/*
		// check BitsPerSample
		if (isset($hIFD['aIFDentries'][0x102]) {
			if (1 < $hIFD['aIFDentries'][0x102]['aValues'][0]) {
				// check PlanarConfiguration
				if (isset($hIFD['aIFDentries'][0x11C]) {
					if (1 != $hIFD['aIFDentries'][0x11C]['aValues'][0])
						return false;
				} else $hIFD['aIFDentries'][0x11C] = array('iTag' => 0x11C,
						'iType' => 3, 'iCountValues' => 1,
						'aValues' => array(1));
			} // if more than 1 BitsPerSample
		} else return false;
		*/


		// check RowsPerStrip
		if (isset($hIFD['aIFDentries'][0x116])) {
			if (1 != count($hIFD['aIFDentries'][0x116]['aValues'])) return false;
		} else return false;

		// calculate how many strips
		$iHeight = $hIFD['aIFDentries'][0x101]['aValues'][0];
		$iRowsPerStrip = $hIFD['aIFDentries'][0x116]['aValues'][0];
		$iStripsPerImage = floor(($iHeight + $iRowsPerStrip - 1) / $iRowsPerStrip);
		$hIFD['iStripsPerImage'] = $iStripsPerImage;

		// check strips offsets and counts  TODO: check each offset for OOB and check each byteCount
		if (isset($hIFD['aIFDentries'][0x111])) {
			if (count($hIFD['aIFDentries'][0x111]['aValues'])
											!= $iStripsPerImage) return false;
		} else return false;

		if (isset($hIFD['aIFDentries'][0x117])) {
			if (count($hIFD['aIFDentries'][0x117]['aValues'])
											!= $iStripsPerImage) return false;
		} else return false;

		// check compression
		if (isset($hIFD['aIFDentries'][0x103])) {
			if (1 != $hIFD['aIFDentries'][0x103]['aValues'][0]) return false;
		} else $hIFD['aIFDentries'][0x103] = array('iTag' => 0x103,
				'iType' => 3, 'iCountValues' => 1, 'aValues' => array(1));

		// check SampleFormat (only required if BitsPerSample > 1)
		if (isset($hIFD['aIFDentries'][0x153])) {
		} else $hIFD['aIFDentries'][0x153] = array('iTag' => 0x153,
				'iType' => 3, 'iCountValues' => $iSamplesPerPixel,
				'aValues' => array_fill(0, $iSamplesPerPixel, 1));

		return true;
	} // isUseableIFD

	// returns how many times the inputfile size need to be available to PHP
	// this reader needs a little more
	// a possible writer subclass may want to override this to return more
	function fMemUseFactor() { return 2.25; } // fMemUseFactor

	function defaultTIFFdict() {
		return array('iEndian' => ((machineByteOrderIsBigEndian()) ? 77 : 73),
					   'aIFDs' => array());
	} // defaultTIFFdict

	function defaultIFDdict() {
		return array(
			//0x100 => 0,					// imageWidth
			//0x101 => 0,					// imageHeight
			//0x102 => array(8, 8, 8),	// aBitsPerSample
			//0x103 => 1,					// iCompression
			//0x106 => 2,					// iPhotometricInterpretation
			//0x112 => 1,					// iOrientation
			//0x115 => 3,					// iSamplesPerPixel
			//0x11C => 1,					// iPlanarConfiguration
			//0x128 => 2,					// iResolutionUnit
			//0x153 => array(1, 1, 1),	// aSampleFormat
			'aNonSpecTags' => array(),		// store unknown tags for possible output
			'aIFDentries' => array(),		// hold read IFD entries headers
			//'iIFDstartPos' => 0, 'iEntries' => 0,
			'aSkippedEntries' => array()	// , '' => 0
			);
			//,
	} // defaultIFDdict

	public function valueForTagOfIFD($iTag, $idx = false) {
		if (false === $idx) list(, $idx) = $this->anyUsebaleIFD();

		if (!$this->isUseableIFD($idx)) return false;

		if (isset($this->hTIFF['aIFDs'][$idx]['aIFDentries'][$iTag])) {
			$aValues = &$this->hTIFF['aIFDs'][$idx]['aIFDentries'][$iTag]['aValues'];
			switch (count($aValues)) {
				case 0: return false;
				case 1: return $aValues[0];
				default: return $aValues;
			}
		} else return false;

	} // valueForTagOfIFD

	public function pixelValuesOfIFD($x, $y, $idx) {
		if (false === $idx) list(, $idx) = $this->anyUsebaleIFD();

		if (!$this->isUseableIFD($idx)) return array();

		if (!isset($this->aPixelValuesPerIFD[$idx]))
				$this->pixelValuesOfIFDasArray($idx);

		return $this->aPixelValuesPerIFD[$idx][$x][$y];
	} // pixelValuesOfIFD

	public function pixelValuesOfIFDasArray($idx = false) {
		if (false === $idx) list(, $idx) = $this->anyUsebaleIFD();

		if (!$this->isUseableIFD($idx)) return array();

		$bBigEndian = (77 == $this->hTIFF['iEndian']);

		$aIFDentries = &$this->hTIFF['aIFDs'][$idx]['aIFDentries'];

		// for more speed we avoid function calls for entries already checked for by
		// isUseableIFD(): //$iHeight = $this->valueForTagOfIFD(0x101, $idx);
		$iWidth = $aIFDentries[0x100]['aValues'][0];
		$iHeight = $aIFDentries[0x101]['aValues'][0];

		$aBitsPerSample = $aIFDentries[0x102]['aValues'];
		$aStripOffsets = $aIFDentries[0x111]['aValues'];
		$aStripByteCounts = $aIFDentries[0x117]['aValues'];
		$aSampleFormats = $aIFDentries[0x153]['aValues'];

		$iTotalStrips = count($aStripOffsets);
		$iSamplesPerPixel = count($aSampleFormats);

		$aPixelValues = array_fill(0, $iWidth, array_fill(0, $iHeight,
				array_fill(0, $iSamplesPerPixel, 0)));

//var_dump($aPixelValues);die();
		$x = 0; $y = 0;
		for ($iCountStrips = 0; $iCountStrips < $iTotalStrips; $iCountStrips++) {
			$sStrip = substr($this->sDataTIFF, $aStripOffsets[$iCountStrips],
											$aStripByteCounts[$iCountStrips]);
			$iPos = 0;
			do {
				// gather all samples of pixel
				$aPixel = array();
				for ($iCountSamples = 0; $iCountSamples < $iSamplesPerPixel;
															$iSamplesPerPixel++) {
					$iSampleFormat = $aSampleFormats[$iCountSamples];
					$iBytes = intval(ceil($aBitsPerSample[$iCountSamples] / 8));
					// TODO: deal with iBytes < 8

					$sSample = substr($sStrip, $iPos, $iBytes);

					switch ($iSampleFormat) {
						case 3: // IEEE floating point data
							$iValue = 0;
							trigger_error('can not yet handle IEEE floats in pixel sample (or anywhere for that matter)', E_USER_NOTICE);
							break;
						case 2: // signed integer
							if (1 == $iBytes) {
								$a = unpack('c*', $sSample);
								$iValue = $a[1];
							} else if (2 == $iBytes) {
								$iValue = signedShortFromString($sSample,
										$$this->bMachineByteOrderIsBigEndian,
										$bBigEndian);
							} else {
								$iValue = 0;
								trigger_error('can not yet handle signed integers in pixel sample (if longer than 2 bytes)', E_USER_NOTICE);
							}
							break;
						case 4:
						case 1:
							// unsigned integer
						default:
							if (1 == $iBytes) {
								$a = unpack('C*', $sSample);
								$iValue = $a[1];
							} else if (2 == $iBytes) {
								unpack((($bBigEndian) ? 'n' : 'v'), $sSample);
								$iValue = $a[1];
							} else {
								$iValue = 0;
								trigger_error('can not yet handle unsigned integers in pixel sample when length is > 2 bytes', E_USER_NOTICE);
							}
					} // switch sampleFormat

					$aPixelValues[$x][$y][$iCountSamples] = $iValue;

					$iPos += $iBytes;
				} // for each sample

				if ($iWidth == ++$x) { $x = 0; $y++; }
			} while ($iPos < $aStripByteCounts[$iCountStrips]);

		} // for each strip

		unset ($iWidth, $iHeight, $iRowsPerStrip, $iStripsPerImage,
				$aStripOffsets, $aStripByteCounts, $idx, $x, $y, $bBigEndian,
				$sSample, $a, $iBytes, $iValue, $iSampleFormat, $aSampleFormats,
				$iTotalStrips, $iSamplesPerPixel);

		$this->aPixelValuesPerIFD[$idx] = aPixelValues;

		return $aPixelValues;
	} // pixelValuesOfIFDasArray

	public function rawDataString() {
		return $this->sData;
	} // rawDataString

} // SssS_TIFFreader

function machineByteOrderIsBigEndian() {
	// detect machine byte order
	// idea from: http://us.php.net/manual/en/function.unpack.php#52527
	// pack a known number of 16bit length as 16 bit unsigned short
	// (machine dependant ordering) and unpack it as unsigned char 8-bit
	$aTest = unpack('C*', pack('S*', 256));
	return ($aTest[1] == 1);
} // machineByteOrderIsBigEndian

function maxMemInBytes() {
	$s = ini_get('memory_limit');
	$f = floatval($s);
	if (stripos($s, 'T')) { // maybe one day, who knows
		return $f * 1024 * 1024 * 1024 * 1024;
	} else if (stripos($s, 'G')) { // since ~PHP 5.1
		return $f * 1024 * 1024 * 1024;
	} else if (stripos($s, 'M')) {
		return $f * 1024 * 1024;
	} else if (stripos($s, 'K')) {
		return $f * 1024;
	} else if (stripos($s, 'B')) {
		return $f;
	} else return $s;
} // maxMemInBytes

// $s must be a multitude of 4 bytes long
function arrayOfSignedLongFromString($s, $bMachineIsBigEndian, $bDataIsBigEndian) {
	$iLen = strlen($s);
	if (4 > $iLen) { trigger_error('arrayOfSignedLongFromString: string ('
			. $s . ') is too short for operation, must have at least 4 bytes',
			E_USER_NOTICE);
	} else if (0 != ($iLen % 4)) { trigger_error('arrayOfSignedLongFromString:'
			. ' string is too long (' . $iLen . ') for operation, must be a'
			. ' multitude of 4 in length',	E_USER_NOTICE);
	}

	$sFormat = 'l*'; // signed long (always 32 bit, machine byte order)

	if ($bMachineIsBigEndian != $bDataIsBigEndian) {
		// not same, need to reverse chunks
		$aWords = str_split($s, 4); // TIFF6 uses 4, PHP is machine dependent...
		$s = implode('', array_map('reverseString', $aWords));
	} //

	// now both are same order, go ahead
	return array_values(unpack($sFormat, $s));
} // arrayOfSignedLongFromString

// $s must be a multitude of 4 bytes long
function arrayOfFloatFromString($s, $bMachineIsBigEndian, $bDataIsBigEndian) {
	$iLen = strlen($s);
	if (4 > $iLen) { trigger_error('arrayOfFloatFromString: string ('
			. $s . ') is too short for operation, must have at least 4 bytes',
			E_USER_NOTICE);
	} else if (0 != ($iLen % 4)) { trigger_error('arrayOfFloatFromString:'
			. ' string is too long (' . $iLen . ') for operation, must be a'
			. ' multitude of 4 in length',	E_USER_NOTICE);
	}

	$sFormat = 'f*'; // float (machine dependent size and representation)

	if ($bMachineIsBigEndian != $bDataIsBigEndian) {
		// not same, need to reverse chunks
		$aWords = str_split($s, 4); // TIFF6 uses 4, PHP is machine dependent...
		$s = implode('', array_map('reverseString', $aWords));
	} //

	// now both are same order, go ahead
	return array_values(unpack($sFormat, $s));
} // arrayOfFloatFromString

// $s must be a multitude of 8 bytes long
function arrayOfDoubleFromString($s, $bMachineIsBigEndian, $bDataIsBigEndian) {
	$iLen = strlen($s);
	if (8 > $iLen) { trigger_error('arrayOfDoubleFromString: string ('
			. $s . ') is too short for operation, must have at least 4 bytes',
			E_USER_NOTICE);
	} else if (0 != ($iLen % 8)) { trigger_error('arrayOfDoubleFromString:'
			. ' string is too long (' . $iLen . ') for operation, must be a'
			. ' multitude of 8 in length',	E_USER_NOTICE);
	}

	$sFormat = 'd*'; // double (machine dependent size and representation)

	if ($bMachineIsBigEndian != $bDataIsBigEndian) {
		// not same, need to reverse chunks
		$aWords = str_split($s, 8); // TIFF6 uses 8, PHP is machine dependent...
		$s = implode('', array_map('reverseString', $aWords));
	} //

	// now both are same order, go ahead
	return array_values(unpack($sFormat, $s));
} // arrayOfDoubleFromString

// $s needs to be 32 bit long
function unsignedLongFromString($s, $bMachineIsBigEndian, $bDataIsBigEndian) {
	// PHP has a bug with this type, so we need to do some checking and use
	// machine's byte ordering
	$iLen = strlen($s);
	if (4 > $iLen) { trigger_error('unsignedLongFromString: string (' . $s
			. ') is too short for operation', E_USER_NOTICE);
	} else if (4 < $iLen) { trigger_error('unsignedLongFromString: string ('
			. $s . ') is too long for operation, unknown behaviour expected'
			. ' use arrayOfUnsignedLongFromString',	E_USER_NOTICE);
	}

	$sFormat = 'L*'; // unsigned long (always 32 bit, machine byte order)

	if ($bMachineIsBigEndian == $bDataIsBigEndian) {
		// both same, go ahead
echo "same\n";
		$a = unpack($sFormat, $s);
	} else $a = unpack($sFormat, strrev($s));

	return $a[1]; // (long)
} // unsignedLongFromString

// the returned array is indexed from 0
// $s needs to be a multiple of 32 bit long i.e a multiple of 4 bytes
function arrayOfUnsignedLongFromString($s, $bMachineIsBigEndian, $bDataIsBigEndian) {
	$iLen = strlen($s);
	if (4 > $iLen) { trigger_error('arrayOfUnsignedLongFromString: string ('
			. $s . ') is too short for operation, must have at least 4 bytes',
			E_USER_NOTICE);
	} else if (0 != ($iLen % 4)) { trigger_error('arrayOfUnsignedLongFromString:'
			. ' string is too long (' . $iLen . ') for operation, must be a'
			. ' multitude of 4 in length',	E_USER_NOTICE);
	}

	$sFormat = 'L*'; // unsigned long (always 32 bit, machine byte order)

	if ($bMachineIsBigEndian != $bDataIsBigEndian) {
		// not same, need to reverse chunks
		$aWords = str_split($s, 4);
		$s = implode('', array_map('reverseString', $aWords));
	} //

	// now both are same order, go ahead
	return array_values(unpack($sFormat, $s));

} // arrayOfUnsignedLongFromString

// $s needs to be 16 bit long
function signedShortFromString($s, $bMachineIsBigEndian, $bDataIsBigEndian) {
	// PHP has no pack() code for signed shorts by specific byte order
	// so we need to do some checking and use machine's byte ordering
	$iLen = strlen($s);
	if (2 > $iLen) { trigger_error('signedShortFromString: string (' . $s
			. ') is too short for operation', E_USER_NOTICE);
	} else if (2 < $iLen) { trigger_error('signedShortFromString: string is too'
			. ' long ('	. $iLen . ') for operation, unknown behaviour expected'
			. ' use arrayOfSignedShortsFromString',	E_USER_NOTICE);
	}

	$sFormat = 's*'; // signed short (always 16 bit, machine byte order)

	if ($bMachineIsBigEndian == $bDataIsBigEndian) {
		// both same, go ahead
		$a = unpack($sFormat, $s);
	} else $a = unpack($sFormat, strrev($s));

	return $a[1]; // (short)
} // signedShortFromString

// the returned array is indexed from 0!
// $s needs to be a multiple of 16 bit long i.e a multiple of 2 bytes
function arrayOfSignedShortsFromString($s, $bMachineIsBigEndian, $bDataIsBigEndian) {
	$iLen = strlen($s);
	if (2 > $iLen) { trigger_error('arrayOfSignedShortsFromString: string'
			. ' is too short ('. $iLen . ') for operation', E_USER_NOTICE);
	} else if (0 != ($iLen % 2)) { trigger_error(
			'arrayOfSignedShortsFromString: string is too long (' . $iLen . ')'
			. ' for operation, must be a multitude of 2 bytes in length.'
			. ' Unknown behaviour expected on last item',	E_USER_NOTICE);
	}

	$sFormat = 's*'; // signed short (always 16 bit, machine byte order)

	if ($bMachineIsBigEndian != $bDataIsBigEndian) {
		// not same, need to reverse chunks
		$aWords = str_split($s, 2);
		$s = implode('', array_map('reverseString', $aWords));
	}

	// now both are same order, go ahead
	return array_values(unpack($sFormat, $s));

} // arrayOfSignedShortsFromString

if (!function_exists('reverseString')) {
	// used for callback for array_map()
	function reverseString($s) { return strrev($s); } // reverseString
} // if not yet declared function

/* * * *\ SssS_TIFFreader.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
