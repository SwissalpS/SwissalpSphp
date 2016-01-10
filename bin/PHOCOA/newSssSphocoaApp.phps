#!/usr/bin/env php
<?php
/* * * *
 * * newSssSphocoaApp.phps
 * * startup SssSphocoaGenerator as cli app to make new phocoa project
 * * or replicate the current one
 * *
 * * syntax for SssSphocoaGenerator::runCLI()
		newSssSphocoaProject [[args] <plist> [[args] <plist>] ...]]
	the following arguments are valid:
		--help	shows this text. no other arguments are parsed when this
				argument is present.

		--target <destination_dir>	ignore the target in plist and use this

		--appName <appName>	override plist appName

		--noInteraction	use all defaults, don\'t ask for confirmation

		--withInteraction (default) prompt for each value

		Note: arguments are case-insensitive.

		<plist> is the path to the plist with values to use as default.
		If no plist is given, built in default values will be offered.

 * * developed and tested on a mac w OS X 10.5.x, also Fedora and Raspbian
 * *
 * * @version 20151229_214554 (CC) Luke JZ aka SwissalpS
 * * @version 20100504_151015 (CC) Luke JZ aka SwissalpS
 * * * */
error_reporting(E_ALL); // 0); //

// bootstrap
require_once(dirname(dirname(dirname(__FILE__))) . DIRECTORY_SEPARATOR . 'SssS_bootstrap.inc');

// run as CLI tool
\SwissalpS\PHOCOA\Project\Generator::runCLI();

/* * * *\ newSssSphocoaApp.phps (CC) Luke JZ aka SwissalpS /* * * */
?>
