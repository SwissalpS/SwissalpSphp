<?php
/* * * *
 * * SwissalpS/CLI/MenuAbstract.inc
 * *
 * * @version 20151228_173233 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\CLI;


if (!defined('NL')) {
    define('NL', chr(10));
} // if no NL yet


abstract class MenuAbstract {

    protected $aMenu;
    protected $aMethods;
    protected $mDefault;
    protected $oDelegate;
    protected $sPrompt;
    protected $sTitle;

    public function __construct(Array $aMenu, Array $aMethods, $sTitle = null, $sPrompt = null, $mDefault = 0, $oDelegate = null) {

        if (empty($sTitle)) {

            $sTitle = 'Menu';

        } // if no title set

        if (empty($sPrompt)) {

            $sPrompt = 'please choose an action:';

        } // if no prompt given

        $this->aMenu = array();
        $this->aMethods = array();

        for ($iCount = 0; $iCount < count($aMethods); $iCount++) {

            $sMenuTitle = $aMenu[$iCount];
            $sMenuTitleEncoded = base64_encode($sMenuTitle);

            $this->aMenu[] = $sMenuTitle;
            $this->aMethods[$sMenuTitleEncoded] = $aMethods[$iCount];

        } // loop all entries

        $this->mDefault = $mDefault;
        $this->oDelegate = $oDelegate;
        $this->sPrompt = $sPrompt;
        $this->sTitle = $sTitle;

    } // __construct


    public function ask($mDefault = null) {

        if (null === $mDefault) {
            $mDefault = $this->mDefault;
        } // if no default given, use object's default

        $bNotFirstRound = false;

        do {

            if ($bNotFirstRound) {

                $mDefault = $this->mDefault;

            } else {

                $bNotFirstRound = true;

            } // if first or subsequent round

            echo NL . $this->sTitle . NL;

            $sChoice = Tools::getChoiceCLI($this->sPrompt, $this->aMenu, $mDefault);

        } while($this->executeEntry($sChoice));

        return $this;

    } // ask


    protected function executeEntry($sChoice) {

        $sMethod = $this->aMethods[base64_encode($sChoice)];
        if (method_exists($this, $sMethod)) {

            return $this->$sMethod();

        } elseif ($this->oDelegate && is_object($this->oDelegate)
                  && method_exists($this->oDelegate, $sMethod)) {

            return $this->oDelegate->$sMethod();

        } // if got the method or delegate has it

        echo NL . 'error: method, ' . $sMethod . ', does not exist in '
            . get_class($this) . ' nor in delegate (if given)' . NL;

        return true;

    } // executeEntry


    protected function notYetImplemented() {

        echo NL . 'not yet implemented.' . NL;

        return true;

    } // notYetImplemented


    protected function quit() {

        return false;

    } // quit

} // MenuAbstract

/* * * *\ SwissalpS/CLI/MenuAbstract.inc (CC) Luke JZ aka SwissalpS /* * * */
