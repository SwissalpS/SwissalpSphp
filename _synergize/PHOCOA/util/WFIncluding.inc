<?php
/* vim: set expandtab tabstop=4 shiftwidth=4: */
/* * * *
 * * WFIncluding.inc
 * *
 * * basically added SwissalpS frameworks that
 * *
 * * @version 20110107_144416 (CC) Luke JZ aka SwissalpS
 * * * */
if (!defined('AUTO_INCLUDING')) define('AUTO_INCLUDING', 'WFIncludingSssS');
/**
 * @package framework-base
 * @copyright Copyright (c) 2005 Alan Pinstein. All Rights Reserved.
 * @version $Id: kvcoding.php,v 1.3 2004/12/12 02:44:09 alanpinstein Exp $
 * @author Alan Pinstein <apinstein@mac.com>
 */

/**
 *  WFIncluding helps PHOCOA improve performance by providing autoload infrastructure.
 */
class WFIncluding {
	/**
	 *  PHOCOA autload callback.
	 *
	 *  This function will load any PHOCOA classes, interfaces, etc.
	 *
	 *  NOTE: autoload() will be called automatically for any new classes, interfaces, etc that are not yet in existence.
	 *
	 *  @param string The className that needs to be loaded.
	 *  @return boolean TRUE if we handled the loading, false otherwise.
	 */
	static public function autoload($sClassName) {

		// using namespace?
		$aNamespaceParts = explode('\\', $sClassName);
		if (1 < count($aNamespaceParts)) {
			if (self::autoloadPSR($aNamespaceParts)) {
				// dealt with
				return true;
			} // if loaded
		} // if got namespace

		// I am guessing that using a hashmap will be faster than a big switch statement... no tests yet, but... in any case I'll do it this way first.
		// other option is to add a bunch of paths to include_path, but that seems like a bad idea... YES that's a VERY BAD IDEA. Searching paths is much more expensive
		// than including files directly because it hits the filesystem a lot to find the files.
		static $hAutoloadClassmapCache = NULL;
		static $aDepricatedClasses = null;
		static $aReplacementClasses = null;
		static $aDirsToCheck = null;
		static $aSuffixesToTry = null;
		$sRequirePath = null;

		if (NULL == $hAutoloadClassmapCache) {

			$aDepricatedClasses = array(
				'SssS_CLI_Tools',
				'SssS_Plist',
				'SssS_ShellScriptRunner',
				'SssS_ShellScriptRunnerForDummies',
				'SssS_XHTML',
				'SssSphocoaGenerator',
			);

			$aReplacementClasses = array(
				'SwissalpS\CLI\Tools',
				'SwissalpS\XML\Plist',
				'SwissalpS\CLI\ShellScriptRunner',
				'SwissalpS\CLI\ShellScriptRunnerForDummies',
				'SwissalpS\XML\XHTML',
				'SwissalpS\PHOCOA\Project\Generator',
			);

			$hAutoloadClassmapCache = array(
			 	// SwissalpS overrides
			 	// SwissalpS framework etc.
			 	'CustomException'
			 	=> SwissalpS_FRAMEWORK_DIR . '/_external/CustomException.inc',
				'Horde_Yaml'
				=> HORDE_FRAMEWORK_DIR . '/Yaml.php',
				'Horde_Yaml_Dumper'
				=> HORDE_FRAMEWORK_DIR . '/Yaml/Dumper.php',
				'Horde_Yaml_Loader'
				=> HORDE_FRAMEWORK_DIR . '/Yaml/Loader.php',
				'Horde_Yaml_Node'
				=> HORDE_FRAMEWORK_DIR . '/Yaml/Node.php',
				'iphp'
				=> SwissalpS_FRAMEWORK_DIR . '/_external/iPHP/iphp.php',
				'JohnJ_ed25519'
				=> SwissalpS_FRAMEWORK_DIR . '/_external/JohnJ_ed25519.inc',
				'Log'
				=> PEAR_LOG_FRAMEWORK_DIR . '/Log.php',
				'medoo'
				=> SwissalpS_FRAMEWORK_DIR . '/_external/medoo.php',
				//'Propel'
				//=> PROPEL_FRAMEWORK_DIR . '/Propel.php',
				'SaferScript'
				=> SwissalpS_FRAMEWORK_DIR . '/_external/SaferScript.inc',
				'Smarty'
				=> SMARTY_DIR . '/Smarty.class.php',
			 	'SssSException'
			 	=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/SssSException.inc',
			 	'SssS_ShellScriptRunner'
			 	=> SwissalpS_FRAMEWORK_DIR . '/SssS_ShellScriptRunner.inc',
			 	'SssS_ShellScriptRunnerForDummies'
			 	=> SwissalpS_FRAMEWORK_DIR . '/SssS_ShellScriptRunnerForDummies.inc',
			 	'SssS_XHTML'
			 	=> SwissalpS_FRAMEWORK_DIR . '/SssS_xhtml.inc',
				'WFException'
			 //   => FRAMEWORK_DIR . '/framework/WFException.php',
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/WFException.inc',
				'WFRequestController'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/WFRequestController.inc',
				'WFRedirectRequestException'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/WFRequestController.inc',
				'WFRequestController_RedirectException'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/WFRequestController.inc',
				'WFRequestController_InternalRedirectException'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/WFRequestController.inc',
				'WFRequestController_NotFoundException'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/WFRequestController.inc',
				'WFRequestController_BadRequestException'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/WFRequestController.inc',
				'WFRequestController_HTTPException'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/WFRequestController.inc',
				'WFSmarty'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/WFSmarty.php',

				// new proposal classes (SwissalpS)
				'WFBundle'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/WFBundle.php',
				'SssSSubmit'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/widgets/SssSSubmit.inc',
				'SssSPaginatorSortLink'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/widgets/SssSPaginatorSortLink.inc',
				'SssSPaginatorPageInfo'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/widgets/SssSPaginatorPageInfo.inc',
				'SssSPaginatorNavigation'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/widgets/SssSPaginatorNavigation.inc',
				'SssSMessageBox'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/widgets/SssSMessageBox.inc',
				'SssSLocalizationTable'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/SssSLocalizationTable.inc',
				'SssSLocalizationHelper'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/SssSLocalizationHelper.inc',
				'SssSLocalizationDelegate'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/SssSLocalizationDelegate.inc',
				'SssSLocalization'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/SssSLocalization.inc',
				'SssSLink'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/widgets/SssSLink.inc',
				'SssSTextField'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/widgets/SssSTextField.inc',
				'SssSLabel'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/widgets/SssSLabel.inc',
				'SssSgdImageBasic'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/SssSgdImageBasic.inc',
				'SssSCheckbox'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/widgets/SssSCheckbox.inc',
				'SssSBlaWidget'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/widgets/SssSBlaWidget.inc',
				'SssSBlaHelper'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/SssSBlaHelper.inc',
				'SssSBla'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/SssSBla.inc',
				'SssSCountries'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/SssSCountries.inc',

				// arbitrary SwissalpS classes

				'SssS_LogRotator'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/SssS_LogRotator.inc',

				'SssSMailer'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/SssSMailer.inc',

				'SssSPlistItem'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/SssSPlistItem.inc',
				'SssSPlistItemHelper'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/SssSPlistItem.inc',

				'SssSURLRewrite'
				=> SwissalpS_FRAMEWORK_DIR . '/_synergize/PHOCOA/SssSURLRewrite.php',

			 	// default phocoa

				'Services_JSON'
				=> FRAMEWORK_DIR . '/libs/JSON.php',

				'Mail_Mailer'
				=> FRAMEWORK_DIR . '/framework/Mailer.php',

				'WFModel'
				=> FRAMEWORK_DIR . '/framework/generator/WFGenerator.php',
				'WFFixture'
				=> FRAMEWORK_DIR . '/framework/generator/WFFixture.php',
				'WFYaml'
				=> FRAMEWORK_DIR . '/framework/util/WFYaml.php',
				'WFJSON'
				=> FRAMEWORK_DIR . '/framework/util/WFJSON.php',
				'WFWebApplication'
				=> FRAMEWORK_DIR . '/framework/WFWebApplication.php',
				'WFMenuTree'
				=> FRAMEWORK_DIR . '/framework/WFMenuItem.php',
				'WFMenuTreeBuilding'
				=> FRAMEWORK_DIR . '/framework/WFMenuItem.php',
				'WFMenuItem'
				=> FRAMEWORK_DIR . '/framework/WFMenuItem.php',
				'WFMenuItemBasic'
				=> FRAMEWORK_DIR . '/framework/WFMenuItem.php',
				'WFObject'
				=> FRAMEWORK_DIR . '/framework/WFObject.php',
				'WFDateTime'
				=> FRAMEWORK_DIR . '/framework/WFDateTime.php',
				'WFDecorator'
				=> FRAMEWORK_DIR . '/framework/WFDecorator.php',
				'WFKeyValueCoding'
				=> FRAMEWORK_DIR . '/framework/WFKeyValueCoding.php',
				'WFKeyValueValidators'
				=> FRAMEWORK_DIR . '/framework/util/WFKeyValueValidators.php',
				'WFSkin'
				=> FRAMEWORK_DIR . '/framework/WFSkin.php',
				'WFModule'
				=> FRAMEWORK_DIR . '/framework/WFModule.php',
				'WFModuleInvocation'
				=> FRAMEWORK_DIR . '/framework/WFModule.php',

				// working
				'WFAction'
				=> FRAMEWORK_DIR . '/framework/WFRPC.php',
				'WFEvent'
				=> FRAMEWORK_DIR . '/framework/WFRPC.php',
				'WFClickEvent'
				=> FRAMEWORK_DIR . '/framework/WFRPC.php',
				'WFJSAction'
				=> FRAMEWORK_DIR . '/framework/WFRPC.php',
				'WFRPC'
				=> FRAMEWORK_DIR . '/framework/WFRPC.php',
				'WFActionResponse'
				=> FRAMEWORK_DIR . '/framework/WFRPC.php',
				'WFActionResponsePlain'
				=> FRAMEWORK_DIR . '/framework/WFRPC.php',
				'WFActionResponseJSON'
				=> FRAMEWORK_DIR . '/framework/WFRPC.php',
				'WFActionResponseXML'
				=> FRAMEWORK_DIR . '/framework/WFRPC.php',
				'WFActionResponseJavascript'
				=> FRAMEWORK_DIR . '/framework/WFRPC.php',

				'WFPage'
				=> FRAMEWORK_DIR . '/framework/WFPage.php',
				'WFPageRendering'
				=> FRAMEWORK_DIR . '/framework/WFPageRendering.php',
				'WFView'
				=> FRAMEWORK_DIR . '/framework/widgets/WFView.php',
				'WFBinding'
				=> FRAMEWORK_DIR . '/framework/WFBinding.php',
				'WFBindingSetup'
				=> FRAMEWORK_DIR . '/framework/WFBinding.php',
				//'WFSmarty' => FRAMEWORK_DIR . '/framework/WFSmarty.php',
				'WFAuthorizationDelegate'
				=> FRAMEWORK_DIR . '/framework/WFAuthorization.php',
				'WFAuthorizationInfo'
				=> FRAMEWORK_DIR . '/framework/WFAuthorization.php',
				'WFAuthorizationException'
				=> FRAMEWORK_DIR . '/framework/WFAuthorization.php',
				'WFAuthorizationManager'
				=> FRAMEWORK_DIR . '/framework/WFAuthorization.php',
				'WFFormatter'
				=> FRAMEWORK_DIR . '/framework/widgets/WFFormatter.php',
				'WFBooleanFormatter'
				=> FRAMEWORK_DIR . '/framework/widgets/WFFormatter.php',
				'WFDateTimeFormatter'
				=> FRAMEWORK_DIR . '/framework/widgets/WFFormatter.php',
				'WFUNIXDateFormatter'
				=> FRAMEWORK_DIR . '/framework/widgets/WFFormatter.php',
				'WFSQLDateFormatter'
				=> FRAMEWORK_DIR . '/framework/widgets/WFFormatter.php',
				'WFNumberFormatter'
				=> FRAMEWORK_DIR . '/framework/widgets/WFFormatter.php',
				'WFSensitiveDataFormatter'
				=> FRAMEWORK_DIR . '/framework/widgets/WFFormatter.php',
				'WFPaginator'
				=> FRAMEWORK_DIR . '/framework/WFPagination.php',
				'WFPaginatorException'
				=> FRAMEWORK_DIR . '/framework/WFPagination.php',
				'WFPagedArray'
				=> FRAMEWORK_DIR . '/framework/WFPagination.php',
				'WFPagedCreoleQuery'
				=> FRAMEWORK_DIR . '/framework/WFPagination.php',
				'WFPagedData'
				=> FRAMEWORK_DIR . '/framework/WFPagination.php',
				'WFDieselSearch'
				=> FRAMEWORK_DIR . '/framework/WFDieselpoint.php',
				'WFDieselSearchHelper'
				=> FRAMEWORK_DIR . '/framework/WFDieselpoint.php',
				'WFDieselKeyword'
				=> FRAMEWORK_DIR . '/framework/widgets/WFDieselKeyword.php',
				'WFDieselNav'
				=> FRAMEWORK_DIR . '/framework/widgets/WFDieselNav.php',
				'WFDieselSearchHelperStateTracking'
				=> FRAMEWORK_DIR . '/framework/WFDieselpoint.php',
				'WFDieselFacet'
				=> FRAMEWORK_DIR . '/framework/widgets/WFDieselFacet.php',
				'WFWidget'
				=> FRAMEWORK_DIR . '/framework/widgets/WFWidget.php',
				'WFDynarchMenu'
				=> FRAMEWORK_DIR . '/framework/widgets/WFDynarchMenu.php',
				'WFDynamic'
				=> FRAMEWORK_DIR . '/framework/widgets/WFDynamic.php',
				'WFSelectionCheckbox'
				=> FRAMEWORK_DIR . '/framework/widgets/WFSelectionCheckbox.php',
				'WFImage'
				=> FRAMEWORK_DIR . '/framework/widgets/WFImage.php',
				'WFForm'
				=> FRAMEWORK_DIR . '/framework/widgets/WFForm.php',
				'WFAutoForm'
				=> FRAMEWORK_DIR . '/framework/widgets/WFAutoForm.php',
				'WFLabel'
				=> FRAMEWORK_DIR . '/framework/widgets/WFLabel.php',
				'WFLink'
				=> FRAMEWORK_DIR . '/framework/widgets/WFLink.php',
				'WFMessageBox'
				=> FRAMEWORK_DIR . '/framework/widgets/WFMessageBox.php',
				'WFPassword'
				=> FRAMEWORK_DIR . '/framework/widgets/WFPassword.php',
				'WFTextField'
				=> FRAMEWORK_DIR . '/framework/widgets/WFTextField.php',
				'WFSearchField'
				=> FRAMEWORK_DIR . '/framework/widgets/WFSearchField.php',
				'WFTextArea'
				=> FRAMEWORK_DIR . '/framework/widgets/WFTextArea.php',
				'WFHTMLArea'
				=> FRAMEWORK_DIR . '/framework/widgets/WFHTMLArea.php',
				'WFSubmit'
				=> FRAMEWORK_DIR . '/framework/widgets/WFSubmit.php',
				'WFSelect'
				=> FRAMEWORK_DIR . '/framework/widgets/WFSelect.php',
				'WFJumpSelect'
				=> FRAMEWORK_DIR . '/framework/widgets/WFJumpSelect.php',
				'WFTimeSelect'
				=> FRAMEWORK_DIR . '/framework/widgets/WFTimeSelect.php',
				'WFHidden'
				=> FRAMEWORK_DIR . '/framework/widgets/WFHidden.php',
				'WFSpamHoneypot'
				=> FRAMEWORK_DIR . '/framework/widgets/WFSpamHoneypot.php',
				'WFCheckbox'
				=> FRAMEWORK_DIR . '/framework/widgets/WFCheckbox.php',
				'WFCheckboxGroup'
				=> FRAMEWORK_DIR . '/framework/widgets/WFCheckboxGroup.php',
				'WFRadio'
				=> FRAMEWORK_DIR . '/framework/widgets/WFRadio.php',
				'WFRadioGroup'
				=> FRAMEWORK_DIR . '/framework/widgets/WFRadioGroup.php',
				'WFUploadError'
				=> FRAMEWORK_DIR . '/framework/widgets/WFUploading.php',
				'WFUploaderUtils'
				=> FRAMEWORK_DIR . '/framework/widgets/WFUploading.php',
				'WFUploadedFile'
				=> FRAMEWORK_DIR . '/framework/widgets/WFUploading.php',
				'WFUploadedFile_Basic'
				=> FRAMEWORK_DIR . '/framework/widgets/WFUploading.php',
				'WFUpload'
				=> FRAMEWORK_DIR . '/framework/widgets/WFUpload.php',
				'WFPostletUpload'
				=> FRAMEWORK_DIR . '/framework/widgets/WFPostletUpload.php',
				'WFHTML5_Uploader'
				=> FRAMEWORK_DIR . '/framework/widgets/WFHTML5_Uploader.php',
				'WFBulkUpload'
				=> FRAMEWORK_DIR . '/framework/widgets/WFBulkUpload.php',
				'WFBulkUploadFile'
				=> FRAMEWORK_DIR . '/framework/widgets/WFBulkUpload.php',
				'WFPaginatorNavigation'
				=> FRAMEWORK_DIR . '/framework/widgets/WFPaginatorNavigation.php',
				'WFPaginatorSortLink'
				=> FRAMEWORK_DIR . '/framework/widgets/WFPaginatorSortLink.php',
				'WFPaginatorSortSelect'
				=> FRAMEWORK_DIR . '/framework/widgets/WFPaginatorSortSelect.php',
				'WFPaginatorState'
				=> FRAMEWORK_DIR . '/framework/widgets/WFPaginatorState.php',
				'WFModuleView'
				=> FRAMEWORK_DIR . '/framework/widgets/WFModuleView.php',
				'WFTabView'
				=> FRAMEWORK_DIR . '/framework/widgets/WFTabView.php',
				'WFTableView'
				=> FRAMEWORK_DIR . '/framework/widgets/WFTableView.php',
				'WFAppcelerator'
				=> FRAMEWORK_DIR . '/framework/widgets/WFAppcelerator.php',
				'WFBreadCrumb'
				=> FRAMEWORK_DIR . '/framework/widgets/WFBreadCrumb.php',
				'WFBreadCrumbSetup'
				=> FRAMEWORK_DIR . '/framework/widgets/WFBreadCrumb.php',
				'WFYAHOO'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO.php',
				'WFYAHOO_widget_YahooScript'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_YahooScript.php',
				'WFYAHOO_yuiloader'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO.php',
				'WFYAHOO_widget_TreeView'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_TreeView.php',
				'WFYAHOO_widget_TreeViewNode'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_TreeView.php',
				'WFYAHOO_widget_Module'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_Module.php',
				'WFYAHOO_widget_Overlay'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_Overlay.php',
				'WFYAHOO_widget_Panel'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_Panel.php',
				'WFYAHOO_widget_Dialog'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_Dialog.php',
				'WFYAHOO_widget_PhocoaDialog'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_PhocoaDialog.php',
				'WFYAHOO_widget_Logger'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_Logger.php',
				'WFYAHOO_widget_Menu'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_Menu.php',
				'WFYAHOO_widget_AutoComplete'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_AutoComplete.php',
				'WFYAHOO_widget_Tab'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_TabView.php',
				'WFYAHOO_widget_TabView'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_TabView.php',
				'WFYAHOO_widget_ColorPicker'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_ColorPicker.php',
				'WFYAHOO_widget_Calendar'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_Calendar.php',
				'WFYAHOO_widget_DateTimePicker'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_DateTimePicker.php',
				'WFYAHOO_widget_Uploader'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_Uploader.php',
				'WFYAHOO_widget_Carousel'
				=> FRAMEWORK_DIR . '/framework/widgets/yahoo/WFYAHOO_widget_Carousel.php',
				'WFPaginatorPageInfo'
				=> FRAMEWORK_DIR . '/framework/widgets/WFPaginatorPageInfo.php',
				'WFValueTransformer'
				=> FRAMEWORK_DIR . '/framework/ValueTransformers/WFValueTransformer.php',
				'WFNegateBooleanTransformer'
				=> FRAMEWORK_DIR . '/framework/ValueTransformers/WFNegateBooleanTransformer.php',
				'WFIsEmptyTransformer'
				=> FRAMEWORK_DIR . '/framework/ValueTransformers/WFIsEmptyTransformer.php',
				'WFEmptyToNullTransformer'
				=> FRAMEWORK_DIR . '/framework/ValueTransformers/WFEmptyToNullTransformer.php',
				'WFIsNotEmptyTransformer'
				=> FRAMEWORK_DIR . '/framework/ValueTransformers/WFIsNotEmptyTransformer.php',
				'WFUrlencodeTransformer'
				=> FRAMEWORK_DIR . '/framework/ValueTransformers/WFUrlencodeTransformer.php',
				'WFObjectController'
				=> FRAMEWORK_DIR . '/framework/WFObjectController.php',
				'WFArrayController'
				=> FRAMEWORK_DIR . '/framework/WFArrayController.php',
				'WFArray'
				=> FRAMEWORK_DIR . '/framework/WFArray.php',
				'WFDictionary'
				=> FRAMEWORK_DIR . '/framework/WFDictionary.php',
				'WFError'
				=> FRAMEWORK_DIR . '/framework/WFError.php',
				'WFErrorArray'
				=> FRAMEWORK_DIR . '/framework/WFError.php',
				'WFErrorsException'
				=> FRAMEWORK_DIR . '/framework/WFError.php',
				'WFErrorCollection'
				=> FRAMEWORK_DIR . '/framework/WFError.php',
				'WFExceptionReporting'
				=> FRAMEWORK_DIR . '/framework/WFExceptionReporting.php',
				'WFFunction'
				=> FRAMEWORK_DIR . '/framework/WFFunction.php',
				'WFUnixDateFormatter'
				=> FRAMEWORK_DIR . '/framework/widgets/WFFormatter.php',
				'WFSQLDateFormatter'
				=> FRAMEWORK_DIR . '/framework/widgets/WFFormatter.php',
				'WFNumberFormatter'
				=> FRAMEWORK_DIR . '/framework/widgets/WFFormatter.php',
				'FCKEditor'
				=> FRAMEWORK_DIR . '/wwwroot/www/framework/FCKEditor/fckeditor.php',

				// propel
				'WFPagedPropelQuery'
				=> FRAMEWORK_DIR . '/framework/WFPagination.php',
				'WFPagedPropelModelCriteria'
				=> FRAMEWORK_DIR . '/framework/WFPagination.php',
				'WFModelBuilderPropel'
				=> FRAMEWORK_DIR . '/framework/generator/WFGeneratorPropel12_13.php',
				'WFModelBuilderPropel12'
				=> FRAMEWORK_DIR . '/framework/generator/WFGeneratorPropel12_13.php',
				'WFModelBuilderPropel13'
				=> FRAMEWORK_DIR . '/framework/generator/WFGeneratorPropel12_13.php',
				'WFModelBuilderPropel15'
				=> FRAMEWORK_DIR . '/framework/generator/WFGeneratorPropel15.php',
				'WFModelBuilderPropel2'
				=> FRAMEWORK_DIR . '/framework/generator/WFGeneratorPropel2.php',
				'WFPropelException'
				=> FRAMEWORK_DIR . '/framework/util/WFPropelIntegration.php',
				'WFObject_Propel'
				=> FRAMEWORK_DIR . '/framework/util/WFPropelIntegration.php',

				// PHPUnit
				'PhocoaControllerTestCase_PHPUnit' => FRAMEWORK_DIR . '/phpunit/PhocoaControllerTestCase_PHPUnit.php',
			);

			// windows box or other non posix '\\'
			str_replace(array('/', '\\'), DIR_SEP, $hAutoloadClassmapCache);
		} // if not yet built cache map

		if (!IS_PRODUCTION) {

			$mPos = array_search($sClassName, $aDepricatedClasses);

			if (false !== $mPos) {

				$sClassNameNew = $aReplacementClasses[$mPos];

				throw new Exception('WARNING: class ' . $sClassName
									. ' is depricated. Use ' . $sClassNameNew
									. ' instead.');

			} // if depricated class

		} // if not production mode

		if (isset($hAutoloadClassmapCache[$sClassName])) {
			// including absolute paths is much faster than relative paths to the include_path dirs because one doesn't have to walk the include path.
			// so, if it's a framework/ dir, then include it absolutely!
			$sRequirePath = $hAutoloadClassmapCache[$sClassName];

		} // if got cached path
		else if ('SssS' == substr($sClassName, 0, 4)) {
			// TODO: check if this is sufficient
			if (!$aDirsToCheck) {
				$aSuffixesToTry = array('.inc', '.liz', '.zil', '.php', '.tmf');
				$aDirsToCheck = array(
					SwissalpS_FRAMEWORK_DIR . DIR_SEP,
					APP_ROOT . DIR_SEP . 'classes' . DIR_SEP);
			} // if got cached

			foreach ($aDirsToCheck as $sDir) {
				$sTestPathWithoutSuffix = $sDir . $sClassName;
				foreach ($aSuffixesToTry as $sSuffix) {
					if (is_file($sTestPathWithoutSuffix . $sSuffix)) {
						$sRequirePath = $sTestPathWithoutSuffix . $sSuffix;
						require($sRequirePath);
						return true;
					} // if found a file
				} // foreach suffix
			} // foreach dir

		} // if starts with SssS -> probably from SwissalpS

		if (null !== $sRequirePath) { require($sRequirePath); return true; }

		if (is_file(APP_ROOT . DIR_SEP . 'classes'
									. DIR_SEP . $sClassName . '.php')) {
			$sRequirePath = APP_ROOT . DIR_SEP . 'classes'
									. DIR_SEP . $sClassName . '.php';
			require($sRequirePath);
			return true;
		} // if in apps root classes

		// give appdelegate a shot at it
		$oApp = WFWebApplication::sharedWebApplication();
		return $oApp->autoload($sClassName);
	} // autoload


	public static function autoloadPSR($aNamespaceParts) {

		$sNamespaceBase = $aNamespaceParts[0];
		$sNamespacePartsPath = DIR_SEP . implode(DIR_SEP, $aNamespaceParts);
		$sFullClassName = implode('\\', $aNamespaceParts);
		$sPathVendor = dirname(APP_ROOT) . DIR_SEP . 'vendor';
//var_dump('--', $sNamespaceBase, $sNamespacePartsPath, $sFullClassName, '/-');
		$sPathFile = null;
		switch($sNamespaceBase) {
			case 'Monolog':
				$sPathFile = dirname(PROPEL_FRAMEWORK_DIR) . '/vendor/monolog/monolog/src' . $sNamespacePartsPath . '.php';
				break;

			case 'Propel':
				$sPathFile = PROPEL_FRAMEWORK_DIR . $sNamespacePartsPath . '.php';
				break;

			case 'Psr':
				$sPathFile = dirname(PROPEL_FRAMEWORK_DIR) . '/vendor/psr/log' . $sNamespacePartsPath . '.php';
				break;

			// generated from database with Propel2
			case 'SssSPropel2':
				$sPathFile = APP_ROOT . DIR_SEP . 'classes' . $sNamespacePartsPath . '.php';
				break;

			case 'SwissalpS':
				$sPathFile = SwissalpS_FRAMEWORK_DIR . substr($sNamespacePartsPath, 10) . '.inc';
				break;

			default;
				// try vendor in project dir (composer)
				$sPathFile = $sPathVendor . $sNamespacePartsPath . '.php';

		} // switch case

//var_dump('got path file', $sPathFile);
		if (is_readable($sPathFile)) {
//var_dump('is readable');
			require_once($sPathFile);
			// 9 == strlen('Interface')
			if ('Interface' == substr($sNamespacePartsPath, -9)) {
				if (interface_exists($sFullClassName)) {
					return true;
				} // if interface now exists
			} // ends in 'Interface' may thus be an interface
//var_dump('==', $bRes, 'class ' . $sFullClassName . ' exists now?', class_exists($sFullClassName), '/=');
			if (class_exists($sFullClassName)) {
				return true;
			} // if class now exists
			return false;
		} // if readable file
//var_dump('is not readable');
		// failed to load
		return false;

	} // autoloadPSR

} // WFIncluding
/* * * *\ WFIncluding.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
