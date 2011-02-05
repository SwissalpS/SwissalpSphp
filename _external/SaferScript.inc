<?php
/* * * *
 * * SaferScript.inc
 * * limit what calls to allow. Good for eval pre-parsing
 * *
 * * taken from a comment on PHP.org and modified to my likings
 * *
 * * @version 20100507_231934 (CC) Luke JZ aka SwissalpS
 * * * */
/*
udo dot schroeter at gmail dot com
26-May-2007 06:40
Safer Eval

eval() is used way to often. It slows down code, makes it harder to maintain and it created security risks. However, sometimes, I found myself wishing I could allow some user-controlled scripting in my software, without giving access to dangerous functions.

That's what the following class does: it uses PHP's tokenizer to parse a script, compares every function call against a list of allowed functions. Only if the script is "clean", it gets eval'd.
*//*
Usage example:

	$ls = new SaferScript('horribleCode();');
	$ls->allowHarmlessCalls();
	print_r($ls->parse());
	$ls->execute();


Of course it is not entirely safe, but it's a start ;-) */
class SaferScript {
	var $sSource, $aAllowedCalls, $aParseErrors;

	function SaferScript($sScriptText = '') {
		$this->sSource = $sScriptText;
		$this->aAllowedCalls = array();
	} // SaferScript

	function allowHarmlessCalls() {
		$this->aAllowedCalls = explode(',',
		'explode,implode,date,time,round,trunc,rand,ceil,floor,srand,'.
		'strtolower,strtoupper,substr,stristr,strpos,print,print_r');
	} // allowHarmlessCalls

	function parse() {
		$vcall = '';
		$this->aParseErrors = array();
		$tokens = token_get_all('<' . '?' . 'php '
				// 20100507_232825 SwissalpS added strtolower as the token is
				// later compared in a sensitive manner with array_search
				. strtolower($this->sSource) .' ?'.'>');

		foreach ($tokens as $token) {
			if (is_array($token)) {
				$id = $token[0];
				switch ($id) {
					case(T_VARIABLE): { $vcall .= 'v'; break; }
					case(T_STRING): { $vcall .= 's'; }
					case(T_REQUIRE_ONCE): case(T_REQUIRE): case(T_NEW):
					case(T_RETURN): case(T_BREAK): case(T_CATCH):
					case(T_CLONE): case(T_EXIT): case(T_PRINT): case(T_GLOBAL):
					case(T_ECHO): case(T_INCLUDE_ONCE): case(T_INCLUDE):
					case(T_EVAL): case(T_FUNCTION):

						$mRes = array_search($token[1], $this->aAllowedCalls);
						if ($mRes === false || $mRes === NULL) // v<4.2.0 - null
							$this->aParseErrors[] = 'illegal call: ' . $token[1];

				} // switch
			} // if array -> token longer than 1 char
			else
				$vcall .= $token;

		} // foreach

		if (stristr($vcall, 'v(') != '')
			$this->aParseErrors[] = array('illegal dynamic function call');

		return $this->aParseErrors;
	} // parse

	function execute($aParameters = array()) {
		foreach ($aParameters as $k => $v) $$k = $v;

		if (sizeof($this->aParseErrors) == 0)
			eval($this->sSource);
		else
			print('cannot execute, script contains errors');
	} // execute
} // SaferScript


/*consider merging with bits of this:
maurice at chandoo dot de
07-Nov-2008 11:20*/
/*
Note: Try to include this after all of your other self-defined functions and consider whether the blacklist is appropriate for your purpose

I wouldn't recommend this function if you're going to use eval extensively in your script. However, it's worth a try if you are going to put user input into eval
*/
class slowbutvigourousououosou_safeEval {
function slowbutvigourousououosou_safeEval () {}
function safe_eval($code,&$status) { //status 0=failed,1=all clear
	//Signs
	//Can't assign stuff
	$bl_signs = array("=");

	//Language constructs
	$bl_constructs = array("print","echo","require","include","if","else",
"while","for","switch","exit","break");

	//Functions
	$funcs = get_defined_functions();
	$funcs = array_merge($funcs['internal'],$funcs['user']);

	//Functions allowed
	//Math cant be evil, can it?
	$whitelist = array("pow","exp","abs","sin","cos","tan");

	//Remove whitelist elements
	foreach($whitelist as $f) {
		unset($funcs[array_search($f,$funcs)]);
	}
	//Append '(' to prevent confusion (e.g. array() and array_fill())
	foreach($funcs as $key => $val) {
		$funcs[$key] = $val."(";
	}
	$blacklist = array_merge($bl_signs,$bl_constructs,$funcs);

	//Check
	$status=1;
	foreach($blacklist as $nono) {
		if(strpos($code,$nono) !== false) {
			$status = 0;
			return 0;
		}
	}

	//Eval
	return @eval($code);
}
} //
/* * * *\ SaferScript.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
