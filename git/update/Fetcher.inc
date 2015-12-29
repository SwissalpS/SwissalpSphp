<?php
/* * *
 * * SwissalpS/git/update/Fetcher.inc
 * *
 * * version 20151228_163809 (CC) Luke JZ aka SwissalpS
 * * */
namespace SwissalpS\git\update;

use Exception;
use SwissalpS\CLI\ShellScriptRunnerForDummies;
use SwissalpS\CLI\Tools;
use SwissalpS\XML\Plist;


if (!defined('NL')) {
    define('NL', chr(10));
} // if no NL yet


class Fetcher {

    private static $_instance;
    protected $aRemotes;
    protected $aStructure;
    private $iMyUID;
    private $oRunner;
    protected $sPathCWDorig;

    public function __construct() {

        $this->aRemotes = [];

        $this->aStructure = [];

        $this->oRunner = ShellScriptRunnerForDummies::sharedRunner();

        if (!$this->hasGit()) {
            throw new Exception('"git" not installed or "which" does not find or exist.');
        } // if no git instaled

        $this->sPathCWDorig = getcwd();

        $sPath = '/tmp/SssSgitUpdateFetcherOwnerTestage';
        file_put_contents($sPath, '-');
        $this->iMyUID = fileowner($sPath);
        unlink($sPath);

    } // __construct


    protected function addKnownRemote($sRemote) {

        $sRemoteEncoded = base64_encode($sRemote);

        if (isset($this->aRemotes[$sRemoteEncoded])) {

            return true;

        } // if already know this one

        $this->aRemotes[$sRemoteEncoded] = [
                                            'name' => $sRemote,
                                            'use' => false,
                                            ];

        return true;

    } // addKnownRemote


    public function applyScan(&$aRepositories) {

        foreach ($aRepositories as $sKey => $aRepo) {

            if (!isset($this->aStructure[$sKey])) {

                $this->aStructure[$sKey] = $aRepo;

                continue;

            } // if new repo found

            // use it?
            if ($aRepo['owner'] == $this->aStructure[$sKey]['owner']) {

                // still same owner -> give old value priority
                // aka nothing to do

            } else {

                // new owner -> give new value priority
                $this->aStructure[$sKey]['owner'] = $aRepo['owner'];
                $this->aStructure[$sKey]['use'] = $aRepo['use'];

            } // if owner changed or not

            $this->aStructure[$sKey]['branches'] = $aRepo['branches'];
            $this->aStructure[$sKey]['remotes'] = $aRepo['remotes'];

        } // loop each repo

        return $this;

    } // applyScan


    public function bail($iStatus = 0) {

        chdir($this->sPathCWDorig);

        echo 'have a nice day' . NL;

        exit($iStatus);

    } // bail


    public function collect() {

        $sPath = getcwd();
        $rDir = opendir($sPath);
        if (false === $rDir) {
            throw new Exception('could not open dir: ' . $sPath);
        } // if could not open dir

        $aRepositories = [];

        while (false !== $sFile = readdir($rDir)) {

            if ('.' == $sFile{0}) {
                continue;
            } // if invisible

            $sPathFile = $sPath . DIRECTORY_SEPARATOR . $sFile;

            if ($this->oRunner->isFolder($sPathFile)) {
                if (!$this->isGitDir($sPathFile)) {
                    continue;
                } // if not a git repo dir

                $aInfo = [];
                $aInfo['path'] = $sPathFile;
                $iOwner = fileowner($sPathFile);
                $aInfo['owner'] = $iOwner;
                if ($iOwner == $this->iMyUID) {
                    $aInfo['use'] = true;
                } else {
                    $aInfo['use'] = false;
                } // if running user is owner
                $aInfo['remotes'] = $this->collectRemotes($sPathFile);
                $aInfo['branches'] = $this->collectBranches($sPathFile);

                $aRepositories[base64_encode($sPathFile)] = $aInfo;

            } // if folder found

        } // loop dir

        $this->applyScan($aRepositories);

        return $aRepositories;

    } // collect


    protected function collectBranches($sPath = null) {

        $aOut = [];
        $sPathInitial = null;

        if (is_dir($sPath)) {

            $sPathInitial = getcwd();
            chdir($sPath);

        } // if need to change path

        if (!$this->git('branch --no-color --all')) {

            return $aOut;

        } // if error

        $sList = $this->sOut();
        $aList = explode(NL, $sList);

        foreach ($aList as $sBranch) {

            if (empty($sBranch)) {
                continue;
            } // if skip empty lines

            $aBranch = [
                        'name' => '',
                        //'isCheckedOut' => false,
                        //'remote' => '',
                        ];

            // checked out?
            if ('*' == $sBranch{0}) {

                $aBranch['isCheckedOut'] = true;

            } // if checked out

            // remove leading space
            $sBranch = substr($sBranch, 2);

            // local or remote?
            // remotes contain '/' and begin with 'remotes/<remote-name>/<branch-name>'
            $aParts = explode('/', $sBranch);

            if (3 == count($aParts)) {

                // begins with 'remote'

                $aBranch['remote'] = $aParts[1];
                $aBranch['name'] = $aParts[2];

            } else {

                // local branch

                $aBranch['name'] = $sBranch;

            } // if is remote or local

            $aOut[] = $aBranch;

        } // loop each branch

        if (null !== $sPathInitial) {

            chdir($sPathInitial);

        } // if path arg given

        return $aOut;

    } // collectBranches


    protected function collectRemotes($sPath = null) {

        $aOut = [];
        $sPathInitial = null;

        if (is_dir($sPath)) {

            $sPathInitial = getcwd();
            chdir($sPath);

        } // if need to change path

        if (!$this->git('remote')) {

            return $aOut;

        } // if error

        $sList = $this->sOut();
        $aList = explode(NL, $sList);

        foreach ($aList as $sRemote) {

            $sRemote = trim($sRemote);

            if (empty($sRemote)) {
                continue;
            } //

            $this->addKnownRemote($sRemote);

            // gather more info
            //$this->git('remote show -n ' . $sRemote);
            // or (outside the loop, above)
            //$this->git('remote -v');

            $aOut[] = $sRemote;

        } // loop all remotes

        if (null !== $sPathInitial) {

            chdir($sPathInitial);

        } // if path arg given

        return $aOut;

    } // collectRemotes


    protected function doS($sScript, $bMuted = false) {

        $this->oRunner->doScript($sScript, $bMuted);

        return (0 === $this->oRunner->iRes());

    } // doS


    public function fetchAllRemotes() {

        $sNewBranchToken = '[new branch]';
        $iNewBranchTokenLength = strlen($sNewBranchToken);
        $sPathFileTmp = '/tmp/SssSgitUpdateFetcherLastFetch';

        foreach ($this->aStructure as $aProject) {

            if (!$aProject['use']) {
                continue;
            } // if skip

            chdir($aProject['path']);

            foreach ($aProject['remotes'] as $sRemote) {

                if ($this->skipRemote($sRemote)) {
                    continue;
                } // if skip

                $this->git('fetch --keep --prune ' . $sRemote
                           . ' > ' . $sPathFileTmp . ' 2>&1');

                $sOut = file_get_contents($sPathFileTmp);

                echo $sOut;

                if (false !== strpos($sOut, $sNewBranchToken)) {

                    $aLines = explode(NL, $sOut);

                    foreach ($aLines as $sLine) {

                        $iPos = strpos($sLine, $sNewBranchToken);

                        if (false === $iPos) {
                            continue;
                        } // if not the line with new branch

                        $sLine = substr($sLine, $iPos + $iNewBranchTokenLength);

                        if (false === $sLine) {
                            echo 'Warning: substring resulted in false near ' . __LINE__ . ' in ' . __FILE__ . NL;
                            continue;
                        } // if error

                        $iPos = strpos($sLine, '->');

                        if (false === $iPos) {
                            echo 'Warning: strpos resulted in false near ' . __LINE__ . ' in ' . __FILE__ . NL;
                            continue;
                        } // if error

                        $sNewBranchName = trim(substr($sLine, 0, $iPos));

                        if (empty($sNewBranchName)) {
                            echo 'Warning: empty branch name found near ' . __LINE__ . ' in ' . __FILE__ . NL;
                            continue;
                        } // if error

                        $this->git('fetch ' . $sRemote . ' ' . $sNewBranchName . ':' . $sNewBranchName);

                    } // loop all lines

                } // if got new branch

                $this->git('fetch --quiet --tags ' . $sRemote);

            } // loop all remotes

        } // loop all projects

        @unlink($sPathFileTmp);

        echo NL . 'It is recommended to do a rescan so new branches are indexed' . NL;

        return $this;

    } // fetchAllRemotes


    public function git($sCommand, $bMuted = false) {

        $sScript = 'git ' . $sCommand . ';';

        return $this->doS($sScript, $bMuted);

    } // git


    public function hasGit() {

        $sScript = 'which git';

        return $this->doS($sScript);

    } // hasGit


    public function init($sPath = null) {

        $this->aRemotes = [];

        $this->aStructure = [];

        if (is_dir($sPath)) {

            chdir($sPath);

        } // if path given

        return $this;

    } // init


    public function interactiveToggleProjects() {

        $aStructureNew = [];

        foreach ($this->aStructure as $sKey => $aProject) {

            $sUse = $aProject['use'] ? 'yes' : 'no';

            echo $aProject['path'] . NL;

            if ($aProject['owner'] != $this->iMyUID) {

                echo 'skipping because of ownership.' . NL;

                $aProject['use'] = false;

                $aStructureNew[$sKey] = $aProject;

                continue;

            } // if ownership conflict

            $sUse = Tools::getInputCLI('Use? (y/n) ', $sUse);

            $sUse = strtolower($sUse);

            if (in_array($sUse, ['true', 'yes', 't', 'y'])) {

                $aProject['use'] = true;

            } else {

                $aProject['use'] = false;

            } // if true or false

            $aStructureNew[$sKey] = $aProject;

        } // loop all remotes

        $this->aStructure = $aStructureNew;

        return $this;

    } // interactiveToggleProjects


    public function interactiveToggleRemotes() {

        $aRemotesNew = [];

        foreach ($this->aRemotes as $sKey => $aRemote) {

            $sUse = $aRemote['use'] ? 'yes' : 'no';

            $sUse = Tools::getInputCLI('Use "'
                                                . $aRemote['name'] . '"? (y/n) ',
                                                $sUse);

            $sUse = strtolower($sUse);

            if (in_array($sUse, ['true', 'yes', 't', 'y'])) {

                $aRemote['use'] = true;

            } else {

                $aRemote['use'] = false;

            } // if true or false

            $aRemotesNew[$sKey] = $aRemote;

        } // loop all remotes

        $this->aRemotes = $aRemotesNew;

        return $this;

    } // interactiveToggleRemotes


    public function isGitDir($sPath = null) {

        $sSnippet = 'status --porcelain > /dev/null 2>&1';

        if (null !== $sPath) {
            $sPathInitial = getcwd();
            chdir($sPath);
            $bRes = $this->git($sSnippet);
            chdir($sPathInitial);
        } else {
            $bRes = $this->git($sSnippet);
        } // if path arg given or not

        return $bRes;

    } // isGitDir


    public function knownRemotes() {

        return $this->aRemotes;

    } // knownRemotes


    public function loadListsFromDisk($sPath) {

        $oP = new Plist($sPath);

        $this->aStructure = $oP->get('structure');

        $this->aRemotes = $oP->get('knownRemotes');

        unset($oP);

        return $this;

    } // loadListsFromDisk


    public function runCLI() {

        //TODO: check if target is given as arg
        //TODO: check if lists.plist is given as arg
        $this->init();

        if ($this->isGitDir()) {
            //TODO: show actions for repos
            throw new Exception('seems to be a git dir: ');
        } // if not git dir

        $this->collect();

        $oMenu = new MenuHome();
        $oMenu->ask();

        // and goodbye
        $this->bail();

    } // run


    public function runningUserID() {

        return $this->iMyUID;

    } // runningUserID


    public function saveListsToDisk($sPath) {

        $oP = new Plist();

        $oP->set('knownRemotes', $this->aRemotes);

        $oP->set('structure', $this->aStructure);

        if (!is_dir(dirname($sPath))) {

            $this->oRunner->makePath(dirname($sPath));

        } // if path does not exist

        $oP->saveTo($sPath);

        return $this;

    } // saveListsToDisk


    public static function sharedInstance() {

        if (!self::$_instance) {
            self::$_instance = new Fetcher();
        }

        return self::$_instance;

    } // sharedInstance


    public function skipRemote($sRemote) {

        $sRemoteEncoded = base64_encode($sRemote);

        if (!isset($this->aRemotes[$sRemoteEncoded])) {
            return true;
        } // if not known, skip

        return !$this->aRemotes[$sRemoteEncoded]['use'];

    } // skipRemote


    protected function sOut() {

        return $this->oRunner->sOut();

    } // sOut


    public function scannedStructure() {

        return $this->aStructure;

    } // scannedStructure

} // Fetcher

/* * * *\ SwissalpS/git/update/Fetcher.inc (CC) Luke JZ aka SwissalpS /* * * */
