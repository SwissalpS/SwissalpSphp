<?php
/* * * *
 * * SwissalpS/CLI/ChmodHelper.inc
 * * chmod -r with check for existance
 * *
 * * if file/folder does not exist, attempts to create them
 * * uses shells chmod command -> more flexible with mode argument
 * * than using the php chmod() function
 * * I use this class in staging scripts after update, to make sure
 * * all the permissions on the project files and folders are correct
 * *
 * * @version 20110711_035631 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\CLI;

class ChmodHelper {

	static function chmodDir($sPath, $sMode = 'a+rw', $bChmodContents = true) {

		if (!is_dir($sPath)) {

			// attempt to make it

			$sScript = 'mkdir -p "' . $sPath . '";';

			self::doScript($sScript);

		} // if dir does not exist

		$sScript = 'chmod ' . $sMode . ' "' . $sPath . '";';

		self::doScript($sScript);

		if (!$bChmodContents) {

			return;

		} // if not recursive

		if ($rD = openDir($sPath)) {

			while ($sFile = readDir($rD)) {

				if ('.' == $sFile{0}) continue; // skip invisible and other . entries

				if (is_dir($sPath . DIR_SEP . $sFile)) {

					self::chmodDir($sPath . DIR_SEP . $sFile, $sMode);

				} else {

					self::chmodFile($sPath . DIR_SEP . $sFile, $sMode);

				} // if dir or file

			} // loop each dir entry

		} // if able to open dir

	} // chmodDir


	static function chmodFile($sPath, $sMode = 'a+rw') {

		if (is_dir($sPath)) {

			return null;

		} // if dir given

		if (!is_file($sPath)) {

			// attempt to create file
			@file_put_contents($sPath, '');

		} // if file does not exist

		$sScript = 'chmod ' . $sMode . ' "' . $sPath . '";';

		return self::doScript($sScript);

	} // chmodFile


	static function doScript($sScript) {

		return self::sharedRunner()->doScript($sScript);

	} // doScript


	static function sharedRunner() {

		static $oRunner = null;

		if (!$oRunner) $oRunner = new ShellScriptRunner();

		return $oRunner;

	} // sharedRunner

} // SssS_ChmodHelper

/* * * *\ SwissalpS/CLI/ChmodHelper.inc (CC) Luke JZ aka SwissalpS /* * * */
