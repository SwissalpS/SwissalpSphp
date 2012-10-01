{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
<p>{WFLabel id="userinfo"}
{if $showLogout}
    <a href="{WFURL page="doLogout"}">{SssSBla value="SharedLogout"}</a>
{/if}
</p>
