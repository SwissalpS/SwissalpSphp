<?php
/* vim: set expandtab tabstop=4 shiftwidth=4: */
/**
 * @package UI
 * @subpackage Widgets
 * @copyright Copyright (c) 2005 Alan Pinstein. All Rights Reserved.
 * @version $Id: kvcoding.php,v 1.3 2004/12/12 02:44:09 alanpinstein Exp $
 * @author Alan Pinstein <apinstein@mac.com>
 */

/**
 * A Checkbox widget for our framework.
 *
 * There are several ways to use checkboxes in your application, depending on the way you want your checkboxes to behave and interact with your data.
 *
 * 1. A single checkbox representing whether a certain option is "on" or "off".
 *    Simply use a WFCheckbox in your application. The {@link WFWidget::$value value} of the WFCheckbox is {@link WFCheckbox::$checkedValue checkedValue} or {@link WFCheckbox::$uncheckedValue uncheckedValue}, depending on the checkbox's state.
 * 2. Multiple checkboxes representing multiple "states" that should be enabled for a single property. <br>
 *    a. If you want to set up the checkboxes statically in .instances/.config, use a {@link WFCheckboxGroup} in conjuction with multiple WFCheckbox widgets.<br>
 *       When using this mode, the {@link WFCheckboxGroup::$values values} property is an array containing the {@link WFCheckbox::$checkedValue checkedValue} of each selected checkbox. With WFCheckboxGroup, the WFCheckbox uncheckedValue's are not used.<br><br>
 *    b. If you want to set up the checkboxes based on the objects in an array, use a {@link WFSelectionCheckbox} control and link it to an array of objects.<br>
 *       When using this mode, which uses {@link WFArrayController}, the array controller's selected objects will be updated to reflect the state of the checkboxes.
 *
 * <b>PHOCOA Builder Setup:</b>
 *
 * <b>Required:</b><br>
 * - (none)
 *
 * <b>Optional:</b><br>
 * - {@link WFWidget::$value value}
 * - {@link WFCheckbox::$checked checked}
 * - {@link WFCheckbox::$checkedValue checkedValue}
 * - {@link WFCheckbox::$uncheckedValue uncheckedValue}
 * - {@link WFCheckbox::$groupMode groupMode}
 * - {@link WFCheckbox::$label label}
 * - {@link WFCheckbox::$labelPosition labelPosition}
 */
class SssSCheckbox extends WFCheckbox {

    /**
      * Constructor.
      */
    function __construct($sID, $oPage) {

        parent::__construct($sID, $oPage);

    } // __construct


    function setLabel($sLabel) {

        $this->label = SssSBla::translateIfNeeded($sLabel, array('noDIV' => true));

    } // setLabel


    function setTitle($sTitle) {

        $this->title = SssSBla::translateIfNeeded($sTitle, array('noDIV' => true));

    } // setTitle


	// add some xhtml compatibility
    function render($blockContent = NULL) {
        if ($this->hidden) return NULL;

        // get there reference to the named item
        // set the name / value
        // render
        $sChecked = $this->checked() ? ' checked="checked" ' : '';
        $sEnabled = $this->enabled() ? ''
        		: ' disabled="disabled" readonly="readonly" ';

        $sLabelLeft = $sLabelRight = (('' !== $this->label())
        		? ' <label for="' . $this->id . '">' . $this->label . '</label>'
        		: '');

        $sTabIndex = ($this->tabIndex ? ' tabIndex="' . $this->tabIndex . '" ' : '');

        $sTitle = ($this->title ? ' title="' . $this->title . '" ' : '');

        if ('right' === $this->labelPosition)
            $sLabelLeft = NULL;

        else
            $sLabelRight = NULL;


		$sName = $this->name() . ($this->groupMode() ? '[]' : '');
		$sOut = $sLabelLeft . '<input type="checkbox" '
        		. 'name="' . $sName . 
        		. '" ' . ($this->class ? ' class="' . $this->class . '" ' : '')
        		. 'id="' . $this->id() . '" '
        		. 'value="' . $this->checkedValue() . '" '
        		. $sChecked . $sEnabled . $sTabIndex . $sTitle . ' />' . $sLabelRight
        		. $this->getListenerJSInScriptTag();

		if (!$this->enabled() && $this->checked() {

			$sOut .= '<input type="hidden" name="' . $sName . '" value="' . $this->checkedValue() . '" />';

		} // if not enabled but checked

		return $sOut;

    } // render

} // SssSCheckbox
?>
