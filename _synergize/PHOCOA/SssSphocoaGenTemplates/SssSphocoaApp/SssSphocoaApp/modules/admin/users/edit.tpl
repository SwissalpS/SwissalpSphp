{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
<h2>{SssSBla value="UsersSing"}</h2>
<div class="form-container">
{WFView id="statusMessage"}
{WFShowErrors id=editUsersForm}

{WFViewBlock id="editUsersForm"}
	{SssSBla value="UsersSing"} {SssSBla value="SharedDetail"}
	<table>
		<tr>
			<td>
				{SssSBla value="AdminUsersHandle"}
			</td>
			<td>
				{WFView id="handle"}
			</td>
		</tr><tr>
			<td>
				{SssSBla value="AdminUsersRealName"}
			</td>
			<td>
				{WFView id="realname"}{WFShowErrors id="realname"}
			</td>
		</tr><tr>
			<td>
				{SssSBla value="AdminUsersEmail"}
			</td>
			<td>
				{WFView id="email"}{WFShowErrors id="email"}
			</td>
		</tr><tr>
			<td>
				{SssSBla value="AdminUsersCountry"}
			</td>
			<td>
				{WFView id="country"}{WFShowErrors id="country"}
			</td>
		</tr><tr>
			<td>
				{SssSBla value="AdminUsersRegion"}
			</td>
			<td>
				{WFView id="region"}{WFShowErrors id="region"}
			</td>
		</tr><tr>
			<td>
				{SssSBla value="AdminUsersURL"}
			</td>
			<td>
				{WFView id="url"}{WFShowErrors id="url"}
			</td>
		</tr><tr>
			<td>
				{SssSBla value="SharedPassword"}
			</td>
			<td>
				{WFView id="pass1"}
			</td>
		</tr><tr>
			<td>
				{SssSBla value="AdminUsersPassRepeat"}
			</td>
			<td>
				{WFView id="pass2"}
				{WFView id="passhash"}{WFShowErrors id="passhash"}
			</td>
		</tr><tr>
			<td>
				{SssSBla value="PPermsPlur"}
			</td>
			<td>
				{WFView id="multiSelectPPerms"}
				{WFView id="permissions"}{WFShowErrors id="permissions"}
			</td>
		</tr><tr>
			<td>
				oAuth
			</td>
			<td>
				{WFView id="oauth"}{WFShowErrors id="oauth"}
			</td>
		</tr><tr>
			<td>
				openID
			</td>
			<td>
				{WFView id="openid"}{WFShowErrors id="openid"}
			</td>
		</tr><tr>
			<td>
				{SssSBla value="AdminUsersAvatarMediaUID"}
			</td>
			<td>
				{WFView id="avatarmediauid"}{WFShowErrors id="avatarmediauid"}
			</td>
		</tr><tr>
			<td>
				{SssSBla value="AdminUsersLangOrder"}
			</td>
			<td>
				{WFView id="languageorder"}{WFShowErrors id="languageorder"}
			</td>
		</tr><tr>
			<td>
				Karma
			</td>
			<td>
				{WFView id="karma"}{WFShowErrors id="karma"}
			</td>
		</tr>
	</table>
	{WFView id="uid"}{WFShowErrors id="uid"}
	<div class="buttonrow">
		{WFView id="saveNew"}{WFView id="save"}{WFView id="deleteObj"}
	</div>
{/WFViewBlock}
{literal}<script>function remakePPermList(){var aOpt=document.getElementById("multiSelectPPerms").options;var iLen=aOpt.length;var a=new Array();for(i=0;iLen>i;i++){if(aOpt[i].selected){a.push(aOpt[i].value);};};document.getElementById("permissions").value=a.join();};function updateMultiSel(){var aOpt=document.getElementById("multiSelectPPerms").options;var a=document.getElementById("permissions").value.split(',');var iLen=aOpt.length;for(i=0;i<iLen;i++){for(j=0;j<a.length;j++){if(aOpt[i].value==parseInt(a[j])){aOpt[i].selected=true;break;};};};};updateMultiSel();</script>{/literal}
</div>{* end form-container *}
