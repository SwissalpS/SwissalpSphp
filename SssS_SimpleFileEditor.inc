<?php

//echo '<pre>';
if(get_magic_quotes_runtime()) {
  set_magic_quotes_runtime(false);
 // echo 'mqr was running...';
} else {
 set_magic_quotes_runtime(false);
  // echo 'mqr not running';
}
if(get_magic_quotes_runtime()){
//  echo '
//still active';
} else {//echo '
//turned off';
}
//echo '</pre>';
/*

$oSFE = new SssS_SimpleFileEditor(array(

		'indexphp' => 'index.php',
		'dbctxt' => 'data/dbc.txt',
		'boxedp' => 'boxed.php'), 'fEditor.php');
// to control the form beginning and end ourself:
echo $oSFE->doItWrappedInCommentTags();

// to have a self contained form: (in production mode, you should at least have GET or cookie based authentication)
echo $oSFE->doItWrappedInForm();

// please note NOT to EDIT THIS FILE with THIS CLASS nor any file that contains any of the ESCAPE STRINGS (beginning with SssS_SFEesc, look at fGetEscapeArrays for exact info)
*/

class SssS_SimpleFileEditor {
	function SssS_SimpleFileEditor($aFiles, $sTargetURL = '') {
		$this->sOut = '
<!-- SssS_SimpleFileEditor START -->
';
		$this->aFiles = $aFiles;
		$this->sTargetURL = $sTargetURL;
	} // SssS_SimpleFileEditor


	public function doItWrappedInCommentTags() {

		$this->doIt();

		$this->sOut .=  '
<!-- SssS_SimpleFileEditor END -->
';
		return $this->sOut;
	} // doItWrappedInCommentTags


	public function doItWrappedInForm() {
		$this->sOut = sprintf('<form action="%s" method="post" enctype="multipart/form-data">
', $this->sTargetURL);

		$this->doIt();

		$this->sOut .=  '
</form>
<!-- SssS_SimpleFileEditor END -->
';
		return $this->sOut;
	} // doItWrappedInForm


	/* * * * * * * P R I V A T E  F U N C T I O N S * * * * * * */


	private function doIt() {
		// save the file?
		if ($this->fDidRequestSave()) $this->fSave();

		// show editor?
		if ($this->fDidRequestEdit()) $this->fPrintEditor();

		// show files available for editing
		$this->fPrintFileButtons();
	} // doIt


	private function fDidRequestEdit() {
		return (isset($_REQUEST['SssSSFEedit']) && in_array($_REQUEST['SssSSFEedit'], array_keys($this->aFiles)));
	} // fDidRequestEdit


	private function fDidRequestSave() {
		return (isset($_REQUEST['SssSSFEfile']) && isset($_REQUEST['SssSSFEsave']) && isset($_REQUEST['SssSSFEtf']) && ('Save' == $_REQUEST['SssSSFEsave']) && (in_array($_REQUEST['SssSSFEfile'], array_keys($this->aFiles))));
	} // fDidRequestSave


	private function fGetEscapeArrays() {
		return array(array(
			'<'), array(
			'SssS_SFEescLT'));
	} // fGetEscapeArrays


	private function fEscapeString($s) {
		list($aSearch, $aReplace) = $this->fGetEscapeArrays();
		return str_replace($aSearch, $aReplace, $s);
	} // fEscapeString


	private function fUnescapeString($s) {
		list($aReplace, $aSearch) = $this->fGetEscapeArrays();
  $aReplace = array_merge($aReplace, array("'", '"', "\n", "\n", "\n", "\n", "\n"));
  $aSearch = array_merge($aSearch, array("\'", '\"', "\n\r", "\r\n", "\r", '\n', '\r'));
		$s = str_replace($aSearch, $aReplace, $s);
/*$s = ($s);
echo '<br/>
html decode: ' . htmlspecialchars_decode($s, ENT_QUOTES);
*/
return $s;
 	} // fUnescapeString



	private function fPrintEditor() {
		$f = $this->aFiles[$_REQUEST['SssSSFEedit']];
		$this->sOut .= 'working on file: ' . $f;

    if(is_file($f) && is_readable($f)) {
		$r = fopen($f, 'rb');
		$sTxt = fread($r, filesize($f));
		fclose($r);
		// escape some characters
		$sTxt = $this->fEscapeString($sTxt);
    } else { $sTxt = ''; }
		// count the rows to make sure we can edit (also on iPhone) and add some more
		$iRows = count(explode("\n", $sTxt)) + 55;
    	$this->sOut .= '<br />
	<textarea name="SssSSFEtf" cols="85" rows="' . $iRows . '">'
    		. $sTxt
    		. '</textarea><br />
			<input name="SssSSFEsave" value="Save" type="submit" />
			<input name="SssSSFEfile" value="'
            . $_REQUEST['SssSSFEedit'] . '" type="hidden" /><hr />';
	} // fPrintEditor


	private function fPrintFileButtons() {
		$aFiles = array_keys($this->aFiles);
		$this->sOut .= '<br /><div class="SssSSFEeditButtonsBlock"><h2>click on a button to edit the file it represents</h2><br />';
		foreach($aFiles as $sTitle) { $this->sOut .= '<input name="SssSSFEedit" value="' . $sTitle . '" type="submit" />'; }
		$this->sOut .= '<br />';
	} // fPrintFileButtons


	private function fSave() {
		// clean editor markings
		$sText = $this->fUnescapeString($_REQUEST['SssSSFEtf']);
		// get the true file name from aray by it's label
		$f = $this->aFiles[$_REQUEST['SssSSFEfile']];

		// backup file
		for ($i = 0; ; $i++) {
			$ff = $f . $i;
			if (!file_exists($ff)) {
				$this->sOut .= system('cp ' . $f . ' ' . $ff);
				break;
		}   } // for until found a valid filename
		// write to file
		$r = fopen($f, 'wb');
		fwrite($r, $sText);
		fclose($r);

		$this->sOut .= 'backed up file: ' . $f . ' to: ' . $ff . ' and saved to file: ' . $f;

		unset($r, $sText, $ff, $f, $i);
	} // fSave

	function __toString() { return print_r($this, true); } // __toString

} // SssS_SimpleFileEditor
?>
