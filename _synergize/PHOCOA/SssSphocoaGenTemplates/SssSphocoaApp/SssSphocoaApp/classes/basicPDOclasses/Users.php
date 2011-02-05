<?php

require 'skyprom_phocoa/om/BaseUsers.php';


/**
 * Skeleton subclass for representing a row from the 'Users' table.
 *
 *
 *
 * You should add additional methods to this class to meet the
 * application requirements.  This class will only be generated as
 * long as it does not already exist in the output directory.
 *
 * @package    skyprom_phocoa
 */
class Users extends BaseUsers {

	/**
	 * Initializes internal state of Users object.
	 * @see        parent::__construct()
	 */
	public function __construct() {
		// Make sure that parent constructor is always invoked, since that
		// is where any default values for this object are set.
		parent::__construct();

	} // __construct


	function validateCountry(&$sValue, &$bEdited, &$aErrors) {

		$bEdited = true;
		$sValue = strtoupper(trim($sValue));
    	if (empty($sValue)) {

    		$sValue = 'CH';

    	} else {

    		if (!in_array($sValue, SssSCountries::countryIDs()))
    			$sValue = 'CH';

    	} // if MT

    	return true;

	} // validateCountry


	function validateEmail(&$sValue, &$bEdited, &$aErrors) {

		$bOk = true;
    	$bEdited = true;
		$sValue = trim($sValue);

		if (!empty($sValue)) {

			if (0 == preg_match('/^[^@]+@[a-z0-9._-]+\.[a-z]+$/i', $sValue)) {

				$bOk = false;
				$aErrors[] = new WFError(
					SssSBla::bla('UsersInvalidEmail'));

			} // if not valid email

		} // if not empty email

		return $bOk;

	} // validateEmail


	function validateRealname(&$sValue, &$bEdited, &$aErrors) {

    	$bEdited = true;
		$sValue = trim($sValue);
    	if (empty($sValue)) {

    		$sValue = SssSBla::bla('NoteNoName', array('noEdit' => true));

    	} else {

    		$sValue = htmlspecialchars($sValue, ENT_QUOTES);

    	} // if MT

    	return true;

	} // validateRealname


	function validateRegion(&$sValue, &$bEdited, &$aErrors) {

    	$bEdited = true;

    	if (empty($sValue)) {

    		$sValue = SssSBla::bla('UnknownRegion', array('noEdit' => true));

    	} else {

    		$sValue = htmlspecialchars($sValue, ENT_QUOTES);

    	} // if MT or not

    	return true;

    } // validateRegion


    function validateUrl(&$sValue, &$bEdited, &$aErrors) {

		$bOk = true;
    	$bEdited = true;
		$sValue = trim($sValue);

    	if (!empty($sValue)) {

    		if ('http' != substr($sValue, 0, 4)) $sValue = 'http://' . $sValue;
			if (0 == preg_match('|^http(s)?://[a-z0-9._-]+\.[a-z]+(:[0-9]+)?(/.*)?$|i', $sValue)) {

				$bOk = false;
				$aErrors[] = new WFError(
					SssSBla::bla('NoteInvalidUrl0'));

			} // if not valid email

    	} // if MT

    	return $bOk;

    } // validateUrl

} // Users
