<?php

/* vim: set expandtab tabstop=4 shiftwidth=4: */
/* * * *
 * * SssS/_synergize/PHOCOA/WFRequestController.inc
 * *
 * * originates phocoa/framework/WFRequestController.php
 * *
 * * @version 20110107_144054 (CC) Luke JZ aka SwissalpS
 * * * */
/**
 * @package WebApplication
 * @copyright Copyright (c) 2005 Alan Pinstein. All Rights Reserved.
 * @version $Id: kvcoding.php,v 1.3 2004/12/12 02:44:09 alanpinstein Exp $
 * @author Alan Pinstein <apinstein@mac.com>
 */

/**
 * The RequestController object is a singleton controller for the entire request-act-respond cycle.
 *
 * Basically, the WFRequestController bootstraps the request into a WFModuleInvocation, executes it, and displays the output.
 *
 * It also has a top-level exception catcher for all uncaught exceptions and displays a friendly error message (or an informative one for development machines).
 */
class WFRequestController extends WFObject
{
    /**
     * @var object The root WFModuleInvocation object used for the request.
     */
    protected $rootModuleInvocation;

    function __construct()
    {
    }

    /* These are the errors that phocoa tries to roll-up and catch and report on. All other errors will be left to their default handling.
             E_ERROR
       //  | E_WARNING
           | E_PARSE
       //  | E_NOTICE
           | E_CORE_ERROR
           | E_CORE_WARNING
           | E_COMPILE_ERROR
           | E_COMPILE_WARNING
           | E_USER_ERROR
       //  | E_USER_WARNING
       //  | E_USER_NOTICE
       //  | E_STRICT
           | E_RECOVERABLE_ERROR
       //  | E_DEPRECATED
       //  | E_USER_DEPRECATED
     */
    private $handleErrors = 4597;

    private function phpErrorAsString($e)
    {
        $el = array(
            1     => 'E_ERROR',
            2     => 'E_WARNING',
            4     => 'E_PARSE',
            8     => 'E_NOTICE',
            16    => 'E_CORE_ERROR',
            32    => 'E_CORE_WARNING',
            64    => 'E_COMPILE_ERROR',
            128   => 'E_COMPILE_WARNING',
            256   => 'E_USER_ERROR',
            512   => 'E_USER_WARNING',
            1024  => 'E_USER_NOTICE',
            2048  => 'E_STRICT',
            4096  => 'E_RECOVERABLE_ERROR',
            8192  => 'E_DEPRECATED',
            16384 => 'E_USER_DEPRECATED',
        );
        return $el[$e];
    }

    /**
     * Error handler callback for PHP catchable errors; helps synthesize PHP errors and exceptions into the same handling workflow.
     */
    function handleError($errNum, $errString, $file, $line, $contextArray)
    {
        $errNum = $this->phpErrorAsString($errNum);

        $this->handleException( new ErrorException("{$errNum}: {$errString}\n\nAt {$file}:{$line}") );
    }

    /**
     * Error handler callback for PHP un-catchable errors; helps synthesize PHP errors and exceptions into the same handling workflow.
     */
    function checkShutdownForFatalErrors()
    {
        $last_error = error_get_last();
        if ($last_error['type'] & $this->handleErrors)
        {
            $last_error['type'] = $this->phpErrorAsString($last_error['type']);
            $this->handleException( new ErrorException("{$last_error['type']}: {$last_error['message']}\n\nAt {$last_error['file']}:{$last_error['line']}") );
        }
    }

    /**
     * PHOCOA's default error handling is to try to catch *all* fatal errors and run them through the framework's error handling system.
     *
     * The error handling system unifies excptions and errors into the same processing stream and additionally gives your application
     * an opportunity to handle the error as well. For instance your app may prefer to email out all errors,
     * or send them to an exception service like Hoptoad/Exceptional/Loggly
     */
    private function registerErrorHandlers()
    {
        // convert these errors into exceptions
        set_error_handler(array($this, 'handleError'), $this->handleErrors);

        // catch non-catchable errors
        register_shutdown_function(array($this, 'checkShutdownForFatalErrors'));
    }

    /**
      * Exception handler for the WFRequestController.
      *
      * This is basically the uncaught exception handler for the request cycle.
      * We want to have this in the request object because we want the result to be displayed within our skin system.
      * This function will display the appropriate error page based on the deployment mode for this machine, then exit.
      *
      * @param Exception The exception object to handle.
      */
    function handleException(Exception $e)
    {
        // give ourselves a little more memory so we can process the exception
        ini_set('memory_limit', memory_get_usage() + 25000000 /* 25MB */);

		// let the current module try to handle the exception
        if ($this->rootModuleInvocation)
        {
            $this->rootModuleInvocation->handleUncaughtException($e);
        }

        $webAppDelegate = WFWebApplication::sharedWebApplication()->delegate();
        if (is_object($webAppDelegate) && method_exists($webAppDelegate, 'handleUncaughtException'))
        {
            $handled = $webAppDelegate->handleUncaughtException($e);
            if ($handled) return;
        }

        WFExceptionReporting::log($e);

        // build stack of errors (php 5.3+)
        if (method_exists($e, 'getPrevious'))
        {
            $tmpE = $e;
            $allExceptions = array();
            do {
                $allExceptions[] = $tmpE;
            } while ($tmpE = $tmpE->getPrevious());
        }
        else
        {
            $allExceptions = array($e);
        }

        $exceptionPage = new WFSmarty();
        $exceptionPage->assign('exceptions', $allExceptions);
        $exceptionPage->assign('exceptionClass', get_class($allExceptions[0]));
        $exceptionPage->assign('home_url', WWW_ROOT . '/');

        // modern format
        $standardErrorData = WFExceptionReporting::generatedStandardizedErrorDataFromException($e);
        $exceptionPage->assign('location', "http://{$_SERVER['HTTP_HOST']}{$_SERVER['REQUEST_URI']}");
        $exceptionPage->assign('headline', "{$standardErrorData[0]['title']}: {$standardErrorData[0]['message']}");
        $exceptionPage->assign('standardErrorData', $standardErrorData);
        $exceptionPage->assign('standardErrorDataJSON', WFJSON::encode(array(
            'error'     => $standardErrorData,
            '$_SERVER'  => $_SERVER,
            '$_REQUEST' => $_REQUEST,
            '$_SESSION' => $_SESSION,
        )));
        // @todo refactor these templates to use WFExceptionReporting::generatedStandardizedErrorDataFromException($e)
        if (IS_PRODUCTION)
        {
            $exceptionPage->setTemplate(WFWebApplication::appDirPath(WFWebApplication::DIR_SMARTY) . '/app_error_user.tpl');
        }
        else
        {
            $exceptionPage->setTemplate(WFWebApplication::appDirPath(WFWebApplication::DIR_SMARTY) . '/app_error_developer.tpl');
        }

        // display the error and exit
        $body_html = $exceptionPage->render(false);

        // output error info
        header("HTTP/1.0 500 Uncaught Exception");
        if (self::isAjax())
        {
            print strip_tags($body_html);
        }
        else
        {
            $skin = new WFSkin();
            $skin->setDelegateName(WFWebApplication::sharedWebApplication()->defaultSkinDelegate());
            $skin->setBody($body_html);
            $skin->setTitle("An error has occurred.");
            $skin->render();
        }
        exit;
    }

    /**
     * Run the web application for the current request.
     *
     * NOTE: Both a module and page must be specified in the URL. If they are not BOTH specified, the server will REDIRECT the request to the full URL.
     *       Therefore, you should be sure that when posting form data to a module/page, you use a full path. {@link WFRequestController::WFURL}
     *
     * Will pass control onto the current module for processing.
     *
     * Create a WFModuleInvocation based on the current HTTP Request, get the results, and output the completed web page.
     *
     * @todo Handle 404 situation better -- need to be able to detect this nicely from WFModuleInvocation.. maybe an Exception subclass?
     * @todo PATH_INFO with multiple params, where one is blank, isn't working correctly. IE, /url/a//c gets turned into /url/a/c for PATH_INFO thus we skip a "null" param.
     *       NOTE: Partial solution; use /WFNull/ to indicate NULL param instead of // until we figure something out.
     *       NOTE: Recent change to REQUEST_URI instead of PATH_INFO to solve decoding problem seems to have also solved the // => / conversion problem... test more!
     *       WORRY: That the new PATH_INFO calculation will fail when using aliases other than WWW_ROOT. IE: /products/myProduct might break it...
     * @todo The set_error_handler doesn't seem to work very well. PHP issue? Or am I doing it wrong? For instance, it doesn't catch $obj->nonExistantMethod().
     */
    function handleHTTPRequest()
    {
        // point all error handling to phocoa's internal mechanisms since anything that happens after this line (try) will be routed through the framework's handler
        $this->registerErrorHandlers();
        try {
            $relativeURI = parse_url($_SERVER['REQUEST_URI'],PHP_URL_PATH); // need to run this to convert absolute URI's to relative ones (sent by SOME http clients)
            if ($relativeURI === false) throw new WFRequestController_NotFoundException("Malformed URI: {$_SERVER['REQUEST_URI']}");
            if (('' !== WWW_ROOT) && (0 === strpos($relativeURI, WWW_ROOT)))
            {
                $relativeURI = substr($relativeURI, strlen(WWW_ROOT));
            } // if path begins with WWW_ROOT, remove that
            $modInvocationPath = ltrim($relativeURI, '/');
        	$paramsPos = strpos($modInvocationPath, '?');
        	if ($paramsPos !== false)
        	{
            	$modInvocationPath = substr($modInvocationPath, 0, $paramsPos);
            } // if there are _GET arguments, drop them

            if ($modInvocationPath == '')
            {
                $modInvocationPath = WFWebApplication::sharedWebApplication()->defaultInvocationPath();
            }
            // allow routing delegate to munge modInvocationPath
            $webAppDelegate = WFWebApplication::sharedWebApplication()->delegate();
            if (is_object($webAppDelegate) && method_exists($webAppDelegate, 'rerouteInvocationPath'))
            {
                $newInvocationPath = $webAppDelegate->rerouteInvocationPath($modInvocationPath);
                if ($newInvocationPath)
                {
                    $modInvocationPath = $newInvocationPath;
                }
            }

            // create the root invocation; only skin if we're not in an XHR
            $this->rootModuleInvocation = new WFModuleInvocation($modInvocationPath, NULL, (self::isAjax() ? NULL : WFWebApplication::sharedWebApplication()->defaultSkinDelegate()) );
            // get HTML result of the module, and output it
            $html = $this->rootModuleInvocation->execute();

            // respond to WFRPC::PARAM_ENABLE_AJAX_IFRAME_RESPONSE_MODE for iframe-targeted XHR. Some XHR requests (ie uploads) must be done by creating an iframe and targeting the form
            // post to the iframe rather than using XHR (since XHR doesn't support uploads methinks). This WFRPC flag makes these such "ajax" requests need to be wrapped slightly differently
            // to prevent the HTML returned in the IFRAME from executing in the IFRAME which would cause errors.
            if (isset($_REQUEST[WFRPC::PARAM_ENABLE_AJAX_IFRAME_RESPONSE_MODE]) && $_REQUEST[WFRPC::PARAM_ENABLE_AJAX_IFRAME_RESPONSE_MODE] == 1)
            {
                header('Content-Type: text/xml');
                $html = "<"."?xml version=\"1.0\"?"."><raw><![CDATA[\n{$html}\n]]></raw>";
            }

            print $html;
        } catch (WFPaginatorException $e) {
            // paginator fails by default should "route" to bad request. This keeps bots from going crazy.
            header("HTTP/1.0 400 Bad Request");
            print "Bad Request: " . $e->getMessage();
            exit;
        } catch (WFRequestController_RedirectException $e) {
            header("HTTP/1.1 {$e->getCode()}");
            header("Location: {$e->getRedirectURL()}");
            exit;
        } catch (WFRequestController_HTTPException $e) {
            header("HTTP/1.0 {$e->getCode()}");
            print $e->getMessage();
            exit;
        } catch (WFRequestController_BadRequestException $e) {
            header("HTTP/1.0 400 Bad Request");
            print "Bad Request: " . $e->getMessage();
            exit;
        } catch (WFRequestController_NotFoundException $e) {
            header("HTTP/1.0 404 Not Found");
            print $e->getMessage();
            exit;
        } catch (WFRequestController_InternalRedirectException $e) {
            // internal redirect are handled without going back to the browser... a little bit of hacking here to process a new invocationPath as a "request"
            // @todo - not sure what consequences this has on $_REQUEST; seems that they'd probably stay intact which could foul things up?
            $_SERVER['REQUEST_URI'] = $e->getRedirectURL();
            WFLog::log("Internal redirect to: {$_SERVER['REQUEST_URI']}");
            self::handleHTTPRequest();
            exit;
        } catch (WFRedirectRequestException $e) {
            header("Location: " . $e->getRedirectURL());
            exit;
        } catch (Exception $e) {
            $this->handleException($e);
        }
    } // handleHTTPRequest

    // returns an array with the headers as key value pair
    // $s is expected to be: base64encodedKey0.base64encodedValue0..base64encodedKeyN.base64encodedValueN
    function decodeCLIHeader($s) {
    	$aHeaders = array();
    	$aRawLines = explode('..', $s);
    	foreach ($aRawLines as $sLine) {
    		$aPair = explode('.', $sLine);
    		if (2 == count($aPair)) {
    			$sKey = base64_decode($aPair[0]);
    			$sValue = base64_decode($aPair[1]);
    			$aHeaders[strtoupper($sKey)] = $sValue;
    		} // if got a pair
    	} // foreach line

    	return $aHeaders;

		// more dangerous and possibly slower way: $aHeaders = @eval('return ' . $s . ';');
		//where $s was something like: array('METHOD'=>'GET','USER-AGENT'=>'Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_5_8; en-us) AppleWebKit/531.22.7 (KHTML, like Gecko) Version/4.0.5 Safari/531.22.7','CACHE-CONTROL'=>'max-age=0','HOST'=>'lan.wg7.swissalps.ws:8080','ACCEPT-LANGUAGE'=>'en-us','ACCEPT-ENCODING'=>'gzip, deflate','REFERER'=>'http://lan.wg7.swissalps.ws:8080/','ACCEPT'=>'*/*','CONNECTION'=>'keep-alive')
	} // decodeCLIHeader


    /**
     * Run the cli application for the current request.
     * added by SwissalpS 20100501_025124
     */
    function handleCLIRequest() {
        // point all error handling to phocoa's internal mechanisms since anything that happens after this line (try) will be routed through the framework's handler
        $this->registerErrorHandlers();

		static $nl = null;

        try {

        	if (!$nl) {
				// prepare iphp
				$nl = chr(10);
			} // first run

			// TODO: populate (global vars) with args to mimic an http request
			// read arguments, set flags and return invocation path
			list($bInteractive, $modInvocationPath) = self::parseCliRequest();

			// create the root invocation; only skin if we're not in an XHR
			$this->rootModuleInvocation = new WFModuleInvocation($modInvocationPath, NULL, (self::isAjax() ? NULL : WFWebApplication::sharedWebApplication()->defaultSkinDelegate()));
			// get HTML result of the module, and output it

			$html = $this->rootModuleInvocation->execute();

			// respond to WFRPC::PARAM_ENABLE_AJAX_IFRAME_RESPONSE_MODE for iframe-targeted XHR. Some XHR requests (ie uploads) must be done by creating an iframe and targeting the form
			// post to the iframe rather than using XHR (since XHR doesn't support uploads methinks). This WFRPC flag makes these such "ajax" requests need to be wrapped slightly differently
			// to prevent the HTML returned in the IFRAME from executing in the IFRAME which would cause errors.
			if (isset($_REQUEST[WFRPC::PARAM_ENABLE_AJAX_IFRAME_RESPONSE_MODE])
					&& (1 == $_REQUEST[WFRPC::PARAM_ENABLE_AJAX_IFRAME_RESPONSE_MODE])) {
				header('Content-Type: text/xml');
				$html = "<?xml version=\"1.0\"?><raw><![CDATA[\n{$html}\n]]></raw>";
			} // if ajax special situation with iframe
			// TODO: print headers headers_list does not seem to work properly is this context
			print implode("\r\n", headers_list()) . "\r\n\r\n";
			//WFLog::logToFile('cliHeaders.log', implode("\r\n", headers_list()));
			print $html;
			//print phpinfo();

			/*
			this would require explodeing args and cocoa side would need to be aware as well and stay alive and adapt syntax. would certainly increase speed
			$iTime = time();
			while ('keep-alive' == strtolower($_SERVER['HTTP_CONNECTION']) || time() < $iTime + $iTimeOut) {
				$sReply = trim(fgets(STDIN));
			} // -cgi keep-alive
			*/

            $oMenuMain = new SssSCLI_Menu_Main();
            $oMenuMain->ask();
            return;

        } catch (WFRequestController_HTTPException $e) {
            //header("HTTP/1.0 {$e->getCode()}");
            print "HTTP/1.0 " . $e->getCode() . ' ' . $e->getMessage() . "\r\n\r\n";
           // echo 0x00;
            return; // TODO: if -interactive reset the request to something that works

        } catch (WFRequestController_BadRequestException $e) {
            //header("HTTP/1.0 400 Bad Request");
            print "HTTP/1.0 400 Bad Request: " . $e->getMessage() . "\r\n\r\n";
            return;

        } catch (WFRequestController_NotFoundException $e) {
            //header("HTTP/1.0 404 Not Found");
			//print implode("\r\n", headers_list()) . "\r\n\r\n";
            print 'HTTP/1.0 404 Not Found ' . $e->getMessage() . "\r\n\r\n";
            return;

        } catch (WFRequestController_InternalRedirectException $e) {
            // internal redirect are handled without going back to the browser... a little bit of hacking here to process a new invocationPath as a "request"
            // @todo - not sure what consequences this has on $_REQUEST; seems that they'd probably stay intact which could foul things up?
            $_SERVER['REQUEST_URI'] = $e->getRedirectURL();
            WFLog::log("Internal redirect to: {$_SERVER['REQUEST_URI']}");
            print 'Internal redirect to: ' . $e->getRedirectURL() . $nl;
            array_splice($_SERVER['argv'], 1, 0, array('-iredirect', $e->getRedirectURL()));
            self::handleCLIRequest();
            return;

        } catch (WFRequestController_RedirectException $e) {
            //header("Location: " . $e->getRedirectURL());
            print 'attempt to redirect to: ' . $e->getRedirectURL() . $nl;
            array_splice($_SERVER['argv'], 1, 0, array('-redirect', $e->getRedirectURL()));
            self::handleCLIRequest();
            return;

        } catch (WFRedirectRequestException $e) {
           // header("Location: " . $e->getRedirectURL());
            print 'attempt to redirect 2: ' . $e->getRedirectURL() . $nl;
            array_splice($_SERVER['argv'], 1, 0, array('-2redirect', $e->getRedirectURL()));
            self::handleCLIRequest();
            return;

        } catch (Exception $e) {
			if (is_object($oAppDelegate) // this is hopefully a cli-aware delegate
				&& method_exists($oAppDelegate, 'handleUncaughtException')) {

				$handled = $oAppDelegate->handleUncaughtException($e);
				if ($handled) {
					if ($bInteractive) self::handleCLIRequest();
					else return;
				} // if handled
        	} // give app delegate a shot

            // well html is better than nothing...
            $this->handleException($e);

        } // try catch

    } // handleCLIRequest


	static function setEnvForPhocoaPhingPropel() {

		static $bDone = false;
		if ($bDone) return true;

		//if (!getenv('PROPEL_GEN_HOME')) putenv('PROPEL_GEN_HOME', //SwissalpS_FRAMEWORK_DIR .
		//'/Volumes/UNIX/ImgSrach/public/SssS/_synergize/PHOCOA/propel_generator-1.3'
		//'/Volumes/UNIX/ImgSrach/public/SssS/_synergize/PHOCOA/propel-1.4/data'
					//);

		ini_set('include_path', '/Volumes/UNIX/ImgSrach/public/SssS/_synergize/PHOCOA/propel-1.4:' . ini_get('include_path'));
		if (!getenv('PHOCOA_HOME')) putenv('PHOCOA_HOME=' . FRAMEWORK_DIR);
		if (!getenv('PHP_CLASSPATH')) putenv('PHP_CLASSPATH=' . PHING_PATH);
		if (!getenv('PHING_COMMAND')) putenv('PHING_COMMAND=' . SssS_ShellScriptRunner::getPHPinterpreterPath() . ' -d html_errors=off -qC ' . PHING_PATH . DIR_SEP . 'phing.php -logger phing.listener.DefaultLogger');
		// . SssS_ShellScriptRunner::getPHPinterpreterPath() . ' -d html_errors=off -qC ' . PHING_PATH . DIR_SEP . 'phing.php -logger phing.listener.AnsiColorLogger -debug');-debug -l

		if (!getenv('PHOCOA_PROJECT_CONF')) putenv('PHOCOA_PROJECT_CONF=' . APP_ROOT . DIR_SEP . 'conf' . DIR_SEP . 'SssSphocoaApp.conf');

		$bDone = true;

		return $bDone;

	} // setEnvForPhocoaPhingPropel


	static function setEnvForPhocoaPropel() {

		static $bDone = false;
		if ($bDone) return true;

		//if (!getenv('PHOCOA_HOME')) putenv('PHOCOA_HOME=' . FRAMEWORK_DIR);
		//if (!getenv('PHP_CLASSPATH')) putenv('PHP_CLASSPATH=' . PHING_PATH);
		//if (!getenv('PHING_COMMAND')) putenv('PHING_COMMAND=' . SssS_ShellScriptRunner::getPHPinterpreterPath() . ' -d html_errors=off -qC ' . PHING_PATH . DIR_SEP . 'phing.php -logger phing.listener.DefaultLogger');
		// . SssS_ShellScriptRunner::getPHPinterpreterPath() . ' -d html_errors=off -qC ' . PHING_PATH . DIR_SEP . 'phing.php -logger phing.listener.AnsiColorLogger -debug');-debug -l

		if (!getenv('PHOCOA_PROJECT_CONF')) putenv('PHOCOA_PROJECT_CONF=' . APP_ROOT . DIR_SEP . 'conf' . DIR_SEP . 'SssSphocoaApp.conf');

		$bDone = true;

		return $bDone;

	} // setEnvForPhocoaPropel


	function parseCliRequest() {

		$bInteractive = false;
		$sModulePath = null;
		$sMethod = null;

		if (isset($_SERVER['argv'])) {

			$a = $_SERVER['argv'];
			$aNewArgs = array(array_shift($a));

			// filter out flags etc, leave rest for module, it might be cli-aware
			while ($sArg = array_shift($a)) {
				switch ($sArg) {
					case '-redirect' :
					case '-2redirect' :
					case '-iredirect' : $sModulePath = array_shift($a);
						break;

					case '-H' :
					case '--header' : $aHeader = $this->decodeCLIHeader(array_shift($a));
						$sMethod = $aHeader['METHOD']; unset($aHeader['METHOD']);
						if (!empty($sMethod)) $_SERVER['REQUEST_METHOD'] = strtoupper($sMethod); // cocoa app alread uppercases but will all? TODO: fix
						foreach($aHeader as $sKey => $sValue) {
							switch ($sKey) {
								case 'USER-AGENT' :
									$_SERVER['HTTP_USER_AGENT'] = $sValue;
									break;
								case 'CACHE-CONTROL' : // TODO: solve this
									break;
								case 'HOST' :
									$_SERVER['HTTP_HOST'] = $sValue;
									break;
								case 'ACCEPT-LANGUAGE' :
									$_SERVER['HTTP_ACCEPT_LANGUAGE'] = $sValue;
									break;
								case 'ACCEPT-ENCODING' :
									$_SERVER['HTTP_ACCEPT_ENCODING'] = $sValue;
									break;
								case 'REFERER' :
									$_SERVER['HTTP_REFERER'] = $sValue;
									break;
								case 'ACCEPT' :
									$_SERVER['HTTP_ACCEPT'] = $sValue;
									break;
								case 'CONNECTION' :
									$_SERVER['HTTP_CONNECTION'] = $sValue;
									break;
								default :
									$_SERVER[$sKey] = $sValue; // TODO: solve this
									WFLog::logToFile('cliHTTPrequestWarnings.log', 'unknown header!! DANGER !! added'
										. ' unchecked to $_SERVER[' . $sKey
										. '] = ' . $sValue);
									break;
							} // switch key
						} // foreach header
						break;

					case '-interactive' :
					case '-cli' : $bInteractive = '-cli';
						break;

					case '-stayalive' : case '-stayAlive' :
						$bInteractive = '-stayalive';
						break;

					case '-cgi' : //  $bInteractive = '-cgi'; // see -H for now
						break;

					default: $aNewArgs[] = $sArg;
				} // switch arg
			} // while args

			if (null == $sModulePath && 1 < count($aNewArgs)) {
				for ($x = 1; $x < count($aNewArgs); $x++) {
					$s = $aNewArgs[$x];

					if ('-' == $s{0}) continue;

					list($sModulePath) = array_splice($aNewArgs, $x, 1);
					break;
				} // for finding a non-flag ... hoping a previous flag doesn't indicate a comming value
			} // if no modulePath found

			$_SERVER['argv'] = $aNewArgs;

		} else {
			// don't know if this could happen at all
			$_SERVER['argv'] = array('no argv');
		} //

		if ($bInteractive) $_SERVER['argv'][] = $bInteractive;

		if (empty($sModulePath)) $sModulePath = WFWebApplication::sharedWebApplication()->defaultInvocationPath();

		//array_splice($_SERVER['argv'], 1, 0, array($sModulePath));

		// fake the uri
		$_SERVER['REQUEST_URI'] = $sModulePath;
		$sRelativeURI = parse_url($_SERVER['REQUEST_URI'], PHP_URL_PATH);

		$sModulePath = ltrim(substr($sRelativeURI, strlen(WWW_ROOT)), '/');

		// don't like asking again, but do we have something?
		if (empty($sModulePath)) $sModulePath = WFWebApplication::sharedWebApplication()->defaultInvocationPath();

		// TODO: respect settings for GET POST REQUEST... when merging inputs
		$aUrlParts = parse_url($sModulePath);
		if (isset($aUrlParts['query'])) {
			if (empty($sMethod)) { $sMethod = $_SERVER['REQUEST_METHOD'] = 'GET'; }
			parse_str($aUrlParts['query'], $aParams);
			array_merge($_GET, $aParams);
		} // if got args

		// allow routing delegate to munge modInvocationPath
		$oWebAppDelegate = WFWebApplication::sharedWebApplication()->delegate();
		if (is_object($oWebAppDelegate) && method_exists($oWebAppDelegate, 'rerouteInvocationPath')) {

			$sNewInvocationPath = $oWebAppDelegate->rerouteInvocationPath($sModulePath);
			if ($sNewInvocationPath) {

				$sModulePath = $sNewInvocationPath;

			} // if got path rewriten

		} // if app-delegate responds to rerouteInvocationPath()

		// filter out file requests, framework resources..
		$bServeYourself = (bool)SssSURLRewrite::doIt(array(
			'logFile' => LOG_DIR . '/SssSURLRewrite.log',
			'sIndexPHP' => null,
			'sAppDir' => APP_ROOT));

		if ($bServeYourself) return array($bInteractive, $sModulePath);

		// TODO: really? or do we stay interactive?
		//print chr(10) . chr(13) . chr(10);
		exit(0);

	} // parseCliRequest

    /**
     *  Is the current request an XHR (XmlHTTPRequest)?
     *
     *  @return boolean
     */
	static function isAjax()
    {
        if (isset($_SERVER['HTTP_X_REQUESTED_WITH']) and strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest') return true;
        if (isset($_REQUEST['HTTP_X_REQUESTED_WITH']) and strtolower($_REQUEST['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest') return true;  // for debugging
        return false;
    }

    /**
     * Determine whether the current request is from a mobile browers
     *
     *
     * @return boolean
     */
    private $isMobileBrowser = NULL;
    function isMobileBrowser()
    {
        if ($this->isMobileBrowser !== NULL) return $this->isMobileBrowser;

        $op = (isset($_SERVER['HTTP_X_OPERAMINI_PHONE']) ? strtolower($_SERVER['HTTP_X_OPERAMINI_PHONE']) : '');
        $ua = (isset($_SERVER['HTTP_USER_AGENT']) ? strtolower($_SERVER['HTTP_USER_AGENT']) : '');
        $ac = (isset($_SERVER['HTTP_ACCEPT']) ? strtolower($_SERVER['HTTP_ACCEPT']) : '');

        $this->isMobileBrowser = strpos($ac, 'application/vnd.wap.xhtml+xml') !== false
                    || $op != ''
                    || strpos($ua, 'sony') !== false
                    || strpos($ua, 'symbian') !== false
                    || strpos($ua, 'nokia') !== false
                    || strpos($ua, 'samsung') !== false
                    || strpos($ua, 'mobile') !== false
                    || strpos($ua, 'windows ce') !== false
                    || strpos($ua, 'epoc') !== false
                    || strpos($ua, 'opera mini') !== false
                    || strpos($ua, 'nitro') !== false
                    || strpos($ua, 'j2me') !== false
                    || strpos($ua, 'midp-') !== false
                    || strpos($ua, 'cldc-') !== false
                    || strpos($ua, 'netfront') !== false
                    || strpos($ua, 'mot') !== false
                    || strpos($ua, 'up.browser') !== false
                    || strpos($ua, 'up.link') !== false
                    || strpos($ua, 'audiovox') !== false
                    || strpos($ua, 'blackberry') !== false
                    || strpos($ua, 'ericsson,') !== false
                    || strpos($ua, 'panasonic') !== false
                    || strpos($ua, 'philips') !== false
                    || strpos($ua, 'sanyo') !== false
                    || strpos($ua, 'sharp') !== false
                    || strpos($ua, 'sie-') !== false
                    || strpos($ua, 'portalmmm') !== false
                    || strpos($ua, 'blazer') !== false
                    || strpos($ua, 'avantgo') !== false
                    || strpos($ua, 'danger') !== false
                    || strpos($ua, 'palm') !== false
                    || strpos($ua, 'series60') !== false
                    || strpos($ua, 'palmsource') !== false
                    || strpos($ua, 'pocketpc') !== false
                    || strpos($ua, 'smartphone') !== false
                    || strpos($ua, 'rover') !== false
                    || strpos($ua, 'ipaq') !== false
                    || strpos($ua, 'au-mic,') !== false
                    || strpos($ua, 'alcatel') !== false
                    || strpos($ua, 'ericy') !== false
                    || strpos($ua, 'up.link') !== false
                    || strpos($ua, 'vodafone/') !== false
                    || strpos($ua, 'wap1.') !== false
                    || strpos($ua, 'wap2.') !== false;

        return $this->isMobileBrowser;
    }

    /**
     *  Get the root {@link WFModuleInvocation} used by the request controller.
     *
     *  @return object The root WFModuleInvocation for the page.
     */
    function rootModuleInvocation()
    {
        return $this->rootModuleInvocation;
    }

    /**
      * Get a reference to this WFRequestController's skin object.
      *
      * @static
      * @return object A reference to the {@link WFSkin} object.
      * @deprecated Use $module->rootSkin()
      */
    public static function sharedSkin()
    {
        $rc = WFRequestController::sharedRequestController();
        $rootInv = $rc->rootModuleInvocation();
        if (!$rootInv) throw( new Exception("No root invocation, thus no shared skin..") );
        return $rootInv->skin();
    }

    /**
      * Generate a "full" URL to the given module / page.
      *
      * It is recommended to use this function to generate all URL's to pages in the application.
      * Of course you may append some PATH_INFO or params afterwards.
      *
      * Guaranteed to *never* end in a trailing slash. Always add your own if you are addition additional stuff to the URL.
      *
      * @static
      * @param string The module name (required).
      * @param string The page name (or NULL to use the default page).
      * @return string a RELATIVE URL to the requested module/page.
      */
    public static function WFURL($moduleName, $pageName = NULL)
    {
        $moduleName = ltrim($moduleName, '/');  // just in case a '/path' path is passed, we normalize it for our needs.
        if (empty($moduleName)) throw( new Exception("Module is required to generate a WFURL.") );
        $url = WWW_ROOT . '/' . $moduleName;
        if ($pageName !== NULL)
        {
            $url .= '/' . $pageName;
        }
        return $url;
    }

    /**
     * Get a reference to the shared WFRequestController object.
     * @static
     * @return object The WFRequestController object.
     */
    public static function sharedRequestController()
    {
        static $singleton = NULL;
        if (!$singleton) {
            $singleton = new WFRequestController();
        }
        return $singleton;
    }
}



abstract class SssSCLI_Menu {

    protected $aMenu;
    protected $aMethods;
    protected $mDefault;
    protected $sPrompt;
    protected $sTitle;

    public function __construct(Array $aMenu, Array $aMethods, $sTitle = null, $sPrompt = null, $mDefault = 0) {

        if (empty($sTitle)) {
            $sTitle = 'Menu';
        } // if no title set
        if (empty($sPrompt)) {
            $sPrompt = 'please choose an action:';
        } // if no prompt given

        $this->aMenu = array();
        $this->aMethods = array();
        for ($iCount = 0; $iCount < count($aMethods); $iCount++) {
            $this->aMenu[] = $aMenu[$iCount];
            $this->aMethods[base64_encode($aMenu[$iCount])] = $aMethods[$iCount];
        } // loop all entries
        $this->mDefault = $mDefault;
        $this->sPrompt = $sPrompt;
        $this->sTitle = $sTitle;

    } // __construct


    public function ask($mDefault = null) {

        if (null === $mDefault) {
            $mDefault = $this->mDefault;
        } // if no default given, use object's default

        do {

            echo '
' . $this->sTitle . '
';

            $sChoice = SssS_CLI_Tools::getChoiceCLI($this->sPrompt, $this->aMenu, $mDefault);

        } while($this->executeEntry($sChoice));

        return $this;

    } // ask


    protected function executeEntry($sChoice) {

        $sMethod = $this->aMethods[base64_encode($sChoice)];
        if (method_exists($this, $sMethod)) {
            return $this->$sMethod();
        } // if got the method

        echo '
error: method, ' . $sMethod . ', does not exist in ' . get_class($this) . '
';

        return true;

    } // executeEntry


    protected function notYetImplemented() {

        echo '
not yet implemented.
';

        return true;

    } // notYetImplemented


    protected function quit() {
        return false;
    } // quit

} // SssSCLI_Menu


class SssSCLI_Menu_Main extends SssSCLI_Menu {

    protected $oAppDelegate;

    public function __construct($aMenu = null, $aMethods = null, $sTitle = '                 WFRequestController "menu"', $sPrompt = null, $mDefault = null) {

        $this->oAppDelegate = WFWebApplication::sharedWebApplication()->delegate();
var_dump($this->oAppDelegate);

        if (is_array($aMenu) && is_array($aMethods) && (count($aMenu) == count($aMethods))) {

            return parent::__construct($aMenu, $aMethods, $sTitle, $sPrompt, $mDefault);

        } // if array given

        $aChoices = array();
        $aMethods = array();
        if (!!$this->oAppDelegate && method_exists($this->oAppDelegate, 'handleCLIRequest')) {
            $aChoices[] = 'call AppDelegate->handleCLIRequest()';
            $aMethods[] = 'callAppDelegate';
        } // if got app delegate and has method handleCLIRequest()

        $aChoices = array_merge($aChoices, array(
            'dump html for an invocation path',
            'enter iphp console...',
            '(Lists...)',
            'Generate...',
            'enter phocoa phing task manager',
            'quit'
        ));
        $aMethods = array_merge($aMethods, array(
            'dumpHTMLforAnInvocatonPath',
            'enterIphpConsole_',
            'notYetImplemented',
            'generate_',
            'enterPhocoaPhingTaskManager_',
            'quit'
        ));

        return parent::__construct($aChoices, $aMethods, $sTitle, $sPrompt, $mDefault);

    } // __construct


    protected function callAppDelegate() {

        // let app delegate give it a go -> should return true if done,
        // false if wants WFRequestController to continue
        if (!!$this->oAppDelegate && method_exists($this->oAppDelegate, 'handleCLIRequest')) {
            return !($this->oAppDelegate->handleCLIRequest());
        } // if got a suitable app delegate

        return true;

    } // callAppDelegate


    protected function dumpHTMLforAnInvocatonPath() {
//TODO: catch 404 and other errors
        $nl = chr(10);
        $oWFRC = WFRequestController::sharedRequestController();

        // testing if page builds
        list($bInteractive, $modInvocationPath) = $oWFRC->parseCliRequest();

        $modInvocationPath = SssS_CLI_Tools::getInputCLI(
                $nl . 'invocation (the default shown is either the first argument '
                . 'passed or the default path):', $modInvocationPath);

        // create the root invocation; only skin if we're not in an XHR
        $oWFRC->rootModuleInvocation = new WFModuleInvocation($modInvocationPath, NULL,
                (WFRequestController::isAjax()
                    ? NULL
                    : WFWebApplication::sharedWebApplication()->defaultSkinDelegate()));

        // get HTML result of the module, and output it
        $html = $oWFRC->rootModuleInvocation->execute();

        // read about this above handleHTTPRequest()
        if (isset($_REQUEST[WFRPC::PARAM_ENABLE_AJAX_IFRAME_RESPONSE_MODE])
            && (1 == $_REQUEST[WFRPC::PARAM_ENABLE_AJAX_IFRAME_RESPONSE_MODE])) {
            //header('Content-Type: text/xml');
            $html = "<?xml version=\"1.0\"?><raw><![CDATA[\n{$html}\n]]></raw>";
        } // if ajax special situation with iframe

        print $html;

        return true;

    } // dumpHTMLforAnInvocatonPath


    protected function enterIphpConsole_() {

        static $oIphpShell = null;
        static $sPathTags = null;
        static $nl = null;

        print $nl . 'starting iphp shell' . $nl;

        if (null == $nl) {
            $nl = chr(10);
            $sPathTags = APP_ROOT . DIR_SEP . 'tags';
            $sPathSssSphocoaAppConf =
                    APP_ROOT . DIR_SEP . 'conf' . DIR_SEP . 'SssSphocoaApp.conf';
            $oIphpShell = new iphp(array(
                            'tags' => $sPathTags,
                            'require' => $sPathSssSphocoaAppConf,
                            'commands' => array('iphp_command_stop')
                            )
            ); // \stop to exit iphp and reenter loop with appDelegate having a go and outputting current request path
        } // if first run

        $oIphpShell->runREPL();

        return true;

    } // enterIphpConsole_


    protected function enterPhocoaPhingTaskManager_() {

        $nl = chr(10);

        print $nl . 'starting phocoa phing task manager' . $nl;

        $sCommand = '';
        WFRequestController::setEnvForPhocoaPhingPropel();
        $bContinuePhocoaTasks = true;
        while ($bContinuePhocoaTasks) {
            // run without args to show menu
            $sShellLine = //'cd "' . APP_ROOT . '";' .
                FRAMEWORK_DIR . DIR_SEP . 'phing' . DIR_SEP . 'phocoa ' . $sCommand;
                //. ';cd ' . getCwd();

            passthru($sShellLine);

            // ask for command
            $sCommand = SssS_CLI_Tools::getInputCLI(
                    $nl . '                 WFRequestController "phing task manager"'
                    . $nl .'what to do? ("back" to go back, -l or -list for '
                    . 'list of phing tasks)', 'back');
            // if not stop
            if (in_array(trim(strtolower($sCommand)),
                    array('back', 'stop', 'exit', 'quit', 'halt', 'die'))) {
                $bContinuePhocoaTasks = false;
                // pass on command
            } // if stop

        } // loop it

        return true;

    } // enterPhocoaPhingTaskManager_


    protected function generate_() {

        static $oMenuGenerate = null;
        if (!$oMenuGenerate) {
            $oMenuGenerate = new SssSCLI_Menu_Generate();
        } // if first run

        $oMenuGenerate->ask();

        return true;

    } // generate_


} // SssSCLI_Menu_Main



class SssSCLI_Menu_Generate extends SssSCLI_Menu {

    protected $sPathBin;
    protected $sPathPropelBuild;
    protected $sPathPropelGenBin;
    protected $sPropelBinArgsBuild;
    protected $sPropelBinArgsClasses;
    protected $sPropelBinArgsConf;
    protected $sPathPhocoaScripts;
    protected $sPHPbin;

    public function __construct($sTitle = '       WFRequestController "menu-Generate"') {

        $this->sPathBin = APP_ROOT . DIR_SEP . 'bin';
        $this->sPathPropelBuild = APP_ROOT . DIR_SEP . 'propel-build';
		//TODO: get this and other values from the .plist that created this project
        $this->sPathPropelGenBin = '/gitSwissalpS/Propel2/bin/propel';
        $this->sPropelBinArgsBuild = ' --config-dir "' . $this->sPathPropelBuild . '"'
								   . ' --output-dir "' . $this->sPathPropelBuild . '"';
        $this->sPropelBinArgsClasses = ' --config-dir "' . $this->sPathPropelBuild . '"'
								   . ' --output-dir "' . APP_ROOT . DIR_SEP . 'classes' . '"';
        $this->sPropelBinArgsConf = ' --config-dir "' . $this->sPathPropelBuild . '"'
								   . ' --output-dir "' . APP_ROOT . DIR_SEP . 'conf' . '"';
        $this->sPathPhocoaScripts = FRAMEWORK_DIR . DIR_SEP . 'framework' . DIR_SEP . 'script';
        $this->sPHPbin = SssS_ShellScriptRunner::getPHPinterpreterPath();

        $aChoices = array(
            'Propel2 database:reverse -> schema.xml',
            'fix schema.xml',
            'Propel2 database:build -> class files',
            '0, 1 and 2 for all connections',
            'Propel2 config:convert -> PROPEL_CONF',
            'de/activate Propel2',
            '---------------------------------------',
            'create scaffolds for Propel classes',
            '---------------------------------------',
            'create a module',
            'create a page',
            '---------------------------------------',
            'Create a test stub for phocoa classes',
            '---------------------------------------',
            'back to main menu'
        );

        $aMethods = array(
            'propel2database_reverse',
            'fixSchema',
            'propel2database_build',
            'do_0_1_2allConnections',
            'propel2config_convert',
            'togglePropel2',
            '-----',
            'createScaffolds',
            '-----',
            'createModule',
            'createPage',
            '----',
            'createTestStub',
            '----',
            'quit'
        );

        return parent::__construct($aChoices, $aMethods, $sTitle, null, 'back to main menu');

    } // __construct


    protected function collectPropel2connectionNames() {

        $nl = chr(10);

        // we can not assume that propel is initiated yet
        $aPropelConfigLocal = array();
        $aPropelConfigRepo = array();
        if (is_file($this->sPathPropelBuild . DIR_SEP . 'propel.php')) {
            $aPropelConfigLocal = include($this->sPathPropelBuild . DIR_SEP . 'propel.php');
        } // if got local config
        if (is_file($this->sPathPropelBuild . DIR_SEP . 'propel.php.dist')) {
            $aPropelConfigRepo = include($this->sPathPropelBuild . DIR_SEP . 'propel.php.dist');
        } // if got distro config
        // merge both
        $aPropelConfig = array_merge($aPropelConfigRepo, $aPropelConfigLocal);

        // extract location names
        $aPropelConnectionNames = array();
        if (!isset($aPropelConfig['propel']['database']['connections'])) {
            echo $nl . 'error: Propel2 main config files not found or incorect' . $nl;
            return $aPropelConnectionNames;
        } // if bad config
        foreach ($aPropelConfig['propel']['database']['connections'] as $aPropelConnectionName => $aPropelConnection) {
            $aPropelConnectionNames[] = $aPropelConnectionName;
        } // loop gathering all names
        if (empty($aPropelConnectionNames)) {
            echo $nl . 'error: no Propel2 connections found.' . $nl;
            return $aPropelConnectionNames;
        } // if no connections found

        return $aPropelConnectionNames;

    } // collectPropel2connectionNames


    protected function createModule() {

        $nl = chr(10);
        $oRunner = SssS_ShellScriptRunnerForDummies();

        $sPathToModules = SssS_CLI_Tools::removeTrailingSlash(SssS_CLI_Tools::getInputCLI(
                            $nl . 'Where should the module be created?',
                            APP_ROOT . DIR_SEP . 'modules'), DIR_SEP);
        if (!is_dir($sPathToModules)) $oRunner->makePath($sPathToModules);

        $sModuleName = SssS_CLI_Tools::getInputCLI($nl . 'Module name:', 'newmodule');
        $sDefaultPageName = SssS_CLI_Tools::getInputCLI($nl . 'Default page [blank for none]:', '');
        //self::setEnvForPhocoaPhingPropel();
        $sShellLine = 'cd "' . $sPathToModules . '";'
                    . $this->sPHPbin . ' ' . $this->sPathPhocoaScripts . DIR_SEP
                    . 'createModule.php ' . $sModuleName . ' ' . $sDefaultPageName
                    . ';cd ' . getCwd();

        $iRes = null;
        passthru($sShellLine, $iRes);

        return true;

    } // createModule


    protected function createPage() {

        $nl = chr(10);

        $oRunner = SssS_ShellScriptRunnerForDummies();

        //TODO: collect dirs and also offer arbitrary like now
        $sPathToPage = SssS_CLI_Tools::removeTrailingSlash(SssS_CLI_Tools::getInputCLI(
                        $nl . 'Where should the page be created?',
                        APP_ROOT . DIR_SEP . 'modules'), DIR_SEP);
        if (!is_dir($sPathToPage)) $oRunner->makePath($sPathToPage);

        $sPageName = SssS_CLI_Tools::getInputCLI($nl . 'Page name:', 'newpage');
        //self::setEnvForPhocoaPhingPropel();
        $sShellLine = 'cd "' . $sPathToPage . '";'
                    . $this->sPHPbin . ' ' . $this->sPathPhocoaScripts . DIR_SEP . 'createPage.php ' . $sPageName
                    . ';cd ' . getCwd();

        $iRes = null;
        passthru($sShellLine, $iRes);

        // now lets add to module, if ok
        $sModuleName = basename($sPathToPage);
        $aChoicesYN = array('no', 'yes');
        $sChoice = trim(strToLower(SssS_CLI_Tools::getChoiceCLI('add page delegate to ' . $sModuleName . '.php?', $aChoicesYN, 'yes')));
        if ('no' == $sChoice) return true;

        require_once($this->sPathPhocoaScripts . DIR_SEP . 'scriptComponents.php');
        $sModuleName = basename($sPathToPage);
        $sModulePageBit = _pageForModule($sModuleName, $sPageName);
        $sModuleFilePath = $sPathToPage . DIR_SEP . $sModuleName . '.php';
        if (is_file($sModuleFilePath)) {
            $sModuleOriginal = file_get_contents($sModuleFilePath);
            $iCount = 0;
            $sModuleNew = str_replace('?' . '>', $sModulePageBit . $nl . '?'
                                      . '>', $sModuleOriginal, $iCount);
            switch ($iCount) {
                case 0 : // no closing php tag
                    file_put_contents($sModuleFilePath, $sModuleOriginal . $sModulePageBit);
                    echo $nl . 'NOTICE: no closing php tag ie ?' . '> in '
                        . $sModuleFilePath . $nl
                        . 'added page module without adding closing tag for php';
                    break;

                case 1 : // fine, let's save
                    file_put_contents($sModuleFilePath, $sModuleNew);
                    echo $nl . 'added page module to ' . $sModuleFilePath . $nl;
                    break;

                default : // oops, multiple occurences
                    file_put_contents($sModuleFilePath . '.prop', $sModuleOriginal
                                      . $nl . '<' . '?' . 'php' . $nl
                                      . '// this is the proposed structure. you need '
                                      . 'to manually check your php opening and '
                                      . 'closing tags' . $nl . $sModulePageBit
                                      . $nl . '?' . '>');
                    echo $nl . 'WARNING:oops, multiple occurences of closing php '
                            . 'tag ie ?' . '> in ' . $sModuleFilePath . $nl
                            . 'made a proposition in ' . $sModuleFilePath . '.prop' . $nl;

            } // switch count of php closing tags

        } else {

            // no module yet
            $sModuleNew = '<' . '?' . 'php' . $nl
                    . _moduleBit($sModuleName, $sPageName) . $nl
                    . $sModulePageBit . $nl . '?' . '>';
            file_put_contents($sModuleFilePath, $sModuleNew);
            echo $nl . 'there was no ' . $sModuleName
                    . '.php so it has been created with ' . $sPageName
                    . ' as default page.' . $nl . $sModuleFilePath . $nl;

        } // if module php file already exists or not

        return true;

    } // createPage


    protected function createScaffolds() {

        $nl = chr(10);
        $aConnections = $this->collectPropel2connectionNames();
        $iLenQuery = strLen('Query');
        $sPathClassesBase = APP_ROOT . DIR_SEP . 'classes' . DIR_SEP . 'SssSPropel2' . DIR_SEP;
        $sAvailableClasses = '';
        foreach ($aConnections as $sConnetion) {
            $sPath = $sPathClassesBase . $sConnetion . DIR_SEP;
            $rDir = opendir($sPath);
            if (false === $rDir) {
                echo $nl . 'error: could not open ' . $sPath . $nl;
                continue;
            } // if could not open dir
            while (false !== $sFile = readdir($rDir)) {
                $sPathFile = $sPath . $sFile;
                $aInfo = pathinfo($sPathFile);
                if (!(isset($aInfo['extension'])
                      && 'php' == $aInfo['extension'])) {
                    continue;
                } // skip any non php

                $sClassName = $aInfo['filename'];

                // ends with Query?
                if ('Query' == substr($sClassName, -$iLenQuery)) {
                    continue;
                } // if query-class, skip

                $sAvailableClasses .= 'SssSPropel2\\' . $sConnetion . '\\' . $sClassName . ' ';

            } // loop all files in dir

        } // loop all connection directories

        if (empty($sAvailableClasses)) {
            echo 'you don\'t seem to have any generated classes yet.' . $nl;
            return true;
        } // if no classes found

        echo $nl . 'available classes to build scaffolds for:' . $nl
            . $sAvailableClasses . $nl;
        $sEntityList = SssS_CLI_Tools::getInputCLI(
            $nl . 'Which entities do you want to create scaffolds for?',
            $sAvailableClasses);
        $sPathToPutScaffolds = SssS_CLI_Tools::getInputCLI(
            $nl . 'Where should the scaffolds be created?', APP_ROOT
            . DIR_SEP . 'modules');
        WFRequestController::setEnvForPhocoaPropel();
        $sShellLine = 'cd "' . $sPathToPutScaffolds . '";'
                    . $this->sPHPbin . ' ' . $this->sPathPhocoaScripts . DIR_SEP
                    . 'scaffold.php ' . escapeshellarg($sEntityList)
                    . ';cd ' . getCwd();
        $iRes = null;
        passthru($sShellLine, $iRes);

        return true;

    } // createScaffolds


    protected function createTestStub() {

        $nl = chr(10);
        $oRunner = new SssS_ShellScriptRunnerForDummies();

        //TODO: offer better default
        $sPathToTest = SssS_CLI_Tools::removeTrailingSlash(SssS_CLI_Tools::getInputCLI(
                $nl . 'Where should the teststub be created?', APP_ROOT), DIR_SEP);
        if (!is_dir($sPathToTest)) $oRunner->makePath($sPathToTest);

        $sTestClassName = SssS_CLI_Tools::getInputCLI($nl . 'Name of class to test:', 'WFObject');
        WFRequestController::setEnvForPhocoaPhingPropel();
        $sShellLine = 'cd "' . $sPathToTest . '";'
                    . $this->sPHPbin . ' ' . $this->sPathPhocoaScripts . DIR_SEP
                    . 'createTestStub.php ' . $sTestClassName
                    . ';cd ' . getCwd();

        return true;

    } // createTestStub


    protected function do_0_1_2allConnections() {

        $nl = chr(10);

        // we can not assume that propel is initiated yet
        // collect connection names
        $aPropelConnectionNames = $this->collectPropel2connectionNames();

        foreach ($aPropelConnectionNames as $sPropelConnection) {
            echo $nl . 'propel database:reverse ' . $sPropelConnection . $nl;
            $iRes = null;
            passthru($this->sPathPropelGenBin . ' database:reverse ' . $sPropelConnection . ' ' . $this->sPropelBinArgsBuild, $iRes);
            if (0 !== $iRes) {
                echo $nl . 'error: abborting jobs on this connection.' . $nl;
                continue;
            } // if error

            echo $nl . 'fix-schema' . $nl;
            $iRes = null;
            passthru($this->sPathBin . DIR_SEP . 'fix-schema.php', $iRes);
            if (0 !== $iRes) {
                echo $nl . 'error: abborting jobs on this connection.' . $nl;
                continue;
            } // if error

            echo $nl . 'propel database:build' . $nl;
            $iRes = null;
            passthru($this->sPathPropelGenBin . ' model:build' . $this->sPropelBinArgsClasses, $iRes);
            if (0 !== $iRes) {
                echo $nl . 'error: oops.' . $nl;
                continue;
            } // if error

        } // loop each connection

        return true;

    } // do_0_1_2allConnections


    protected function fixSchema() {

        $iRes = null;
        passthru($this->sPathBin . DIR_SEP . 'fix-schema.php', $iRes);

        return true;

    } // fixSchema


    protected function propel2config_convert() {

        $nl = chr(10);

        $iRes = null;
        $sPathPropelBuildConf = $this->sPathPropelBuild . DIR_SEP . 'config.php';
        if (is_file($sPathPropelBuildConf)) {
            rename($sPathPropelBuildConf, $sPathPropelBuildConf . '.last');
            echo 'backed up existing conf in build dir to ' . $sPathPropelBuildConf . '.last' . $nl;
        } // make sure old file is gone
        passthru($this->sPathPropelGenBin . ' config:convert' . $this->sPropelBinArgsBuild, $iRes);
        if (is_file($sPathPropelBuildConf)) {
            if (is_file(PROPEL2_RUNTIME_CONFIG)) {
                rename(PROPEL2_RUNTIME_CONFIG, PROPEL2_RUNTIME_CONFIG . '.last');
                echo 'backed up current conf to ' . PROPEL2_RUNTIME_CONFIG . '.last' . $nl;
            } // if got old config
            rename($sPathPropelBuildConf, PROPEL2_RUNTIME_CONFIG);
            echo 'moved conf from build dir to ' . PROPEL2_RUNTIME_CONFIG . $nl;
        } else {
            echo $nl . 'error, did not generate new runtime config!';
        } // if new file created

        return true;

    } // propel2config_convert


    protected function propel2database_build() {

        $iRes = null;
        passthru($this->sPathPropelGenBin . ' model:build' . $this->sPropelBinArgsClasses, $iRes);

        return true;

    } // propel2database_build


    protected function propel2database_reverse() {

        $nl = chr(10);

        // which connection?
        $aPropelConnectionNames = $this->collectPropel2connectionNames();
        if (empty($aPropelConnectionNames)) {
            return true;
        } // if no connections found
        $sPropelConnection = SssS_CLI_Tools::getChoiceCLI(
                                'which connection to reverse',
                                $aPropelConnectionNames, $aPropelConnectionNames[0]);
        $iRes = null;
        passthru($this->sPathPropelGenBin . ' database:reverse ' . $sPropelConnection . ' ' . $this->sPropelBinArgsBuild, $iRes);

        return true;

    } // propel2database_reverse


    protected function togglePropel2() {

        static $nl = null;
        static $sSearchPropelAct, $sPathAppConfActive, $sPathAppConfTemplate;
        static $sCommentOFF, $sCommentON, $sCommentRestart;
        if (!$nl) {
            $nl = chr(10);
            $sSearchPropelAct = '/define\(["\']PROPEL2_ACTIVATED["\'],\s?(\w+)\);/';
            $sPathAppConfActive = APP_ROOT . DIR_SEP . 'conf' . DIR_SEP . 'SssSphocoaApp.conf';
            $sPathAppConfTemplate = $sPathAppConfActive . '.SssStemplate';
            $sCommentOFF = 'Switching OFF Propel2 integration' . $nl . 'Repeat to reactivate.'
                    . '(after restarting app)' . $nl;
            $sCommentON = 'Switching ON Propel2 integration' . $nl . 'Repeat to deactivate.'
                    . '(after restarting app)'  . $nl
                    . 'Make sure you have something like this in your '
                    . 'application delegate class::initialize()' . $nl
                    . '
    if (PROPEL2_ACTIVATED) {
        if (is_readable(PROPEL2_RUNTIME_CONFIG)) {
            ////////////// THIS IS IMPORTANT ////////////////
            require_once(PROPEL2_RUNTIME_CONFIG);
        } else {
            throw new WFException(\'could not read Propel2 config file at \' . PROPEL2_RUNTIME_CONFIG);
        } // if got propel config at all
    } // if propel is activated' . $nl . $nl;
            $sCommentRestart = $nl . 'You need to restart the app to toggle again.' . $nl . 'Or you can edit your conf and template manually.' . $nl;
        } // if first run

        if (defined('PROPEL2_ACTIVATED') && PROPEL2_ACTIVATED) {
            $sNewValue = 'false';
            $sComment = $sCommentOFF;
        } else {
            $sNewValue = 'true';
            $sComment = $sCommentON;
        } // if currently activated or not

        foreach (array($sPathAppConfActive, $sPathAppConfTemplate) as $sPathAppConf) {
            if (is_writable($sPathAppConf)) {
                $sAppConf = @file_get_contents_safely($sPathAppConf);
                echo 'Searching in ' . $sPathAppConf . $nl . 'for "define("PROPEL2_ACTIVATED", ...);"' . $nl;
                $aRes = array();
                $iRes = preg_match($sSearchPropelAct, $sAppConf, $aRes);
                if ($iRes) {
                    echo 'Found: ' . $aRes[0] . $nl;
                    $sAppConf = str_replace($aRes[0], 'define(\'PROPEL2_ACTIVATED\', ' . $sNewValue . ');', $sAppConf);
                    file_put_contents_safely($sPathAppConf, $sAppConf, 'wb');
                } else {
                    $sComment2 = 'error: could not find "define("PROPEL2_ACTIVATED", ...);" in ' . $sPathAppConf . $nl
                        . 'add define("PROPEL2_ACTIVATED", true); manually.' . $nl;
                    $sComment .= $nl . $sComment;
                    echo $sComment2;
                } // if found
            } else {
                $sComment2 = $nl . 'error, can not write to ' . $sPathAppConf . $nl;
                $sComment .= $sComment2;
                echo $sComment2;
                if ($sPathAppConf == $sPathAppConfActive) {
                    $sComment2 = 'try to remakeTemplatedFiles and try again.' . $nl;
                    $sComment .= $nl . $sComment2;
                    echo $sComment2;
                } // if has template for conf
            } // if can write to config or not
        } // loop both config and template

        echo $sComment . $sCommentRestart;

        return true;

    } // togglePropel2

} // SssSCLI_Menu_Generate



/**
 * Helper class to allow modules to easily redirect the client to a given URL.
 *
 * Modules can throw a WFRedirectRequestException anytime to force the client to redirect.
 *
 * @deprecated
 * @see WFRequestController_RedirectException
 */
class WFRedirectRequestException extends WFException
{
    protected $redirectUrl;

    function __construct($message = NULL, $code = 0)
    {
        parent::__construct($message, $code);
        $this->redirectUrl = $message;
    }

    function getRedirectURL()
    {
        return $this->redirectUrl;
    }
}

class WFRequestController_InternalRedirectException extends WFRedirectRequestException {}
class WFRequestController_NotFoundException extends WFException {}
class WFRequestController_BadRequestException extends WFException {}
class WFRequestController_HTTPException extends WFException
{
    public function __construct($message = NULL, $code = 500) { parent::__construct($message, $code); }
}
class WFRequestController_RedirectException extends WFRequestController_HTTPException
{
    protected $redirectUrl;

    /**
     * By default use http code 302, but allow user to override it
     * to use e.g. a 301.
     */
    public function __construct($url, $code = 302)
    {
        $this->redirectUrl = $url;
        return parent::__construct($url, $code);
    }

    public function getRedirectURL()
    {
        return $this->redirectUrl;
    }
}

/**
 * There are certain classes that are needed to successfully handle errors. We need to make sure that they are loaded up front so that
 * they don't need to be autoloaded during error handling, which can result in errors during error handling such as:
 *
 * - Fatal error: Class declarations may not be nested in /Users/alanpinstein/dev/sandbox/showcaseng/showcaseng/externals/phocoa/phocoa/framework/WFExceptionReporting.php on line 14
 *
 * Simply running a class_exists on each class will force an autoload when WFRequestController is parsed, preventing the problem.
 * We don't do a hard require('file.php') here since that would break our automated opcode-cache-friendly require('/full/path/to/file.php') system in phocoa's autoloader.
 */
class_exists('WFExceptionReporting');
/* * * *\ WFRequestController.inc (CC) Luke JZ aka SwissalpS /* * * */
?>
