<?php
/* * * *
 * * SssS_URLStretcher.inc
 * *
 * * @version 20110705_023747 (CC) Luke JZ aka SwissalpS
 * * * */
if (!defined('CRLF')) define('CRLF', "\r\n");

/*
TODO: clean up usage example:
http://www.postcourier.com.pg/images/Wkndxpix/Grass-Roots-20110414.gif
$aHops = array();
$sLongUrl = SssS_URLStretcher::longUrlFor('http://cot.ag/9WRLb7', $aHops) . chr(10);
//echo SssS_URLStretcher::longUrlFor('http://adage.com/smallagency/post?article_id=143272');
//echo SssS_URLStretcher::longUrlFor('http://adage.com/article/143272');
//echo SssS_URLStretcher::longUrlFor('http://adage.com/article/small-agency-diary/hell-a-creative-director-supposed/143272/');
var_dump($aHops);
echo $sLongUrl
*/ /*
$aHops = array();
echo SssS_URLStretcher::longUrlFor('http://www.postcourier.com.pg/20110704/grassruts.htm', $aHops) . chr(10);
var_dump($aHops);
*/
class SssS_URLStretcher {

	const ItIMEoUT = 20;

	const SuSERaGENT = 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.1.11) Gecko/20071127 Firefox/2.0.0.11';

	static function longUrlFor($sShortUrl, &$aHops = null, $sUserAgent = null, $iTimeOut = null) {

		if (!is_array($aHops)) $aHops = array();
		if (!$sUserAgent) $sUserAgent = self::SuSERaGENT;
		if (!$iTimeOut) $iTimeOut = self::ItIMEoUT;

		// this follows redirects, not what we want
		//return file_get_contents($sShortUrl);

		// uncomment to use curl method (comment to use next method)
		return self::longUrlForByMethodCurl($sShortUrl, $aHops, $sUserAgent, $iTimeOut);

		// curl method is mostly better... but if it's not on your server, try this
		return self::longUrlForByMethodFsock($sShortUrl, $aHops, $sUserAgent, $iTimeOut);

	} // longUrlFor


	static function longUrlForByMethodCurl($sShortUrl, &$aHops = null, $sUserAgent = null, $iTimeOut = null) {

		if (!is_array($aHops)) $aHops = array();
		if (!$sUserAgent) $sUserAgent = self::SuSERaGENT;
		if (!$iTimeOut) $iTimeOut = self::ItIMEoUT;

		$rC = curl_init();

		curl_setopt ($rC, CURLOPT_RETURNTRANSFER, true);
		curl_setopt ($rC, CURLOPT_URL, $sShortUrl);
		curl_setopt ($rC, CURLOPT_CONNECTTIMEOUT, $iTimeOut);
		curl_setopt ($rC, CURLOPT_USERAGENT, $sUserAgent);

		// Only calling the head
		curl_setopt ($rC, CURLOPT_HEADER, true); // header will be at output
		curl_setopt ($rC, CURLOPT_CUSTOMREQUEST, 'HEAD'); // HTTP request is 'HEAD'

		$sResponse = curl_exec($rC);

		$iResponseCode = curl_getInfo($rC, CURLINFO_HTTP_CODE);

		curl_close($rC);

		// split up lines being lax about syntax
		$aLines = explode("\r", $sResponse); // not '\r\n'
		if (2 > count($aLines)) $aLines = explode("\n", $sResponse);

		forEach ($aLines as $sLine) {

			$sLine = trim($sLine);

			if (empty($sLine)) continue;

			if ('Location:' == subStr($sLine, 0, 9)) {

				$aHops[] = array('url' => $sShortUrl, 'code' => $iResponseCode);

				$sLongUrl = trim(subStr($sLine, 9));

				// follow all redirects...
				return self::longUrlForByMethodCurl($sLongUrl, $aHops);

			} // if found location line

		} // loop each line

		$aHops[] = array('url' => $sShortUrl, 'code' => $iResponseCode);
		// if no redirect occured, we can return the given url
		return $sShortUrl;

	} // longUrlForByMethodCurl


	// this method is not so cool as we need to analyze the url and extract the domain
	// but some servers don't have curl installed
	static function longUrlForByMethodFsock($sShortUrl, &$aHops = null, $sUserAgent = null, $iTimeOut = null) {

		if (!is_array($aHops)) $aHops = array();

		if (!$sUserAgent) $sUserAgent = self::SuSERaGENT;

		if (!$iTimeOut) $iTimeOut = self::ItIMEoUT;

		$aUrl = parse_url($sShortUrl);

		//return print_r($aUrl, true);

		if (isset($aUrl['host'])) {

			// cool, this makes it easy
			$sHost = $aUrl['host'];

			$sPath = $aUrl['path'];

		} else {

			// more complicated, but we'll be cheap for now and cut the path
			$iPos = strPos($aUrl['path'], '/');

			// if exists and not first character
			if (false != $iPos) {

				$sHost = subStr($aUrl['path'], 0, $iPos);

				$sPath = subStr($aUrl['path'], $iPos);

			} else return 'could not determine host from: ' . $sShortUrl;

		} // if got host

		if (isset($aUrl['port'])) {

			$iPort = $aUrl['port'];

		} else {

			$iPort = 80;

		} // if got port

		// not actually used...
		if (isset($aUrl['scheme'])) {

			$sScheme = $aUrl['scheme'];

		} else {

			$sScheme = 'http';

		} // if got scheme
		$iErr = $sErr = null; //$sScheme . '://' .
		$rS = fSockOpen ($sHost, $iPort, $iErr, $sErr, $iTimeOut);
		if (!$rS) return 'could not connect to: ' . $sShortUrl . ' (' . $iErr . ') ' . $sErr;

		$sRequest = 'HEAD ' . $sPath . ' HTTP/1.1' . CRLF
			. 'Host: ' . $sHost . CRLF
			. 'Content-type: application/x-www-form-urlencoded' . CRLF
			. 'Cache-Control: no-cache' . CRLF
			. 'User-Agent: ' . $sUserAgent . CRLF
			. 'Connection: close' . CRLF
			. CRLF;

		fPuts($rS, $sRequest);

		$sResponse = '';
		while (!fEOF($rS)) $sResponse .= fGets($rS, 128);

		fClose($rS);

		// split up lines being lax about syntax
		$aLines = explode("\r", $sResponse); // not '\r\n'
		if (2 > count($aLines)) $aLines = explode("\n", $sResponse);

		forEach ($aLines as $sLine) {

			$sLine = trim($sLine);

			if (empty($sLine)) continue;

			if ('Location:' == subStr($sLine, 0, 9)) {

				$aHops[] = $sShortUrl;

				$sLongUrl = trim(subStr($sLine, 9));

				// follow all redirects...
				return self::longUrlForByMethodFsock($sLongUrl, $aHops, $sUserAgent);

			} // if found location line

		} // loop each line

		// if no redirect occured, we can return the given url
		return $sShortUrl;

	} // longUrlForByMethodFsock

} // SssS_URLStretcher

/* * * *\ SssS_URLStretcher (CC) Luke JZ aka SwissalpS /* * * */
?>
