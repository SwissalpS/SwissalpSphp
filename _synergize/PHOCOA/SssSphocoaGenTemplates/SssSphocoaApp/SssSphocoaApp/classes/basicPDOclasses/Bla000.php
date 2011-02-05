<?php

require 'skyprom_phocoa/om/BaseBla000.php';


/**
 * Skeleton subclass for representing a row from the 'Bla000' table.
 *
 *
 *
 * You should add additional methods to this class to meet the
 * application requirements.  This class will only be generated as
 * long as it does not already exist in the output directory.
 *
 * @package	skyprom_phocoa
 */
class Bla000 extends BaseBla000 {

	/**
	 * Initializes internal state of Bla000 object.
	 * @see		parent::__construct()
	 */
	public function __construct()
	{
		// Make sure that parent constructor is always invoked, since that
		// is where any default values for this object are set.
		parent::__construct();
	}

	function validateComment(&$sValue, &$bEdited, &$aErrors) {

		SssSBla::cleanSpecialChars($sValue);
		$bEdited = true;

		return true;

	} // validateComment

	function validateDe(&$sValue, &$bEdited, &$aErrors) {

		SssSBla::cleanSpecialChars($sValue);
		$bEdited = true;

		return true;

	} // validateDe

	function validateEn(&$sValue, &$bEdited, &$aErrors) {

		SssSBla::cleanSpecialChars($sValue);
		$bEdited = true;

		return true;

	} // validateEn

	function validateFr(&$sValue, &$bEdited, &$aErrors) {

		SssSBla::cleanSpecialChars($sValue);
		$bEdited = true;

		return true;

	} // validateFr

	function validateIt(&$sValue, &$bEdited, &$aErrors) {

		SssSBla::cleanSpecialChars($sValue);
		$bEdited = true;

		return true;

	} // validateIt

	function validateRm(&$sValue, &$bEdited, &$aErrors) {

		SssSBla::cleanSpecialChars($sValue);
		$bEdited = true;

		return true;

	} // validateRm

} // Bla000
