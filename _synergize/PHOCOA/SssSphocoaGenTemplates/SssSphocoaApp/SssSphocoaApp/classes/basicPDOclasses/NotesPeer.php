<?php

require 'skyprom_phocoa/om/BaseNotesPeer.php';


/**
 * Skeleton subclass for performing query and update operations on the 'Notes' table.
 *
 *
 *
 * You should add additional methods to this class to meet the
 * application requirements.  This class will only be generated as
 * long as it does not already exist in the output directory.
 *
 * @package    skyprom_phocoa
 */
class NotesPeer extends BaseNotesPeer {


	static function notesForBridgeUID($iUID, $iLimit = null) {

		if (!$iLimit) $iLimit = SRPskypromPlistSettings::sharedInstance()->get('rss/notes/numitems', 77);

		$oCriteria = new Criteria();

		// select only those of given bridge id
		$oCriteria->add(NotesPeer::BRIDGEUID, $iUID, Criteria::EQUAL);

		// select only those marked ready to publish
		$oCriteria->add(NotesPeer::PUBLISH, 1, Criteria::EQUAL);

		// sort by karma // TODO: sift out too low karma
		//$oCriteria->addDescendingOrderByColumn(NotesPeer::KARMA);

		// sort by date descending = newest first
		$oCriteria->addDescendingOrderByColumn(NotesPeer::DATE);
		$oCriteria->setLimit($iLimit);

		return NotesPeer::doSelect($oCriteria);

	} // notesForBridgeUID


	static function notesMostRecent($iLimit = null) {

		if (!$iLimit) $iLimit = SRPskypromPlistSettings::sharedInstance()->get('rss/notes/numitems', 77);

		$oCriteria = new Criteria();

		// select only those marked ready to publish
		$oCriteria->add(NotesPeer::PUBLISH, 1, Criteria::EQUAL);

		// sort by karma // TODO: sift out too low karma
		//$oCriteria->addDescendingOrderByColumn(NotesPeer::KARMA);

		// sort by date descending = newest first
		$oCriteria->addDescendingOrderByColumn(NotesPeer::DATE);
		$oCriteria->setLimit($iLimit);

		return NotesPeer::doSelect($oCriteria);

	} // notesMostRecent


	static function notesPopularThumbnails($iLimit = null) {

		if (!$iLimit) $iLimit = SRPskypromPlistSettings::sharedInstance()->get('rss/notes/numitems', 77);

		$oCriteria = new Criteria();

		// select only those marked ready to publish
		$oCriteria->add(NotesPeer::PUBLISH, 1, Criteria::EQUAL);

		// sort by karma // TODO: sift out too low karma
		$oCriteria->addDescendingOrderByColumn(NotesPeer::KARMA);

		// sort by date descending = newest first
		$oCriteria->addDescendingOrderByColumn(NotesPeer::DATE);
		$oCriteria->setLimit($iLimit);

		// only if has thumbnail image
		$oCriteria->add(NotesPeer::MEDIAUID, '', Criteria::NOT_LIKE);

		return NotesPeer::doSelect($oCriteria);

	} // notesPopularThumbnails


	static function notesPopularThumbnailsForBridgeUID($iUID, $iLimit = null) {

		if (!$iLimit) $iLimit = SRPskypromPlistSettings::sharedInstance()->get('rss/notes/numitems', 77);

		$oCriteria = new Criteria();

		$oCriteria->add(NotesPeer::BRIDGEUID, $iUID, Criteria::EQUAL);

		// select only those marked ready to publish
		$oCriteria->add(NotesPeer::PUBLISH, 1, Criteria::EQUAL);

		// sort by karma // TODO: sift out too low karma
		$oCriteria->addDescendingOrderByColumn(NotesPeer::KARMA);

		// sort by date descending = newest first
		$oCriteria->addDescendingOrderByColumn(NotesPeer::DATE);
		$oCriteria->setLimit($iLimit);

		// only if has thumbnail image
		$oCriteria->add(NotesPeer::MEDIAUID, '', Criteria::NOT_LIKE);

		return NotesPeer::doSelect($oCriteria);

	} // notesPopularThumbnailsForBridgeUID

} // NotesPeer
