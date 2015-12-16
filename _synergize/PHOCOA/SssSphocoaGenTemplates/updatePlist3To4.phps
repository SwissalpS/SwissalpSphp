#!/usr/bin/php
<?php
/* * * *
 * * updatePlist3To4.phps
 * *
 * * @version 20151213_212206 (CC) Luke JZ aka SwissalpS
 * * * */
error_reporting(E_ALL); // 0); //

// add framework to include path
ini_set('include_path', '/usr/local/share/SwissalpS:' . ini_get('include_path'));
require_once('_synergize/PHOCOA/SssSphocoaGenerator.inc');

// run as CLI tool
SssSphocoaGenerator::runCLIupdatePlistVersion3To4();

/* * * *\ updatePlist3To4.phps (CC) Luke JZ aka SwissalpS /* * * */
?>
