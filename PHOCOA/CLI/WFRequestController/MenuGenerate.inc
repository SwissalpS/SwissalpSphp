<?php
/* * * *
 * * SwissalpS/PHOCOA/CLI/WFRequestController/MenuGenerate.inc
 * *
 * * @version 20151230_171558 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\CLI\WFRequestController;

use SwissalpS\CLI\MenuAbstract;
use SwissalpS\CLI\ShellScriptRunnerForDummies;
use SwissalpS\CLI\Tools;
use WFRequestController;

class MenuGenerate extends MenuAbstract {

    protected $sPathBin;
    protected $sPathPropelBuild;
    protected $sPathPropelGenBin;
    protected $sPropelBinArgsBuild;
    protected $sPropelBinArgsClasses;
    protected $sPropelBinArgsConf;
    protected $sPathPhocoaScripts;
    protected $sPHPbin;

    public function __construct($sTitle = '       WFRequestController "menu-Generate"') {

        $this->sPathBin = APP_ROOT . DIR_SEP . 'bin';
        $this->sPathPropelBuild = APP_ROOT . DIR_SEP . 'propel-build';
		//TODO: get this and other values from the .plist that created this project
        $this->sPathPropelGenBin = '/gitSwissalpS/Propel2/bin/propel';
        $this->sPropelBinArgsBuild = ' --config-dir "' . $this->sPathPropelBuild . '"'
								   . ' --output-dir "' . $this->sPathPropelBuild . '"';
        $this->sPropelBinArgsClasses = ' --config-dir "' . $this->sPathPropelBuild . '"'
								   . ' --output-dir "' . APP_ROOT . DIR_SEP . 'classes' . '"';
        $this->sPropelBinArgsConf = ' --config-dir "' . $this->sPathPropelBuild . '"'
								   . ' --output-dir "' . APP_ROOT . DIR_SEP . 'conf' . '"';
        $this->sPathPhocoaScripts = FRAMEWORK_DIR . DIR_SEP . 'framework' . DIR_SEP . 'script';
        $this->sPHPbin = ShellScriptRunnerForDummies::getPHPinterpreterPath();

        $aChoices = array(
            'Propel2 database:reverse -> schema.xml',
            'fix schema.xml',
            'Propel2 database:build -> class files',
            '0, 1 and 2 for all connections',
            'Propel2 config:convert -> PROPEL_CONF',
            'de/activate Propel2',
            '---------------------------------------',
            'create scaffolds for Propel classes',
            '---------------------------------------',
            'create a module',
            'create a page',
            '---------------------------------------',
            'Create a test stub for phocoa classes',
            '---------------------------------------',
            'back to main menu'
        );

        $aMethods = array(
            'propel2database_reverse',
            'fixSchema',
            'propel2database_build',
            'do_0_1_2allConnections',
            'propel2config_convert',
            'togglePropel2',
            '-----',
            'createScaffolds',
            '-----',
            'createModule',
            'createPage',
            '----',
            'createTestStub',
            '----',
            'quit'
        );

        return parent::__construct($aChoices, $aMethods, $sTitle, null, 'back to main menu');

    } // __construct


    protected function collectPropel2connectionNames() {

        // we can not assume that propel is initiated yet
        $aPropelConfigLocal = array();
        $aPropelConfigRepo = array();
        if (is_file($this->sPathPropelBuild . DIR_SEP . 'propel.php')) {
            $aPropelConfigLocal = include($this->sPathPropelBuild . DIR_SEP . 'propel.php');
        } // if got local config
        if (is_file($this->sPathPropelBuild . DIR_SEP . 'propel.php.dist')) {
            $aPropelConfigRepo = include($this->sPathPropelBuild . DIR_SEP . 'propel.php.dist');
        } // if got distro config
        // merge both
        $aPropelConfig = array_merge($aPropelConfigRepo, $aPropelConfigLocal);

        // extract location names
        $aPropelConnectionNames = array();
        if (!isset($aPropelConfig['propel']['database']['connections'])) {
            echo NL . 'error: Propel2 main config files not found or incorect' . NL;
            return $aPropelConnectionNames;
        } // if bad config
        foreach ($aPropelConfig['propel']['database']['connections'] as $aPropelConnectionName => $aPropelConnection) {
            $aPropelConnectionNames[] = $aPropelConnectionName;
        } // loop gathering all names
        if (empty($aPropelConnectionNames)) {
            echo NL . 'error: no Propel2 connections found.' . NL;
            return $aPropelConnectionNames;
        } // if no connections found

        return $aPropelConnectionNames;

    } // collectPropel2connectionNames


    protected function createModule() {

        $oRunner = ShellScriptRunnerForDummies::sharedRunner();

        $sPathToModules = Tools::removeTrailingSlash(Tools::getInputCLI(
                            NL . 'Where should the module be created?',
                            APP_ROOT . DIR_SEP . 'modules'), DIR_SEP);
        if (!is_dir($sPathToModules)) $oRunner->makePath($sPathToModules);

        $sModuleName = Tools::getInputCLI(NL . 'Module name:', 'newmodule');
        $sDefaultPageName = Tools::getInputCLI(NL . 'Default page [blank for none]:', '');
        //self::setEnvForPhocoaPhingPropel();
        $sShellLine = 'cd "' . $sPathToModules . '";'
                    . $this->sPHPbin . ' ' . $this->sPathPhocoaScripts . DIR_SEP
                    . 'createModule.php ' . $sModuleName . ' ' . $sDefaultPageName
                    . ';cd ' . getCwd();

        $iRes = null;
        passthru($sShellLine, $iRes);

        return true;

    } // createModule


    protected function createPage() {

        $oRunner = ShellScriptRunnerForDummies::sharedRunner();

        //TODO: collect dirs and also offer arbitrary like now
        $sPathToPage = Tools::removeTrailingSlash(Tools::getInputCLI(
                        NL . 'Where should the page be created?',
                        APP_ROOT . DIR_SEP . 'modules'), DIR_SEP);
        if (!is_dir($sPathToPage)) $oRunner->makePath($sPathToPage);

        $sPageName = Tools::getInputCLI(NL . 'Page name:', 'newpage');
        //self::setEnvForPhocoaPhingPropel();
        $sShellLine = 'cd "' . $sPathToPage . '";'
                    . $this->sPHPbin . ' ' . $this->sPathPhocoaScripts . DIR_SEP . 'createPage.php ' . $sPageName
                    . ';cd ' . getCwd();

        $iRes = null;
        passthru($sShellLine, $iRes);

        // now lets add to module, if ok
        $sModuleName = basename($sPathToPage);
        $aChoicesYN = array('no', 'yes');
        $sChoice = trim(strToLower(Tools::getChoiceCLI('add page delegate to ' . $sModuleName . '.php?', $aChoicesYN, 'yes')));
        if ('no' == $sChoice) return true;

        require_once($this->sPathPhocoaScripts . DIR_SEP . 'scriptComponents.php');
        $sModuleName = basename($sPathToPage);
        $sModulePageBit = _pageForModule($sModuleName, $sPageName);
        $sModuleFilePath = $sPathToPage . DIR_SEP . $sModuleName . '.php';
        if (is_file($sModuleFilePath)) {
            $sModuleOriginal = file_get_contents($sModuleFilePath);
            $iCount = 0;
            $sModuleNew = str_replace('?' . '>', $sModulePageBit . NL . '?'
                                      . '>', $sModuleOriginal, $iCount);
            switch ($iCount) {
                case 0 : // no closing php tag
                    file_put_contents($sModuleFilePath, $sModuleOriginal . $sModulePageBit);
                    echo NL . 'NOTICE: no closing php tag ie ?' . '> in '
                        . $sModuleFilePath . NL
                        . 'added page module without adding closing tag for php';
                    break;

                case 1 : // fine, let's save
                    file_put_contents($sModuleFilePath, $sModuleNew);
                    echo NL . 'added page module to ' . $sModuleFilePath . NL;
                    break;

                default : // oops, multiple occurences
                    file_put_contents($sModuleFilePath . '.prop', $sModuleOriginal
                                      . NL . '<' . '?' . 'php' . NL
                                      . '// this is the proposed structure. you need '
                                      . 'to manually check your php opening and '
                                      . 'closing tags' . NL . $sModulePageBit
                                      . NL . '?' . '>');
                    echo NL . 'WARNING:oops, multiple occurences of closing php '
                            . 'tag ie ?' . '> in ' . $sModuleFilePath . NL
                            . 'made a proposition in ' . $sModuleFilePath . '.prop' . NL;

            } // switch count of php closing tags

        } else {

            // no module yet
            $sModuleNew = '<' . '?' . 'php' . NL
                    . _moduleBit($sModuleName, $sPageName) . NL
                    . $sModulePageBit . NL . '?' . '>';
            file_put_contents($sModuleFilePath, $sModuleNew);
            echo NL . 'there was no ' . $sModuleName
                    . '.php so it has been created with ' . $sPageName
                    . ' as default page.' . NL . $sModuleFilePath . NL;

        } // if module php file already exists or not

        return true;

    } // createPage


    protected function createScaffolds() {

        $aConnections = $this->collectPropel2connectionNames();
        $iLenQuery = strLen('Query');
        $sPathClassesBase = APP_ROOT . DIR_SEP . 'classes' . DIR_SEP . 'SssSPropel2' . DIR_SEP;
        $sAvailableClasses = '';
        foreach ($aConnections as $sConnetion) {
            $sPath = $sPathClassesBase . $sConnetion . DIR_SEP;
            $rDir = opendir($sPath);
            if (false === $rDir) {
                echo NL . 'error: could not open ' . $sPath . NL;
                continue;
            } // if could not open dir
            while (false !== $sFile = readdir($rDir)) {
                $sPathFile = $sPath . $sFile;
                $aInfo = pathinfo($sPathFile);
                if (!(isset($aInfo['extension'])
                      && 'php' == $aInfo['extension'])) {
                    continue;
                } // skip any non php

                $sClassName = $aInfo['filename'];

                // ends with Query?
                if ('Query' == substr($sClassName, -$iLenQuery)) {
                    continue;
                } // if query-class, skip

                $sAvailableClasses .= 'SssSPropel2\\' . $sConnetion . '\\' . $sClassName . ' ';

            } // loop all files in dir

        } // loop all connection directories

        if (empty($sAvailableClasses)) {
            echo 'you don\'t seem to have any generated classes yet.' . NL;
            return true;
        } // if no classes found

        echo NL . 'available classes to build scaffolds for:' . NL
            . $sAvailableClasses . NL;
        $sEntityList = Tools::getInputCLI(
            NL . 'Which entities do you want to create scaffolds for?',
            $sAvailableClasses);
        $sPathToPutScaffolds = Tools::getInputCLI(
            NL . 'Where should the scaffolds be created?', APP_ROOT
            . DIR_SEP . 'modules');
        WFRequestController::setEnvForPhocoaPropel();
        if (!is_dir($sPathToPutScaffolds)) {
            ShellScriptRunnerForDummies::sharedRunner()->makePath($sPathToPutScaffolds);
        }
        $sShellLine = 'cd "' . $sPathToPutScaffolds . '";'
                    . $this->sPHPbin . ' ' . $this->sPathPhocoaScripts . DIR_SEP
                    . 'scaffold.php ' . escapeshellarg(trim($sEntityList))
                    . ';cd ' . getCwd();
        $iRes = null;
        passthru($sShellLine, $iRes);

        return true;

    } // createScaffolds


    protected function createTestStub() {

        $oRunner = ShellScriptRunnerForDummies::sharedRunner();

        //TODO: offer better default
        $sPathToTest = Tools::removeTrailingSlash(Tools::getInputCLI(
                NL . 'Where should the teststub be created?', APP_ROOT), DIR_SEP);
        if (!is_dir($sPathToTest)) $oRunner->makePath($sPathToTest);

        $sTestClassName = Tools::getInputCLI(NL . 'Name of class to test:', 'WFObject');
        WFRequestController::setEnvForPhocoaPhingPropel();
        $sShellLine = 'cd "' . $sPathToTest . '";'
                    . $this->sPHPbin . ' ' . $this->sPathPhocoaScripts . DIR_SEP
                    . 'createTestStub.php ' . escapeshellarg($sTestClassName)
                    . ';cd "' . getCwd() . '";';

        if (0 !== $oRunner->doScript($sShellLine)) {

            echo $oRunner->sOut();

        } // if error

        return true;

    } // createTestStub


    protected function do_0_1_2allConnections() {

        // we can not assume that propel is initiated yet
        // collect connection names
        $aPropelConnectionNames = $this->collectPropel2connectionNames();

        foreach ($aPropelConnectionNames as $sPropelConnection) {
            echo NL . 'propel database:reverse ' . $sPropelConnection . NL;
            $iRes = null;
            passthru($this->sPathBin . DIR_SEP . 'propel2.phps database:reverse ' . $sPropelConnection . ' ' . $this->sPropelBinArgsBuild, $iRes);
            if (0 !== $iRes) {
                echo NL . 'error: abborting jobs on this connection.' . NL;
                continue;
            } // if error

            echo NL . 'fix-schema' . NL;
            $iRes = null;
            passthru($this->sPathBin . DIR_SEP . 'fix-schema.php', $iRes);
            if (0 !== $iRes) {
                echo NL . 'error: abborting jobs on this connection.' . NL;
                continue;
            } // if error

            echo NL . 'propel database:build' . NL;
            $iRes = null;
			passthru($this->sPathBin . DIR_SEP . 'propel2.phps' . ' model:build' . $this->sPropelBinArgsClasses, $iRes);
            if (0 !== $iRes) {
                echo NL . 'error: oops.' . NL;
                continue;
            } // if error

        } // loop each connection

        return true;

    } // do_0_1_2allConnections


    protected function fixSchema() {

        $iRes = null;
        passthru($this->sPathBin . DIR_SEP . 'fix-schema.php', $iRes);

        return true;

    } // fixSchema


    protected function propel2config_convert() {

        $iRes = null;
        $sPathPropelBuildConf = $this->sPathPropelBuild . DIR_SEP . 'config.php';
        if (is_file($sPathPropelBuildConf)) {
            rename($sPathPropelBuildConf, $sPathPropelBuildConf . '.last');
            echo 'backed up existing conf in build dir to ' . $sPathPropelBuildConf . '.last' . NL;
        } // make sure old file is gone
        passthru($this->sPathBin . DIR_SEP . 'propel2.phps config:convert' . $this->sPropelBinArgsBuild, $iRes);
        if (is_file($sPathPropelBuildConf)) {
            if (is_file(PROPEL2_RUNTIME_CONFIG)) {
                rename(PROPEL2_RUNTIME_CONFIG, PROPEL2_RUNTIME_CONFIG . '.last');
                echo 'backed up current conf to ' . PROPEL2_RUNTIME_CONFIG . '.last' . NL;
            } // if got old config
            rename($sPathPropelBuildConf, PROPEL2_RUNTIME_CONFIG);
            echo 'moved conf from build dir to ' . PROPEL2_RUNTIME_CONFIG . NL;
        } else {
            echo NL . 'error, did not generate new runtime config!';
        } // if new file created

        return true;

    } // propel2config_convert


    protected function propel2database_build() {

        $iRes = null;
        passthru($this->sPathBin . DIR_SEP . 'propel2.phps' . ' model:build' . $this->sPropelBinArgsClasses, $iRes);

        return true;

    } // propel2database_build


    protected function propel2database_reverse() {

        // which connection?
        $aPropelConnectionNames = $this->collectPropel2connectionNames();
        if (empty($aPropelConnectionNames)) {
            return true;
        } // if no connections found
        $sPropelConnection = Tools::getChoiceCLI(
                                'which connection to reverse',
                                $aPropelConnectionNames, $aPropelConnectionNames[0]);
        $iRes = null;
        passthru($this->sPathBin . DIR_SEP . 'propel2.phps database:reverse ' . $sPropelConnection . ' ' . $this->sPropelBinArgsBuild, $iRes);

        return true;

    } // propel2database_reverse


    protected function togglePropel2() {

        static $sSearchPropelAct = null;
		static $sPathAppConfActive = null;
		static $sPathAppConfTemplate = null;
        static $sCommentOFF = null;
		static $sCommentON = null;
		static $sCommentRestart = null;
        if (!$sSearchPropelAct) {

            $sSearchPropelAct = '/define\(["\']PROPEL2_ACTIVATED["\'],\s?(\w+)\);/';
            $sPathAppConfActive = APP_ROOT . DIR_SEP . 'conf' . DIR_SEP . 'SssSphocoaApp.conf';
            $sPathAppConfTemplate = $sPathAppConfActive . '.SssStemplate';
            $sCommentOFF = 'Switching OFF Propel2 integration' . NL . 'Repeat to reactivate.'
                    . '(after restarting app)' . NL;
            $sCommentON = 'Switching ON Propel2 integration' . NL . 'Repeat to deactivate.'
                    . '(after restarting app)'  . NL
                    . 'Make sure you have something like this in your '
                    . 'application delegate class::initialize()' . NL
                    . '
    if (PROPEL2_ACTIVATED) {
        if (is_readable(PROPEL2_RUNTIME_CONFIG)) {
            ////////////// THIS IS IMPORTANT ////////////////
            require_once(PROPEL2_RUNTIME_CONFIG);
        } else {
            throw new WFException(\'could not read Propel2 config file at \' . PROPEL2_RUNTIME_CONFIG);
        } // if got propel config at all
    } // if propel is activated' . NL . NL;
            $sCommentRestart = NL . 'You need to restart the app to toggle again.' . NL . 'Or you can edit your conf and template manually.' . NL;
        } // if first run

        if (defined('PROPEL2_ACTIVATED') && PROPEL2_ACTIVATED) {
            $sNewValue = 'false';
            $sComment = $sCommentOFF;
        } else {
            $sNewValue = 'true';
            $sComment = $sCommentON;
        } // if currently activated or not

        foreach (array($sPathAppConfActive, $sPathAppConfTemplate) as $sPathAppConf) {
            if (is_writable($sPathAppConf)) {
                $sAppConf = @file_get_contents_safely($sPathAppConf);
                echo 'Searching in ' . $sPathAppConf . NL . 'for "define("PROPEL2_ACTIVATED", ...);"' . NL;
                $aRes = array();
                $iRes = preg_match($sSearchPropelAct, $sAppConf, $aRes);
                if ($iRes) {
                    echo 'Found: ' . $aRes[0] . NL;
                    $sAppConf = str_replace($aRes[0], 'define(\'PROPEL2_ACTIVATED\', ' . $sNewValue . ');', $sAppConf);
                    file_put_contents_safely($sPathAppConf, $sAppConf, 'wb');
                } else {
                    $sComment2 = 'error: could not find "define("PROPEL2_ACTIVATED", ...);" in ' . $sPathAppConf . NL
                        . 'add define("PROPEL2_ACTIVATED", true); manually.' . NL;
                    $sComment .= NL . $sComment;
                    echo $sComment2;
                } // if found
            } else {
                $sComment2 = NL . 'error, can not write to ' . $sPathAppConf . NL;
                $sComment .= $sComment2;
                echo $sComment2;
                if ($sPathAppConf == $sPathAppConfActive) {
                    $sComment2 = 'try to remakeTemplatedFiles and try again.' . NL;
                    $sComment .= NL . $sComment2;
                    echo $sComment2;
                } // if has template for conf
            } // if can write to config or not
        } // loop both config and template

        echo $sComment . $sCommentRestart;

        return true;

    } // togglePropel2

} // MenuGenerate

/* * * *\ SwissalpS/PHOCOA/CLI/WFRequestController/MenuGenerate.inc (CC) Luke JZ aka SwissalpS /* * * */
