#!/usr/bin/php -q
<?php
/* * * *
 * * SwissalpS/bin/git/update.phps
 * *
 * * @version 20151228_203457 (CC) Luke JZ aka SwissalpS
 * * * */
error_reporting(E_ALL); // 0); //

require_once(dirname(dirname(dirname(__FILE__))) . DIRECTORY_SEPARATOR . 'SssS_bootstrap.inc');

SwissalpS\git\update\Fetcher::sharedInstance()->runCLI();

/* * * *\ SwissalpS/bin/git/update.phps (CC) Luke JZ aka SwissalpS /* * * */
?>
