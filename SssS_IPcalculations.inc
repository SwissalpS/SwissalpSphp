<?php
/* * * *
 * * SssS_IPcalculations.inc
 * *
 * * possibly there are built-in functions for this, it's
 * * still a good reference I hope
 * *
 * * @version 20100512_030115 (CC) Luke JZ aka SwissalpS
 * * * */

if (!function_exists('ip42int')) {
function ip42int($sIPv4) {
	$a = explode('.', $sIPv4);

	if (4 != count($a)) return -1;

	return (int)$a[0] * 0x1000000
		 + (int)$a[1] * 0x10000
		 + (int)$a[2] * 256
		 + (int)$a[3];
} // ip42int
} // if function ip42int not yet defined

if (!function_exists('int2ip4')) {
function int2ip4($i) {
	$s = '';

	$iTmp = intval($i / 0x1000000); // pow(256, 3)
	$s .= $iTmp . '.';
	$i -= $iTmp * 0x1000000;

	$iTmp = intval($i / 0x10000); // pow(256, 2)
	$s .= $iTmp . '.';
	$i -= $iTmp * 0x10000;

	$iTmp = intval($i / 0x100); // 256
	$s .= $iTmp . '.';
	$i -= $iTmp * 256;

	$s .= $i;

	return $s;
} // int2ip4
} // if function int2ip4 not yet defined

/* test
echo int2ip4(ip42int('127.0.0.1')); */
/* * * *\ SssS_IPcalculations (CC) Luke JZ aka SwissalpS /* * * */
?>
