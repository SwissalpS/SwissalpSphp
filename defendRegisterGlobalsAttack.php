<?php
/* * * *
 * * The_Header
 * *
 * * @version 20091026_233344 (CC) Luke JZ aka SwissalpS
 * * * */
//error_reporting(E_ALL); // 0); //

function defendRegisterGlobalsAttack() {
	// Prevent hacking attempts that make use of register_globals
	ini_set('register_globals', '0');
	foreach ($_GET as $key => $val) {
		if (isset($$key)) {
			unset($$key);
		}
	}
	foreach ($_POST as $key => $val) {
		if (isset($$key)) {
			unset($$key);
		}
	}
	foreach ($_COOKIE as $key => $val) {
		if (is_array($_COOKIE[$key])) {
			foreach($_COOKIE[$key] as $key2 => $val2) {
				if (isset($$key)) {
					unset($$key[$key2]);
				}
			}
		} else {
			if (isset($$key)) {
				unset($$key);
			}
		}
	}
} // defendRegisterGlobalsAttack

/* * * *\ THE_FOOTER (CC) Luke JZ aka SwissalpS /* * * */
?>
