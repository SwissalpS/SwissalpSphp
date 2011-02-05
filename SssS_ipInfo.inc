<?php
/* * * *
 * * SssS_ipInfo
 * * !!!!!!!!!depricated!!!!!!! use SssS_Xtend.inc
 * * version 20091011_170801 (CC) Luke JZ aka SwissalpS
 * * * */
// If you use this service on a regular basis, please consider making a donation. Our API are totally free but we need donations in order to pay for hosting and dedicated servers.

/*
$o = new SssS_ipInfo('83.76.39.77');
//$a = $o->geoLocIPinfoDBcountry();
// or
//$a = $o->geoLocIPinfoDBcity();
var_dump($a);
*/

if (!defined('AUTO_INCLUDING')) {
	require_once('SssS_Plist.inc');
} // if not autoincluding but on php's include path

class SssS_ipInfo extends SssS_Plist {
	/**
	 * SssS_ipInfo($m_plist = '', $s_charset = '', $s_lang = 'en', $s_XMLversion = '1.0')
	 * Function_description_here
	 *
	 * @param mixed $( parameter_description_here
	 *
	 * @return void return_description_here
	 *
	 *
	 * @uses something - uses_desciption_here
	 *
	 * @version 20091011_153012 (CC)2007 Luke JZ aka SwissalpS
	 */
	function SssS_ipInfo($s_ip = '', $s_charset = '', $s_lang = 'en', $s_XMLversion = '1.0') {
		if ($this->isIP($s_ip)) {
			$this->s_ip = $s_ip;
		} else {
			$this->getRemIP();
		} // if valid ip passed use it else take the remote ip from browser header

		// the plist parser downcases tags
		$this->a_GeoLocResponse = array(
				'ip' => $this->s_ip,
				'status' => 'n.a.',
				'countrycode' => 'n.a.',
				'countryname' => 'n.a.',
				'regioncode' => 'n.a.',
				'regionname' => 'n.a.',
				'city' => 'n.a.',
				'zippostalcode' => 'n.a.',
				'latitude' => 'n.a.',
				'longitude' => 'n.a.',
				'timezone' => 'n.a.',
				'gmtoffset' => 'n.a.',
				'dstoffset' => 'n.a.'
			);
	} // SssS_ipInfo

	/**
	 * isIP($sIP = '')
	 * is the given string a valid ip
	 *
	 * @param string $sIP ip to check for validity (defaults to: '')
	 *
	 * @return bool false if not IP
	 *
	 * @version 20091011_145745 (CC)2007 Luke JZ aka SwissalpS
	 */
	function isIP($sIP = '') {
		if ($sIP && '' != trim($sIP)) {
			if (preg_match('/^(?:25[0-5]|2[0-4]\d|1\d\d|[1-9]\d|\d)(?:[.](?:25[0-5]|2[0-4]\d|1\d\d|[1-9]\d|\d)){3}$/', $sIP)) return true;
		} // not null, false or MT

		return false;
	} // isIP

	/**
	 * parseGeoLocIPinfoDBReply($sXML = '')
	 * parse the xml from the db query and return an array
	 *
	 * @param string $sXML the result of http://ipinfodb.com/ip_query.php or ...ip_query_country.php (defaults to: '')
	 *
	 * @return array a_GeoLocResponse
	 *
	 *
	 * @uses something - uses_desciption_here
	 *
	 * @version 20091011_160554 (CC)2007 Luke JZ aka SwissalpS
	 */
	function parseGeoLocIPinfoDBReply($sXML = '') {
		$this->SssS_Plist($sXML, $s_charset = '', $s_lang = 'en', $s_XMLversion);
		// the plist parser downcases tags
		if ('response' == $this->a_tags[0]['tag']) {
			$aChildrenIDs = $this->a_tags[0]['children'];
			// extract the returned data
			$aResponses = array();
			foreach($aChildrenIDs as $iChild) {
				$aResponses[$this->a_tags[$iChild]['tag']] = $this->a_tags[$this->a_tags[$iChild]['children'][0]]['contents'];
			} // foreach child

			// clean the data and add to self
			foreach($aResponses as $sKey => $mValue) {
				$this->a_GeoLocResponse[$sKey] = (null == $mValue) ? 'n.a' : $mValue;
			} // foreach response - clean

			// return
			//return $this->a_GeoLocResponse; // $aChildrenIDs; // $this->a_tags; //
		} // if reply has response base tag

		return $this->a_GeoLocResponse;
	} // parseGeoLocIPinfoDBReply

	/**
	 * geoLocIPinfoDBcity($sIP = '')
	 * For city precision, simply do a query at this address
	 * http://ipinfodb.com/ip_query.php?ip=74.125.45.100
	 * (if you omit the IP parameter it will check with your own IP ie the servers ip)
	 *
	 * @param string $sIP ip to lookup
	 *
	 * @return array hash containing the results from ipinfodb.com
	 *
	 *
	 * @uses something - uses_desciption_here
	 *
	 * @version 20091011_143220 (CC)2007 Luke JZ aka SwissalpS
	 */
	function geoLocIPinfoDBcity($sIP = '') {
		$sURL = 'http://ipinfodb.com/ip_query.php' . ( $this->isIP($sIP) ? '?ip=' . $sIP : '');
		$sReply = @file_get_contents_safely($sURL);
		return $this->parseGeoLocIPinfoDBReply($sReply);
	} // geoLocIPinfoDBcity

	/**
	 * geoLocIPinfoDBcountry($sIP)
	 * For country precision (faster), simply do a query at this address
	 * http://ipinfodb.com/ip_query_country.php?ip=74.125.45.100
	 * (if you omit the IP parameter it will check with your own IP ie the servers ip)
	 *
	 * @param string $sIP ip to lookup
	 *
	 * @return array hash containing the results from ipinfodb.com
	 *
	 *
	 * @uses something - uses_desciption_here
	 *
	 * @version 20091011_143255 (CC)2007 Luke JZ aka SwissalpS
	 */
	function geoLocIPinfoDBcountry($sIP = '') {
		$sURL = 'http://ipinfodb.com/ip_query_country.php' . ( $this->isIP($sIP) ? '?ip=' . $sIP : '');
		$sReply = @file_get_contents_safely($sURL);
		return $this->parseGeoLocIPinfoDBReply($sReply);
	} // geoLocIPinfoDBcountry
} // SssS_ipInfo
/* * * *\ SssS_ipInfo.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
