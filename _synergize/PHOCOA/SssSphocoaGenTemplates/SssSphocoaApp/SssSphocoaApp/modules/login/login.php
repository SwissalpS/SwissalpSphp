<?php

class login extends WFModule {

    /**
      * Tell system which page to show if none specified.
      */
    function defaultPage() {

        return 'promptLogin';

    } // defaultPage


    function gotoURL($url) {

        // use an internal redirect for ajax requests (otherwise might not work), but use 302 for normal logins. This ensures that the URL in the address bar is correct.
        if (WFRequestController::isAjax()) {

            throw(new WFRequestController_InternalRedirectException($url));

        } else {

            throw(new WFRequestController_RedirectException($url));

        } // if is ajax or normal request

    } // gotoURL


    function doLogout_ParameterList() {

        return array('continueURL');    // will be WFWebApplication::serializeURL() encoded

    } // doLogout_ParameterList


    function doLogout_PageDidLoad($page, $params) {

        $ac = WFAuthorizationManager::sharedAuthorizationManager();

        // calculate continueURL
        $continueURL = NULL;
        if (empty($params['continueURL'])) {

            // need to get this before we log out as the delegate might want access to the credentials to figure this out
            $continueURL = $ac->defaultLogoutContinueURL();

        } else {

            $continueURL = WFWebApplication::unserializeURL($params['continueURL']);

        } // if continue-url passed or not

        $ac->logout();
        if ($ac->shouldShowLogoutConfirmation()) {

            $page->assign('continueURL', $continueURL);
            $page->setTemplateFile('showLogoutSuccess.tpl');

        } else {

            if (!$continueURL) {

                throw(new WFException("No continueURL found... defaultLogoutContinueURL cannot be empty if shouldShowLogoutConfirmation is false."));

            } // if not got continue-url

            $this->gotoURL($continueURL);

        } // if go directly to continue url

    } // doLogout_PageDidLoad


    function promptLogin_ParameterList() {

        return array('continueURL');

    } // promptLogin_ParameterList


    function promptLogin_PageDidLoad($page, $params) {

        $ac = WFAuthorizationManager::sharedAuthorizationManager();
        $authinfo = $ac->authorizationInfo();

        // calculate continueURL
        $continueURL = NULL;
        if (!empty($params['continueURL'])) {

            $continueURL = $params['continueURL'];

        } // if got continue-url

        $page->outlet('continueURL')->setValue($continueURL);

        // if already logged in, bounce to home
        if ($authinfo->isLoggedIn()) {

            if (!$continueURL) {

                $continueURL = $ac->defaultLoginContinueURL();

            } else {

                $continueURL = WFWebApplication::unserializeURL($continueURL);

            } // if got continue-url or not

            $this->gotoURL($continueURL);

        } // if is logged in or not

        // continue to normal promptLogin setup
        $page->assign('loginMessage', $ac->loginMessage());
        $page->assign('usernameLabel', $ac->usernameLabel());
        $page->outlet('rememberMe')->setHidden( !$ac->shouldEnableRememberMe() );
        $page->outlet('forgottenPasswordLink')->setHidden( !$ac->shouldEnableForgottenPasswordReset() );
        $page->outlet('forgottenPasswordLink')->setValue( WFRequestController::WFURL('login', 'doForgotPassword') . '/' . $page->outlet('username')->value());
        $page->outlet('signUp')->setLabel( $ac->signUpLabel() );
        $page->outlet('signUp')->setValue( $ac->signUpUrl() );
        $page->outlet('signUp')->setHidden( $ac->signUpUrl() === NULL );

        if (!$page->hasSubmittedForm()) {

            $page->outlet('rememberMe')->setChecked($ac->shouldRememberMeByDefault());

        } // if page does not have submitted form

    } // promptLogin_PageDidLoad


    function promptLogin_doLogin_Action($page) {

        $ac = WFAuthorizationManager::sharedAuthorizationManager();
        $ok = $ac->login($page->outlet('username')->value(), $page->outlet('password')->value());
        if ($ok) {

            // login was successful
            // remember me stuff
            // ...

            // continue to next page
            if ($page->outlet('continueURL')->value()) {

                $continueURL = WFWebApplication::unserializeURL($page->outlet('continueURL')->value());

            } else {

                $continueURL = $ac->defaultLoginContinueURL();

            } // if got continue-url or not

            $this->gotoURL($continueURL);

        } else {

            // login failed

            $failMsg = $ac->loginFailedMessage($page->outlet('username')->value());
            if (!is_array($failMsg)) {

                $failMsg = array($failMsg);

            } // if fail messages is just one message

            foreach ($failMsg as $msg) {

                $page->addError(new WFError($msg));

            } // loop fail messages adding errors to page

        } // if logged in or not

    } // promptLogin_doLogin_Action


    function promptLogin_SetupSkin($skin) {

        $skin->setTitle(WFLocalizedString('ModuleLoginPromptLoginPageTitle'));

    } // promptLogin_SetupSkin


    function doForgotPassword_ParameterList() {

        return array('username');

    } // doForgotPassword_ParameterList


    function doForgotPassword_PageDidLoad($page, $params) {

        // IE sometimes lower-cases URLs for some reason. Help it out.
        if (!$page->hasOutlet('username')) {

            throw new WFRequestController_RedirectException(
                WFRequestController::WFURL(
                        $page->module()->moduleName(), 'doForgotPassword'));

        } // if username outlet missing

        $ac = WFAuthorizationManager::sharedAuthorizationManager();
        $page->outlet('username')->setValue($params['username']);
        $page->assign('usernameLabel', $ac->usernameLabel());

    } // doForgotPassword


    function doForgotPassword_reset_Action($page) {

        $ac = WFAuthorizationManager::sharedAuthorizationManager();
        try {
            $username = $page->outlet('username')->value();
            $okMessage = sprintf(WFLocalizedString('ModuleLoginDoForgotResetDone'),
                                 $ac->usernameLabel(), $username);

            $newMessage = $ac->resetPassword($username);
            if ($newMessage) {

                $okMessage = $newMessage;

            } // if got a new message

            $page->assign('okMessage', $okMessage);
            $page->setTemplateFile('forgotPasswordSuccess.tpl');

        } catch (WFException $e) {

            $page->addError(new WFError($e->getMessage()));

        } // try catch

    } // doForgotPassword_reset_Action


    function showLoginSuccess_SetupSkin($skin) {

        $skin->setTitle(WFLocalizedString('ModuleLoginSuccessPageTitle'));

    } // showLoginSuccess_SetupSkin


    // simple debug function to see who's logged in and give an option to log out.
    function showLogin_PageDidLoad($page, $params) {

        $ac = WFAuthorizationManager::sharedAuthorizationManager();
        $authinfo = $ac->authorizationInfo();
        if ($authinfo->isLoggedIn()) {

            $sInfo = sprintf(WFLocalizedString('ModuleLoginShowLoginUserIs'), $authinfo->userid());
            $page->assign('showLogout', true);

        } else {

            $sInfo = WFLocalizedString('ModuleLoginShowLoginUserNotLoggedIn');
            $page->assign('showLogout', false);
        } // if is logged in or not

        $page->outlet('userinfo')->setValue($sInfo);

    } // showLogin_PageDidLoad


    function notAuthorized_SetupSkin($skin) {

        $skin->setTitle(WFLocalizedString('ModuleLoginNotAuthorizedPageTitle'));

    } // notAuthorized_SetupSkin

} // login
