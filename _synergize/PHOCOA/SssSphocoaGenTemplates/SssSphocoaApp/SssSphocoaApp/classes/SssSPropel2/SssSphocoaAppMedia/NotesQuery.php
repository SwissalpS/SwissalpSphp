<?php

namespace SssSPropel2\SssSphocoaAppMedia;

use SssSPropel2\SssSphocoaAppMedia\Base\NotesQuery as BaseNotesQuery;
use Propel\Runtime\ActiveQuery\Criteria;
use SssSPropel2\SssSphocoaAppMedia\Map\NotesTableMap;

/**
 * Skeleton subclass for performing query and update operations on the 'Notes' table.
 *
 *
 *
 * You should add additional methods to this class to meet the
 * application requirements.  This class will only be generated as
 * long as it does not already exist in the output directory.
 *
 */
class NotesQuery extends BaseNotesQuery {


	static function notesForBridgeUID($iUID, $iLimit = null) {

        if (!$iLimit) {

            if (class_exists('SRPskypromPlistSettings')) {

                $iLimit = SRPskypromPlistSettings::sharedInstance()->get('rss/notes/numitems', 77);

            } else {

                $iLimit = 77;

            } // if got class with settings or not

        } // if no limit set

		$oCriteria = self::create();

		// select only those of given bridge id
		$oCriteria->filterByBridgeuid($iUID);

		// select only those marked ready to publish
		$oCriteria->filterByPublish(1);

		// sort by karma // TODO: sift out too low karma
		//$oCriteria->addDescendingOrderByColumn(NotesPeer::KARMA);

		// sort by date descending = newest first
		$oCriteria->addDescendingOrderByColumn(NotesTableMap::COL_DATE);
		$oCriteria->setLimit($iLimit);

		return $oCriteria->find()->getData();

	} // notesForBridgeUID


	static function notesMostRecent($iLimit = null) {

        if (!$iLimit) {

            if (class_exists('SRPskypromPlistSettings')) {

                $iLimit = SRPskypromPlistSettings::sharedInstance()->get('rss/notes/numitems', 77);

            } else {

                $iLimit = 77;

            } // if got class with settings or not

        } // if no limit set

		$oCriteria = self::create();

		// select only those marked ready to publish
		$oCriteria->filterByPublish(1);

		// sort by karma // TODO: sift out too low karma
		//$oCriteria->addDescendingOrderByColumn(NotesPeer::KARMA);

		// sort by date descending = newest first
		$oCriteria->addDescendingOrderByColumn(NotesTableMap::COL_DATE);
		$oCriteria->setLimit($iLimit);

		return $oCriteria->find()->getData();

	} // notesMostRecent


	static function notesPopularThumbnails($iLimit = null) {

        if (!$iLimit) {

            if (class_exists('SRPskypromPlistSettings')) {

                $iLimit = SRPskypromPlistSettings::sharedInstance()->get('rss/notes/numitems', 77);

            } else {

                $iLimit = 77;

            } // if got class with settings or not

        } // if no limit set

		$oCriteria = self::create();

		// select only those marked ready to publish
		$oCriteria->filterByPublish(1);

		// sort by karma // TODO: sift out too low karma
		$oCriteria->addDescendingOrderByColumn(NotesTableMap::COL_KARMA);

		// sort by date descending = newest first
		$oCriteria->addDescendingOrderByColumn(NotesTableMap::COL_DATE);
		$oCriteria->setLimit($iLimit);

		// only if has thumbnail image
		$oCriteria->filterByMediauid('', Criteria::NOT_LIKE);

		return $oCriteria->find()->getData();

	} // notesPopularThumbnails


	static function notesPopularThumbnailsForBridgeUID($iUID, $iLimit = null) {

        if (!$iLimit) {

            if (class_exists('SRPskypromPlistSettings')) {

                $iLimit = SRPskypromPlistSettings::sharedInstance()->get('rss/notes/numitems', 77);

            } else {

                $iLimit = 77;

            } // if got class with settings or not

        } // if no limit set

		$oCriteria = self::create();

		$oCriteria->filterByBridgeuid($iUID);

		// select only those marked ready to publish
		$oCriteria->filterByPublish(1);

		// sort by karma // TODO: sift out too low karma
		$oCriteria->addDescendingOrderByColumn(NotesTableMap::COL_KARMA);

		// sort by date descending = newest first
		$oCriteria->addDescendingOrderByColumn(NotesTableMap::COL_DATE);
		$oCriteria->setLimit($iLimit);

		// only if has thumbnail image
		$oCriteria->filterByMediauid('', Criteria::NOT_LIKE);

		return $oCriteria->find()->getData();

	} // notesPopularThumbnailsForBridgeUID

} // NotesQuery
