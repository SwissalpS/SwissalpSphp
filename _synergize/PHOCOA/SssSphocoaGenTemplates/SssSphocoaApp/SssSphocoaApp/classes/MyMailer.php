<?php
/* * * *
 * * MyMailer.inc
 * *
 * * @version 20160410_115052 (CC) Luke JZ aka SwissalpS
 * * @version 20101129_123052 (CC) Luke JZ aka SwissalpS
 * * * */
use SwissalpS\PHOCOA\Communication\Mailer;

class MyMailer extends Mailer {

	function __construct($sFromMeMail, $sFromMeName, $sLogFileName = null) {

		// modify these to connect to your mailserver
		parent::__construct(
							// SMTP server string aka hostname
							'example.com',
							// port
							25,
							// username string mostly senders email address
							// some servers may require to replace '@' with '+'
							'noreply+exaple.com',
							// password
							'foopassword',
							// use SSL?
							false,
							// from email respond to email
							$sFromMeMail,
							// sender name
							$sFromMeName,
							// optional, defaults to 'Mailer.log'
							$sLogFileName);

	} // __construct

} // MyMailer
/* * * *\ MyMailer (CC) Luke JZ aka SwissalpS /* * * */
