<?php
/* * * *
 * * SwissalpS/PHOCOA/Project/URLRewrite.inc
 * * handle all redirections on setups that don't allow configuration of httpd.conf and .htaccess rewrites don't allow other directories over htdocs
 * *
 * * @version 20110203_144000 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\Project;

/*
SssSURLRewrite::doIt(array(
	'logFile' => '/home/skyproco/phocoaApps/skyprom/log/404.log',
	'sIndexPHP' => '/home/skyproco/public_html/index.php',
	'sAppDir' => '/home/skyproco/phocoaApps/skyprom/skyprom',
	'sPhocoaDir' => '/usr/local/share', // defaults to /usr/local/share/phocoa
));
*/

// we need to define a time-zone to avoid flooding with warnings
date_default_timezone_set('UTC');

if (!defined('DIR_SEP')) define('DIR_SEP', DIRECTORY_SEPARATOR);

if (!function_exists('mime_content_type')) {
	function mime_content_type($sPath) {
		static $a = null;
		if (!$a) $a = array(
	  '.pdf'          =>      'application/pdf',
	  '.sig'          =>      'application/pgp-signature',
	  '.spl'          =>      'application/futuresplash',
	  '.class'        =>      'application/octet-stream',
	  '.ps'           =>      'application/postscript',
	  '.torrent'      =>      'application/x-bittorrent',
	  '.dvi'          =>      'application/x-dvi',
	  '.gz'           =>      'application/x-gzip',
	  '.pac'          =>      'application/x-ns-proxy-autoconfig',
	  '.swf'          =>      'application/x-shockwave-flash',
	  '.tar.gz'       =>      'application/x-tgz',
	  '.tgz'          =>      'application/x-tgz',
	  '.tar'          =>      'application/x-tar',
	  '.zip'          =>      'application/zip',
	  '.aif'		  =>	  'audio/x-aiff',
	  '.aiff'		  =>	  'audio/x-aiff',
	  '.mp3'          =>      'audio/mpeg',
	  '.m3u'          =>      'audio/x-mpegurl',
	  '.wma'          =>      'audio/x-ms-wma',
	  '.wax'          =>      'audio/x-ms-wax',
	  '.ogg'          =>      'application/ogg',
	  '.wav'          =>      'audio/x-wav',
	  '.gif'          =>      'image/gif',
	  '.jpg'          =>      'image/jpeg',
	  '.jpeg'         =>      'image/jpeg',
	  '.png'          =>      'image/png',
	  '.xbm'          =>      'image/x-xbitmap',
	  '.xpm'          =>      'image/x-xpixmap',
	  '.xwd'          =>      'image/x-xwindowdump',
	  '.css'          =>      'text/css',
	  '.html'         =>      'text/html',
	  '.htm'          =>      'text/html',
	  '.ico'		  =>	  'image/x-icon',
	  '.js'           =>      'text/javascript',
	  '.asc'          =>      'text/plain',
	  '.c'            =>      'text/plain',
	  '.cpp'          =>      'text/plain',
	  '.log'          =>      'text/plain',
	  '.conf'         =>      'text/plain',
	  '.text'         =>      'text/plain',
	  '.txt'          =>      'text/plain',
	  '.dtd'          =>      'text/xml',
	  '.xml'          =>      'text/xml',
	  '.mpeg'         =>      'video/mpeg',
	  '.mpg'          =>      'video/mpeg',
	  '.mov'          =>      'video/quicktime',
	  '.qt'           =>      'video/quicktime',
	  '.avi'          =>      'video/x-msvideo',
	  '.asf'          =>      'video/x-ms-asf',
	  '.asx'          =>      'video/x-ms-asf',
	  '.wmv'          =>      'video/x-ms-wmv',
	  '.bz2'          =>      'application/x-bzip',
	  '.tbz'          =>      'application/x-bzip-compressed-tar',
	  '.tar.bz2'      =>      'application/x-bzip-compressed-tar'/*,
	  # default mime type
	  ''              =>      'application/octet-stream'*/);

		$sLastSuffix = strtolower(strrchr($sPath, '.'));
		if ('.bz2' == $sLastSuffix || '.gz' == $sLastSuffix) {

			$s2ndLastSuffix = strtolower(strrchr(substr($sPath, 0, -strlen($sLastSuffix))), '.');
			if ('.tar' == $s2ndLastSuffix)
				return $a[$s2ndLastSuffix . $sLastSuffix];

			else
				return $a[$sLastSuffix];

		} elseif (isset($a[$sLastSuffix])) {
			return $a[$sLastSuffix];

		} else {
			return 'application/octet-stream';

		} // if found mime type by extension

	} // mime_content_type
} // if no function mime_content_type


class URLRewrite {

	static function doIt($aProps) {

		self::lg($aProps);

		//preDumpServer();

		// check if phocoa path is given
		if ( ! isset($aProps['sPhocoaDir']))
			$aProps['sPhocoaDir'] = '/usr/local/share/phocoa/';

		$bYouServe = self::respond(
							self::check($_SERVER['REQUEST_URI'],
										$aProps['sAppDir'],
										$aProps['sPhocoaDir']),
							$aProps['sIndexPHP'],
							$aProps['sPhocoaDir']
						);

		// if not run as root script, we don't want to kill the app using the filter
		if (!isset($aProps['sIndexPHP'])) {

			return $bYouServe;

		} // if no index.php defined

		exit(0);

	} // doIT


	static function preDumpServer() {
		echo '<pre>';
		var_dump($_SERVER);
		echo '</pre>';
	} // preDumpServer


	static function mime_content_type($sPath) {
		static $a = null;
		//http://www.w3schools.com/media/media_mimeref.asp
		if (!$a) $a = array(
	  '.pdf'          =>      'application/pdf',
	  '.sig'          =>      'application/pgp-signature',
	  '.spl'          =>      'application/futuresplash',
	  '.class'        =>      'application/octet-stream',
	  '.ps'           =>      'application/postscript',
	  '.torrent'      =>      'application/x-bittorrent',
	  '.dvi'          =>      'application/x-dvi',
	  '.gz'           =>      'application/x-gzip',
	  '.pac'          =>      'application/x-ns-proxy-autoconfig',
	  '.swf'          =>      'application/x-shockwave-flash',
	  '.tar.gz'       =>      'application/x-tgz',
	  '.tgz'          =>      'application/x-tgz',
	  '.tar'          =>      'application/x-tar',
	  '.zip'          =>      'application/zip',
	  '.aif'		  =>	  'audio/x-aiff',
	  '.aiff'		  =>	  'audio/x-aiff',
	  '.mp3'          =>      'audio/mpeg',
	  '.m3u'          =>      'audio/x-mpegurl',
	  '.wma'          =>      'audio/x-ms-wma',
	  '.wax'          =>      'audio/x-ms-wax',
	  '.ogg'          =>      'application/ogg',
	  '.wav'          =>      'audio/x-wav',
	  '.gif'          =>      'image/gif',
	  '.jpg'          =>      'image/jpeg',
	  '.jpeg'         =>      'image/jpeg',
	  '.png'          =>      'image/png',
	  '.xbm'          =>      'image/x-xbitmap',
	  '.xpm'          =>      'image/x-xpixmap',
	  '.xwd'          =>      'image/x-xwindowdump',
	  '.css'          =>      'text/css',
	  '.html'         =>      'text/html',
	  '.htm'          =>      'text/html',
	  '.ico'		  =>	  'image/x-icon',
	  '.js'           =>      'text/javascript',
	  '.asc'          =>      'text/plain',
	  '.c'            =>      'text/plain',
	  '.cpp'          =>      'text/plain',
	  '.log'          =>      'text/plain',
	  '.conf'         =>      'text/plain',
	  '.text'         =>      'text/plain',
	  '.txt'          =>      'text/plain',
	  '.dtd'          =>      'text/xml',
	  '.xml'          =>      'text/xml',
	  '.mpeg'         =>      'video/mpeg',
	  '.mpg'          =>      'video/mpeg',
	  '.mov'          =>      'video/quicktime',
	  '.qt'           =>      'video/quicktime',
	  '.avi'          =>      'video/x-msvideo',
	  '.asf'          =>      'video/x-ms-asf',
	  '.asx'          =>      'video/x-ms-asf',
	  '.wmv'          =>      'video/x-ms-wmv',
	  '.bz2'          =>      'application/x-bzip',
	  '.tbz'          =>      'application/x-bzip-compressed-tar',
	  '.tar.bz2'      =>      'application/x-bzip-compressed-tar'/*,
	  # default mime type
	  ''              =>      'application/octet-stream'*/);

		$sLastSuffix = strtolower(strrchr($sPath, '.'));
		if ('.bz2' == $sLastSuffix || '.gz' == $sLastSuffix) {

			$s2ndLastSuffix = strtolower(strrchr(substr($sPath, 0, -strlen($sLastSuffix)), '.'));
			if ('.tar' == $s2ndLastSuffix)
				return $a[$s2ndLastSuffix . $sLastSuffix];

			else
				return $a[$sLastSuffix];

		} elseif (isset($a[$sLastSuffix])) {
			return $a[$sLastSuffix];

		} else {
			return 'application/octet-stream';

		} // if found mime type by extension

	} // mime_content_type


	static function check($sRequestURI, $sAppDir, $sPhocoaFrameWorkDir = '/usr/local/share/phocoa') {

		// remove any trailing slashes and add one to make sure we have a path at all
		$sPhocoaFrameWorkDir = rtrim($sPhocoaFrameWorkDir, DIR_SEP) . DIR_SEP;
		// same goes for app dir
		$sAppDir = rtrim($sAppDir, DIR_SEP) . DIR_SEP;

		static $aPatterns = null;
		if (!$aPatterns) {
			$aPatterns = array(
				'favic' => '|^/favicon.ico|i',
				'menu1' => '|^/menu/menu/mainMenu/menubaritem_submenuindicator.png|i',
				'wwwf1' => '|^/www/framework(/[0-9\.]*)?/?(.*)|i',
				//'wwwf3' => '|^/www/framework/FCKEditor/(.*)|i',
				'wwwf4' => '|^/www/framework/(.*)|i',
				'wwwf2' => '|^/www/?(.*)|i',
				'skin1' => '|^/skins/([^/]*)/www/(.*)|i',
				'skin2' => '|^/skins/([^/]*)/([^/]*)/([^/]*)/(.*)|i',
				'docs1' => '|^/docs/?(.*)|i'
				//'asset' => '|^/assets/?(.*)|i',
			);
		} // first run

		$sPath = null;

		foreach ($aPatterns as $sPatID => $sPattern) {

			$aMatches = array();
			$mResult = preg_match($sPattern, $sRequestURI, $aMatches);

			//var_dump($sPattern, $mResult, $aMatches, '-----------');

			if (false === $mResult) {
				// error
				self::lg('
				error with match
	');

			} elseif (0 < $mResult) {
				// found matches
				self::lg('found match: ' . $sPatID);

				switch ($sPatID) {

					case 'favic' : // ^/favicon.ico /SwissalpS/phocoaApps/SssSAppPIiCal/SssSAppPIiCal/wwwroot/
						$sPath = sprintf('%1$swwwroot%2$sfavicon.ico', $sAppDir, DIR_SEP);
						break;

					case 'wwwf1' : //  ^/www/framework(/[0-9\.]*)?/?(.*) /private/var/mobile/SwissalpS/git/phocoa/phocoa/wwwroot/www/framework/$2
						$sPath = sprintf(
							'%1$sphocoa%2$swwwroot%2$swww%2$sframework%2$s%3$s',
							$sPhocoaFrameWorkDir, DIR_SEP, $aMatches[2]);
						break;
	/*
					case 'wwwf3' : // ^/www/framework/FCKEditor/(.*) /private/var/mobile/SwissalpS/git/phocoa/phocoa/wwwroot/www/framework/FCKEditor/$1
						$sPath = $sAppDir . DIR_SEP . 'wwwroot' . DIR_SEP
								. 'www' . DIR_SEP . 'FCKEditor' . DIR_SEP . $aMatches[1];
						break;
	*/
					case 'wwwf4' : // ^/www/framework/(.*) /private/var/mobile/SwissalpS/git/phocoa/phocoa/wwwroot/www/framework/$1
						$sPath = sprintf(
							'%1$sphocoa%2$swwwroot%2$swww%2$sframework%2$s%3$s',
							$sPhocoaFrameWorkDir, DIR_SEP, $aMatches[1]);
						break;

					case 'wwwf2' :
						// ^/www/?(.*) /SwissalpS/phocoaApps/SssSAppPIiCal/SssSAppPIiCal/wwwroot/www/$1
						$sPath = $sAppDir . 'wwwroot' . DIR_SEP
								. 'www' . DIR_SEP . $aMatches[1];
						break;

					case 'skin1' : // ^/skins/([^/]*)/www/(.*) /SwissalpS/phocoaApps/SssSAppPIiCal/SssSAppPIiCal/skins/$1/www/$2
						$sPath = $sAppDir . 'skins' . DIR_SEP
									. $aMatches[1] . DIR_SEP . 'www' . DIR_SEP
									. $aMatches[2];
						break;

					case 'skin2' : // ^/skins/([^/]*)/([^/]*)/([^/]*)/(.*) /SwissalpS/phocoaApps/SssSAppPIiCal/SssSAppPIiCal/skins/$1/$2/www/$3/$4
							$sPath = $sAppDir . 'skins' . DIR_SEP
									. $aMatches[1] . DIR_SEP . $aMatches[2]
									. DIR_SEP . 'www' . DIR_SEP . $aMatches[3]
									. DIR_SEP . $aMatches[4];
						break;

					case 'docs1' : // ^/docs/?(.*) /private/var/mobile/SwissalpS/git/phocoa/phocoa/docs/phpdocs/$1
						$sPath = sprintf(
								'%1$sphocoa%2$sdocs%2$sphpdocs%2$s%3$s',
								$sPhocoaFrameWorkDir, DIR_SEP, $aMatches[1]);
						break;
					/*
					case 'asset' : //
						$sPath = sprintf('%1$s%2$s', $sPhocoaFrameWorkDir, DIR_SEP);
	*/

					case 'menu1' : // /menu/menu/mainMenu/menubaritem_submenuindicator.png /private/var/mobile/SwissalpS/git/phocoa/phocoa/wwwroot/www/framework/yui/menu/assets/menubaritem_submenuindicator.png
						$sPath = sprintf(
							'%1$sphocoa%2$swwwroot%2$swww%2$sframework%2$syui%2$smenu%2$sassets%2$smenubaritem_submenuindicator.png',
							$sPhocoaFrameWorkDir, DIR_SEP);
						break;

				} // switch pattern id

				// match found break out of loop
				break;

			} else {
				// no match
				//echo chr(10) . 'NO match' . chr(10);

			} // if matched anything

		} // foreach pattern

		if (null === $sPath) {

			self::lg('no match found');

		} // if no match found

		return $sPath;

	} // check


	static function lg($a) {
		if (defined('IS_PRODUCTION') && true == IS_PRODUCTION) return;

		static $sLogFile = null;
		static $nl = null;
		if (!$sLogFile && isset($a['logFile'])) {
			$sLogFile = $a['logFile'];
		} // if not yet set but given logFile
		if (!$nl) {
			$nl = chr(10);
		} // if first call

		$sLine = (empty($a)) ? $nl
				: date('Ymd_His') . ' ' . $_SERVER['REQUEST_URI'] . ' '
				. (isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : 'no HTTP_REFERER') . $nl . print_r($a, true) . $nl;

		$mRes = @file_put_contents($sLogFile, $sLine, FILE_APPEND);

		if (false === $mRes) echo '<!-- -problem writing to 404.log- ' . $sLine . ' ->';

	} // lg


	static function respond($sPath, $sIndexPHP, $sPhocoaFrameWorkDir = '/usr/local/share/phocoa') {

		$sOrigPath = $sPath;

		$mPos = strpos($sPath, '?');

		if (false !== $mPos) $sPath = substr($sPath, 0, $mPos);

		static $nl = null; if ( ! $nl) $nl = chr(10);

		if (null !== $sPath) {
			// serve up or redirect
			//echo $nl . 'should now serve up or redirect to: ' . $sPath;

			if (is_dir($sPath)) {

				$sHeader = 'HTTP/1.0 500 Go Away';

				self::lg('is dir: ' . $sPath . $nl . 'returning: ' . $sHeader);

				//header($sHeader);

				$sHeaderFull = $sHeader . "\r\n\r\n";

				print $sHeaderFull;

				echo 'dirlisting disabled';

			} elseif (is_file($sPath)) {

				$sMime = self::mime_content_type($sPath);

				$sHeader = 'Content-type: ' . $sMime;

				//$sHeader .= 'Content-Length: ' . fileSize($sPath);

				self::lg('1) is file: ' . $sPath . $nl . 'returning: ' . $sHeader);

				//var_dump('mime:', $sMime);

				//header($sHeader);

				$sHeaderFull = $sHeader . "\r\n\r\n";

				print $sHeaderFull;

				readfile($sPath);
				//print file_get_contents($sPath);

			} else {

				/// try include index.php
				//return self::ii($sIndexPHP);

				$sHeader = 'HTTP/1.0 404 Not Found';

				self::lg('is NEITHER file nor folder: ' . $sPath . $nl . 'returning: ' . $sHeader);

				//header($sHeader);
				$sHeaderFull = $sHeader . "\r\n\r\n";

				print $sHeaderFull;

			} // if path is dir or file or neither

			return false; // -> already served, caller doesn't need to

		} else {
			// no match at all
			$sRequestURI = $_SERVER['REQUEST_URI'];

			if (DIR_SEP == $sRequestURI) return self::ii($sIndexPHP);

			self::lg(' no match found trying yui framework');

			// remove any trailing slashes and add one
			$sPhocoaFrameWorkDir = rtrim($sPhocoaFrameWorkDir, DIR_SEP) . DIR_SEP;

			$sPath = $sPhocoaFrameWorkDir . 'phocoa/wwwroot/www/framework/yui' . $sRequestURI;

			if (is_dir($sPath)) {

				$sHeader = 'HTTP/1.0 500 Go Away';

				self::lg('is dir: ' . $sPath . $nl . 'returning: ' . $sHeader);

				//header($sHeader);

				$sHeaderFull = $sHeader . "\r\n\r\n";

				print $sHeaderFull;

				self::lg('dirlisting disabled');

				return false;

			} elseif (is_file($sPath)) {

				$sMime = self::mime_content_type($sPath);

				$sHeader = 'Content-type: ' . $sMime;

				self::lg('2) is file: ' . $sPath . $nl . 'returning: ' . $sHeader);

				//header($sHeader);

				$sHeaderFull = $sHeader . "\r\n\r\n";

				print $sHeaderFull;

				readfile($sPath);

				return false;

			} else {

			/// try include index.php
				return self::ii($sIndexPHP);

			} // if dir, file or other

		} // if found match

	} // respond


	static function ii($sIndexPHP = null) {

		self::lg('including: ' . basename($sIndexPHP));

		// if not run as root script, we don't want to kill the app using the filter
		if (null == $sIndexPHP) return true;

		include($sIndexPHP);

		exit(0);

	} // ii

} // URLRewrite

/* * * *\ SwissalpS/PHOCOA/Project/URLRewrite.inc (CC) Luke JZ aka SwissalpS /* * * */
