{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
{WFShowErrors}
<p>{$loginMessage}</p>
{WFForm id="loginForm"}
    {WFHidden id="continueURL"}
    <table border="0">
        <tr>
            <td>{$usernameLabel}:</td>
            <td>{WFTextField id="username"}</td>
        </tr>
        <tr>
            <td>{SssSBla value="SharedPassword"}:</td>
            <td>{WFPassword id="password"} {WFLink id="forgottenPasswordLink"}</td>
        </tr>
        {WFViewHiddenHelper id="rememberMe"}
        <tr>
            <td>{SssSBla value="LoginRememberMe"}?</td>
            <td>{WFCheckbox id="rememberMe"}</td>
        </tr>
        {/WFViewHiddenHelper}
        <tr>
            <td colspan="2" align="center">
				{WFSubmit id="login"}
                {WFViewHiddenHelper id="signUp"}
                    {SssSBla value="LoginOrSignup"} {WFView id="signUp"}
                {/WFViewHiddenHelper}
			</td>
        </tr>
    </table>
{/WFForm}
<script>
document.observe('dom:loaded', function() {ldelim} $('username').activate(); {rdelim});
</script>
