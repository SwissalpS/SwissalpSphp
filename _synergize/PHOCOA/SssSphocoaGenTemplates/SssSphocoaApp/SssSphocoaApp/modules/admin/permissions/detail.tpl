{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
<h2>{SssSBla value="PermsSing"}</h2>

<table border="0" cellpadding="3" cellspacing="0" class="datadetail">
    <tr>
        <td valign="top">uid:</td>
        <td valign="top">{WFView id="uid"}</td>
    </tr>
    <tr>
        <td valign="top">{SssSBla value="AdminPermsDomain"}:</td>
        <td valign="top">{WFView id="domain"}</td>
    </tr>
    <tr>
        <td valign="top">{SssSBla value="AdminPermsHash"}:</td>
        <td valign="top">{WFView id="hash"}  {WFView id="isSuperUser"} {WFView id="mayRead"} {WFView id="mayWrite"} {WFView id="mayAdmin"}</td>
    </tr>
</table>
{literal}<script>function updateCheckboxes(){var tf_hash=document.getElementById("hash");var cb_SU=document.getElementById("isSuperUser");var cb_MR=document.getElementById("mayRead");var cb_MW=document.getElementById("mayWrite");var cb_MA=document.getElementById("mayAdmin");var hash=parseFloat(tf_hash.innerHTML);cb_SU.checked=(hash&cb_SU.value);cb_MR.checked=(hash&cb_MR.value);cb_MW.checked=(hash&cb_MW.value);cb_MA.checked=(hash&cb_MA.value);};updateCheckboxes();</script>{/literal}
