#!/usr/bin/php
<?php
/* * * *
 * * updatePlist1To2.phps
 * *
 * * @version 20110619_160243 (CC) Luke JZ aka SwissalpS
 * * * */
error_reporting(E_ALL); // 0); //

// add framework to include path
ini_set('include_path', '/usr/local/share/SwissalpS:' . ini_get('include_path'));
require_once('_synergize/PHOCOA/SssSphocoaGenerator.inc');

// run as CLI tool
SssSphocoaGenerator::runCLIupdatePlistVersion1To2();

/* * * *\ updatePlist1To2.phps (CC) Luke JZ aka SwissalpS /* * * */
?>
