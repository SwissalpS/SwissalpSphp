#!/usr/bin/php
<?php
/* * * *
 * * updatePlist2To3.phps
 * *
 * * @version 20131117_213418 (CC) Luke JZ aka SwissalpS
 * * * */
error_reporting(E_ALL); // 0); //

// add framework to include path
ini_set('include_path', '/usr/local/share/SwissalpS:' . ini_get('include_path'));
require_once('_synergize/PHOCOA/SssSphocoaGenerator.inc');

// run as CLI tool
SssSphocoaGenerator::runCLIupdatePlistVersion2To3();

/* * * *\ updatePlist2To3.phps (CC) Luke JZ aka SwissalpS /* * * */
?>
