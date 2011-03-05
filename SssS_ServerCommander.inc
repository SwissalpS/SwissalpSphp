<?php
/* * * *
 * * SssS_ServerCommander.inc is a subclass of SssS_Commander.inc
 * * parses commands, executes them by calling for it's own
 * * functions or that of it's delegate
 * * initializes from plist and kills app on failure
 * *
 * * @version 20091030_114551 (CC) Luke JZ aka SwissalpS
 * * * */
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_Commander.inc');
	require_once('SssS_AnsiEscapes.inc');
	require_once('SssS_ShellScriptRunner.inc');
	require_once('SssS_PHPinlineScriptRunner.inc');
} // if not autoincluding but on php's include path

//$echo $a = SssS_Sandbox::sharedSandbox()
class SssS_Sandbox {
	var $sInfo = '';
	static function sharedSandbox() {
		static $oSSbox = null;
		if (!$oSSbox) {
			$oSSbox = new SssS_Sandbox();
		} // if first call

		return $oSSbox;
	} // sharedSandbox

	function __toString() { $sClass = get_class($this); return '+class: ' . $sClass . chr(10) . '+methods: ' . chr(10) . '  ' . join('()' . chr(10) . '  ',  get_class_methods($this)) . '()' . chr(10) . chr(10) . '+variables: ' . chr(10) . ' ::' . join(chr(10) . ' ::',  get_class_vars($sClass)) . chr(10) . chr(10) . '-object vars: ' . chr(10) . ' ->' . join(chr(10) . ' ->',  array_keys(get_object_vars($this)));//print_r($this, true);
	} // __toString

} // SssS_Sandbox
class SssS_SandboxPHP extends SssS_Sandbox {} // SssS_SandboxPHP

class SssS_ServerCommander extends SssS_Commander {

	private static $aPerms = array(
			'CREATOR'	=> 4096,	// progamer may do anything
			'MURPHY'	=> 2048,	// sysadmin or some other power figure
			'PRESIDENT'	=> 1024,	// reseller?
			'LANDLORD'	=> 512,		// paying client?
			'LANDFE'	=> 256,		// http frontend to the virtual cluster
			'ROOMFE'	=> 128,		// http frontend to the room (when not acting on behalf)
			'ROOMOWNER'	=> 64,		// initiator of room or one given these perms
			'GRANTER'	=> 32,		// grant others perms, change their display
			'CYPHER'	=> 16,		// toggle encryption of room, block logging
			'GUARD'		=> 8,		// may kick, deny access
			'INVITER'	=> 4,		// invite others to room
			'GUEST'		=> 2,		// standard user may logout change name etc.
			'NOID'		=> 1,		// during logon
			'BLOCKED'	=> 0,		// blocked user
			);



	function SssS_ServerCommander($sSettingsPlistPath, $oDelegate) {
		// init or die
		$this->SssS_Commander($sSettingsPlistPath, $oDelegate);

		$oSSbox = SssS_Sandbox::sharedSandbox();
		$oSSbox->roomController = $this->oDelegate;
		$oSSbox->serverCommander = $this;
		$oSSbox->sInfo = 'This is the shared sandbox to which both superusertypes have access.';

	} // SssS_Commander

	// override SssS_Commander defaults
	protected function initDefaults() {

		// unique in SssS_ServerCommander: domains
		$this->getOrSet('translationDomain',
				array(
				'my'		=> 'my',
				'user'		=> 'user',
				'system'	=> 'system',
				'sys'		=> 'system',
				'room'		=> 'system',
				'rooms'		=> 'system',
				'commands'	=> 'commands',
				'comm'		=> 'commands',
				'coms'		=> 'commands',
				'com'		=> 'commands',
				'men'		=> 'menu',
				'menu'		=> 'menu',
				'menue'		=> 'menu',
				'menü'		=> 'menu',
				'smile'		=> 'smile'
				));

		// keys can be used to alias commands
		$this->getOrSet('translationCommands',
				array(
				// multiple ways to logout
				'exit'			=> 'exit', 'oao'		=> 'exit',
				'logout'		=> 'exit', 'logoff'		=> 'exit',
				'quit'			=> 'exit', 'bye'		=> 'exit',
				// 4 ways to get help
				'help'			=> 'help', 'hilfe'		=> 'help',
				'???'			=> 'help',	'?'			=> 'help',
				// 4 ways to call menu
				'menu'		=> 'menu', '*'		=> 'menu',
				'menue'			=> 'menu', 'menü'		=> 'menu',
				// get and set
				'get'			=> 'get', 'set'			=> 'set',
				// symbols - on telnet it's basic color syntax
				// on http side it's smilies -> char linked to image url
				'listsymbols'	=> 'listSymbols',
				'removesymbol'	=> 'removeSymbol',
				'definesymbol'	=> 'defineSymbol',
				// user functions
				'removeuser'	=> 'removeUser',
				'newuser'		=> 'newUser',
				'listusers'		=> 'listUsers',
				'ipof'			=> 'ipOf', 'uidof'		=> 'uidOf',
				'colour'		=> 'colour', 'color'	=> 'colour',
				'password'		=> 'password', 'name'	=> 'name',
				'repeatmylastpost'=> 'repeatMyLastPost',
				'repeat'		=> 'repeatMyLastPost',
				'flush'			=> 'flush', 'flushall'	=> 'flushAll',
				'run'			=> 'run', 'runf'		=> 'runf',
				'shell'			=> 'shell',	'kick'		=> 'kick',
				'end'			=> 'end', 'kill'		=> 'end'));

		// keys must be translated commands
		$this->getOrSet('translationHelp',
				array(
				'colour' 		=> ' - /colour
    change your colour',
				'defineSymbol'	=> ' -** no_help_written_yet ',
				'end'			=> ' +** /end
    terminate the room server kicking all off -> { -/(kill|end) [n] [msg] }',
				'exit'			=> ' + /exit
    logout',
				/*'flash'			=> ' -',*/
				'fl'			=> ' - name unclear:
    point is to delay a flush for another 7 seconds -> which means we need also
    commands or priv levels that can prevent this from happening....
    guess flash wars will simply be part of it as I don\'t reckon it\'s simple
    to refrain the use of ctrl-c',
				'flush'			=> ' + /flush             { ~/flush [n] [msg] }
    flushes your history, frees up some memory and may save your settings
    to file if roomServerSettings allow to do so. To flush your telnet
    screen press esc followed by c  to flush on html frontend use the button ',
				'flushAll'		=> ' +** /flushAll
    flushes all in the room, logged on or not ',
				'flushchildren' => ' -*** /flushChildren
	flushAll but also down the tree into all private rooms
    (that aren\'t savehavens...)',
				'get'			=> ' +*** /get [domain] path/in/plist
    access properties directly where domain is one of:
    ([my] | (room | sys | system) or (com | comm | commands) or smile
    or (user (uid | name)))
    Paths are case sensitive.
    Without domain, user currentUID is inserted.

    domains are: my, room, system, commands, smile
    there is a special domain: user
        syntax: /get user (uid|name) /path/in/plist
           the / at the beginning of paths is optional',
				'help'			=> ' + /help [command]
    shows the help text to certain command
    or to all commands if command is left out.',
				'ipOf'			=> ' -** no_help_written_yet
    for http(s) frontend authentication ',
				'kick'			=> ' +* /kick (uid | name)
    force a user to leave without blacklisting.
    Is often a friendly gesture when a connection hangs
    and the user can\'t log in anymore.',
				'listSymbols'	=> ' -** no_help_written_yet',
				'listUsers'		=> ' -* no_help_written_yet',
				'menu'			=> ' - /* sytax unclear
    show the items I could use according to my privs.',
				'name'			=> ' - /name newname
	change your displayed name ',
				'nameOf'		=> ' -* /nameof user newname
    change displayed name of other user ',
				'newUser'		=> ' -* /newuser newname
    add new user ',
				'password'		=> ' - /password newpassword
    change your password ',
				'recho'			=> ' - /recho [n]
    repeat the last n posts to me, I was typing... defaults to whatever the
    room setting has as default for it\'s users',
				'passwordof'	=> ' -* /passwordof user newpassword
    change password of other user ',
				'removeSymbol'	=> ' -** no_help_written_yet',
				'repeatMyLastPost'=> ' - /repeat
    repeat My Last Post (if available)',
				'run'			=> ' +** /run command [arg1 arg2 ...]
    run an escaped sh command.',
				'runf'			=> ' +** /(#|runf) command [arg1 arg2 ...]
    run an unescaped sh command. This allows | and ;
    Be aware to escape strings yourself! ',
				'set'			=> ' +** /set domain path/in/plist value
    /set domain plist/Path [-t (a | b | d | da | de | f | i | n | s)] value
    set properties directly.
    No value generally sets an empty string. Exceptions occur when
    type casting with -t or -type. e.g. "/set paths -t a" replaces paths with
    an empty array.
    domains are: my, room, system, commands, smile
        there is a special domain: user
        syntax: /set user (uid | name) /path/in/plist [-t ...] value
    the / at the beginning of paths is optional. They are case-sensitive.
    -type | -t : type modifier
                 define the value-type:
          a (array) if "-t a" present followed by nothing an empty array.
                 if value is present, it is parsed by explode(",,,,", $sValue)
                  (four commas) seperate each item. values are treated as PHP
                  strings, no sub arrays possible at this time nor type casting
                  subvalues.
                 value for path is set to an empty array which can then be
                 populated: "/set path/to/arrayKey/0 value" where 0 is the
                 first index.
          b (bool, boolean) case insensitive values (true, yes, 1) set TRUE
                 any other value or no value at all, set FALSE
          d (dict, dictionary) if no value, a PHP keyed array:
                      array("_dict_" => "_dict_marker info")
                 value is parsed as key value pairs separated with the same
                 as when casting arrays. The key and value are separated by
                 "...." (four periods)
                 e.g. "/set myDict -d Key1....value one,,,,key2....5,,,,7....ha"
                 would make eq of array("Key1" => "value one",
                                        "key2" => 5,
                                             7 => "ha")
                 if you don\'t supply equal amount of keys and values, the key
                 is ignored.
                 [[[this syntax is likely change if I was to support more depth]]]
          da (data) not yet implemented, possibly never will, use pathstrings
          de (date) not yet implemented, possibly never will, use integer/floats
          f (float) set floatval(value) -> novalue = 0
          i (integer, int) sets intval(value) -> novalue = 0 (chops off any .xx)
          n (null, nil) removes key
          s (string, str) sets strval(value) which it would anyway more or less
                  as string is the default

          inexistant paths are created. When existing, value type and
          key must match.',
				'shell'			=> ' ~** /($|shell) phpToPassThruEval
    ... just an idea of having an interactive php shell, maybe iPHP can help out',
				'uidOf'			=> ' -* /uidOf name
    get the uid of a name'));

	} // initDefaults


	function command_colour($aArgs = null, $oUser = null) {


		return ' -- not yet coded -- ';
	} // command_colour



	function command_defineSymbol($aArgs = null, $oUser = null) {
		// only allow system wide murphy and upward
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();


		return ' -- not yet coded -- ';
	} // command_defineSymbol



	function command_end($aArgs = null, $oUser = null) {
		if (!$this->oDelegate) return 'no delegate to command';
		// only allow system wide murphy and upward
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();

		$sMsg = $oUser->uid() . ' aka ' . $oUser->name()
			. ' has issued me to shutdown this room
			good bye everybody';
		$this->oDelegate->shutdown($sMsg);

		// just in case shutdown didn't do it
		exit('failed to shutdown correctly');
	} // command_end



	function command_exit($aArgs = null, $oUser = null) {
		if ($oUser) return $oUser->logout();
		// just in case, we'd rather breakdown than swarm
		// out of controll
		//parent::command_exit($aArgs);
	} // command_exit



	function command_flush($aArgs = null, $oUser = null) {
		// allow anyone, who's logged on
		if (!$this->isAllowed($oUser, 2)) return $this->_insufficientPermission();

		$bRes = $oUser->flushHistory();

		return (($bRes) ? ' ok flushed your history'
				: ' failed to flush your history')
				. ', to flush your telnet screen press esc followed by c ';
	} // command_flush



	function command_flushAll($aArgs = null, $oUser = null) {
		// only allow system wide murphy and upward
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();

		foreach($this->oDelegate->aUo as $oOtherUser) { $oOtherUser->flushHistory(); }

		return ' ok, guess I flushed all ';
	} // command_flushAll



	// syntax /get domain path/in/plist
	function command_get($aArgs = null, $oUser = null) {
		// only allow system wide murphy and upward
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();

		$mRes = null;
		if ('array' != gettype($aArgs)) $aArgs = array('');
		$aArgs = array_merge($aArgs, array('', '', ''));
		$mTmp = array_shift($aArgs);
		$sDomain = $this->translateDomain($mTmp);
		if (!$sDomain) {
			// if no domain was given we try with my
			$sDomain = 'my';
			$sPath = $mTmp;
		} else {
			$sPath = array_shift($aArgs);
		} // if got valid domain

		switch($sDomain) {
			case 'my' : // modify my settings
				$mRes = $oUser->get($sPath); // if there is more than 1 bit left
			break;
			case 'user' : // modify other users settings
				$sUserNameOrUID = $sPath;
				$sPath = array_shift($aArgs);
				if ($oOtherUser = $this->_findUser($sUserNameOrUID))
						$mRes = $oOtherUser->get($sPath);
			break;
			case 'system' : // dito
				$mRes = $this->oDelegate->oPrefs->get($sPath);
			break;
			case 'commands' : // this's settings
				$mRes = $this->get($sPath);
			break;
			case 'smile' : // the symbol converter / decorator
				$mRes = $this->oDelegate->oEmmotion->get($sPath);
			break;
			default: $sReturn = $this->command_help(array('get'));
		} // switch first argument: my, room, system ....

		if (null === $mRes) {
			$mRes = SssS_AnsiEscapes::gimme('redOnWhite')
//					. ' error getting whatever'
					. 'NULL'
					. SssS_AnsiEscapes::gimme() . "\n";
		} else {
			// got a result, format output
			$sUserOutputMode = $oUser->getOrset('outputOptions/getOutput', 'var_export');
			switch($sUserOutputMode) {
				case 'var_dump' :
					ob_start();	var_dump($mRes); $mRes = ob_get_clean();
				break;
				case 'print_r' :
					$mRes = print_r($mRes, true);
				break;
				default :
				case 'var_export' :
					$mRes = var_export($mRes, true);
				break;
			} // switch output mode
		} // if got a result or not
		return $mRes;
	} // command_get


	function command_ipOf($aArgs = null, $oUser = null) {
		// only allow system wide murphy and upward
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();


		return ' -- not yet coded -- ';

	} // command_ipOf



	// syntax: /kick (uid | name)
	function command_kick($aArgs = null, $oUser = null) {
		// only allow system wide murphy and upward for now
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();

		$mRes = null;
		$sUserNameOrUID = array_shift($aArgs);
		if ($oOtherUser = $this->_findUser($sUserNameOrUID))
				$mRes = $oOtherUser->logout();
		if (!$mRes) $mRes = ' failed to kick user ';
		return $mRes;
	} // command_kick



	function command_listSymbols($aArgs = null, $oUser = null) {
		// only allow system wide murphy and upward
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();


		return ' -- not yet coded -- ';

	} // command_listSymbols



	function command_listUsers($aArgs = null, $oUser = null) {
		// only allow system wide murphy and upward
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();


		return print_r(array_keys($this->oDelegate->aUo), true)
				. ' -- not yet coded -- ';

	} // command_listUsers



	function command_name($aArgs = null, $oUser = null) {
		// only allow system wide murphy and upward for now
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();

		//$sUID->leaseNewUID()

		return ' -- not yet coded -- ';

	} // command_name



	function command_newUser($aArgs = null, $oUser = null) {
		// only allow system wide murphy and upward
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();


		return ' -- not yet coded -- ';

	} // command_newUser



	function command_password($aArgs = null, $oUser = null) {


		return ' -- not yet coded -- ';

	} // command_password



	function command_removeSymbol($aArgs = null, $oUser = null) {

		return ' -- not yet coded -- ';

	} // command_removeSymbol



	/**
	 * command_removeUser($aArgs = null, $oUser = null)
	 * __Function_description_here__
	 *
	 * @param string $aArgs __parameter_description_here__ (defaults to: null)
	 * @param string $oUser __parameter_description_here__ (defaults to: null)
	 *
	 * @return string __return_description_here__
	 *
	 * @uses __something__ - __uses_desciption_here__
	 *
	 * @package __SssS__ - __package_desciption_here__
	 * @author Luke JZ aka SwissalpS <SwissalpS@LukeZimmerman.com>
	 * @license __license_url_here__
	 * @copyright 2007 Luke JZ aka SwissalpS
	 * @version 20091106_163237 (CC)2007 Luke JZ aka SwissalpS
	 */
	 // syntax: /removeUser (uid | name)
	function command_removeUser($aArgs = null, $oUser = null) {
		// only allow system wide murphy and upward
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();

		// parse args
		$sUserNameOrUID = array_shift($aArgs);
		$oOtherUser = $this->_findUser($sUserNameOrUID);
		if (!$oOtherUser) return $this->_userNotFound();
		$sUID = $oOtherUser->uid();


		// notify parents and children
		// TODO:


		// kick
		echo $oOtherUser->logout();

		// unlink userfile
		$sPath = $oOtherUser->s_path;
		var_dump(@unlink($sPath));

		// unlink uid file containing path to above
		$sUIDpath = $this->oDelegate->oPrefs->getOrSet(
				'paths/uidPool', 'data/UIDs/') . $sUID;

		$bUIDsExpireImmediatlyAfterDeletion = $this->oDelegate->oPrefs->getOrSet(
				'bUIDsExpireImmediatlyAfterDeletion', false);

		$bUIDsTouchWhenDeleting = $this->oDelegate->oPrefs->getOrSet(
				'bUIDsOnlyTouchWhenDeleting', false);

		if ($bUIDsExpireImmediatlyAfterDeletion) {

			// unlink uid file containing path
			var_dump(@unlink($sUIDpath));

		} elseif ($bUIDsTouchWhenDeleting) {

			var_dump(touch($sUIDpath));

		} else {

			// or write in expire date
			$iExpire = gmdate('U', time() + ($this->oDelegate->oPrefs->getOrSet(
					'iUIDsValidForHours', 24 * 7 * 3) * 3600));

			var_dump(file_put_contents_safely($sUIDpath, $iExpire, 'wb'));

		} // if uids expire immediatly or not

		// remove from aUo
		$this->oDelegate->aUo[$sUID] = null;
		// unset
		//$this->oPusers->set($sUID, null);
		unset($this->oDelegate->aUo[$sUID]);


		// save
		$this->oDelegate->oPusers->set('/', array_keys($this->oDelegate->aUo));
		$this->oDelegate->oPusers->save();

		return ' -- done, user deleted, uid and name are available again depending on settings -- ';

	} // command_removeUser




	// syntax: /set  [-t (a | b | da | de | f | i | s)] value
	function command_set($aArgs = null, $oUser = null) {
		// only allow system wide murphy and upward
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();

		$mRes = null;
		$mTmp = array_shift($aArgs);
		$sDomain = $this->translateDomain($mTmp);
		if (!$sDomain) {
			// if no domain was given we try with my
			$sDomain = 'my';
			// so if domain is missing we have the path
			$sPath = $mTmp;
		} elseif ('user' == $sDomain) {
			// special case
			$sUserNameOrUID = array_shift($aArgs);
			$sPath = array_shift($aArgs);
		} else {
			// got domain, path must be next
			$sPath = array_shift($aArgs);
		} // if got valid domain or not

		$mTmp = array_shift($aArgs);
		if ('-t' == strtolower($mTmp) || '-type' == strtolower($mTmp)) {
			// got a type indicator
			$sType = strtolower(array_shift($aArgs));
			$mValue = implode(' ', $aArgs);
			switch($sType) {
				case 'array' :
				case 'a' : // array
					if (0 < strlen($mValue)) {
						// use arguments passed as "php,,strict array definition,,9,,4"
						// without keys and ',,,,' as splitter
						$aItems = explode(',,,,', $mValue);
						$mValue = array();
						foreach ($aItems as $mItem) {
							$mValue[] = trim($mItem); /// need to check type?
						} // loop arguments passed delimited with ',,,,'
					} else {
						// just hand over a default
						$mValue = array();
					} // if got arguments or not
					// TODO: debate if allow definition of simple arrays
				break;
				case 'boolean' :
				case 'bool' :
				case 'b' : // boolean
					switch (strtolower($mValue)) {
						case 'true'	:
						case 'yes'	:
						case '1'	:
						case 1		:
							$mValue = true;
						break;
						default		:
							$mValue = false;
					} // switch bool values
				break;
				case 'data' :
				case 'da' : // data
					// TODO:
				break;
				case 'date' :
				case 'de' : // date
					// TODO:
				break;
				case 'dictionary' :
				case 'dict' :
				case 'd' : // dictionary / hash -> keyed array need at least 1 non numeric key to go thru as dict instead of array
					if (0 < strlen($mValue)) {
						// use arguments passed as ('php'....'strict array definition',,,,9....4)
						$aEntries = explode(',,,,', $mValue);
						$mValue = array('_dict_' => '_dict_marker must be first'
								. ' if no other non numerical index supplied');
						foreach ($aEntries as $sKeyValuePair) {
							$aKeyValuePair = explode('....', $sKeyValuePair);
							if (isset($aKeyValuePair[0], $aKeyValuePair[1]))
								$mValue[$aKeyValuePair[0]] = $aKeyValuePair[1];
						} // loop entries
					} else {
						// just hand over a default
						$mValue = array('_dict_' => '_dict_marker must be first'
								. ' if no other non numerical index supplied');
					} // if got arguments or not
				break;
				case 'float' :
				case 'f' : // float
					$mValue = floatval($mValue);
				break;
				case 'integer' :
				case 'int' :
				case 'i' : // integer
					$mValue = intval($mValue);
				break;
				case 'null' :
				case 'nil' :
				case 'n' :
					$mValue = null;
				break;
				case 'string' :
				case 'str' :
				case 's' : // string - is the default anyway
					$mValue = strval($mValue);
				break;
				default : // syntax error
					return ' syntax error type casting error 106';
			} // switch type to cast to
		} else {
			$mValue = implode(' ', array_merge(array($mTmp), $aArgs));
		} // if type casting
var_dump(trim($mValue), $mValue);
		//if ('' == trim($mValue)) $mValue = null;

var_dump(trim($mValue), $mValue);
		switch($sDomain) {
			case 'my' : // modify my settings
				$mRes = $oUser->set($sPath, $mValue);
			break;
			case 'user' : // modify other users settings
				if ($oOtherUser = $this->_findUser($sUserNameOrUID))
						$mRes = $oOtherUser->set($sPath, $mValue);
			break;
			case 'system' : // modify room settings
				$mRes = $this->oDelegate->oPrefs->set($sPath, $mValue);
			break;
			case 'commands' : // this's settings
				$mRes = $this->set($sPath, $mValue);
			break;
			case 'smile' : // the symbol converter / decorator
				$mRes = $this->oDelegate->oEmmotion->set($sPath, $mValue);
			break;
			default: $mRes = $this->command_help(array('set'));
		} // switch first argument: my, room, system ....
// TODO: format output
		if (!$mRes) $mRes = $this->command_help(array('set'));
		if (!$mRes) $mRes = 'error setting whatever';

		return $mRes;
	} // command_set

	/*
	 * syntax
	 * /run command [arg1 arg2 ...]
	 *
	 */
	// run a command line and show results
	function command_run($aArgs = null, $oUser = null) {
		// only allow system wide murphy and upward
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();

		return $this->_runScript($aArgs);
	} // command_run

	/*
	 * syntax
	 * /runf command [arg1 arg2 ...]
	 *
	 */
	// run a command line and show results without escaping
	function command_runf($aArgs = null, $oUser = null) {
		// only allow system wide murphy and upward
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();

		return $this->_runScript($aArgs, false);
	} // command_run

	// run interactive shell
	function command_shell($aArgs = null, $oUser = null) {
		// only allow system wide murphy and upward
		if (!$this->isAllowed($oUser)) return $this->_insufficientPermission();

		// quick way to pass on values between last call
		//$sScript = '$a='. var_export($oUser->aLastPHPresults, true) . ';'
		//		. implode(' ', $aArgs);
		// was too iritating so I have now just what the inline runner aranges
		// and what our sandbox(es) do
		$sScript = implode(' ', $aArgs);

		if (3 < $this->oDelegate->iDebugLevel())
			echo 'about to eval(php) ->'
				. $sScript . '<-' . "\n";

		// reinitialize sandbox and shell if needed
		if (!$oUser->oSandBox) $oUser->oSandBox = new SssS_Sandbox();
		if (!$oUser->oShellPHP) {
			$oUser->oShellPHP =	new SssS_PHPinlineScriptRunner($oUser->oSandBox,
					(array)$oUser->get('optionsPiSR', array('bAllowAll' => true))); // TODO: set other default when allowing other users to use the crippled shell
			//$oUser->oShellPHP->sharedSandBox = SssS_Sandbox::sharedSandbox();
			$oUser->oSandbox->sInfo = 'This object contains all variables and values set without prepending $this. It belongs to the users inline script runnig object. There is also a shared sandbox that can be accessed with: $o = SssS_Sandbox::sharedSandbox();return get_class_methods($o);';
		}
		$oPiSR = $oUser->oShellPHP;

		// run the inclusion hoping syntax is ok script
		$aRes = $oPiSR->doScript($sScript, true, true);
		array_unshift(@$oUser->aLastPHPresults, $aRes['scriptReturn']);

		// prefix & postfix output with message if exit code not zero
		$sUserOutputMode = $oUser->getOrset('outputOptions/phpShellOutput', 'var_export');
		try {
		switch($sUserOutputMode) {
			case 'var_dump' :
				ob_start();	var_dump($aRes['scriptReturn']); $sRet = ob_get_clean();
			break;
			case 'print_r' :
				$sRet = print_r($aRes['scriptReturn'], true);
			break;
			default :
			case 'var_export' :
				$sRet = @var_export($aRes['scriptReturn'], true);
			break;
		} // switch output mode
		} catch(Exception $e) {
			echo ' problem here, probably too large object';
		}
		$sOut = $aRes['scriptOutput'];
		$sFormat = "snippet returned:\n-->" . '%1$s' . "<-- \n"
				. "snippet outputed:\n-->" . '%2$s' . "<-- \n";
		$sReturn = sprintf($sFormat, $sRet,	$sOut);

		return $sReturn;

		return
	'I don\'t yet know how I\'m going to solve this gimmick without' . "\n"
	. 'disturbing the chat. I think it\'s not required as you can ssh' . "\n"
	. 'into the host.' . "\n"
	. 'would be cool tho to have a php-console where you can activly' . "\n"
	. 'modify the running app. I doubt php can do that. Ruby is special' . "\n"
	. 'in that aspect.' . "\n";

	} // command_shell
	function command_uidOf($aArgs = null, $oUser = null) {

		return ' -- not yet coded -- ';

	} // command_uidOf

	function runCommandLine($sCommandLine = '', $oUser = null) {
echo '
ServerCommander
';
		$aArgs = explode(' ', $sCommandLine);
		// remove command from args
		$sCommand = array_shift($aArgs);
		if ($mRes = $this->translateCommand($sCommand)) {
echo '
ServerCommander exists in list
';
			// reply with output
			// translate command
			$sCommand = 'command_' . $mRes;
			if (is_callable(array($this, $sCommand), false)) {
echo '
ServerCommander is callable self
';
				$sOut = call_user_func(array($this, $sCommand), $aArgs, $oUser);
			} elseif ($this->oDelegate &&
					is_callable(array($this->oDelegate, $sCommand), false)) {
echo '

ServerCommander is callable delegate

';
				$sOut = call_user_func(array($this->oDelegate, $sCommand), $aArgs, $oUser);
			} else {
				// tell user no go
				$sOut = $sCommand . ' is not a valid command (1
';
			} // if can call me, delegate or no function found
		} else {
			// tell user no go
			$sOut = $sCommand . ' is not a valid command (2
';
		} // if a registered command or not

		return $sOut;
	} // runCommandLine
/*
	function translateCommand($sCommand = '') {
		$s = strtolower($sCommand);
		$aC = $this->getOrSet('translationCommands',
				array(
				// multiple ways to logout
				'exit'			=> 'exit', 'oao'		=> 'exit',
				'logout'		=> 'exit', 'logoff'		=> 'exit',
				'quit'			=> 'exit', 'bye'		=> 'exit',
				// 4 ways to get help
				'help'			=> 'help', 'hilfe'		=> 'help',
				'???'			=> 'help',	'?'			=> 'help',
				'*'				=> 'menu', 'menu'		=> 'menu',
				'menue'			=> 'menu', 'menü'		=> 'menu',
				// get and set
				'get'			=> 'get', 'set'			=> 'set',
				// symbols - on telnet it's basic color syntax
				// on http side it's smilies -> char linked to image url
				'listsymbols'	=> 'listSymbols',
				'removesymbol'	=> 'removeSymbol',
				'definesymbol'	=> 'defineSymbol',
				// user functions
				'removeuser'	=> 'removeUser',
				'newuser'		=> 'newUser',
				'listusers'		=> 'listUsers',
				'ipof'			=> 'ipOf', 'uidof'		=> 'uidOf',
				'colour'		=> 'colour', 'color'	=> 'colour',
				'password'		=> 'password', 'name'	=> 'name',
				'repeat'		=> 'repeatMyLastPost',
				'flush'			=> 'flush', 'flushall'	=> 'flushAll',
				'run'			=> 'run', 'runf'		=> 'runf',
				'shell'			=> 'shell',	'kick'		=> 'kick',
				'end'			=> 'end', 'kill'		=> 'end'));
		if (isset($aC[$s])) return $aC[$s];
		return null;
	} // translateCommand
*/
	function translateDomain($sInput = 'my') {
		$s = strtolower($sInput);
		$aDT = $this->get('translationDomain', array());
		if (isset($aDT[$s])) return $aDT[$s];
		return null;
	} // translateDomain
/*
	function translateHelp($sCommand = '') {
		if (! $s = $this->translateCommand($sCommand)) return null;
		$aH = $this->getOrSet('translationHelp',
				array(
				'colour' 		=> ' -(colour | color) change your colour ',
				'definesymbol'	=> ' -**no_help_written_yet ',
				'end'			=> ' +** /(kill | end) terminate the room server kicking all off -> { -/(kill|end) [n] [msg] }',
				'exit'			=> ' +(exit | oao | logoff | logout | quit | bye) logout ',
				'flash'			=> ' -',
				'fl'			=> ' -name unclear: point is to delay a flush for another 7 seconds -> which means we need also commands or priv levels that can prevent this from happening.... guess flash wars will simply be part of it as I don\'t reckon it\'s simple to refrain the use of ctrl-c',
				'flush'			=> ' +syntax: /flush             { ~/flush [n] [msg] }
				flushes your history, frees up some memory and may save your settings to file if roomServerSettings allow to do so. To flush your telnet screen press esc followed by c  to flush on html frontend use the button ',
				'flushall'		=> ' +**syntax: /flushAll
				flushes all in the room, logged on or not ',
				'flushchildren' => ' -***syntax: /flushChildren
				flushAll but also down the tree into all private rooms (that aren\'t savehavens...)',
				'get'			=> ' +***syntax: /get [([my] | (room | sys | system) | (com | comm | commands) | smile | (user (uid | name)))] path/in/plist
				access properties directly. Paths are case sensitive.
			domains are: my, room, system, commands, smile
			there is a special domain: user
			syntax: /get user (uid|name) /path/in/plist
			the / at the beginning of paths is optional',
				'help'			=> ' ~syntax: /(help | hilfe | ???) [command] ',
				'ipof'			=> ' -**no_help_written_yet for http(s) frontend authentication ',
				'kick'			=> ' +*syntax: /kick (uid | name)
				force a user to leave without blacklisting. Is often a friendly gesture when a connection hangs and the user can\'t log in anymore.',
				'listsymbols'	=> ' -**no_help_written_yet ',
				'listusers'		=> ' -*no_help_written_yet ',
				'menu'			=> ' -show the items I could use according to my privs.',
				'name'			=> ' -change your displayed name ',
				'nameof'		=> ' -*change displayed name of other user ',
				'newuser'		=> ' -*add new user ',
				'password'		=> ' -change your password ',
				'recho'			=> ' -syntax: /(repeat | recho) [n]
				repeat the last n posts to me, I was typing... defaults to whatever the room setting has as default for it\'s users',
				'passwordof'	=> ' -*change password of other user ',
				'removesymbol'	=> ' -**no_help_written_yet ',
				'repeat'		=> ' -repeat My Last Post (if available)',
				'run'			=> ' +**run an escaped sh command. syntax /run command [arg1 arg2 ...] ',
				'runf'			=> ' +**syntax: /(#|runf) command [arg1 arg2 ...] run an unescaped sh command. This allows | and ; but be aware to escape strings yourself! ',
				'set'			=> ' +**set properties directly sytax /set domain path/in/plist value
			domains are: my, room, system, commands, smile
			there is a special domain: user
			syntax: /set user (uid | name) /path/in/plist
			the / at the beginning of paths is optional',
				'shell'			=> ' ~**syntax: /($|shell) phpToPassThruEval  ... just an idea of having an interactive php shell ',
				'uidof'			=> ' -*get the uid of a name '));

		if (isset($aH[$s])) return $aH[$s];
		return null;
	} // translateHelp
*/


	private function _findUser($sUserNameOrUID) {
var_dump(array_keys($this->oDelegate->aUo));
		if (isset($this->oDelegate->aUo[$sUserNameOrUID])) {
echo '
found user by uid
';
			return $this->oDelegate->aUo[$sUserNameOrUID];
		} else {
			// loop thru users and find one with this name
			$sUserNameOrUID = strtolower($sUserNameOrUID); // make it easier
			foreach ($this->oDelegate->aUo as $oOtherUser) {
				if (strtolower($oOtherUser->name()) == $sUserNameOrUID)
					// found a match, let's hope it's the right guy
					return $oOtherUser;
			} // foreach user
		} // if found by uid or name

echo '
didn\'t find user by uid or name
';
		return null;
	} // _findUser



	/**
	 * _insufficientPermission()
	 * output insufficient permission message
	 *
	 * @return string the currently set string for this message
	 *
	 * @uses __something__ - __uses_desciption_here__
	 *
	 * @package __SssS__ - __package_desciption_here__
	 * @author Luke JZ aka SwissalpS <SwissalpS@LukeZimmerman.com>
	 * @license __license_url_here__
	 * @copyright 2007 Luke JZ aka SwissalpS
	 * @version 20091106_162208 (CC)2007 Luke JZ aka SwissalpS
	 */
	 private function _insufficientPermission() {

		return ' ' . $this->oDelegate->oPrefs->getOrSet(
				'lang/en/insufficientPermission',
				'insufficient permission to do so');

	} // _insufficientPermission



	private function _runScript($aArgs, $bEscape = true, $bMute = false) {
		$sScript = implode(' ', $aArgs);
		if ($bEscape) $sScript = escapeshellcmd($sScript);

		if (3 < $this->oDelegate->iDebugLevel()) echo 'about to run script ->'
				. $sScript . '<-' . "\n";

		// run the shell script TODO: attach the runner ala inline runner [or unset it again?]
		$oSSR = new SssS_ShellScriptRunner();
		$aRes = $oSSR->doScript($sScript, $bMute);

		// prefix & postfix output with message if exit code not zero
		$sFormat = (0 !== $aRes['scriptExitCode'])
				? 'Notice, script exited with non zero code: %1$s'
						. "\n-->" . '%2$s' . "<-- \n"
						. 'Script exited with non zero code: %1$s' . "\n"
				: "\n-->" . '%2$s' . "<-- \n";

		$sReturn = sprintf($sFormat, $aRes['scriptExitCode'],
					$aRes['fullOutput']);

		return $sReturn;
	} // _runScript


} // SssS_ServerCommander

/* * * *\ SssS_ServerCommander.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
