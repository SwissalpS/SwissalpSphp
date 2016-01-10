<?php

namespace SssSPropel2\SssSphocoaAppAuth;

use SssSPropel2\SssSphocoaAppAuth\Base\Users as BaseUsers;

use SwissalpS\PHOCOA\Localization\Bla as SssSBla;
use SwissalpS\PHOCOA\Localization\Countries as SssSCountries;

/**
 * Skeleton subclass for representing a row from the 'Users' table.
 *
 *
 *
 * You should add additional methods to this class to meet the
 * application requirements.  This class will only be generated as
 * long as it does not already exist in the output directory.
 *
 */
class Users extends BaseUsers {

	function validateCountry(&$sValue, &$bEdited, &$aErrors) {

        $sValueCached = $sValue;

		$sValue = strtoupper(trim($sValue));
    	if (empty($sValue)) {

    		$sValue = 'CH';

    	} else {

    		if (!in_array($sValue, SssSCountries::countryIDs())) {

    			$sValue = 'CH';

    		} // if not a valid country

    	} // if MT

        $bEdited = ($sValueCached !== $sValue);

    	return true;

	} // validateCountry


	function validateEmail(&$sValue, &$bEdited, &$aErrors) {

		$bOk = true;
        $sValueCached = $sValue;
		$sValue = trim($sValue);

		if (!empty($sValue)) {

			if (0 == preg_match('/^[^@]+@[a-z0-9._-]+\.[a-z]+$/i', $sValue)) {

				$bOk = false;
				$aErrors[] = new WFError(SssSBla::bla('UsersInvalidEmail'));

			} // if not valid email

		} // if not empty email

    	$bEdited = ($sValueCached !== $sValue);

		return $bOk;

	} // validateEmail


	function validateRealname(&$sValue, &$bEdited, &$aErrors) {

        $sValueCached = $sValue;
		$sValue = trim($sValue);
    	if (empty($sValue)) {

    		$sValue = SssSBla::bla('NoteNoName', array('noEdit' => true));

    	} else {

    		$sValue = htmlspecialchars($sValue, ENT_QUOTES);

    	} // if MT

        $bEdited = ($sValueCached !== $sValue);

    	return true;

	} // validateRealname


	function validateRegion(&$sValue, &$bEdited, &$aErrors) {

        $sValueCached = $sValue;

    	if (empty($sValue)) {

    		$sValue = SssSBla::bla('UnknownRegion', array('noEdit' => true));

    	} else {

    		$sValue = htmlspecialchars($sValue, ENT_QUOTES);

    	} // if MT or not

    	$bEdited = ($sValueCached !== $sValue);

    	return true;

    } // validateRegion


    function validateUrl(&$sValue, &$bEdited, &$aErrors) {

		$bOk = true;
        $sValueCached = $sValue;
		$sValue = trim($sValue);

    	if (!empty($sValue)) {

    		if ('http' != substr($sValue, 0, 4)) {

                $sValue = 'http://' . $sValue;

            } // if not starting with http

			if (0 == preg_match('|^http(s)?://[a-z0-9._-]+\.[a-z]+(:[0-9]+)?(/.*)?$|i', $sValue)) {

				$bOk = false;
				$aErrors[] = new WFError(
					SssSBla::bla('NoteInvalidUrl0'));

			} // if not valid url

    	} // if not MT

    	$bEdited = ($sValueCached !== $sValue);

    	return $bOk;

    } // validateUrl

} // Users
