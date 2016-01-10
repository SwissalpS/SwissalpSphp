<?php
/* * * *
 * * SwissalpS/PHOCOA/Localization/Countries.inc
 * *
 * *
 * * @version 20160109_042954 (CC) Luke JZ aka SwissalpS
 * * * */
namespace SwissalpS\PHOCOA\Localization;

use WFObject;

class Countries extends WFObject implements TableInterface {

	protected $aUIDs;

    static function sharedInstance() {

    	static $oCountries = null;

    	if (!$oCountries) {

    		$oCountries = new Countries();
    		Localization::sharedInstance()->addTable($oCountries);

    	} // first request

    	return $oCountries;

    } // sharedInstance


    function selectCountriesLabels() {

    	static $aResult = null;

    	if (!$aResult) {

    		$aCountryNames = array();

	    	$aLangs = Localization::languageOrder();

    		foreach ($this->aUIDs as $aCountry) {

    			foreach ($aLangs as $sLang) {

    				if (isset($aCountry[$sLang])) {

    					$aCountryNames[] = $aCountry[$sLang];
    					break;

    				} // if got translation

    			} // loop language order

    		} // loop countries

    	} // first request

		return $aCountryNames;

    } // selectCountriesLabels


    function selectCountriesValues() {

		return array_keys($this->aUIDs);

    } // selectCountriesValues


    static function countryIDs() {

    	return self::sharedInstance()->selectCountriesValues();

    } // countryIDs


    static function countryNames() {

    	return self::sharedInstance()->selectCountriesLabels();

    } // countryIDs


	// return translated value or the given key
	// fall back to next language in list of SssSLocalization::languageOrder()
	// if sLang isn't given use the first in the order
	function translate($sKey, $sLang = null) {

		$sKeyUC = strToUpper($sKey);

		if (!$sLang) $sLang = Localization::currentLanguage();

		$aLangs = array_merge(array($sLang), Localization::languageOrder());

		if (isset($this->aUIDs[$sKeyUC])) {

			foreach ($aLangs as $sLang) {

				if (isset($this->aUIDs[$sKeyUC][$sLang])) {

					return $this->aUIDs[$sKeyUC][$sLang];

				} // if have translation

			} // loop languages

		} // if got key

		return $sKey;

	} // translate


	// set a string for a key in this table, writable sources also save
	function set($sValue, $sKey, $sLang) {

		if (!isset($this->aUIDs[$sKey])) $this->aUIDs[$sKey] = array();

		$this->aUIDs[$sKey][$sLang] = $sValue;

		return false; // we don't save, so return false

	} // set


	// return the tables name, this will probably be the same as the name of the class
	function name() {

		return get_class($this);

	} // name


	function __construct() {

		parent::__construct();

		$this->aUIDs = array(
			'AF' => array(
				'en' => 'Afghanistan'),
			'AL' => array(
				'en' => 'Albania',
				'de' => 'Albanien',
				'fr' => 'Albanie',
				'it' => 'Albania'),
			'DZ' => array(
				'en' => 'Algeria',
				'de' => 'Algerien',
				'fr' => 'Algérie',
				'it' => 'Algeria'),
			'AS' => array(
				'en' => 'American Samoa',
				'de' => 'Amerikanisch-Samoa'),
			'AD' => array(
				'en' => 'Andorra',
				'de' => 'Andorra',
				'fr' => 'Andorre',
				'it' => 'Andorra'),
			'AO' => array(
				'en' => 'Angola'),
			'AI' => array(
				'en' => 'Anguilla'),
			'AQ' => array(
				'en' => 'Antarctica',
				'de' => 'Antarktis'),
			'AG' => array(
				'en' => 'Antigua And Barbuda',
				'de' => 'Antigua und Barbuda',
				'fr' => 'Antigua-et-Barbuda',
				'it' => 'Antigua e Barbuda'),
			'AR' => array(
				'en' => 'Argentina',
				'de' => 'Argentinien',
				'fr' => 'Argentine',
				'it' => 'Argentina',
				'rm' => 'Argentina'),
			'AM' => array(
				'en' => 'Armenia',
				'de' => 'Armenien',
				'fr' => 'Arménie',
				'it' => 'Armenia'),
			'AW' => array(
				'en' => 'Aruba'),
			'AU' => array(
				'en' => 'Australia',
				'de' => 'Australien',
				'fr' => 'Australie',
				'it' => 'Australia',
				'rm' => 'Australia'),
			'AT' => array(
				'en' => 'Austria',
				'de' => 'Österreich',
				'fr' => 'Autriche',
				'it' => 'Austria',
				'rm' => 'Austria'),
			'AZ' => array(
				'en' => 'Azerbaijan',
				'de' => 'Aserbaidschan',
				'fr' => 'Azerbaïdjan',
				'it' => 'Azerbaigian'),
			'BS' => array(
				'en' => 'Bahamas'),
			'BH' => array(
				'en' => 'Bahrain',
				'de' => 'Bahrain',
				'fr' => 'Bahreïn',
				'it' => 'Bahrain'),
			'BD' => array(
				'en' => 'Bangladesh',
				'de' => 'Bangladesch',
				'fr' => 'Bangladesh',
				'it' => 'Bangladesh'),
			'BB' => array(
				'en' => 'Barbados',
				'de' => 'Barbados',
				'fr' => 'Barbade',
				'it' => 'Barbados'),
			'BY' => array(
				'en' => 'Belarus',
				'de' => 'Weißrussland',
				'fr' => 'Biélorussie',
				'it' => 'Bielorussia',
				'rm' => 'Bielorussia'),
			'BE' => array(
				'en' => 'Belgium',
				'de' => 'Belgien',
				'fr' => 'Belgique',
				'it' => 'Belgio',
				'rm' => 'Belgia'),
			'BZ' => array(
				'en' => 'Belize'),
			'BJ' => array(
				'en' => 'Benin',
				'de' => 'Benin',
				'fr' => 'Bénin',
				'it' => 'Benin'),
			'BM' => array(
				'en' => 'Bermuda',
				'de' => 'Bermuda',
				'fr' => 'Bermudes',
				'it' => 'Bermude'),
			'BT' => array(
				'en' => 'Bhutan',
				'de' => 'Bhutan',
				'fr' => 'Bhoutan',
				'it' => 'Bhutan'),
			'BO' => array(
				'en' => 'Bolivia',
				'de' => 'Bolivien',
				'fr' => 'Bolivie',
				'it' => 'Bolivia'),
			'BA' => array(
				'en' => 'Bosnia And Herzegowina',
				'de' => 'Bosnien und Herzegowina',
				'fr' => 'Bosnie-Herzégovine',
				'it' => 'Bosnia-Erzegovina'),
			'BW' => array(
				'en' => 'Botswana'),
			'BV' => array(
				'en' => 'Bouvet Island'),
			'BR' => array(
				'en' => 'Brazil',
				'de' => 'Brasilien',
				'fr' => 'Brésil',
				'it' => 'Brasile',
				'rm' => 'Brasilia'),
			'IO' => array(
				'en' => 'British Indian Ocean Territory'),
			'BN' => array(
				'en' => 'Brunei Darussalam'),
			'BG' => array(
				'en' => 'Bulgaria',
				'de' => 'Bulgarien',
				'fr' => 'Bulgarie',
				'it' => 'Bulgaria'),
			'BF' => array(
				'en' => 'Burkina Faso'),
			'BI' => array(
				'en' => 'Burundi'),
			'KH' => array(
				'en' => 'Cambodia',
				'de' => 'Kambodscha',
				'fr' => 'Cambodge',
				'it' => 'Cambogia',
				'rm' => ''),
			'CM' => array(
				'en' => 'Cameroon',
				'de' => 'Kamerun',
				'fr' => 'Cameroun',
				'it' => 'Camerun'),
			'CA' => array(
				'en' => 'Canada',
				'de' => 'Kanada',
				'fr' => 'Canada',
				'it' => 'Canada',
				'rm' => 'Canada'),
			'CV' => array(
				'en' => 'Cape Verde',
				'de' => 'Kap Verde',
				'fr' => 'Cap-Vert',
				'it' => 'Capo Verde'),
			'KY' => array(
				'en' => 'Cayman Islands',
				'de' => 'Kaimaninseln',
				'fr' => 'Îles Caïmans',
				'it' => 'Isole Cayman'),
			'CF' => array(
				'en' => 'Central African Republic',
				'de' => 'Zentralafrikanische Republik',
				'fr' => 'République centrafricaine',
				'it' => 'Repubblica Centroafricana'),
			'TD' => array(
				'en' => 'Chad',
				'de' => 'Tschad',
				'fr' => 'Tchad',
				'it' => 'Ciad'),
			'CL' => array(
				'en' => 'Chile',
				'de' => 'Chile',
				'fr' => 'Chili',
				'it' => 'Cile',
				'rm' => 'Chile'),
			'CN' => array(
				'en' => 'China',
				'de' => 'China',
				'fr' => 'Chine',
				'it' => 'Cina',
				'rm' => 'China'),
			'CX' => array(
				'en' => 'Christmas Island',
				'de' => 'Weihnachtsinsel',
				'fr' => 'Île Christmas',
				'it' => 'Isola del Natale'),
			'CC' => array(
				'en' => 'Cocos (Keeling) Islands',
				'de' => 'Kokosinseln',
				'fr' => 'Îles Cocos',
				'it' => 'Isole Cocos e Keeling'),
			'CO' => array(
				'en' => 'Colombia',
				'de' => 'Kolumbien',
				'fr' => 'Colombie',
				'it' => 'Colombia'),
			'KM' => array(
				'en' => 'Comoros',
				'de' => 'Komoren',
				'fr' => 'Comores',
				'it' => 'Comore'),
			'CG' => array(
				'en' => 'Congo',
				'de' => 'Kongo',
				'fr' => 'Congo',
				'it' => 'Congo'),
			'CD' => array(
				'en' => 'Congo',
				'de' => 'Kongo',
				'fr' => 'Congo',
				'it' => 'Congo'),
			'CK' => array(
				'en' => 'Cook Islands',
				'de' => 'Cook-Inseln',
				'fr' => 'Îles Cook',
				'it' => 'Isole Cook'),
			'CR' => array(
				'en' => 'Costa Rica'),
			'CI' => array(
				'en' => 'Côte d\'Ivoire',
				'de' => 'Côte d\'Ivoire',
				'fr' => 'Côte d\'Ivoire',
				'it' => 'Costa d\'Avorio'),
			'HR' => array(
				'en' => 'Croatia (Local Name: Hrvatska)',
				'de' => 'Kroatien',
				'fr' => 'Croatie',
				'it' => 'Croazia',
				'rm' => 'Croazia'),
			'CU' => array(
				'en' => 'Cuba',
				'de' => 'Kuba',
				'fr' => 'Cuba',
				'it' => 'Cuba'),
			'CY' => array(
				'en' => 'Cyprus',
				'de' => 'Zypern',
				'fr' => 'Chypre',
				'it' => 'Cipro'),
			'CZ' => array(
				'en' => 'Czech Republic',
				'de' => 'Tschechien',
				'fr' => 'Tchéquie',
				'it' => 'Repubblica Ceca',
				'rm' => 'Tschechia'),
			'DK' => array(
				'en' => 'Denmark',
				'de' => 'Dänemark',
				'fr' => 'Danemark',
				'it' => 'Danimarca',
				'rm' => 'Danemarc'),
			'DJ' => array(
				'en' => 'Djibouti',
				'de' => 'Dschibuti',
				'fr' => 'Djibouti',
				'it' => 'Gibuti'),
			'DM' => array(
				'en' => 'Dominica'),
			'DO' => array(
				'en' => 'Dominican Republic',
				'de' => 'Dominikanische Republik',
				'fr' => 'République dominicaine',
				'it' => 'Repubblica Dominicana'),
			'TP' => array(
				'en' => 'East Timor',
				'de' => 'Osttimor',
				'fr' => 'Timor Oriental',
				'it' => 'Timor Est'),
			'EC' => array(
				'en' => 'Ecuador',
				'de' => 'Ecuador',
				'fr' => 'Équateur',
				'it' => 'Ecuador'),
			'EG' => array(
				'en' => 'Egypt',
				'de' => 'Ägypten',
				'fr' => 'Égypte',
				'it' => 'Egitto',
				'rm' => 'Egipta'),
			'SV' => array(
				'en' => 'El Salvador',
				'de' => 'El Salvador',
				'fr' => 'Salvador',
				'it' => 'El Salvador'),
			'GQ' => array(
				'en' => 'Equatorial Guinea',
				'de' => 'Äquatorialguinea',
				'fr' => 'Guinée équatoriale',
				'it' => 'Guinea Equatoriale '),
			'ER' => array(
				'en' => 'Eritrea',
				'de' => 'Eritrea',
				'fr' => 'Érythrée',
				'it' => 'Eritrea'),
			'EE' => array(
				'en' => 'Estonia',
				'de' => 'Estland',
				'fr' => 'Estonie',
				'it' => 'Estonia',
				'rm' => 'Estonia'),
			'ET' => array(
				'en' => 'Ethiopia',
				'de' => 'Äthiopien',
				'fr' => 'Éthiopie',
				'it' => 'Etiopia'),
			'FK' => array(
				'en' => 'Falkland Islands (Malvinas)',
				'de' => 'Falklandinseln',
				'fr' => 'Îles Malouines',
				'it' => 'Isole Falkland (Malvine)'),
			'FO' => array(
				'en' => 'Faroe Islands',
				'de' => 'Färöer Inseln',
				'fr' => 'Îles Féroé',
				'it' => 'Isole Fær Øer'),
			'FJ' => array(
				'en' => 'Fiji',
				'de' => 'Fidschi',
				'fr' => 'Fidji',
				'it' => 'Viti'),
			'FI' => array(
				'en' => 'Finland',
				'de' => 'Finnland',
				'fr' => 'Finlande',
				'it' => 'Finlandia',
				'rm' => 'Finlanda'),
			'FR' => array(
				'en' => 'France',
				'de' => 'Frankreich',
				'fr' => 'France',
				'it' => 'Francia',
				'rm' => 'Frantscha'),
			'FX' => array(
				'en' => 'France, Metropolitan'),
			'GF' => array(
				'en' => 'French Guiana',
				'de' => 'Französisch-Guayana',
				'fr' => 'Guyane',
				'it' => 'Guyana Francese'),
			'PF' => array(
				'en' => 'French Polynesia',
				'de' => 'Französisch-Polynesien',
				'fr' => 'Polynésie française',
				'it' => 'Polinesia Francese'),
			'TF' => array(
				'en' => 'French Southern Territories',
				'de' => 'Französische Süd- und Antarktisgebiete',
				'fr' => 'Terres australes et antarctiques françaises',
				'it' => 'Territori Francesi del Sud'),
			'GA' => array(
				'en' => 'Gabon',
				'de' => 'Gabun',
				'fr' => 'Gabon',
				'it' => 'Gabon'),
			'GM' => array(
				'en' => 'Gambia',
				'de' => 'Gambia',
				'fr' => 'Gambie',
				'it' => 'Gambia'),
			'GE' => array(
				'en' => 'Georgia',
				'de' => 'Georgien',
				'fr' => 'Géorgie',
				'it' => 'Georgia'),
			'DE' => array(
				'en' => 'Germany',
				'de' => 'Deutschland',
				'fr' => 'Allemagne',
				'it' => 'Germania',
				'rm' => 'Terra Tudestga'),
			'GH' => array(
				'en' => 'Ghana'),
			'GI' => array(
				'en' => 'Gibraltar'),
			'GR' => array(
				'en' => 'Greece',
				'de' => 'Griecheland',
				'fr' => 'Grèce',
				'it' => 'Grecia',
				'rm' => 'Grezia'),
			'GL' => array(
				'en' => 'Greenland',
				'de' => 'Grönland',
				'fr' => 'Groenland',
				'it' => 'Groenlandia',
				'rm' => 'Grönlanda'),
			'GD' => array(
				'en' => 'Grenada',
				'de' => 'Grenada',
				'fr' => 'Grenade',
				'it' => 'Grenada'),
			'GP' => array(
				'en' => 'Guadeloupe',
				'de' => 'Guadeloupe',
				'fr' => 'Guadeloupe',
				'it' => 'Guadalupa'),
			'GU' => array(
				'en' => 'Guam'),
			'GT' => array(
				'en' => 'Guatemala'),
			'GN' => array(
				'en' => 'Guinea',
				'de' => 'Guinea',
				'fr' => 'Guinée',
				'it' => 'Guinea'),
			'GW' => array(
				'en' => 'Guinea-Bissau',
				'de' => 'Guinea-Bissau',
				'fr' => 'Guinée-Bissau',
				'it' => 'Guinea-Bissau'),
			'GY' => array(
				'en' => 'Guyana'),
			'HT' => array(
				'en' => 'Haiti',
				'de' => 'Haiti',
				'fr' => 'Haïti',
				'it' => 'Haiti'),
			'HM' => array(
				'en' => 'Heard And Mc Donald Islands'),
			'VA' => array(
				'en' => 'Holy See (Vatican City State)',
				'de' => 'Heiliger Stuhl',
				'fr' => 'Saint-Siège',
				'it' => 'Santa Sede'),
			'HN' => array(
				'en' => 'Honduras'),
			'HK' => array(
				'en' => 'Hong Kong'),
			'HU' => array(
				'en' => 'Hungary',
				'de' => 'Ungarn',
				'fr' => 'Hongrie',
				'it' => 'Ungheria',
				'rm' => 'Ungaria'),
			'IS' => array(
				'en' => 'Iceland',
				'de' => 'Island',
				'fr' => 'Islande',
				'it' => 'Islanda',
				'rm' => 'Islanda'),
			'IN' => array(
				'en' => 'India',
				'de' => 'Indien',
				'fr' => 'Inde',
				'it' => 'India',
				'rm' => 'India'),
			'ID' => array(
				'en' => 'Indonesia',
				'de' => 'Indonesien',
				'fr' => 'Indonésie',
				'it' => 'Indonesia',
				'rm' => 'Indonesia'),
			'IR' => array(
				'en' => 'Iran'),
			'IQ' => array(
				'en' => 'Iraq',
				'de' => 'Irak',
				'fr' => 'Irak',
				'it' => 'Iraq',
				'rm' => 'Irac'),
			'IE' => array(
				'en' => 'Ireland',
				'de' => 'Irland',
				'fr' => 'Irlande',
				'it' => 'Irlanda',
				'rm' => 'Irlanda'),
			'IL' => array(
				'en' => 'Israel',
				'de' => 'Israel',
				'fr' => 'Israël',
				'it' => 'Israele',
				'rm' => 'Israel'),
			'IT' => array(
				'en' => 'Italy',
				'de' => 'Italien',
				'fr' => 'Italie',
				'it' => 'Italia',
				'rm' => 'Italia'),
			'JM' => array(
				'en' => 'Jamaica',
				'de' => 'Jamaika',
				'fr' => 'Jamaïque',
				'it' => 'Giamaica'),
			'JP' => array(
				'en' => 'Japan',
				'de' => 'Japan',
				'fr' => 'Japon',
				'it' => 'Giappone',
				'rm' => 'Giapun'),
			'JO' => array(
				'en' => 'Jordan',
				'de' => 'Jordanien',
				'fr' => 'Jordanie',
				'it' => 'Giordania'),
			'KZ' => array(
				'en' => 'Kazakhstan',
				'de' => 'Kasachstan',
				'fr' => 'Kazakhstan',
				'it' => 'Kazakistan'),
			'KE' => array(
				'en' => 'Kenya',
				'de' => 'Kenia',
				'fr' => 'Kenya',
				'it' => 'Kenya'),
			'KI' => array(
				'en' => 'Kiribati'),
			'KP' => array(
				'en' => 'North Korea',
				'de' => 'Nordkorea',
				'fr' => 'Corée du Nord',
				'it' => 'Corea del Nord'),
			'KR' => array(
				'en' => 'South Korea',
				'de' => 'Südkorea',
				'fr' => 'Corée du Sud',
				'it' => 'Corea del Sud'),
			'KW' => array(
				'en' => 'Kuwait',
				'de' => 'Kuwait',
				'fr' => 'Koweït',
				'it' => 'Kuwait'),
			'KG' => array(
				'en' => 'Kyrgyzstan',
				'de' => 'Kirgisistan',
				'fr' => 'Kirghizistan',
				'it' => 'Kirghizistan'),
			'LA' => array(
				'en' => 'Laos'),
			'LV' => array(
				'en' => 'Latvia',
				'de' => 'Lettland',
				'fr' => 'Lettonie',
				'it' => 'Lettonia',
				'rm' => 'Lettonia'),
			'LB' => array(
				'en' => 'Lebanon',
				'de' => 'Libanon',
				'fr' => 'Liban',
				'it' => 'Libano',
				'rm' => 'Libanon'),
			'LS' => array(
				'en' => 'Lesotho'),
			'LR' => array(
				'en' => 'Liberia'),
			'LY' => array(
				'en' => 'Libya',
				'de' => 'Libyen',
				'fr' => 'Libye',
				'it' => 'Libia',
				'rm' => 'Libia'),
			'LI' => array(
				'en' => 'Liechtenstein'),
			'LT' => array(
				'en' => 'Lithuania',
				'de' => 'Litauen',
				'fr' => 'Lituanie',
				'it' => 'Lituania',
				'rm' => 'Lituania'),
			'LU' => array(
				'en' => 'Luxembourg',
				'de' => 'Luxemburg',
				'fr' => 'Luxembourg',
				'it' => 'Lussemburgo',
				'rm' => 'Luxemburg'),
			'MO' => array(
				'en' => 'Macau'),
			'MK' => array(
				'en' => 'Macedonia, Former Yugoslav Republic Of',
				'de' => 'Mazedonien',
				'fr' => 'Macédoine',
				'it' => 'Macedonia'),
			'MG' => array(
				'en' => 'Madagascar',
				'de' => 'Madagaskar',
				'fr' => 'Madagascar',
				'it' => 'Madagascar'),
			'MW' => array(
				'en' => 'Malawi'),
			'MY' => array(
				'en' => 'Malaysia',
				'de' => 'Malaysia',
				'fr' => 'Malaisie',
				'it' => 'Malaysia'),
			'MV' => array(
				'en' => 'Maldives',
				'de' => 'Malediven',
				'fr' => 'Maldives',
				'it' => 'Maldive'),
			'ML' => array(
				'en' => 'Mali'),
			'MT' => array(
				'en' => 'Malta',
				'de' => 'Malta',
				'fr' => 'Malte',
				'it' => 'Malta',
				'rm' => 'Malta'),
			'MH' => array(
				'en' => 'Marshall Islands',
				'de' => 'Marshallinseln',
				'fr' => 'Îles Marshall',
				'it' => 'Isole Marshall'),
			'MQ' => array(
				'en' => 'Martinique'),
			'MR' => array(
				'en' => 'Mauritania',
				'de' => 'Mauretanien',
				'fr' => 'Mauritanie',
				'it' => 'Mauritania'),
			'MU' => array(
				'en' => 'Mauritius',
				'de' => 'Mauritius',
				'fr' => 'Maurice',
				'it' => 'Mauritius'),
			'YT' => array(
				'en' => 'Mayotte'),
			'MX' => array(
				'en' => 'Mexico',
				'de' => 'Mexiko',
				'fr' => 'Mexique',
				'it' => 'Messico',
				'rm' => 'Mexico'),
			'FM' => array(
				'en' => 'Micronesia',
				'de' => 'Mikronesien',
				'fr' => 'Micronésie',
				'it' => 'Micronesia'),
			'MD' => array(
				'en' => 'Moldova',
				'de' => 'Moldawien',
				'fr' => 'Moldavie',
				'it' => 'Moldova'),
			'MC' => array(
				'en' => 'Monaco'),
			'MN' => array(
				'en' => 'Mongolia',
				'de' => 'Mongolei',
				'fr' => 'Mongolie',
				'it' => 'Mongolia'),
			'MS' => array(
				'en' => 'Montserrat'),
			'MA' => array(
				'en' => 'Morocco',
				'de' => 'Marokko',
				'fr' => 'Maroc',
				'it' => 'Marocco',
				'rm' => 'Maroc'),
			'MZ' => array(
				'en' => 'Mozambique',
				'de' => 'Mosambik',
				'fr' => 'Mozambique',
				'it' => 'Mozambico'),
			'MM' => array(
				'en' => 'Myanmar'),
			'NA' => array(
				'en' => 'Namibia',
				'de' => 'Namibia',
				'fr' => 'Namibie',
				'it' => 'Namibia'),
			'NR' => array(
				'en' => 'Nauru'),
			'NP' => array(
				'en' => 'Nepal',
				'de' => 'Nepal',
				'fr' => 'Népal',
				'it' => 'Nepal'),
			'NL' => array(
				'en' => 'Netherlands',
				'de' => 'Niederlande',
				'fr' => 'Pays-Bas',
				'it' => 'Paesi Bassi',
				'rm' => 'Pajais bass'),
			'AN' => array(
				'en' => 'Netherlands Antilles',
				'de' => 'Niederländische Antillen',
				'fr' => 'Antilles néerlandaises',
				'it' => 'Antille Olandesi'),
			'NC' => array(
				'en' => 'New Caledonia',
				'de' => 'Neukaledonien',
				'fr' => 'Nouvelle-Calédonie',
				'it' => 'Nuova Caledonia'),
			'NZ' => array(
				'en' => 'New Zealand',
				'de' => 'Neuseeland',
				'fr' => 'Nouvelle-Zélande',
				'it' => 'Nuova Zelanda',
				'rm' => 'Nova Zelanda'),
			'NI' => array(
				'en' => 'Nicaragua'),
			'NE' => array(
				'en' => 'Niger'),
			'NG' => array(
				'en' => 'Nigeria'),
			'NU' => array(
				'en' => 'Niue'),
			'NF' => array(
				'en' => 'Norfolk Island',
				'de' => 'Norfolkinsel',
				'fr' => 'Norfolk Island',
				'it' => 'Norfolk Island'),
			'MP' => array(
				'en' => 'Northern Mariana Islands',
				'de' => 'Nördliche Marianen',
				'fr' => 'Îles Mariannes du Nord',
				'it' => 'Isole Marianne Settentrionali'),
			'NO' => array(
				'en' => 'Norway',
				'de' => 'Norwegen',
				'fr' => 'Norvège',
				'it' => 'Norvegia',
				'rm' => 'Norvegia'),
			'OM' => array(
				'en' => 'Oman'),
			'PK' => array(
				'en' => 'Pakistan'),
			'PW' => array(
				'en' => 'Palau',
				'de' => 'Palau',
				'fr' => 'Palaos',
				'it' => 'Palau'),
			'PA' => array(
				'en' => 'Panama',
				'de' => 'Panama',
				'fr' => 'Panamá',
				'it' => 'Panamá'),
			'PG' => array(
				'en' => 'Papua New Guinea',
				'de' => 'Papua-Neuguinea',
				'fr' => 'Papouasie-Nouvelle-Guinée',
				'it' => 'Papua-Nuova Guinea'),
			'PY' => array(
				'en' => 'Paraguay',
				'de' => 'Paraguay',
				'fr' => 'Paraguay',
				'it' => 'Paraguay',
				'rm' => 'Paraguai'),
			'PE' => array(
				'en' => 'Peru',
				'de' => 'Peru',
				'fr' => 'Pérou',
				'it' => 'Perù'),
			'PH' => array(
				'en' => 'Philippines',
				'de' => 'Philippinen',
				'fr' => 'Philippines',
				'it' => 'Filippine'),
			'PN' => array(
				'en' => 'Pitcairn',
				'it' => 'Isole Pitcairn'),
			'PL' => array(
				'en' => 'Poland',
				'de' => 'Polen',
				'fr' => 'Pologne',
				'it' => 'Polonia',
				'rm' => 'Pologna'),
			'PT' => array(
				'en' => 'Portugal',
				'de' => 'Portugal',
				'fr' => 'Portugal',
				'it' => 'Portogallo',
				'rm' => 'Portugal'),
			'PR' => array(
				'en' => 'Puerto Rico',
				'de' => 'Puerto Rico',
				'fr' => 'Puerto Rico',
				'it' => 'Porto Rico'),
			'QA' => array(
				'en' => 'Qatar',
				'de' => 'Katar ',
				'fr' => 'Qatar',
				'it' => 'Qatar'),
			'RE' => array(
				'en' => 'Reunion',
				'de' => 'Réunion',
				'fr' => 'Réunion',
				'it' => 'Riunione'),
			'RO' => array(
				'en' => 'Romania',
				'de' => 'Rumänien',
				'fr' => 'Roumanie',
				'it' => 'Romania',
				'rm' => 'Rumania'),
			'RU' => array(
				'en' => 'Russian Federation',
				'de' => 'Russland',
				'fr' => 'Russie',
				'it' => 'Russia',
				'rm' => 'Russia'),
			'RW' => array(
				'en' => 'Rwanda',
				'de' => 'Ruanda',
				'fr' => 'Rouanda',
				'it' => 'Ruanda'),
			'KN' => array(
				'en' => 'Saint Kitts And Nevis',
				'de' => 'Saint Kitts und Nevis',
				'fr' => 'Saint-Christophe-et-Niévès',
				'it' => 'Saint Kitts e Nevis'),
			'LC' => array(
				'en' => 'Saint Lucia',
				'de' => 'Saint Lucia',
				'fr' => 'Sainte-Lucie',
				'it' => 'Santa Lucia'),
			'VC' => array(
				'en' => 'Saint Vincent And The Grenadines',
				'de' => 'Saint Vincent und die Grenadinen',
				'fr' => 'Saint-Vincent-et-les Grenadines',
				'it' => 'Saint Vincent e Grenadine'),
			'WS' => array(
				'en' => 'Samoa'),
			'SM' => array(
				'en' => 'San Marino',
				'de' => 'San Marino',
				'fr' => 'Saint-Marin',
				'it' => 'San Marino'),
			'ST' => array(
				'en' => 'Sao Tome And Principe',
				'de' => 'São Tomé und Príncipe',
				'fr' => 'São Tomé-et-Príncipe',
				'it' => 'São Tomé e Príncipe'),
			'SA' => array(
				'en' => 'Saudi Arabia',
				'de' => 'Saudi-Arabien',
				'fr' => 'Arabie Saoudite',
				'it' => 'Arabia Saudita',
				'rm' => 'Arabia Saudita'),
			'SN' => array(
				'en' => 'Senegal',
				'de' => 'Senegal',
				'fr' => 'Sénégal',
				'it' => 'Senegal'),
			'SC' => array(
				'en' => 'Seychelles',
				'de' => 'Seychellen',
				'fr' => 'Seychelles',
				'it' => 'Seychelles'),
			'SL' => array(
				'en' => 'Sierra Leone'),
			'SG' => array(
				'en' => 'Singapore',
				'de' => 'Singapur',
				'fr' => 'Singapour',
				'it' => 'Singapore'),
			'SK' => array(
				'en' => 'Slovakia (Slovak Republic)',
				'de' => 'Slowakei',
				'fr' => 'Slovaquie',
				'it' => 'Slovacchia',
				'it' => 'Slovachia'),
			'SI' => array(
				'en' => 'Slovenia',
				'de' => 'Slowenien',
				'fr' => 'Slovénie',
				'it' => 'Slovenia',
				'rm' => 'Slovenia'),
			'SB' => array(
				'en' => 'Solomon Islands',
				'de' => 'Salomonen',
				'fr' => 'Îles Salomon',
				'it' => 'Isole Salomone'),
			'SO' => array(
				'en' => 'Somalia',
				'de' => 'Somalia',
				'fr' => 'Somalie',
				'it' => 'Somalia',),
			'ZA' => array(
				'en' => 'South Africa',
				'de' => 'Südafrika',
				'fr' => 'Afrique du Sud',
				'it' => 'Sud Africa',
				'rm' => 'Africa dal Sid'),
			'GS' => array(
				'en' => 'South Georgia, South Sandwich Islands',
				'de' => 'Südgeorgien und Südliche Sandwichinseln',
				'fr' => 'Géorgie du Sud, îles Sandwich du Sud',
				'it' => 'Georgia del Sud e Isole Sandwich Australi'),
			'ES' => array(
				'en' => 'Spain',
				'de' => 'Spanien',
				'fr' => 'Espagne',
				'it' => 'Spagna',
				'rm' => 'Spagna'),
			'LK' => array(
				'en' => 'Sri Lanka'),
			'SH' => array(
				'en' => 'St. Helena'),
			'PM' => array(
				'en' => 'St. Pierre And Miquelon',
				'de' => 'St. Pierre und Miquelon',
				'fr' => 'St. Pierre et Miquelon',
				'it' => 'St. Pierre e Miquelon'),
			'SD' => array(
				'en' => 'Sudan',
				'de' => 'Sudan',
				'fr' => 'Soudan',
				'it' => 'Sudan'),
			'SR' => array(
				'en' => 'Suriname'),
			'SJ' => array(
				'en' => 'Svalbard And Jan Mayen Islands'),
			'SZ' => array(
				'en' => 'Swaziland',
				'de' => 'Swasiland',
				'fr' => 'Swaziland',
				'it' => 'Swaziland'),
			'SE' => array(
				'en' => 'Sweden',
				'de' => 'Schweden',
				'fr' => 'Suède',
				'it' => 'Svezia',
				'rm' => 'Svezia'),
			'CH' => array(
				'en' => 'Switzerland',
				'de' => 'Schweiz',
				'fr' => 'Suisse',
				'it' => 'Svizzera',
				'rm' => 'Svizra'),
			'SY' => array(
				'en' => 'Syrian Arab Republic',
				'de' => 'Syrien',
				'fr' => 'Syrie',
				'it' => 'Siria',
				'rm' => 'Siria'),
			'TW' => array(
				'en' => 'Taiwan',
				'de' => 'Taiwan',
				'fr' => 'Taïwan',
				'it' => 'Taiwan'),
			'TJ' => array(
				'en' => 'Tajikistan',
				'de' => 'Tadschikistan',
				'fr' => 'Tadjikistan',
				'it' => 'Tagikistan'),
			'TZ' => array(
				'en' => 'Tanzania',
				'de' => 'Tansania',
				'fr' => 'Tanzanie',
				'it' => 'Tanzania'),
			'TH' => array(
				'en' => 'Thailand',
				'de' => 'Thailand',
				'fr' => 'Thaïlande',
				'it' => 'Tailandia'),
			'TG' => array(
				'en' => 'Togo'),
			'TK' => array(
				'en' => 'Tokelau'),
			'TO' => array(
				'en' => 'Tonga'),
			'TT' => array(
				'en' => 'Trinidad And Tobago',
				'de' => 'Trinidad und Tobago',
				'fr' => 'Trinité-et-Tobago',
				'it' => 'Trinidad e Tobago'),
			'TN' => array(
				'en' => 'Tunisia',
				'de' => 'Tunesien',
				'fr' => 'Tunisie',
				'it' => 'Tunisia',
				'rm' => 'Tunesia'),
			'TR' => array(
				'en' => 'Turkey',
				'de' => 'Türkei',
				'fr' => 'Turquie',
				'it' => 'Turchia',
				'rm' => 'Tirchia'),
			'TM' => array(
				'en' => 'Turkmenistan',
				'de' => 'Turkmenistan',
				'fr' => 'Turkménistan',
				'it' => 'Turkmenistan'),
			'TC' => array(
				'en' => 'Turks And Caicos Islands',
				'de' => 'Turks- und Caicosinseln'),
			'TV' => array(
				'en' => 'Tuvalu'),
			'UG' => array(
				'en' => 'Uganda',
				'de' => 'Uganda',
				'fr' => 'Ouganda',
				'it' => 'Uganda'),
			'UA' => array(
				'en' => 'Ukraine',
				'de' => 'Ukraine',
				'fr' => 'Ukraine',
				'it' => 'Ucraina',
				'rm' => 'Ucraina'),
			'AE' => array(
				'en' => 'United Arab Emirates',
				'de' => 'Vereinigte Arabische Emirate',
				'fr' => 'Émirats arabes unis',
				'it' => 'Emirati Arabi Uniti'),
			'GB' => array(
				'en' => 'United Kingdom',
				'de' => 'Großbritannien',
				'fr' => 'Royaume-Uni de Grande-Bretagne',
				'it' => 'Regno Unito di Gran Bretagna',
				'rm' => 'Reginavel unì da la Gronda Britannia'),
			'US' => array(
				'en' => 'United States',
				'de' => 'Vereinigte Staaten',
				'fr' => 'États-Unis',
				'it' => 'Stati Uniti',
				'rm' => 'Stadis Unids'),
			'UM' => array(
				'en' => 'United States Minor Outlying Islands',
				'de' => 'Vereinigte Staaten Ozeanien',
				'fr' => 'États-Unis Îles mineures éloignées',
				'it' => 'Stati Uniti Isole Minori'),
			'UY' => array(
				'en' => 'Uruguay',
				'de' => 'Uruguay',
				'fr' => 'Uruguay',
				'it' => 'Uruguay',
				'rm' => 'Uruguai'),
			'UZ' => array(
				'en' => 'Uzbekistan',
				'de' => 'Usbekistan',
				'fr' => 'Ouzbékistan',
				'it' => 'Uzbekistan'),
			'VU' => array(
				'en' => 'Vanuatu'),
			'VE' => array(
				'en' => 'Venezuela'),
			'VN' => array(
				'de' => 'Vietnam'),
			'VG' => array(
				'en' => 'Virgin Islands (British)',
				'de' => 'Britischen Jungferninseln',
				'fr' => 'Îles Vierges (britanniques)',
				'it' => 'Isole Vergini britanniche'),
			'VI' => array(
				'en' => 'Virgin Islands (U.S.)',
				'de' => 'Amerikanische Jungferninseln',
				'fr' => 'Iles Vierges (US)',
				'it' => 'Isole Vergini (USA)'),
			'WF' => array(
				'en' => 'Wallis And Futuna Islands',
				'de' => 'Wallis und Futuna',
				'fr' => 'Wallis et Futuna',
				'it' => 'Isole Wallis e Futuna'),
			'EH' => array(
				'en' => 'Western Sahara',
				'de' => 'West-Sahara',
				'fr' => 'Sahara occidental',
				'it' => 'Sahara occidentale'),
			'YE' => array(
				'en' => 'Yemen',
				'de' => 'Jemen',
				'fr' => 'Yémen',
				'it' => 'Yemen'),
			'YU' => array(
				'en' => 'Yugoslavia',
				'de' => 'Jugoslawien',
				'fr' => 'Yougoslavie',
				'it' => 'Jugoslavia'),
			'ZM' => array(
				'en' => 'Zambia',
				'de' => 'Sambia',
				'fr' => 'Zambie',
				'it' => 'Zambia'),
			'ZW' => array(
				'en' => 'Zimbabwe',
				'de' => 'Simbabwe',
				'fr' => 'Zimbabwe',
				'it' => 'Zimbabwe')
			);

	} // __construct

} // Countries