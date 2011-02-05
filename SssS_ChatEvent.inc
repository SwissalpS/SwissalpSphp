<?php
/* * * *
 * * SssS_ChatEvent.inc
 * *
 * * @version 20091104_034927 (CC) Luke JZ aka SwissalpS
 * * * */
error_reporting(E_ALL); // 0); //
if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_Plist.inc');
} // if not autoincluding but on php's include path

class SssS_ChatEvent extends SssS_Plist {
	function SssS_ChatEvent($sMyJob = '', $iExpireUMT = -1, $iRepeatInterval = -1) {
		$this->SssS_Plist(array(
			'sMyJob'		=> $sMyJob, // eg '@room3 hahaha starting radio in 5
			'iExpireUMT'	=> $iExpireUMT, // trigger time
			'iRepeat'		=> $iRepeat));
	} // SssS_ChatEvent

	function isExpired() {
		$i = $this->get('iExpireUMT');
		// or same result
		$i = $this->a_plist['iExpireUMT'];
		if (
	} // isExpired
} // SssS_ChatEvent
/* * * *\ SssS_ChatEvent.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
