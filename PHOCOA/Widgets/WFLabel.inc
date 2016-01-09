<?php
/* vim: set expandtab tabstop=4 shiftwidth=4: */
/**
 * SwissalpS/PHOCOA/Widgets/WFLabel.inc
 *
 * @package UI
 * @subpackage Widgets
 * @copyright Copyright (c) 2005 Alan Pinstein. All Rights Reserved.
 * @version $Id: kvcoding.php,v 1.3 2004/12/12 02:44:09 alanpinstein Exp $
 * @author Alan Pinstein <apinstein@mac.com>
 */
namespace SwissalpS\PHOCOA\Widgets;

use WFLabel as WFLabelParent;
use SwissalpS\PHOCOA\Localization\Bla as SssSBla;

/**
 * A Label widget for our framework.
 *
 * <b>PHOCOA Builder Setup:</b>
 *
 * <b>Required:</b><br>
 * - {@link WFWidget::$value value} or {@link WFSelect::$values values}, depending on {@link WFSelect::$multiple multiple}.
 *   Note that you can create complex strings by using the value binding option "ValuePattern" along with multiple value<N> bindings.
 *   For instance, a ValuePattern of "%1% of %2% selected." with a value binding that resolves to "5" and a value2 binding that resolves to "10" will say "5 of 10 selected."
 *   In case it's not obvious, "%1%" is substituted with the value from the "value" binding, and "%n%" is substituted with the value from "value<N>" binding. N starts at 2 and goes up consecutively.
 *
 * <b>Optional:</b><br>
 * - {@link WFLabel::$ellipsisAfterChars ellipsisAfterChars}
 * - {@link WFLabel::$textAsHTML textAsHTML}
 * - {@link WFLabel::$raw raw}
 */
class WFLabel extends WFLabelParent {

    /**
      * Constructor.
      */
    function __construct($sID, $oPage) {

        parent::__construct($sID, $oPage);

    } // __construct


    // adds localized value
    public function setValue($sValue) {

        $this->value = SssSBla::translateIfNeeded($sValue);

    } // setLabel

} // WFLabel
