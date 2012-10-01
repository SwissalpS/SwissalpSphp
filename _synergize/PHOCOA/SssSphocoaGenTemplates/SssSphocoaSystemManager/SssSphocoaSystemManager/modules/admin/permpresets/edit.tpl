{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
<h2>{SssSBla value="PPermsSing"}</h2>
<div class="form-container">
	{WFView id="statusMessage"}
	{WFShowErrors id=editPermpresetsForm}

	{WFViewBlock id="editPermpresetsForm"}

	<table style="width:60%;border:none;align:left;cellpadding:1;cellspacing:1">
        <tbody>
            <tr>
                <td>uid</td>
                <td>{WFView id="uidl"}{WFView id="uid"}</td>
                <td colspan="1" rowspan="4">{WFView id="multiSelectPerms"}</td>
            </tr>
            <tr>
                <td>{SssSBla value="AdminPPermsName"}</td>
                <td>{WFView id="name"}</td>
            </tr>
            <tr>
                <td>{SssSBla value="PermsPlur"}</td>
                <td>{WFView id="permissions"}</td>
            </tr>
            <tr>
                <td colspan="2">{WFShowErrors id="name"}{WFShowErrors id="permissions"}</td>
            </tr>
        </tbody>
    </table>
	<div class="buttonrow">{WFView id="saveNew"}{WFView id="save"}{WFView id="deleteObj"}</div>
	{/WFViewBlock}
	{literal}<script>function remakePermList(){var sel_pl=document.getElementById("multiSelectPerms");var aOpt=sel_pl.options;var iLen=aOpt.length;var a=new Array();for(i=0;iLen>i;i++){if(aOpt[i].selected){a.push(aOpt[i].value);};};document.getElementById("permissions").value=a.join();};function updateMultiSel(){var sel_pl=document.getElementById("multiSelectPerms");var aOpt=sel_pl.options;var a=document.getElementById("permissions").value.split(',');var iLen=aOpt.length;for(i=0;i<iLen;i++){for(j=0;j<a.length;j++){if(aOpt[i].value==parseInt(a[j])){aOpt[i].selected=true;break;};};};};updateMultiSel();</script>{/literal}
</div>{* end form-container *}
