{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
<h2>{SssSBla value="UsersSing"}</h2>
<div class="form-container">
{WFView id="statusMessage"}
{WFShowErrors id=editUsersForm}

{WFViewBlock id="editUsersForm"}
	{SssSBla value="UsersSing"} {SssSBla value="SharedDetail"}
	<table>
		<tr>
			<td class="rightJust">
				{SssSBla value="AdminUsersHandle"}
			</td>
			<td>
				{WFView id="handle"}{WFView id="handlel"}
			</td>
		</tr><tr>
			<td class="rightJust">
				{SssSBla value="AdminUsersRealName"}
			</td>
			<td>
				{WFView id="realname"}{WFShowErrors id="realname"}
			</td>
		</tr><tr>
			<td class="rightJust">
				{SssSBla value="SharedEmail"}
			</td>
			<td>
				{WFView id="email"}{WFShowErrors id="email"}
			</td>
		</tr><tr>
			<td class="rightJust">
				{SssSBla value="BridgesCountry"}
			</td>
			<td>
				{WFView id="country"}{WFShowErrors id="country"}{WFView id="selectCountry"}
			</td>
		</tr><tr>
			<td class="rightJust">
				{SssSBla value="BridgesRegion"}
			</td>
			<td>
				{WFView id="region"}{WFShowErrors id="region"}
			</td>
		</tr><tr>
			<td class="rightJust">
				{SssSBla value="SharedURL"}
			</td>
			<td>
				{WFView id="url"}{WFShowErrors id="url"}
			</td>
		</tr><tr>
			<td colspan="2">
				{SssSBla value="UserProfilePasswordChangeInfo"}
			</td>
		</tr><tr>
			<td class="rightJust">
				{SssSBla value="SharedPassword"}
			</td>
			<td>
				{WFView id="pass1"}
			</td>
		</tr><tr>
			<td class="rightJust">
				{SssSBla value="AdminUsersPassRepeat"}
			</td>
			<td>
				{WFView id="pass2"}
			</td>
		</tr><tr>
			<td class="rightJust">
				{SssSBla value="AdminUsersLangOrder"}
			</td>
			<td>
				{WFView id="languageorderl"}
			</td>
		</tr>{* only with certain privs may user see and/or change these <tr>
			<td>
				{SssSBla value="PPermsPlur"}
			</td>
			<td>
				{WFView id="multiSelectPPerms"}
				{WFView id="permissions"}{WFShowErrors id="permissions"}
			</td>
		</tr><tr>
			<td class="rightJust">
				oAuth
			</td>
			<td>
				{WFView id="oauth"}{WFShowErrors id="oauth"}
			</td>
		</tr><tr>
			<td class="rightJust">
				openID
			</td>
			<td>
				{WFView id="openid"}{WFShowErrors id="openid"}
			</td>
		</tr><tr>
			<td class="rightJust">
				{SssSBla value="AdminUsersAvatarMediaUID"}
			</td>
			<td>
				{WFView id="avatarmediauid"}{WFShowErrors id="avatarmediauid"}
			</td>
		</tr><tr>
			<td class="rightJust">
				Karma
			</td>
			<td>
				{WFView id="karma"}{WFShowErrors id="karma"}
			</td>
		</tr>*}
	</table>
	<div class="buttonrow">
		{WFView id="saveNew"}{WFView id="save"}{WFView id="deleteObj"}
	</div>
{/WFViewBlock}
{literal}<script>function remakePPermList(){var aOpt=document.getElementById("multiSelectPPerms").options;var iLen=aOpt.length;var a=new Array();for(i=0;iLen>i;i++){if(aOpt[i].selected){a.push(aOpt[i].value);};};document.getElementById("permissions").value=a.join();};

/*function updateMultiSel(){var aOpt=document.getElementById("multiSelectPPerms").options;var a=document.getElementById("permissions").value.split(',');var iLen=aOpt.length;for(i=0;i<iLen;i++){for(j=0;j<a.length;j++){if(aOpt[i].value==parseInt(a[j])){aOpt[i].selected=true;break;};};};};updateMultiSel();*/

/*var showHelp=function(forThis) {var oDialog=PHOCOA.runtime.getObject('helpDialog');if(!oDialog)return false;oDialog.cfg.setProperty('moduleViewInvocationPath','/help/for/noteNew/'+forThis);oDialog.show();return true;}*/

var updateCountry=function(){var oCountry=document.getElementById('country');var oSelectCountry=document.getElementById('selectCountry');if(!oCountry||!oSelectCountry)return;else oCountry.value=oSelectCountry.value;}

/*var updateLang=function() {var oLang=document.getElementById('lang');var oSelectLang=document.getElementById('selectLang');if(!oLang||!oSelectLang)return;else oLang.value=oSelectLang.value;}*/

var updateCountrySelector=function(){var oCountry=document.getElementById('country');var oSelectCountry=document.getElementById('selectCountry');if(!oCountry||!oSelectCountry)alert('Error switching country');else oSelectCountry.value=oCountry.value;}

/*var updateLangSelector=function(){var oLang=document.getElementById('lang');var oSelectLang=document.getElementById('selectLang');if(!oLang||!oSelectLang)alert('Error switching language');else oSelectLang.value=oLang.value;}*/

updateCountrySelector();</script>{/literal}
</div>{* end form-container *}
