#!/usr/bin/php
<?php
/* * * *
 * * updateTemplates2To4.phps
 * * updateTemplates3To4.phps
 * *
 * * this is just an example of how to use this portion of generator
 * *
 * * keys changed between versions 1 and 2, then again between 3 and 4
 * *
 * * @version 20151213_213427 (CC) Luke JZ aka SwissalpS
 * * * */
error_reporting(E_ALL); // 0); //

// add framework to include path
ini_set('include_path', '/usr/local/share/SwissalpS:' . ini_get('include_path'));
require_once('_synergize/PHOCOA/SssSphocoaGenerator.inc');

// run as CLI tool
SssSphocoaGenerator::runCLIupdateTemplatesVersion2To4();

/* * * *\ updateTemplates2To4.phps (CC) Luke JZ aka SwissalpS /* * * */
?>
