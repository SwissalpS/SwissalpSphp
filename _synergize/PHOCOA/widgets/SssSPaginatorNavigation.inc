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
 * A Paginator Navigation widget for our framework.
 *
 * Creates a clickable navigation element for the paged data. Something along the lines of:
 *
 * Previous 10 items [ Jump to Page 1 2 3 4 5 ] Next 10 Items
 *
 * <b>Required:</b><br>
 * - {@link WFPaginatorNavigation::$paginator Paginator}
 *
 * <b>Optional:</b><br>
 * - {@link WFPaginatorNavigation::$maxJumpPagesToShow maxJumpPagesToShow}
 *
 * @todo Make sure that the pagination links work properly when this widget is in a composited view. Not sure how the params will work from __construct etc...
 */
class SssSPaginatorNavigation extends WFPaginatorNavigation {

	/**
	  * Constructor.
	  */
	function __construct($sID, $oPage) {

		parent::__construct($sID, $oPage);

	} // __construct


	function render($blockContent = NULL) {

		if (!$this->paginator) throw( new Exception("No paginator assigned.") );
		if ($this->paginator->itemCount() == 0) return NULL;
		if ($this->paginator->pageSize() == WFPaginator::PAGINATOR_PAGESIZE_ALL) return NULL;

		$sOutput = '';

		$aND = array('noDIV' => true);

		$sEditMeTextFull = SssSBla::bla('SssSBlaInternalEditText', $aND);

		$bShowBlaEditLinks =  WFAuthorizationManager::sharedAuthorizationManager()->authorizationInfo()->showBlaEditLinks();

		if ($this->paginator->mode() == WFPaginator::MODE_URL) {

			if ($this->paginator->prevPage()) {

				$sUrl = $this->paginator->urlForPaginatorState(
								$this->page,
								$this->paginator->paginatorState(
										$this->paginator->prevPage()));

				$sHTML = '<a href="' . $sUrl	. '">&lt;&lt; '
					. SssSBla::bla('SharedPrevious', $aND) . ' '
					. $this->itemsPhraseSssS($this->paginator->pageSize())
					. '</a>';

				if ($bShowBlaEditLinks) {

					$aBlas = array(array(
						'sUID' => 'SharedPrevious', 'sEditMeTextShort' => 'eT',
						'sEditMeTextFull' => $sEditMeTextFull . ': SharedPrevious'));

					// additional item editable text, is it localizable?
					$mUID = SssSBla::removeBlaPrefixIfHas($this->paginator->itemPhrase($this->paginator->pageSize()));

					// if so add it to the editable texts
					if ($mUID) $aBlas[] = array(
						'sUID' => $mUID, 'sEditMeTextShort' => 'eI',
						'sEditMeTextFull' => $sEditMeTextFull . ': ' . $mUID);

					$sOutput .= SssSBla::wrapHTMLinEditDiv($aBlas, $sHTML, false);

				} else {

					$sOutput .= $sHTML;

				} // if super user or not

			} // if has prevPage -> show link to it

			if ((1 < $this->paginator->pageCount())
					and (0 != $this->maxJumpPagesToShow)) {

				$iFirstJumpPage = max(1, $this->paginator->currentPage()
						- (floor($this->maxJumpPagesToShow / 2)));
				$iLastJumpPage = min($iFirstJumpPage
						+ ($this->maxJumpPagesToShow - 1),
						$this->paginator->pageCount());

				$sOutput .= ' [ ' . SssSBla::bla('SharedJumpTo') . ': ';

				if ($iFirstJumpPage != 1) {

					$sURL = $this->paginator->urlForPaginatorState(
							$this->page, $this->paginator->paginatorState(1));

					$sHTML = '<a href="' . $sURL . '">'
						. SssSBla::bla('SharedFirst', $aND) . '</a> ...';

					if ($bShowBlaEditLinks) {

						$aBlas = array(array(
								'sUID' => 'SharedFirst',
								'sEditMeTextShort' => 'eT',
								'sEditMeTextFull' => $sEditMeTextFull . ': SharedFirst'));

						$sOutput .= SssSBla::wrapHTMLinEditDiv($aBlas, $sHTML, false);

					} else $sOutput .= $sHTML;

				} // if first jump is not 1

				for ($iP = $iFirstJumpPage; $iP <= $iLastJumpPage; $iP++) {
					if ($iP == $this->paginator->currentPage())
						$sOutput .= ' -' . $iP . '- ';

					else {
						$sOutput .= ' <a href="'
						. $this->paginator->urlForPaginatorState(
								$this->page, $this->paginator->paginatorState($iP))
						. '">' . $iP . '</a>';

					} // if current page or other index

				} // for first to last page

				if ($iLastJumpPage != $this->paginator->pageCount()) {

					$sHTML = '... <a href="'
							. $this->paginator->urlForPaginatorState(
									$this->page,
									$this->paginator->paginatorState(
											$this->paginator->pageCount()))
							. '">' . SssSBla::bla('SharedLast', $aND) . '</a>';

					if ($bShowBlaEditLinks) {

						$aBlas = array(array(
								'sUID' => 'SharedLast',
								'sEditMeTextShort' => 'eT',
								'sEditMeTextFull' => $sEditMeTextFull . ': SharedLast'));

						$sOutput .= SssSBla::wrapHTMLinEditDiv($aBlas, $sHTML, false);

					} else {

						$sOutput .= $sHTML;

					} // if super user or not

				} // if not last page

				$sOutput .= ' ] ';

			} else if ($this->paginator->prevPage()
					and $this->paginator->nextPage())
				$sOutput .= ' | ';

			if ($this->paginator->nextPage()) {

				$sHTML = '<a href="'
						. $this->paginator->urlForPaginatorState(
								$this->page,
								$this->paginator->paginatorState(
										$this->paginator->nextPage()))
						. '">' . SssSBla::bla('SharedNext', $aND) . ' '
						. $this->itemsPhraseSssS(
								min($this->paginator->pageSize(),
										($this->paginator->itemCount()
										- $this->paginator->endItem())))
						. ' &gt;&gt;</a>';

				if ($bShowBlaEditLinks) {

					$aBlas = array(array(
							'sUID' => 'SharedNext',
							'sEditMeTextShort' => 'eT',
							'sEditMeTextFull' => $sEditMeTextFull . ': SharedNext'));

					// additional item editable text, is it localizable?
					$mUID = SssSBla::removeBlaPrefixIfHas($this->paginator->itemPhrase(
								min($this->paginator->pageSize(),
										($this->paginator->itemCount()
										- $this->paginator->endItem()))));

					// if so add it to the editable texts
					if ($mUID) $aBlas[] = array(
						'sUID' => $mUID, 'sEditMeTextShort' => 'eI',
						'sEditMeTextFull' => $sEditMeTextFull . ': ' . $mUID);

					$sOutput .= SssSBla::wrapHTMLinEditDiv($aBlas, $sHTML, false);

				} else {

					$sOutput .= $sHTML;

				} // if super user or not

			} // if got next page

		} else {
			// JS to edit form, then click submit button
			if ($this->paginator->prevPage()) {

				$sUrl = $this->paginator->urlForPaginatorState(
								$this->page,
								$this->paginator->paginatorState(
										$this->paginator->prevPage()));

				$sClick = $this->paginator->jsForState(
								$this->paginator->paginatorState(
										$this->paginator->prevPage()));


				$sHTML = '<a href="' . $sUrl . '" onClick="' . $sClick
					. ' return false;">&lt;&lt; '
					. SssSBla::bla('SharedPrevious', $aND) . ' '
					. $this->itemsPhraseSssS($this->paginator->pageSize())
					. '</a>';

				if ($bShowBlaEditLinks) {
					$aBlas = array(array(
							'sUID' => 'SharedPrevious',
							'sEditMeTextShort' => 'eT',
							'sEditMeTextFull' => $sEditMeTextFull . ': SharedPrevious'));

					// additional item editable text, is it localizable?
					$mUID = SssSBla::removeBlaPrefixIfHas($this->paginator->itemPhrase($this->paginator->pageSize()));

					// if so add it to the editable texts
					if ($mUID) $aBlas[] = array(
						'sUID' => $mUID, 'sEditMeTextShort' => 'eI',
						'sEditMeTextFull' => $sEditMeTextFull . ': ' . $mUID);

					$sOutput .= SssSBla::wrapHTMLinEditDiv($aBlas, $sHTML, false);

				} else {

					$sOutput .= $sHTML;

				} // if super user or not

			} // if has prevPage

			if ((1 < $this->paginator->pageCount())
					and (0 != $this->maxJumpPagesToShow)) {

				$iFirstJumpPage = max(1, $this->paginator->currentPage()
						- (floor($this->maxJumpPagesToShow / 2)));
				$iLastJumpPage = min($iFirstJumpPage
						+ ($this->maxJumpPagesToShow - 1),
						$this->paginator->pageCount());

				$sOutput .= ' [ ' . SssSBla::bla('SharedJumpTo') . ': ';

				if (1 != $iFirstJumpPage) {

					$sHTML = '<a href="'
							. $this->paginator->urlForPaginatorState(
									$this->page,
									$this->paginator->paginatorState(1))
							. '" onClick="'
							. $this->paginator->jsForState(
									$this->paginator->paginatorState(1))
							. ' return false;">' . SssSBla::bla('SharedFirst', $aND)
							. '</a> ...';

					if ($bShowBlaEditLinks) {

						$aBlas = array(array(
								'sUID' => 'SharedFirst',
								'sEditMeTextShort' => 'eT',
								'sEditMeTextFull' => $sEditMeTextFull . ': SharedFirst'));

						$sOutput .= SssSBla::wrapHTMLinEditDiv($aBlas, $sHTML, false);

					} else {

						$sOutput .= $sHTML;

					} // if super user or not

				} // if not first page

				for ($iP = $iFirstJumpPage; $iP <= $iLastJumpPage; $iP++) {
					if ($iP == $this->paginator->currentPage())
						$sOutput .= ' ' . $iP . ' ';

					else {
						$sOutput .= ' <a href="'
								. $this->paginator->urlForPaginatorState(
										$this->page,
										$this->paginator->paginatorState($iP))
								. '" onClick="'
								. $this->paginator->jsForState(
										$this->paginator->paginatorState($iP))
								. ' return false;">' . $iP . '</a>';

					} // if current page or other

				} // for each showed page index

				if ($iLastJumpPage != $this->paginator->pageCount()) {

					$sHTML = '... <a href="'
							. $this->paginator->urlForPaginatorState(
									$this->page,
									$this->paginator->paginatorState(
											$this->paginator->pageCount()))
							. '" onClick="'
							. $this->paginator->jsForState(
									$this->paginator->paginatorState(
											$this->paginator->pageCount()))
							. ' return false;">' . SssSBla::bla('SharedLast', $aND)
							. '</a>';

					if ($bShowBlaEditLinks) {

						$aBlas = array(array(
								'sUID' => 'SharedLast',
								'sEditMeTextShort' => 'eT',
								'sEditMeTextFull' => $sEditMeTextFull . ': SharedLast'));

						$sOutput .= SssSBla::wrapHTMLinEditDiv($aBlas, $sHTML, false);

					} else {

						$sOutput .= $sHTML;

					} // if super user or not

				} // if not last page

				$sOutput .= ' ] ';

			} else if ($this->paginator->prevPage()
					and $this->paginator->nextPage())
				$sOutput .= ' | ';

			if ($this->paginator->nextPage()) {

				$sHTML = '<a href="'
						. $this->paginator->urlForPaginatorState(
								$this->page,
								$this->paginator->paginatorState(
										$this->paginator->nextPage()))
						. '" onClick="'
						. $this->paginator->jsForState(
								$this->paginator->paginatorState(
										$this->paginator->nextPage()))
						. ' return false;">' . SssSBla::bla('SharedNext', $aND) . ' '
						. $this->itemsPhraseSssS(
								min($this->paginator->pageSize(),
										($this->paginator->itemCount()
										- $this->paginator->endItem())))
						. ' &gt;&gt;</a>';

				if ($bShowBlaEditLinks) {

					$aBlas = array(array(
							'sUID' => 'SharedNext',
							'sEditMeTextShort' => 'eT',
							'sEditMeTextFull' => $sEditMeTextFull . ': SharedNext'));

					// additional item editable text, is it localizable?
					$mUID = SssSBla::removeBlaPrefixIfHas($this->paginator->itemPhrase(
								min($this->paginator->pageSize(),
										($this->paginator->itemCount()
										- $this->paginator->endItem()))));

					// if so add it to the editable texts
					if ($mUID) $aBlas[] = array(
						'sUID' => $mUID, 'sEditMeTextShort' => 'eI',
						'sEditMeTextFull' => $sEditMeTextFull . ': ' . $mUID);

					$sOutput .= SssSBla::wrapHTMLinEditDiv($aBlas, $sHTML, false);

				} else {

					$sOutput .= $sHTML;

				} // if super user or not

			} // if has nextPage

		} // if url or js mode

		return $sOutput;

	} // render


	// need this as itemsPhrase is declared private
    protected function itemsPhraseSssS($count) {

    	$aND = array('noDIV' => true);
        if ($count == 1)
            return SssSBla::translateIfNeeded($this->paginator->itemPhrase($count), $aND);

        return $count . ' ' . SssSBla::translateIfNeeded($this->paginator->itemPhrase($count), $aND);

    } // itemsPhraseSssS

} // SssSPaginatorNavigation
?>
