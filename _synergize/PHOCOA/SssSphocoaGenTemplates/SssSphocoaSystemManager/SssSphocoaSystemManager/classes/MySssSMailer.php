<?php
/* * * *
 * * MySssSMailer.inc
 * *
 * * @version 20101129_123052 (CC) Luke JZ aka SwissalpS
 * * * */
class MySssSMailer extends SssSMailer {

	function __construct($sFromMeMail, $sFromMeName, $sLogFileName = null) {

		parent::__construct('example.com', 25, 'noreply+exaple.com', 'foopassword', false, $sFromMeMail, $sFromMeName, $sLogFileName);

	} // __construct

} // MySssSMailer
/* * * *\ MySssSMailer (CC) Luke JZ aka SwissalpS /* * * */
?>
