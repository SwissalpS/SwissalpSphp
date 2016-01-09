<?php
/* vim: set expandtab tabstop=4 shiftwidth=4: */
/**
 * SwissalpS/PHOCOA/Widgets/WFPaginatorSortLink.inc
 *
 * @package UI
 * @subpackage Widgets
 * @copyright Copyright (c) 2005 Alan Pinstein. All Rights Reserved.
 * @version $Id: kvcoding.php,v 1.3 2004/12/12 02:44:09 alanpinstein Exp $
 * @author Alan Pinstein <apinstein@mac.com>
 */
namespace SwissalpS\PHOCOA\Widgets;

use WFPaginator;
use WFPaginatorSortLink as WFPaginatorSortLinkParent;
use SwissalpS\PHOCOA\Localization\Bla as SssSBla;

/**
 * A Paginator sort link widget.
 *
 * Creates a link along the lines of:
 *
 * Price (+/-)
 *
 * Where +/- is a graphic indicating ascending or descending sort. The text will be BOLD if the data is currently sorted by this key.
 *
 * The WFPaginatorSortLink supports only single-key sorting. Multi-key userland sorting must be done with another method.
 *
 * When the user click on the sort link, the page will refresh using the new sort order. If the user click on the same link that is the current sort, the sort
 * will now be in the reverse direction.
 *
 * Note that when a sort link is clicked, the paginator will reset to the first page since the current page is meaningless once the sort changes.
 *
 * <b>PHOCOA Builder Setup:</b>
 *
 * <b>Required:</b><br>
 * - {@link WFWidget::$value value} The value is the {@link WFPaginator::setSortOptions() sort key} that this item represents (setup without the +/-).
 * - {@link WFPaginatorPageInfo::$paginator Paginator}
 *
 * <b>Optional:</b><br>
 * None.
 *
 * @todo Make sure that the pagination links work properly when this widget is in a composited view. Not sure how the params will work from __construct etc...
 */
 /* 20101106_003125 SwissalpS version supporting localized SssSBla */
class WFPaginatorSortLink extends WFPaginatorSortLinkParent {

	/**
	  * Constructor.
	  */
	function __construct($sId, $oPage) {
		parent::__construct($sId, $oPage);

		$this->paginator = NULL;

	} // __construct

	function render($blockContent = NULL) {

		if (!$this->paginator)
				throw (new Exception("No paginator assigned."));

		$sImgSrc = '/www/framework/0.3.6.000.1/widgets/WFPaginatorSortLink'; //$this->getWidgetWWWDir();
		$sSortIndicator = NULL;
		$sLinkKey = '+' . $this->value;
		$aSortKeys = $this->paginator->sortKeys();
		// if the paginator is currently using our sort key, or the reverse of our sort key, then we need to show a "toggle sort dir" link. Otherwise, just show the toggle ascending link.
		if (in_array('+' . $this->value, $aSortKeys)) {

			//$sSortIndicator = '+';
			$sSortIndicator = '<img src="' . $sImgSrc . '/arrow-up.gif" width="11" height="6" border="0" />';

			$sLinkKey = '-' . $this->value;

		} elseif (in_array('-' . $this->value, $aSortKeys)) {

			//$sSortIndicator = "-";
			$sSortIndicator = '<img src="' . $sImgSrc . '/arrow-dn.gif" width="11" height="6" border="0" />';

			$sLinkKey = '+' . $this->value;

		} //

		$aSortOptions = $this->paginator->sortOptions();

		$sLabelRaw = $aSortOptions[$sLinkKey];

		$sLabelTranslated = SssSBla::translateIfNeeded($sLabelRaw, array('noDIV' => true));

		if ($this->paginator->mode() == WFPaginator::MODE_URL) {

			$sLinkURL = $this->paginator->urlForPaginatorState($this->page, $this->paginator->paginatorState(NULL, NULL, array($sLinkKey)));

			$sOut1 = '<a href="' . $sLinkURL . '">'
					. $sLabelTranslated . '</a>';

			$sOut2 = '<a href="'
					. $sLinkURL . '">' . $sSortIndicator . '</a>';

		} else {

			$sLinkURL = $this->paginator->jsForState($this->paginator->paginatorState(NULL, NULL, array($sLinkKey)));

			$sOut1 = '<a href="javascript:' . $sLinkURL . ' return false;">'
					. $sLabelTranslated
					. '</a>';

			$sOut2 = '<a href="javascript:' . $sLinkURL . ' return false;">'
					. $sSortIndicator . '</a>';

		} // if mode url or js

		if ($sSortIndicator) $sOut1 = '<b>' . $sOut1 . '</b>';

		if (SssSBla::showBlaEditLinks()
		&& $sLabelTranslated != $sLabelRaw) {

			$sLabelBla = substr($sLabelRaw, strlen(SssSBla::TRANS_PREFIX));

			$aBlas = array(array('sUID' => $sLabelBla,
				'sEditMeTextFull' => SssSBla::bla('SharedEditLabel', array('noDIV' => true)) . ': ' . $sLabelBla,
				'sEditMeTextShort' => 'eL'));

			$sOut1 = SssSBla::wrapHTMLinEditDiv($aBlas, $sOut1, false);

		} // if logged in member is super user

		return $sOut1 . ' ' . $sOut2;

	} // render

} // WFPaginatorSortLink
