<?php
/* * * *
 * * SssS_PHPinlineScriptRunner.inc
 * * was worth a try. Works for correctly when given correct code syntax.
 * * Can be used for interactive shell. Also take a look at iphp which is
 * * different mainly that it doesn't crash the mothership quite as much :-)
 * * The crucial difference is that this class uses eval() -> inline
 * * while iphp uses exec() -> runs on seperate thread from file rebuilding
 * * per successfull call. This is a little disk intensive but has a clear
 * * stableizing advantage.
 * * I have tested the $_-feature and did not like it, that\'s why this class
 * * does not have a "last result" variable. It works more like a bash would
 * * in that you must echo or return a value to be printed (less screen filling
 * * when working with objects). With "return" the value is printed using
 * * var_dump().
 * *
 * * The advantage of eval() is that you can define new functions on the fly:
 * * enter "function help() { echo "hi there :-)";}" to see what I mean type the
 * * equivelant in iphp prepending some value to satisfy $_:
 * *  "3; function help() { echo "hi there :-)";}" so far so good. Now comes the
 * * difference, type "help()" in both script runners. iphp will roll up some
 * * error while SssS_PHPinlineScriptRunner will print "hi there :-)".
 * *
 * * to have a value in aResults['scriptReturn'] your snippet must actually
 * * return something e.g. '$a = 45*123; return $a;
 * *
 * * + you can instantiate new objects modify existing.
 * * + declare functions and classes but be aware not to redefine as that
 * *   crashes uncatchably.
 * *
 * * $this is SssS_PHPinlineScriptRunner object if you omit $this-> and just
 * * type $a, it will be converted to $this->oDelegate->a (or if no delegate
 * * just $this->a). $$a becomes $$this->oDelegate->a resp $$this->a
 * *
 * * $_a stays $_a which unless it's a global (e.g. $_SERVER) it will not be
 * * accessable outside the snippet scope. To keep this speedy do not declare
 * * functions and classes or other loops (unless you prepend function vars with _)
 * * wherever the snippet contains $this, it is not touched same goes for $this->oDelegate
 * *
 * * remember: using tools like this one holds a security risk. Be very carefull.
 * * do a lot of testing before going live!
 * *
 * * @version 20100502_203256 + treatScript() $a -> $this->a conversions
 * * @version 20091104_143909 (CC) Luke JZ aka SwissalpS
 * * * */

//require_once('SssS_FileGetPutContents.inc');

class SssS_PHPinlineScriptRunner {

	const OPT_bAllowAll							= 'bAllowAll'; // for devs
	const OPT_bUseWhitelist						= 'bUseWhitelist';
	const OPT_bUseBlacklist						= 'bUseBlacklist'; // no longer used, will go sometime
	const OPT_bAllowAssignment					= 'bAllowAssignment';
	const OPT_bForbidVariableVaribales			= 'bForbidVariableVaribales';
	const OPT_bForbidVariableFunctionCalls		= 'bForbidVariableFunctionCalls';
	const OPT_bForbidVariableVariableAssignment	= 'bForbidVariableVariableAssignment';

	var $aOptions = array(	  self::OPT_bAllowAll => false,	  // run anything the dev sends (except T_EXIT)
			   			  self::OPT_bUseWhitelist => true,   // true: allow valid calls in whitelist minus those on blacklist; false: allow valid calls not in blacklist; note: T_EXIT allways gets caught or converted into trigger_error(..)
						self::OPT_bUseBlacklist => false,  // allow valid calls not on blacklist (going)
					   self::OPT_bAllowAssignment => true,	 // allow $var =
			   self::OPT_bForbidVariableVaribales => true,	 // fordbid $$var
		   self::OPT_bForbidVariableFunctionCalls => true,	 // forbid $var()
	  self::OPT_bForbidVariableVariableAssignment => true);	 // forbid $$var =

	var $oDelegate = null; // my delegate if any to hold objects and values created without prefixing '$this'
	var $aErrors = array();

	protected $aWhiteList = array();
	protected $aBlackList = array('exit', 'die', 'file_get_contents', 'file_get_contents_safely', 'fgets', 'fgetss', 'fgetc', 'fgetcsv', 'fflush', 'file_exists', 'file_put_contents', 'file_put_contents_safely', 'file', 'fileperms', 'filegroup', 'fileinode', 'fopen', 'flock', 'fpassthru', 'fprintf', 'fputs', 'fread', 'fscanf', 'fseek', 'fsockopen', 'ftruncate', 'fwrite', 'gzfile', 'is_file', 'pcntl_fork');

	function SssS_PHPinlineScriptRunner($oDelegate = null, $aOptions = array()) {

		$this->oDelegate = $oDelegate;

		$this->aOptions = array_merge($this->aOptions, $aOptions);

		if ($this->allowAllCalls()) { // if true === aOptions['bAllowAll']

			$this->aWhiteList = array();
			$this->aBlackList = array();

		} else {

			if (isset($aOptions['aWhiteList'])) {
				unset($this->aOptions['aWhiteList']);
				$this->setWhiteList($aOptions['aWhiteList']);
			} // aWhiteList

			if (isset($aOptions['aBlackList'])) {
				unset($this->aOptions['aBlackList']);
				$this->setWhiteList($aOptions['aBlackList']);
			} // aBlackList

		} // if everything allowed no need for blacklist etc

	} // SssS_PHPinlineScriptRunner


	public function fReset() {

		$this->sScript = '';

		$this->aResults = array(
				'scriptReturn'		=> null,
				'scriptOutput'		=> null);

		$this->aErrors = array();

	} // fReset

	function allowAllCalls() {

		if (isset($this->aOptions[self::OPT_bAllowAll])
			&& (true === $this->aOptions[self::OPT_bAllowAll])) {

							   $this->aOptions[self::OPT_bUseWhitelist] = false;
							   $this->aOptions[self::OPT_bUseBlacklist] = false; // going
							$this->aOptions[self::OPT_bAllowAssignment] = true;
					$this->aOptions[self::OPT_bForbidVariableVaribales] = false;
				$this->aOptions[self::OPT_bForbidVariableFunctionCalls] = false;
		   $this->aOptions[self::OPT_bForbidVariableVariableAssignment] = false;
			return true;
		} else return false;

	} // allowAllCalls

	function allowHarmlessCalls() {

							   $this->aOptions[self::OPT_bUseWhitelist] = true;
							   $this->aOptions[self::OPT_bUseBlacklist] = false; // going
							$this->aOptions[self::OPT_bAllowAssignment] = false;
					$this->aOptions[self::OPT_bForbidVariableVaribales] = true;
				$this->aOptions[self::OPT_bForbidVariableFunctionCalls] = true;
		   $this->aOptions[self::OPT_bForbidVariableVariableAssignment] = true;
		$this->setWhiteList(explode(',',
		'explode,implode,date,time,round,trunc,rand,ceil,floor,srand,str_repeat,ord,chr,asc'.
		'strtolower,strtoupper,substr,stristr,strpos,print,print_r,echo,pow,exp,abs,sin,cos,tan'));

		return true;
	} // allowHarmlessCalls


	public function doScript($sScript = '', $bMuted = true, $bCatchDie = false) {

		$this->fReset();

		$errset = E_ERROR | E_CORE_ERROR | E_COMPILE_ERROR;
       	if (defined("E_RECOVERABLE_ERROR")) $errset |= E_RECOVERABLE_ERROR;
		$lastHandler = set_error_handler(array($this, 'handleError'), $errset);

		$this->sScript = $this->treatScript($sScript, $bCatchDie);

echo "treated script-->>$this->sScript<<--" . chr(10);

		if (0 != sizeof($this->aErrors)) {
			// no need to continue, return the errors listed
			$sOut = 'Errors:' . chr(10);
			foreach ($this->aErrors as $aE) {
				$sOut .= '(' . $aE['number'] . ') ' . $aE['message'] . chr(10);
			} //
			$this->aResults['scriptOutput'] = $sOut;
			restore_error_handler();
			return $this->aResults;
		} // if got errors

		try {
			ob_start();

			$sReturn = ($bMuted) ? @eval($this->sScript) : eval($this->sScript);

			$sOut = ob_get_clean();

		} catch (Exception $e) {

			// somehow this text never shows up, but it does keep the mothership going (nothing can keep exit() from working
			//echo ' --- something bad happened ---' . "\n"
			//		. '   with PHPsnippet --->' . $sScript . '<-- ';

			if (!isset($sOut)) $sOut =
					' --- something bad happened ---' . "\n" .
					'   with PHPsnippet --->' . $sScript . '<-- ';

		} // try catch errors

		restore_error_handler();
		$aE = error_get_last();var_dump($aE);
		if ($aE && ('SwissalpS_CLEAN_SwissalpS' != $aE['message']))
				$sOut .= "\n" . '*** ' . $aE['message'] . ' ***';

		$this->aResults = array(
				//'scriptResult'		=> $__SwissalpS_PHPinlineScriptRunner_Eval_Result,
				'scriptReturn'		=> $sReturn,
				'scriptOutput'		=> $sOut);

		return $this->aResults;
	} // doScript

	// it never has seen action, but without it things are far less stable
	function handleError($errNum, $errString) { //, $file, $line, $contextArray) {
			echo '!!##!' . chr(10) . $errNum . ' -*- ' . $errString . chr(10) . '!##!!';
			$this->aErrors[] = array('number' => $errNum, 'message' => $errString);
			return true;
	} // handleError


	function isForbiddenCall($sCall, $bReload = false) {
		static $aFunctions = null;
		static $aAllowed = null;

		if ($this->aOptions[self::OPT_bAllowAll]) return false;

		if (!$aFunctions or $bReload) {
			// build array of valid calls
			$aAllowed = array();
			$aFunctions = get_defined_functions();
			$aFunctions = array_merge($aFunctions['internal'],
					$aFunctions['user'],
					explode(',', 'print,echo,require,include,if,else,while,for'
						. ',switch,exit,true,false,null,var,public,static'
						. ',protected,private,return,break'),
						get_defined_constants(), get_declared_classes(),
						get_declared_interfaces());

			if ($this->aOptions[self::OPT_bUseWhitelist])
				// only allow those on whitelist which are defined
				$aAllowed = array_intersect($this->aWhiteList, $aFunctions);

			else// if ($this->aOptions[self::OPT_bUseBlacklist])
				// allow all that are not on blacklist
				$aAllowed = array_diff($aFunctions, $this->aBlacklist);

			//else $aAllowed = $aFunctions;

		} // first call or forced to reload

		if (empty($sCall)) return true; // :-P just called to reload perhaps

		$mRes = array_search($sCall, $aAllowed);
		return ($mRes === false || $mRes === NULL); // v<4.2.0 - null
	} // isForbiddenCall

	public function whiteList() {
		return $this->aWhiteList;
	} // whiteList

	public function setWhiteList($aList) {
		$this->aWhiteList = array_map('strtolower', (array)$aList);
		$this->isForbiddenCall('', true);
	} // setWhiteList

	public function blackList() {
		return $this->aBlackList;
	} // blackList

	public function setBlackList($aList) {
		$this->aBlackList = array_map('strtolower', (array)$aList);
		$this->isForbiddenCall('', true);
	} // setBlackList

	public function treatScript($sScript = '', $bCatchDie = true) {
		$sAttachTo = '$this->' . ((is_object($this->oDelegate))
														? 'oDelegate->' : '');

		$s = trim($sScript); if (empty($s)) return '';
		$s .= ';';

		//syntax check
		@trigger_error('SwissalpS_CLEAN_SwissalpS');
		$sOut = 'syntax check: ';
		if (true === @eval('return true;' . $s . ';'))
			$sOut .= 'PASSED'; // and loaded the functions already!

		else {
			$sOut .= 'FAILED ';
			$this->sScript = $s;

			$aE = error_get_last();
			if ($aE && ('SwissalpS_CLEAN_SwissalpS' != $aE['message'])) {
				$sOut .= sprintf('(%2$s:%3$s) %4$s%1$s%5$s ',"\n",
					$aE['type'], $aE['line'], $aE['message'], $aE['file']);
				$this->aErrors[] = array('number' => $aE['type'],
						'message' => $sOut);

				return $s;
			} // if realy an error
		} // if syntax error
		@trigger_error('SwissalpS_CLEAN_SwissalpS');
		$sOut .= chr(10);

		// analyze and rebuild from tokens

		//get some settings local
		$bAllowAssignment = $this->aOptions[self::OPT_bAllowAssignment];
		$bForbidVariableVaribales = $this->aOptions[self::OPT_bForbidVariableVaribales];
		$bForbidVariableFunctionCalls = $this->aOptions[self::OPT_bForbidVariableFunctionCalls];
		$bForbidVariableVariableAssignment = $this->aOptions[self::OPT_bForbidVariableVariableAssignment];

		// init
		$sVarCall = ''; $sNew = '';
		$bNotInclassOrFunction = true; $bNotInString = true;
		$iLevelParenthesis = $iLevelCurly = $iLevelSquare = 0;
		// I hate to call token_get_all twice here but seems better than calling
		// strtolower over and over again
		//$aTokens = token_get_all($s);
		//if (T_INLINE_HTML == $aTokens[0][0]) {
			$s = '<' . '?' . 'php ' . $s; // I can see problems comming here someone can still prepend ? > and it will parse as T_INLINE_HTML
			$aTokens = token_get_all($s);
		//} //
		$aTokensLower = token_get_all(strtolower($s));
		//$s = '<' . '?' . 'php ' . $s; // . ' ?'.'>';

		$iMax = count($aTokens); $bFirstOpenTag = true;
		for ($x = 0; $x < $iMax; $x++) {
			$sCall = false;
			$mToken = $aTokensLower[$x];
			if (is_array($mToken)) {
				$id = $mToken[0];
				switch ($id) {
					case T_OPEN_TAG :
						if ($bFirstOpenTag) {
							$bFirstOpenTag = false;
						} // if first tag we injected ourselves
						else $sNew .= '<?php ';
						break;

					case T_VARIABLE :
						$sVarCall .= 'v';
						if ($bForbidVariableVaribales
							&& ('$v' == substr($sVarCall, -2))) {
							$this->aErrors[] = array('number' => -1,
								'message' => 'variable variables forbidden');
							return $s;
						} // if not allowed var-vars and detected

						if ($bNotInclassOrFunction)
							$sNew .= $aTokens[$x][1];
							//if (!in_array(allowed vars)) -> error
						break;

					case T_STRING :
						$sVarCall .= 's';
						$sCall = $aTokens[$x][1];
						break; // SafeScript doesn't break here, why? A: because functions are T_String+(
						// you cannot create new classes (unless dev mode) or functions as they won't be in the aAllowed -> actually if it were in there you wouldn't be allowed to define it again

					case T_CLASS :
					case T_FUNCTION :
						$bNotInclassOrFunction = false;
						$sCall = $aTokens[$x][1];
						break;

					case T_EXIT :
						if ($bCatchDie) {
							$sNew .= 'trigger_error(';
							// a) dieeof
							//if ($iMax == $x+1) {} // as we allways add a ; this is allways true
							if (is_array($aTokens[++$x]))
								// T_WHITESPACE
								$y = $aTokens[++$x];
							else $y = $aTokens[$x];
							if ('(' == $y) {
								// next is either ws, num or string or )
								$z = $aTokens[++$x];
								if (is_array($z)) {
									if (T_WHITESPACE == $z[0]) {
										$z = $aTokens[++$x];
										if (!is_array($z)) {
											// )
											$x++;
											$sNew .=
											'"inputed script would die here")';
										} // if )
										else {
											$sNew .=
											'"inputed script would die here with code/msg: '
											. $z[1] . '")';
											$x += (is_array($aTokens[++$x])) ? 2 : 1;

										} // some string or number
									} // if ws
									else {
										$sNew .=
											'"inputed script would die here with code/msg: '
											. $z[1] . '")';
											$x += (is_array($aTokens[++$x])) ? 2 : 1;

									} // some string or number
								} else {
									// )
									$x++;
									$sNew .= '"inputed script would die here")';
								} //
							} else {	// ;
								$sNew .= '"inputed script would die here")';
							} // if next is ws, ( or ;
							// c) die (345
							// d) die(345
							// e) die('nthuas
							// f) die ("toehu
						} // if catch exit into trigger_error
						else {
							// don't let it run at all
							$this->aErrors[] = array('number' => -3,
								'message' => 'die and exit not permitted');
							return $s;
						} //
						break;
					case(T_NEW): // need to check if object exists and permitted
					case(T_REQUIRE_ONCE): case(T_REQUIRE):
					case(T_RETURN): case(T_BREAK): case(T_CATCH):
					case(T_CLONE):  case(T_PRINT): case(T_GLOBAL):
					case(T_ECHO): case(T_INCLUDE_ONCE): case(T_INCLUDE):
					case(T_EVAL):
						$sCall = $aTokens[$x][1];
						break;

					// 'clean cases' that don't need to be checkde
					case T_INLINE_HTML : // let's hope so...
					case T_CLOSE_TAG : // hmm...
					case T_LNUMBER : case T_DNUMBER :
					case T_ENCAPSED_AND_WHITESPACE :
					case T_CONSTANT_ENCAPSED_STRING :
					case T_OBJECT_OPERATOR : // ->   possibly need to scan in it's properties and methods...
					case T_BOOLEAN_OR : case T_LOGICAL_OR : case T_LOGICAL_XOR :
					case T_BOOLEAN_AND : case T_LOGICAL_AND :
					case T_IS_NOT_IDENTICAL : case T_IS_IDENTICAL :
					case T_IS_EQUAL : case T_IS_NOT_EQUAL :
					case T_PLUS_EQUAL : case T_MINUS_EQUAL :
					case T_AND_EQUAL : case T_OR_EQUAL :
					case T_MUL_EQUAL : case T_DIV_EQUAL : case T_MOD_EQUAL :
						if ($bNotInclassOrFunction)	$sNew .= $aTokens[$x][1];
						break;
					case T_WHITESPACE : $sNew .= ' '; break;

					// dropables
					case T_COMMENT : break;

					default:
						echo '!!untested token found: ' . $id . ' '
								. token_name($id) . chr(10);

				} // switch

				if ($sCall) {
					if ($this->isForbiddenCall($mToken[1])) {
						$this->aErrors[] = array('number' => -4,
							'message' => 'illegal call: ' . $sCall);
						return $s;
					} // if forbidden call
					if ($bNotInclassOrFunction)
						$sNew .= $aTokens[$x][1];

				} // if check call
			} // if array -> token longer than 1 char
			else {
				$sVarCall .= $mToken;
				if ($bNotInclassOrFunction)
					$sNew .= $aTokens[$x][0];

				switch ($mToken) {
					case '<' : // can be comparitor or opening of xml tag
						break;
					case '?' : // can be < ? or ternary or just text
						break;
					case '=' : // can be just text or assignement
						if ($bAllowAssignment) {
							// check for variable-variable assignement
							if ($bForbidVariableVariableAssignment
								&& ('$v' == substr($sVarCall, -3, 2))) {
							} // if not allowed var-var =

							// check with array of allowed variables

						} else {
							// not allowed
							$this->aErrors[] = array('number' => -2,
								'message' => 'assingments not allowed');
						} // if allowed assignment or not
						break;
					case ':' : //
						break;
					case ';' : //
						break;
					case '+' : //
						break;
					case '-' : //
						break;
					case '>' : //
						break;
					case '.' : //
						break;
					case ',' : //
						break;
					case '/' : //
						break;
					case '*' : //
						break;
					case '%' : //
						break;
					case '^' : //
						break;
					case '!' : // not
						break;
					case '@' : // mute, no prob?
						break;
					case '$' : // variable-variable
						break;
					case '&' : //
						break;
					case '(' : //
						++$iLevelParenthesis;
						if ($bForbidVariableFunctionCalls
									&& ('v' == substr($sVarCall, -2, 1))) {
							$this->aErrors[] = array('number' => -5,
								'message' => 'illegal dynamic function call');
							return $s;
						}
						break;
					case ')' : //
						--$iLevelParenthesis;
						break;
					case '{' : //
						 ++$iLevelCurly;
						break;
					case '}' : //
						if (0 == --$iLevelCurly) $bNotInclassOrFunction = true;
						break;
					case '[' : //
						 ++$iLevelSquare;
						break;
					case ']' : //
						 --$iLevelSquare;
						break;
					case '"' : // don't come alone normally unless has items to expand {$i} syntax
						break;
					case "'" : // don't come alone normally
						break;
					default :
						echo '!!unidentified token found: ' . $mToken . chr(10);
				} // switch

			} // if single char

		} // foreach PHP token


		// todo: replace magic __FILE__ etc

// possibly 'namespace' could solve this part of the issue
// being rather new, I'll wait

/*$s= $sScript;*/ // TODO: incorporate this in the foreach loop above
		$s = str_replace('$_', '####globalOrProtected####', $sNew);
		$s = str_replace('$this->oDelegate', '####oDelegate####', $s);
		$s = str_replace('$this', '####this####', $s);
		$s = str_replace('$$', '####string####', $s);

		$s = str_replace('$', $sAttachTo, $s);

		$s = str_replace('####string####', '$' . $sAttachTo, $s);
		$s = str_replace('####this####', '$this', $s);
		$s = str_replace('####oDelegate####', '$this->oDelegate', $s);
		$s = str_replace('####globalOrProtected####', '$_', $s);

//echo 'treated script: ' . $s . chr(10);
		return $s . ';'; // can't hurt to add an extra ';'
	} // treatScript

	/* from PHP online Manual to eval
	luke at cywh dot com
	17-Sep-2008 08:12
	Finally, a good use for eval :)!

	If you want to be able to check for syntax errors WITHOUT executing the code, add "return true;" before all the code. All execution of the code stops after that mark, and makes eval returns true on no syntax errors, and false on syntax errors.

	This is especially useful for anyone making a template system.

	Here's a working example:

	function check_syntax($code) {
	    return @eval('return true;' . $code);
	}

	print "<b>No Code execution:</b><br />\n";
	$code = "print \"<b><i>hello! you don't want me...</i></b><br />\n\";";
	var_dump(check_syntax($code)); // Good syntax
	print "<br />\n";
	var_dump(check_syntax($code . ' ==')); // Bad syntax
	print "<br />\n";

	print "<b>Code Executed...Bad</b>:<br />\n";
	var_dump(eval($code) === null); // Good syntax
	print "<br />\n";
	var_dump(@eval($code . ' ==') === null); // Bad syntax


	*/// this hurts function definitions, they get defined here and when the
	// code is re-eval-ed without the 'return true;' -> function already defined
	static function checkSyntax($s = ' ==') {
		@trigger_error('SwissalpS_CLEAN_SwissalpS');
		return @eval('return true;' . $s);
	} // checkSyntax


	static function helpCLI() {
		static $s = null; if (!$s) $s =
' * * SssS_PHPinlineScriptRunner.inc
 * * was worth a try. Works for correctly when given correct code syntax.
 * * Can be used for interaktive shell. Also take a look at iphp which is
 * * different mainly that it doesn\'t crash the mothership quit as much :-)
 * * The crucial difference is that this class uses eval() -> inline
 * * while iphp uses exec() -> runs on seperate thread from file rebuilding
 * * per successfull call. This is a little disk intensive but has a clear
 * * stableizing advantage.
 * * I have tested the $_-feature and did not like it, that\'s why this class
 * * does not have a "last result" variable. It works more like a bash would
 * * in that you must echo or return a value to be printed (less screen filling
 * * when working with objects). With "return" the value is printed using
 * * var_dump().
 * *
 * * The advantage of eval() is that you can define new functions on the fly:
 * * enter "function help() { echo "hi there :-)";}" to see what I mean type the
 * * equivelant in iphp prepending some value to satisfy $_:
 * *  "3; function help() { echo "hi there :-)";}" so far so good. Now comes the
 * * difference, type "help()" in both script runners. iphp will roll up some
 * * error while SssS_PHPinlineScriptRunner will print "hi there :-)".
 * *
 * * to have a value in aResults[\'scriptReturn\'] your snippet must actually
 * * return something e.g. \'$a = 45*123; return $a;
 * *
 * * + you can instantiate new objects modify existing.
 * * + declare functions and classes but be aware not to redefine as that
 * *   crashes uncatchably.
 * *
 * * $this is SssS_PHPinlineScriptRunner object if you omit $this-> and just
 * * type $a, it will be converted to $this->oDelegate->a (or if no delegate
 * * just $this->a). $$a becomes $$this->oDelegate->a resp $$this->a
 * *
 * * $_a stays $_a which unless it\'s a global (e.g. $_SERVER) it will not be
 * * accessable outside the snippet scope. To keep this speedy do not declare
 * * functions and classes or other loops (unless you prepend function vars with _)
 * * wherever the snippet contains $this, it is not touched same goes for $this->oDelegate
 * *
 * * @version 20100502_203256 + treatScript() $a -> $this->a conversions
 * * @version 20091104_143909 (CC) Luke JZ aka SwissalpS
 ';
		return $s;
	} // helpCLI

    function __toString() { return print_r($this, true); } // __toString

} // SssS_PHPinlineScriptRunner

/* * * *\ SssS_PHPinlineScriptRunner.inc (CC)2009 Luke JZ aka SwissalpS /* * * */
?>
