<?php
/* * * *
 * * SwissalpS/PHOCOA/CLI/WFRequestController/MenuMain.inc
 * *
 * * @version 20151230_171619 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\CLI\WFRequestController;

use SwissalpS\CLI\MenuAbstract;
use SwissalpS\CLI\Tools;
use SwissalpS\PHOCOA\CLI\WFRequestController\MenuGenerate;
use WFModuleInvocation;
use WFRequestController;
use WFRPC;
use WFWebApplication;

class MenuMain extends MenuAbstract {

    protected $oAppDelegate;

    public function __construct($aMenu = null, $aMethods = null, $sTitle = '                 WFRequestController "menu"', $sPrompt = null, $mDefault = null) {

        $this->oAppDelegate = WFWebApplication::sharedWebApplication()->delegate();

        if (is_array($aMenu) && is_array($aMethods) && (count($aMenu) == count($aMethods))) {

            return parent::__construct($aMenu, $aMethods, $sTitle, $sPrompt, $mDefault);

        } // if array given

        $aChoices = array();
        $aMethods = array();
        if (!!$this->oAppDelegate && method_exists($this->oAppDelegate, 'handleCLIRequest')) {
            $aChoices[] = 'call AppDelegate->handleCLIRequest()';
            $aMethods[] = 'callAppDelegate';
        } // if got app delegate and has method handleCLIRequest()

        $aChoices = array_merge($aChoices, array(
            'dump html for an invocation path',
            'enter iphp console...',
            '(Lists...)',
            'Generate...',
            'enter phocoa phing task manager (not maintained)',
            'quit'
        ));
        $aMethods = array_merge($aMethods, array(
            'dumpHTMLforAnInvocatonPath',
            'enterIphpConsole_',
            'notYetImplemented',
            'generate_',
            'enterPhocoaPhingTaskManager_',
            'quit'
        ));

        return parent::__construct($aChoices, $aMethods, $sTitle, $sPrompt, $mDefault);

    } // __construct


    protected function callAppDelegate() {

        // let app delegate give it a go -> should return true if done,
        // false if wants WFRequestController to continue
        if (!!$this->oAppDelegate && method_exists($this->oAppDelegate, 'handleCLIRequest')) {
            return !($this->oAppDelegate->handleCLIRequest());
        } // if got a suitable app delegate

        return true;

    } // callAppDelegate


    protected function dumpHTMLforAnInvocatonPath() {
//TODO: catch 404 and other errors
        $nl = chr(10);
        $oWFRC = WFRequestController::sharedRequestController();

        // testing if page builds
        list($bInteractive, $modInvocationPath) = $oWFRC->parseCliRequest();

        $modInvocationPath = Tools::getInputCLI(
                $nl . 'invocation (the default shown is either the first argument '
                . 'passed or the default path):', $modInvocationPath);

        // create the root invocation; only skin if we're not in an XHR
        $oWFRC->setRootModuleInvocation(new WFModuleInvocation($modInvocationPath, NULL,
                (WFRequestController::isAjax()
                    ? NULL
                    : WFWebApplication::sharedWebApplication()->defaultSkinDelegate())));

        // get HTML result of the module, and output it
        $html = $oWFRC->rootModuleInvocation()->execute();

        // read about this above handleHTTPRequest()
        if (isset($_REQUEST[WFRPC::PARAM_ENABLE_AJAX_IFRAME_RESPONSE_MODE])
            && (1 == $_REQUEST[WFRPC::PARAM_ENABLE_AJAX_IFRAME_RESPONSE_MODE])) {
            //header('Content-Type: text/xml');
            $html = "<?xml version=\"1.0\"?><raw><![CDATA[\n{$html}\n]]></raw>";
        } // if ajax special situation with iframe

        print $html;

        return true;

    } // dumpHTMLforAnInvocatonPath


    protected function enterIphpConsole_() {

        static $oIphpShell = null;
        static $sPathTags = null;
        static $nl = null;

        print $nl . 'starting iphp shell' . $nl;

        if (null == $nl) {
            $nl = chr(10);
            $sPathTags = APP_ROOT . DIR_SEP . 'tags';
            $sPathSssSphocoaAppConf =
                    APP_ROOT . DIR_SEP . 'conf' . DIR_SEP . 'SssSphocoaApp.conf';
            $oIphpShell = new iphp(array(
                            'tags' => $sPathTags,
                            'require' => $sPathSssSphocoaAppConf,
                            'commands' => array('iphp_command_stop')
                            )
            ); // \stop to exit iphp and reenter loop with appDelegate having a go and outputting current request path
        } // if first run

        $oIphpShell->runREPL();

        return true;

    } // enterIphpConsole_


    protected function enterPhocoaPhingTaskManager_() {

        $nl = chr(10);

        print $nl . 'starting phocoa phing task manager' . $nl;

        $sCommand = '';
        WFRequestController::setEnvForPhocoaPhingPropel();
        $bContinuePhocoaTasks = true;
        while ($bContinuePhocoaTasks) {
            // run without args to show menu
            $sShellLine = //'cd "' . APP_ROOT . '";' .
                FRAMEWORK_DIR . DIR_SEP . 'phing' . DIR_SEP . 'phocoa ' . $sCommand;
                //. ';cd ' . getCwd();

            passthru($sShellLine);

            // ask for command
            $sCommand = Tools::getInputCLI(
                    $nl . '                 WFRequestController "phing task manager"'
                    . $nl .'what to do? ("back" to go back, -l or -list for '
                    . 'list of phing tasks)', 'back');
            // if not stop
            if (in_array(trim(strtolower($sCommand)),
                    array('back', 'stop', 'exit', 'quit', 'halt', 'die'))) {
                $bContinuePhocoaTasks = false;
                // pass on command
            } // if stop

        } // loop it

        return true;

    } // enterPhocoaPhingTaskManager_


    protected function generate_() {

        static $oMenuGenerate = null;
        if (!$oMenuGenerate) {
            $oMenuGenerate = new MenuGenerate();
        } // if first run

        $oMenuGenerate->ask();

        return true;

    } // generate_

} // MenuMain

/* * * *\ SwissalpS/PHOCOA/CLI/WFRequestController/MenuMain.inc (CC) Luke JZ aka SwissalpS /* * * */
