<?php
/* * * *
 * * SssSTextField
 * * extends WFTextField
 * *
 * * added SssSBla localized nullPlaceholder value
 * *
 * * @version 20110516_172655 (CC) Luke JZ aka SwissalpS
 * * * */
class SssSTextField extends WFTextField {

    public function setNullPlaceholder($sValue) {

		$this->nullPlaceholder = SssSBla::translateIfNeeded($sValue, array('noDIV' => true));

    } // setNullPlaceholder

} // SssSTextField

/* * * *\ SssSTextField (CC) Luke JZ aka SwissalpS /* * * */
?>
