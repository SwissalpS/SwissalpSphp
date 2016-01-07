<?php
/* vim: set expandtab tabstop=4 shiftwidth=4: */
/**
 * SwissalpS/PHOCOA/Widgets/WFLink.inc
 *
 * @package UI
 * @subpackage Widgets
 * @copyright Copyright (c) 2005 Alan Pinstein. All Rights Reserved.
 * @version $Id: kvcoding.php,v 1.3 2004/12/12 02:44:09 alanpinstein Exp $
 * @author Alan Pinstein <apinstein@mac.com>
 */
namespace SwissalpS\PHOCOA\Widgets;

use WFLink as WFLinkParent;
use SwissalpS\PHOCOA\Localization\Bla as SssSBla;

/**
 * A Link widget for our framework.
 *
 * <b>PHOCOA Builder Setup:</b>
 *
 * <b>Required:</b><br>
 * - {@link WFWidget::$value value} The URL for the link. Supports ValuePattern binding.
 * - {@link WFLink::$label label} The label for the link. Supports ValuePattern binding.
 *
 * <b>Optional:</b><br>
 * - {@link WFLink::$class class} The class of the <a> tag.
 * - {@link WFLink::$title title} The title for the link. Supports ValuePattern binding.
 *
 * @todo Do we need a way to specify a module/page for creating links dynamically? Either a parseable syntax in the "value" field or additional properties?
 */
class WFLink extends WFLinkParent {

    /**
      * Constructor.
      */
    function __construct($sID, $oPage) {

        parent::__construct($sID, $oPage);

    } // __construct


    // adds localized labels
    public function setLabel($sLabel) {

        $this->label = SssSBla::translateIfNeeded($sLabel, array('noDIV' => true));

		// becomes the BlaID or false if isn't prefixed with the BlaPrefix
		$this->sLabelBlaID = SssSBla::removeBlaPrefixIfHas($sLabel);

    } // setLabel


    // adds localized titles
    public function setTitle($sTitle) {

        $this->title = SssSBla::translateIfNeeded($sTitle, array('noDIV' => true));

        $this->sTitleBlaID = SssSBla::removeBlaPrefixIfHas($sTitle);

    } // setLabel


    // adds editable links
    function render($blockContent = NULL) {

        if ($this->hidden) return NULL;

		$class = NULL;
		if ($this->class) $class = ' class="' . $this->class . '"';

		$target = NULL;
		if ($this->target) $target = ' target="' . $this->target . '"';

		$label = (null != $this->label ? $this->label : $this->value);
		$title = ($this->title ? ' title="' . $this->title . '"' : NULL);
		$bShowBla = SssSBla::showBlaEditLinks();
		/*$sEditLabelLink = ($bShowBla && $this->sLabelBlaID) //SssSBla::wrapWithEditLink(
		//old bla$this->sLabelBlaID, $this->sLabelBlaID, '&nbsp;', 'edit label text', '', '', '', 'el') : null;
		//		$, $this->sLabelBlaID,
		//		'el', 'edit label text ' // TODO: translate this!
		//		)
			? '<a href="' . WFRequestController::WFURL('admin/bla000/edit/'
				. $this->sLabelBlaID)
				. '" title="edit label text" target="_blank">el</a>'
			: null;

		$sEditTitleLink = ($bShowBla && $this->sTitleBlaID)
			? '<a href="' . WFRequestController::WFURL('admin/bla000/edit/'
				. $this->sTitleBlaID)
				. '" title="edit title text" target="_blank">et</a>'
			: null;
*/
		$sDone = '<a id="' . $this->id . '" href="' . $this->value . '"'
				. $class . $target . $title . $this->getJSActions() . '>'
				. $label . '</a>' . "\n" . $this->getListenerJSInScriptTag();

		if ($bShowBla && ($this->sLabelBlaID || $this->sTitleBlaID)) {

			$aND = array('noDIV' => true);

			$aBlas = array();
			if ($this->sLabelBlaID) $aBlas[] = array(
				'sUID' => $this->sLabelBlaID,
				'sEditMeTextFull' => SssSBla::bla('SharedEditLabel', $aND) . ': ' . $this->sLabelBlaID,
				'sEditMeTextShort' => 'eL');

			if ($this->sTitleBlaID) $aBlas[] = array(
				'sUID' => $this->sTitleBlaID,
				'sEditMeTextFull' => SssSBla::bla('SssSBlaInternalEditText', $aND) . ': ' . $this->sTitleBlaID,
				'sEditMeTextShort' => 'eT');

			return SssSBla::wrapHTMLinEditDiv($aBlas, $sDone, false);
			//return SssSBla::wrapWithEditLink(null, null, $sDone, $sEditLabelLink . $sEditTitleLink);
		}

		return $sDone;

    } // render

} // SssSLink
?>
