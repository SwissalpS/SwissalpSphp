<?php
/* vim: set expandtab tabstop=4 shiftwidth=4: */
/**
 * SwissalpS/PHOCOA/Widgets/WFSubmit.inc
 *
 * @package UI
 * @subpackage Widgets
 * @copyright Copyright (c) 2005 Alan Pinstein. All Rights Reserved.
 * @version $Id: kvcoding.php,v 1.3 2004/12/12 02:44:09 alanpinstein Exp $
 * @author Alan Pinstein <apinstein@mac.com>
 */
namespace SwissalpS\PHOCOA\Widgets;

use WFSubmit as WFSubmitParent;
use SwissalpS\PHOCOA\Localization\Bla as SssSBla;

/**
 * A Submit widget for our framework.
 *
 * <b>PHOCOA Builder Setup:</b>
 *
 * <b>Required:</b><br>
 * - {@link WFLink::$label label} The label for the button.
 * - {@link WFLink::$postSubmitLabel postSubmitLabel} The label for the button that will be shown after the button is clicked. Requires JS. This does NOT prevent duplicate submission.
 * - {@link WFLink::$duplicateSubmitMessage duplicateSubmitMessage} The message that will be displayed if the submit button is pressed more than once. This also prevents duplicate submission. NOTE: with AJAX forms, you cannot prevent duplicate submission.
 *
 * <b>Optional:</b><br>
 * - {@link WFLink::$class class} The class of the <a> tag.
 *
 * Bindable Properties:
 *  label       The text value to display.
 */
class WFSubmit extends WFSubmitParent {

    /**
      * Constructor.
      */
    function __construct($sID, $oPage) {

        parent::__construct($sID, $oPage);

    } // __construct


    function setLabel($sLabel) {

        $this->label = SssSBla::translateIfNeeded($sLabel, array('noDIV' => true));

        $this->labelBlaID = SssSBla::removeBlaPrefixIfHas($sLabel);

    } // setLabel


    function setDuplicateMessage($str) {

        $this->duplicateSubmitMessage = SssSBla::translateIfNeeded($str, array('noDIV' => true));

        $this->duplicateSubmitMessageBlaID = SssSBla::removeBlaPrefixIfHas($str);

    } // setDuplicateMessage


    function setPostSubmitLabel($str) {

        $this->postSubmitLabel = SssSBla::cleanForTitle(SssSBla::translateIfNeeded($str, array('noDIV' => true)));

        $this->postSubmitLabelBlaID = SssSBla::removeBlaPrefixIfHas($str);

    } // setDuplicateMessage


    function render($sBlockContent = NULL) {
        if ($this->hidden) return NULL;

        $sOut = parent::render($sBlockContent);

        if (SssSBla::showBlaEditLinks()
        	&& ($this->labelBlaID
        	|| $this->duplicateSubmitMessageBlaID
        	|| $this->postSubmitLabelBlaID)) {

        	$aBlas = array();
			$aND = array('noDIV' => true);

			if ($this->labelBlaID) $aBlas[] = array(
				'sUID' => $this->labelBlaID,
				'sEditMeTextFull' => SssSBla::bla('SharedEditLabel', $aND) . ': ' . $this->labelBlaID,
				'sEditMeTextShort' => 'eL');

			if ($this->duplicateSubmitMessageBlaID) $aBlas[] = array(
				'sUID' => $this->duplicateSubmitMessageBlaID,
				'sEditMeTextFull' => SssSBla::bla('SssSBlaInternalEditText', $aND) . ': ' . $this->duplicateSubmitMessageBlaID,
				'sEditMeTextShort' => 'eDSM');

			if ($this->postSubmitLabelBlaID) $aBlas[] = array(
				'sUID' => $this->postSubmitLabelBlaID,
				'sEditMeTextFull' => SssSBla::bla('SssSBlaInternalEditText', $aND) . ': ' . $this->postSubmitLabelBlaID,
				'sEditMeTextShort' => 'ePSL');

			return SssSBla::wrapHTMLinEditDiv($aBlas, $sOut, false);

        } // if show edit links

        return $sOut;

    } // render


    /**
     * Renders the button as hidden (for use with {@link WFForm::$defaultSubmitID defaultSubmitID}).
     *
     * @internal Some browsers by default submit the "first" button they see if ENTER is pressed in a text field. Thus we need to
     *           insert an invisible "fake" version of the default button to ensure consistent cross-browser operation of defaultSubmitID.
     *           The widget must be both display: block and visibility: visible to ensure proper operation in all browsers.
     *           IIRC Safari requires visbility and IE requires display. FF is cool... The 0px is needed on the input as well to make it invisible on IE7.
     * @return string The HTML of the button's core attrs with no ID.
     */
    function renderDefaultButton() {

        return '<div style="position: relative; height: 0px; width: 0px;">
                <input type="submit"' .
                    ' name="action|' . $this->id() . '"' .
                    ' value="' . $this->label() . '"' .
                    ' style="position: absolute; left: -20000px; width: 0px;"' .
                    ' />
                </div>';

    } // renderDefaultButton

} // WFSubmit
