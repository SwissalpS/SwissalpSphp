<?php
/* * * *
 * * SssSPlistItem.inc
 * * used as objects for a WFArrayController
 * *
 * * @version 20110205_094107 (CC) Luke JZ aka SwissalpS
 * * * */
class SssSPlistItemHelper {

	/**
	 * arrayOfPlistItemsForPlist($mPlist)
	 * makes an array of SssSPlistItem(s) for a plist. Arrays are not included, only their children (if they have any) // TODO: allow dates(, data?)
	 *
	 * @param mixed $mPlist may be a SssS_Plist object, a plist stirng or path to an xml plist
	 *
	 * @return array array of SssSPlistItem instances
	 *
	 * @uses SssSPlist - to read the plist and get it's hash (dictionary)
	 * @uses SssSPlistItem - to create the instances
	 *
	 * @package __SssS__ - __package_desciption_here__
	 * @author Luke JZ aka SwissalpS <SwissalpS@LukeZimmerman.com>
	 * @license __license_url_here__
	 * @copyright 2007 Luke JZ aka SwissalpS
	 * @version 20110205_094712 (CC)2007 Luke JZ aka SwissalpS
	 */
	static function arrayOfPlistItemsForPlist($mPlist) {

		$oP = new SssS_Plist($mPlist);

		$aPlist = $oP->__toPlistArray();

		return self::plistItemsForArray($aPlist);

	} // arrayOfPlistItemsForPlist


	static function plistItemsForArray($a, $sBasePath = '') {

		$aOut = array();

		forEach ($a as $sKey => $mValue) {

			$sPath = $sBasePath . '/' . $sKey;

			if (is_array($mValue)) {

				$aOut = array_merge($aOut, self::plistItemsForArray($mValue, $sPath));

			} elseif (in_array(getType($mValue), array('string', 'boolean', 'integer', 'double', 'float'))) { // TODO: allow dates

				$aOut[] = new SssSPlistItem($sPath, $mValue);

			} // if array or other valid value

		} // loop each array item

		return $aOut;

	} // plistItemsForArray


	/**
	 * insertPlistItemsToPlist($aPlistItems, $sPlistPath)
	 * apply the values of the SssSPlistItem instances to a plist
	 *
	 * @param array $aPlistItems array holding instances of SssSPlistItem
	 * @param string $sPlistPath the full path to the plist
	 *
	 * @return void
	 *
	 * @uses SssS_Plist - to read & write the xml plist specified
	 *
	 * @package __SssS__ - __package_desciption_here__
	 * @author Luke JZ aka SwissalpS <SwissalpS@LukeZimmerman.com>
	 * @license __license_url_here__
	 * @copyright 2007 Luke JZ aka SwissalpS
	 * @version 20110205_100351 (CC)2007 Luke JZ aka SwissalpS
	 */
	static function insertPlistItemsToPlist($aPlistItems, $sPlistPath) {

		$oP = new SssS_Plist($sPlistPath);

		forEach ($aPlistItems as $oPlistItem) {

			$oP->set($oPlistItem->path(), $oPlistItem->value());

		} // loop each plist item setting it's value without any checks

		$oP->saveAs($sPlistPath);

	} // insertPlistItemsToPlist


	// convenience
	static function updatePlistItemsToPlist($aPlistItems, $sPlistPath) {

		self::insertPlistItemsToPlist($aPlistItems, $sPlistPath);

	} // updatePlistItemsToPlist

} // SssSPlistItemHelper



class SssSPlistItem extends WFObject {

	protected $mValue;
	protected $sPath;
	protected $sType;

	function __construct($sPath, $mValue) {

		$this->sPath = $sPath;

		$this->mValue = $mValue;

		$this->sType = getType($mValue);

	} // __construct


	function path() {

		return $this->sPath;

	} // path


	function value() {

		return $this->mValue;

	} // value


	function type() {

		return $this->sType;

	} // type


	function setPath($sNewPath) {

		$this->sPath = (string)$sNewPath;

	} // setPath



	function setType($sNewType) {

		if (in_array($sNewType, array('boolean', 'integer', 'string', 'double', 'float', 'array', 'dictionary'))) $this->sType = $sNewType;

	} // setType


	function setValue($mNewValue) {

		$this->mValue = $mNewValue;

	} // setValue

} // SssSPlistItem

/* * * *\ SssSPlistItem.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
