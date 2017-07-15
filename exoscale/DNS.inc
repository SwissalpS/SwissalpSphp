<?php
/* * * *
 * * SwissalpS/exoscale/DNS.inc
 * *
 * * exoscale.ch DNS API class
 * *
 * * @version 20170714_142724 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\exoscale;



class DNS {

	// the base URL for all requests to exoscale.ch
	const API_URL = "https://api.exoscale.ch/dns/v1/domains";

	// key and secret used to authenticate with exoscale.ch
	// combination of API-key and secret
	var $sAPItoken = "";

	// curl object handle
	var $oC;

	// array of extra headers
	var $aHeaders = null;

	var $bInit = false;
	var $iError = 0;
	var $aInfo = null;
	var $iHTTPcode = 0;
	var $mRes = null;

	/**
	 * construct a DNS object giving the exoscale key and secret
	 *
	 * @param String $sKey API-Key
	 * @param String $sSecret API-Secret
	 *
	 * @version 20170715_183057 (CC) Luke JZ aka SwissalpS
	 */
	public function __construct(String $sKey, String $sSecret) {

		$this->sAPItoken = $sKey . ':' . $sSecret;

		$this->oC = false;

		$this->init();

	} // __construct


	/**
	 * __destruct
	 */
	function __destruct() {

		$this->closeCurl();

	} // __destruct


	/**
	 * close curl instance
	 */
	function closeCurl() {

		if (false !== $this->oC) {

			curl_close($this->oC);

			$this->oC = false;

		} // if still open

		$this->bInit = false;

	} // closeCurl


	/**
	 * delete a domain from zone
	 * e.g. example.com, foo => delete the entry for foo.example.com
	 *
	 * @param $sZone, $sDomain
	 *
	 * @return null or false on error, true on success or if domain isn't registered
	 */
	function deleteDomain(String $sZone, String $sDomain) {

		if (!$this->bInit) return null;

		$aDomain = $this->getDomain($sZone, $sDomain);

		if (null === $aDomain) return true;

		$this->init();

		return $this->deleteRecord($sZone, $aDomain['id']);

	} // deleteDomain


	/**
	 * delete a record from zone
	 *
	 * @param $sZone, $iRecord
	 *
	 * @return null, false or true
	 */
	function deleteRecord(String $sZone, $iRecord) {

		if (!$this->bInit) return null;

		$sURL = '' . self::API_URL . '/' . $sZone . '/records/' . $iRecord;

		$mRes = $this->executeCurlDELETE($sURL);

		if (null === $mRes) return false;

		return true;

	} // deleteRecord


	/**
	 * execute curl to given URL
	 *
	 * @param $sURL
	 */
	protected function executeCurl(String $sURL) {

		$this->iError = 0;
		$this->aInfo = array();
		$this->mRes = false;

		if (!$this->bInit) return null;

		if (!curl_setopt($this->oC, CURLOPT_URL, $sURL)) return null;

		if (!curl_setopt($this->oC, CURLOPT_HTTPHEADER, $this->aHeaders)) return null;

		$this->mRes = curl_exec($this->oC);

		$this->iError = curl_errno($this->oC);

		$this->iHTTPcode = curl_getinfo($this->oC, CURLINFO_HTTP_CODE);

		$this->closeCurl();

	} // executeCurl

	/**
	 * execute HTTP DELETE request at given URL
	 *
	 * @param String sURL
	 *
	 * @return null or an empty array
	 */
	function executeCurlDELETE(String $sURL) {

		if (!curl_setopt($this->oC, CURLOPT_CUSTOMREQUEST, 'DELETE')) return null;

		$this->executeCurl($sURL);

		if (0 !== $this->iError) return null;

		if (200 !== $this->iHTTPcode) return null;

		return json_decode($this->mRes, true);

	} // executeCurlDELETE


	/**
	 * execute HTTP GET request to given URL
	 *
	 * @param String sURL
	 *
	 * @return null or the response array
	 *
	 * @version 20170715_190835 (CC) Luke JZ aka SwissalpS
	 */
	function executeCurlGET(String $sURL) {

		$this->executeCurl($sURL);

		if (false === $this->mRes) return null;

		if (0 !== $this->iError) return null;

		if (200 !== $this->iHTTPcode) return null;

		return json_decode($this->mRes, true);

	} // executeCurlGET


	/**
	 * execute HTTP POST request to given URL
	 *
	 * @param String sURL
	 * @param String JSON request as string
	 *
	 * @return null or the response array
	 *
	 * @version 20170715_191038 (CC) Luke JZ aka SwissalpS
	 */
	function executeCurlPOST(String $sURL, String $sJSON) {

		if (!curl_setopt($this->oC, CURLOPT_POST, true)) return null;

		$this->aHeaders[] = 'Content-Type: application/json';

		if (!curl_setopt($this->oC, CURLOPT_POSTFIELDS, $sJSON)) return null;

		$this->executeCurl($sURL);

		if (0 !== $this->iError) return null;

		// sometimes it's 201 and that's OK
		//if (200 !== $this->iHTTPcode) return null;

		return json_decode($this->mRes, true);

	} // executeCurlPOST


	/**
	 * get entry data for domain of zone
	 *
	 * @param String sZone
	 * @param String sDomain
	 *
	 * @return null or the response array
	 *
	 * @version 20170715_191316 (CC) Luke JZ aka SwissalpS
	 */
	function getDomain(String $sZone, String $sDomain) {

		$aRecords = $this->getRecords($sZone);
		if (null === $aRecords) return null;

		$sDomain = strtolower($sDomain);

		foreach($aRecords as $s => $aRecord) {

			if ($aRecord['record']['name'] == $sDomain) return $aRecord['record'];

		} // loop each record extracting domain

		return null;

	} // getDomain


	/**
	 * get records of all domains in zone
	 *
	 * @param String sZone
	 *
	 * @return null or the response array
	 *
	 * @version 20170715_191316 (CC) Luke JZ aka SwissalpS
	 */
	function getDomains(String $sZone, bool $bIncludeMT = false) {

		$iCountEmpty = 0;

		$aRecords = $this->getRecords($sZone);
		if (null === $aRecords) return null;

		$aDomains = array();

		foreach($aRecords as $s => $aRecord) {

			$sDomain = $aRecord['record']['name'];
			if (0 == strlen($sDomain)) {
				if ($bIncludeMT) {
					$sDomain = 'SssSemptyDomain_' . $iCountEmpty++;
				} else {
					$iCountEmpty++;
					continue;
				} // if include empty domains
			} // if empty domain

			$aDomains[$sDomain] = $aRecord['record'];

		} // loop each record extracting domain

		return $aDomains;

	} // getDomains


	/**
	 * get IP of domain in zone
	 * @param $sZone, $sDomain
	 *
	 * @return null or IP as string
	 */
	function getIPofDomain(String $sZone, String $sDomain) {

		$aDomain = $this->getDomain($sZone, $sDomain);
		if (null === $aDomain) return null;

		return $aDomain['content'];

	} // getIPofDomain


	/**
	 * get a record for a domain by it's id
	 *
	 * @param String sZone
	 * @param int record id
	 *
	 * @return null or array
	 *
	 * @version 20170715_192232 (CC) Luke JZ aka SwissalpS
	 */
	function getRecord(String $sZone, $iRecord) {

		if (!$this->bInit) return null;

		$sURL = '' . self::API_URL . '/' . $sZone . '/records/' . $iRecord;

		return $this->executeCurlGET($sURL);

	} // getRecord


	/**
	 * get records for zone
	 *
	 * @param String sZone
	 *
	 * @return null or array
	 *
	 * @version 20170715_192709 (CC) Luke JZ aka SwissalpS
	 */
	function getRecords(String $sZone) {

		if (!$this->bInit) return null;

		$sZone = strtolower($sZone);

		$sURL = '' . self::API_URL . '/' . $sZone . '/records';

		return $this->executeCurlGET($sURL);

	} // getRecords


	/**
	 * get zone information
	 *
	 * @param String sZone
	 *
	 * @return null or array
	 *
	 * @version 20170715_192751 (CC) Luke JZ aka SwissalpS
	 */
	function getZone(String $sZone) {

		if (!$this->bInit) return null;

		$sURL = '' . self::API_URL . '/' . $sZone;

		return $this->executeCurlGET($sURL);

	} // getZone


	/**
	 * get all zones
	 *
	 * @return null or array
	 */
	function getZones() {

		if (!$this->bInit) return null;

		$sURL = '' . self::API_URL . '';

		return $this->executeCurlGET($sURL);

	} //getZones


	/**
	 * init the object for next use
	 */
	function init() {

		$this->closeCurl();

		$this->oC = curl_init();

		if (false === $this->oC) return;

		$this->aHeaders = array(
						'X-DNS-Token: ' . $this->sAPItoken,
						'Accept: application/json',
						);

	   if (!curl_setopt($this->oC, CURLOPT_RETURNTRANSFER, TRUE)) return;

	   $this->bInit = true;

	} // init


	/**
	 * alias to init()
	 */
	function reset() {

		$this->init();

	} // reset


	/**
	 * set IP of domain in zone
	 *
	 * @param String sZone
	 * @param String sDomain
	 * @param String sIP
	 * @param int ttl time-to-live (optional)
	 * @param int prio priority (optional, not aplicable)
	 *
	 * @return null or a result array
	 *
	 * @version 20170715_193746 (CC) Luke JZ aka SwissalpS
	 */
	function setIPofDomain(String $sZone, String $sDomain, String $sIP, $iTTL = null, $iPrio = null) {

		if (!$this->bInit) return null;

		$aDomain = $this->getDomain($sZone, $sDomain);

		$aRecord = array('name' => $sDomain, 'content' => $sIP);

		if (null === $aDomain) {
			// domain does not yet exist

			$aRecord['record_type'] = 'A';
			$aRecord['ttl'] = 3600;

		} else {
			// domain already exists, we need to delete it first

			$aRecord['record_type'] = $aDomain['record_type'];
			$aRecord['ttl'] = $aDomain['ttl'];

			$iRecord = $aDomain['id'];

			$this->init();

			$mRes = $this->deleteRecord($sZone, $iRecord);

			if (true !== $mRes) return null;

		} // if domain exists or not

		if (null !== $iTTL && 0 < $iTTL) $aRecord['ttl'] = $iTTL;

		if (null !== $iPrio && 0 < $iPrio) $aRecord['prio'] = $iPrio;

		$this->init();

		$aRecord = array('record' => $aRecord);

		$sRecord = json_encode($aRecord);

		if (false === $sRecord) return null;

		$sURL = '' . self::API_URL . '/' . $sZone . '/records';

		return $this->executeCurlPOST($sURL, $sRecord);

	} // setIPofDomain


	//function setIPofRecord(String $sZone, String $sIP, $iRecord, $iTTL = null, $iPrio = null) {
	//
	//	if (!$this->bInit) return null;
	//
	//	if (!curl_setopt($this->oC, CURLOPT_POST, true)) return null;
	//
	//	$this->aHeaders[] = 'Content-Type: application/json';
	//
	//	$aRecord = array(//'name' => 'dd',
	//					 'content' => $sIP,
	//					 //'type' => 'A',
	//					 //'prio' => $iPrio
	//					);
	//
	//	if (null !== $iTTL && 0 < $iTTL) $aRecord['ttl'] = $iTTL;
	//
	//	if (null !== $iPrio && 0 < $iPrio) $aRecord['prio'] = $iPrio;
	//
	//	$aRecord = array('record' => $aRecord);
	//
	//	$sRecord = json_encode($aRecord);
	//
	//	if (false === $sRecord) return null;
	//
	//	if (!curl_setopt($this->oC, CURLOPT_POSTFIELDS, $sRecord)) return null;
	//
	//	$sURL = '' . self::API_URL . '/' . $sZone . '/records/' . $iRecord;
	//
	//	$mRes = $this->executeCurl($sURL);
	//
	//	return $mRes;
	//
	//} // setIPofRecord

} // DNS

/* * * *\ SwissalpS/exoscale/DNS.inc (CC) Luke JZ aka SwissalpS /* * * */
