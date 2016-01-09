<?php
/* * * *
 * * SwissalpS/PHOCOA/Widgets/WFTextField.inc
 * * extends WFTextField
 * *
 * * added SssSBla localized nullPlaceholder value
 * *
 * * @version 20110516_172655 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\Widgets;

use WFTextField as WFTextFieldParent;
use SwissalpS\PHOCOA\Localization\Bla as SssSBla;

class WFTextField extends WFTextFieldParent {

    public function setNullPlaceholder($sValue) {

		$this->nullPlaceholder = SssSBla::cleanForTitle(SssSBla::translateIfNeeded($sValue, array('noDIV' => true)));

    } // setNullPlaceholder

} // WFTextField

/* * * *\ SwissalpS/PHOCOA/Widgets/WFTextField.inc (CC) Luke JZ aka SwissalpS /* * * */
