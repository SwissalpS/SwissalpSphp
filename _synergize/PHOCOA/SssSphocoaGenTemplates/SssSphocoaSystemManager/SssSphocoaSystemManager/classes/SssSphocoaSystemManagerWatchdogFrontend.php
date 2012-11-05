<?php
/* * * *
 * * SssSphocoaSystemManagerWatchdogFrontend
 * *
 * * @version 20121027_205836 (CC) Luke JZ aka SwissalpS
 * * * */
class SssSphocoaSystemManagerWatchdogFrontend() {

	private $oPID;

	public function __construct() {

		$this->oPID = new SssS_PIDlock(RUNTIME_DIR, 'SssSphocoaSystemManagerWatchdogFrontend.pid.lock');

	} // __construct


	protected function isTaskClassPermitted($sClassName) {

		// TODO: check that class exists and is permitted (production and dev modes must differ)
		// for now we'll simply not allow if in production mode

		if (IS_PRODUCTION) return false;

		return true;

	} // isTaskClassPermitted


	static function log($sMessage) {

		WFLog::logToFile('SssSphocoaSystemManagerWatchdogFrontend.log', $sMessage);

	} // log

	
	static function main() {

		return self::sharedInstance()->run();

	} // main


	function run() {

		// there could be a running instance, we 'silently fail' in that case
		if ($this->oPID->isLocked()) {

			self::log('attempted to run but was locked');

			return false;

		} // if already running an instance

		// if run by incrond, we can take a shortcut
		if (1 < $argc) {
			
			self::log('running with arguments' . join(' <-> ', $argv));
			
			$sPathFile = $argv[1];

			$this->runTask($sPathFile);
			
		} else {
			
			self::log('running without arguments');
			
			// scan watch folder and sort by modification date
			$aTimes = $aNames = array();
			$sPoolDir = self::watchPath() . '/';
			if (is_dir($sPoolDir)) {
				if ($rDir = opendir($sPoolDir)) {
				
					while (false !== ($sFile = readdir($rDir))) {
						
						$sPathFile = $sPoolDir . $sFile;
						$aInfo = pathinfo($sPathFile);
						if (is_file($sPathFile) && ('plist' == $aInfo['extension'])) {
							
							$aTimes[]= filemtime($sPathFile);
							$aNames[]= $sPathFile;
													
						} // if is a file
				
					} // while loop
				
					closedir($rDir);
					
					// sort by date, oldest first
					array_multisort($aTimes, $aNames, SORT_ASC, SORT_NUMERIC);
					
					foreach($aNames as $sPathFile) {
						
						$this->runTask($sPathFile);
						
					} // loop all plist files in order of modification date
				
				} else {
					
					// failed to open
					self::log('failed to open path: ' . $sPoolDir);
							  
				} // if opened
				
			} else {
				
				// dir not found
				self::log('path does not exist: ' . $sPoolDir);
						  
			} // if is valid dir
			
		} // if path given as argument or not

		// work is done, unlock (and quit)
		$this->oPID->unLock();

		return true;

	} // run
	
	
	function runTask($sPathTaskPlist) {
		
		$oP = new SssS_Plist($sPathTaskPlist);
		$sClassName = $oP->getString('/phpClass', 'SssSphocoaSystemManagerTaskBase');
		
		if (!$this->isTaskClassPermitted($sClassName)) {

			$sFile = basename($sPathTaskPlist);
			$this->log('illegal class encountered: ' . $sClassName . ' in ' . $sFile);

			// 'move' the file to refused
			$oP->setString('/runErrors/0', 'illegal class');

			$oP->saveTo(self::watchPath() . '/rejects/' . $sFile);

			unlink($sPathTaskPlist);

			return false;

		} // if invalid class


		// instantiate the task and have it run
		$oTask = new $sClassName($oP);
		$aResult = $oTask->run();

		unlink($sPathTaskPlist);

		return true;
		
	} // runTask


	static function sharedInstance() {

		static $oSingleton = null;
		if (!$oSingleton) {

			$oSingleton = new SssSphocoaSystemManagerWatchdogFrontend();

		} // if first call

		return $oSingleton;

	} // sharedInstance


	static function watchPath() {

		return RUNTIME_DIR . '/eventsFrontend';

	} // watchPath

} // SssSphocoaSystemManagerWatchdogFrontend

/* * * *\ SssSphocoaSystemManagerWatchdogFrontend (CC) Luke JZ aka SwissalpS /* * * */
?>
