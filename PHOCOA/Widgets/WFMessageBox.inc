<?php
/* vim: set expandtab tabstop=4 shiftwidth=4: */
/**
 * SwissalpS/PHOCOA/Widgets/WFMessageBox.inc
 * 
 * @package UI
 * @subpackage Widgets
 * @copyright Copyright (c) 2005 Alan Pinstein. All Rights Reserved.
 * @version $Id: kvcoding.php,v 1.3 2004/12/12 02:44:09 alanpinstein Exp $
 * @author Alan Pinstein <apinstein@mac.com>
 */
namespace SwissalpS\PHOCOA\Widgets;

use WFMessageBox as WFMessageBoxParent;
use SwissalpS\PHOCOA\Localization\Bla as SssSBla;

/**
 * A Label widget for our framework.
 *
 * <b>PHOCOA Builder Setup:</b>
 *
 * <b>Required:</b><br>
 * - {@link WFWidget::$value value} The message to display. Can use ValuePattern.
 *
 * <b>Optional:</b><br>
 * - {@link WFMessageBox::$mode mode} Built-in types are Info, Warning, Error, and Confirm. You can supply any string, which will be used as the "class" of the div. This allows you to customize the icons for custom messages. Default is "Info".
 */
class WFMessageBox extends WFMessageBoxParent {

	/**
	  * Constructor.
	  */
	function __construct($sID, $oPage) {

		parent::__construct($sID, $oPage);

	} // __construct


	function setValue($sValue) {

		$this->value = SssSBla::translateIfNeeded($sValue);

	} // setValue


	function render($blockContent = NULL) {

		if ($this->hidden or empty($this->value)) return NULL;

		$imgSrc = $this->getWidgetWWWDir() . '/';
		switch ($this->mode) {
			case WFMessageBox::WFMESSAGEBOX_INFO:
				$imgSrc .= 'WFMessageBox_Icon_Info.jpg';
				break;
			case WFMessageBox::WFMESSAGEBOX_CONFIRM:
				$imgSrc .= 'WFMessageBox_Icon_Confirm.jpg';
				break;
			case WFMessageBox::WFMESSAGEBOX_ERROR:
				$imgSrc .= 'WFMessageBox_Icon_Error.jpg';
				break;
			case WFMessageBox::WFMESSAGEBOX_WARNING:
				$imgSrc .= 'WFMessageBox_Icon_Warning.jpg';
				break;
		} // switch mode

		$modeClass = 'phocoaWFMessageBox_' . $this->mode;

		return '
<div id="' . $this->id . '" style="margin: 5px 0; padding: 0;"><div class="phocoaWFMessageBox ' . $modeClass . '">
<img class="phocoaWFMessageBox_icon" src="' . $imgSrc . '" />
<div class="phocoaWFMessageBox_message">' . $this->value . '</div>
</div></div>
			';

	} // render

} // WFMessageBox
