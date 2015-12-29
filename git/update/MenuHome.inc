<?php
/* * *
 * * SwissalpS/git/update/MenuHome.inc
 * *
 * * version 20151228_170231 (CC) Luke JZ aka SwissalpS
 * * */
namespace SwissalpS\git\update;

use SwissalpS\git\update\Fetcher;

use SwissalpS\CLI\MenuAbstract;
use SwissalpS\CLI\Tools;


class MenuHome extends MenuAbstract {

    protected $sPathListDefault;

    public function __construct() {

        $this->sPathListDefault = $_SERVER['HOME'] . '/.local/share/data/SwissalpS/gitFetcher.plist';

        $aChoices = [
                     'Scan current directory for git projects',
                     'Toggle remotes list...',
                     'Toggle projects list...',
                     'Load lists from disk...',
                     'Save lists to disk...',
                     'Fetch all remotes',
                     //'Fetch all branches of all remotes',
                     'Quit',
                     ];

        $aMethods = [
                     'scanDirContainingProjects_',
                     'toggleRemotesList_',
                     'toggleProjectsList_',
                     'loadListsFromDisk_',
                     'saveListsToDisk_',
                     'fetchAllRemotes',
                     //'fetchAllBranchesOfAllRemotes',
                     'quit',
                     ];

        $sTitle = '     Home';

        return parent::__construct($aChoices, $aMethods, $sTitle, null, 'Load lists from disk...');

    } // __construct


    protected function fetchAllRemotes() {

        Fetcher::sharedInstance()->fetchAllRemotes();

        $this->mDefault = 'Scan a directory containing projects...';

        return true;

    } // fetchAllRemotes


    protected function fetchAllBranchesOfAllRemotes() {

        Fetcher::sharedInstance()->fetchAllRemotes();

        $this->mDefault = 'Quit';

        return true;

    } // fetchAllBranchesOfAllRemotes


    protected function loadListsFromDisk_() {

        $sPath = $this->sPathListDefault;

        $sPath = Tools::getInputCLI('Load from which list? ', $sPath);

        if (!is_readable($sPath)) {

            echo NL . 'Error: file not existant or not readable: ' . $sPath . NL;

            return true;

        } // if invalid path

        Fetcher::sharedInstance()->loadListsFromDisk($sPath);

        $this->mDefault = 'Fetch all remotes';

        return true;

    } // loadListsFromDisk_


    protected function saveListsToDisk_() {

        $sPath = $this->sPathListDefault;

        $sPath = Tools::getInputCLI('Save lists where? ', $sPath);

        Fetcher::sharedInstance()->saveListsToDisk($sPath);

        $this->mDefault = 'Fetch all remotes';

        return true;

    } // saveListsToDisk_


    protected function scanDirContainingProjects() {

        Fetcher::sharedInstance()->collect();

        echo 'Scan done.' . NL;

        $this->mDefault = 'Toggle remotes list...';

        return true;

    } // scanDirContainingProjects

    //NOTE: this will have unexpected results as it is not maintained
    protected function scanDirContainingProjects_() {

        $sPath = getcwd();

        $sPath = Tools::getInputCLI('Which directory?', $sPath);

        if (!is_dir($sPath)) {

            echo NL . 'Error: not a directory: ' . $sPath . NL;

            return true;

        } // if invalid path given

        $oG = Fetcher::sharedInstance();

        $oG->init($sPath);

        if ($oG->isGitDir()) {

            //TODO: show actions for repos
            throw new Exception('seems to be a git dir: ');

        } // if not git dir

        $oG->collect();

        echo 'Scan done.' . NL;

        $this->mDefault = 'Toggle remotes list...';

        return true;

    } // scanDirContainingProjects_


    protected function toggleProjectsList_() {

        Fetcher::sharedInstance()->interactiveToggleProjects();

        $this->mDefault = 'Save lists to disk...';

        return true;

    } // toggleProjectsList_


    protected function toggleRemotesList_() {

        Fetcher::sharedInstance()->interactiveToggleRemotes();

        $this->mDefault = 'Toggle projects list...';

        return true;

    } // toggleRemotesList_

} // MenuHome

/* * * *\ SwissalpS/git/update/MenuHome.inc (CC) Luke JZ aka SwissalpS /* * * */
