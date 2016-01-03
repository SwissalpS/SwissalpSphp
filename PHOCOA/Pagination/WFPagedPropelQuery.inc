<?php
/* vim: set expandtab tabstop=4 shiftwidth=4: */
/* * * *
 * * SwissalpS/PHOCOA/Pagination/WFPagedPropelQuery.inc
 * *
 * * @version 20160103_195135 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\Pagination;

use WFPagedData;
use WFPaginator;
use Propel\Runtime\ActiveQuery\Criteria;

/**
 * A WFPagedData implementation for a Propel2 query.
 *
 * For PHOCOA, use this instead of PropelPager.
 *
 * Sorting support: The sortKeys should be the "XXXTableMap::COLUMN" with +/- prepended.
 */
class WFPagedPropelQuery implements WFPagedData
{
    protected $oCriteria;

    /**
     *  Constructor.
     *
     *  @param object Criteria The Propel criteria for the query. example: XXXQuery::create()->filterByXYZ($sSearch);
     */
    function __construct(Criteria $oCriteria)
    {

        $this->oCriteria = $oCriteria;

    } // __construct


    function itemCount()
    {

        return $this->oCriteria->count();

    } // itemCount


    // 1 based index
    function itemsAtIndex($iStartIndex, $iNumItems, $aSortKeys)
    {
        $oCriteria = clone $this->oCriteria;

        $oCriteria->setOffset($iStartIndex - 1);

        if ($iNumItems !== WFPaginator::PAGINATOR_PAGESIZE_ALL) {

            $oCriteria->setLimit($iNumItems);

        } // if not max/default, set limit

        foreach ($aSortKeys as $sSortKey) {

            $sDirection = $sSortKey{0};
            $sKey = substr($sSortKey, 1);

            if ('-' == $sDirection) {

                $oCriteria->addDescendingOrderByColumn($sKey);

            } else {

                $oCriteria->addAscendingOrderByColumn($sKey);

            } // if down or up sorting

        } // loop each sortKey

        return $oCriteria->find()->getData();

    } // itemsAtIndex

} //WFPagedPropelQuery

/* * * *\ SwissalpS/PHOCOA/Pagination/WFPagedPropelQuery.inc (CC) Luke JZ aka SwissalpS /* * * */
