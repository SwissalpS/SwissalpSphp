<?php

namespace SssSPropel2\SssSphocoaAppBla;

use SssSPropel2\SssSphocoaAppBla\Base\Bla000 as BaseBla000;
use SwissalpS\PHOCOA\Localization\Bla as SssSBla;
use WFError;

/**
 * Skeleton subclass for representing a row from the 'Bla000' table.
 *
 *
 *
 * You should add additional methods to this class to meet the
 * application requirements.  This class will only be generated as
 * long as it does not already exist in the output directory.
 *
 */
class Bla000 extends BaseBla000 {

    protected function _validateGeneral(&$sValue, &$bEdited, &$aErrors) {

        // cache the original value
        $sValueCache = $sValue;

		SssSBla::cleanSpecialChars($sValue);

        if ($sValueCache !== $sValue) {

            $bEdited = true;

        } // if something changed

		return true;

    } // _validateGeneral


	function validateComment(&$sValue, &$bEdited, &$aErrors) {

        return $this->_validateGeneral($sValue, $bEdited, $aErrors);

	} // validateComment


	function validateDe(&$sValue, &$bEdited, &$aErrors) {

        return $this->_validateGeneral($sValue, $bEdited, $aErrors);

	} // validateDe


	function validateEn(&$sValue, &$bEdited, &$aErrors) {

        return $this->_validateGeneral($sValue, $bEdited, $aErrors);

	} // validateEn


	function validateFr(&$sValue, &$bEdited, &$aErrors) {

        return $this->_validateGeneral($sValue, $bEdited, $aErrors);

	} // validateFr


	function validateIt(&$sValue, &$bEdited, &$aErrors) {

        return $this->_validateGeneral($sValue, $bEdited, $aErrors);

	} // validateIt


	function validateRm(&$sValue, &$bEdited, &$aErrors) {

        return $this->_validateGeneral($sValue, $bEdited, $aErrors);

	} // validateRm

} // Bla000
