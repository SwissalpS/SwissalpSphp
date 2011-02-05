{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
<h2>{SssSBla value="NotePlur"}</h2>
<div class="form-container">
{WFView id="statusMessage"}
{WFShowErrors id=editNotesForm}

{WFViewBlock id="editNotesForm"}
	{WFView id="uid"}{WFView id="lang"}{WFView id="country"}
	<table>
		<tr>
			<td class="rightJust"><b>{SssSBla value="SharedName"}</b></td>
			<td>{WFView id="name"}{WFShowErrors id="name"}</td>
			<td>{WFView id="helpName"}</td>
			<td class="rightJust"><b>{SssSBla value="SharedLanguage"}</b></td>
			<td>{WFView id="selectLang"}</td>
			<td>{WFView id="helpLanguage"}</td>
		</tr>
		<tr>
			<td class="rightJust"><b>{SssSBla value="SharedEmail"}</b></td>
			<td>{WFView id="email"}{WFShowErrors id="email"}</td>
			<td>{WFView id="helpEmail"}</td>
			<td class="rightJust"><b>{SssSBla value="BridgesCountry"}</b></td>
			<td>{WFView id="selectCountry"}</td>
			<td>{WFView id="helpCountry"}</td>
		</tr>
		<tr>
			<td class="rightJust"><b>{SssSBla value="SharedURL"}</b></td>
			<td>{WFView id="url0"}{WFShowErrors id="url0"}</td>
			<td>{WFView id="helpUrl0"}</td>
			<td class="rightJust" width="50px"><b>{SssSBla value="BridgesRegion"}</b></td>
			<td>{WFView id="region"}{WFShowErrors id="region"}</td>
			<td>{WFView id="helpRegion"}</td>
		</tr>
		<tr>
			<td class="rightJust" colspan="4"><b>{SssSBla value="SharedUploadMedia"}</b><br />{SssSBla value="NoteCopyrightUL"}</td>
			<td>{WFView id="uploadMedia"}{WFShowErrors id="uploadMedia"}</td>
			<td>{WFView id="helpUploadMedia"}</td>
		</tr>
		<tr>
			<td class="rightJust"><b>{SssSBla value="NoteSing"}</b><br /><font color="red">{SssSBla value="NoteMandatory"}</font></td>
			<td colspan="4">{WFView id="note"}{WFShowErrors id="note"}</td>
			<td>{WFView id="helpNote"}</td>
		</tr>
		<tr>
			<td colspan="6">{SssSBla value="AdminNotesBeCarefull"}</td>
		</tr>
		<tr>
			<td colspan="6">
			<div class="AdminNoteEditItem">
				{SssSBla value="BridgesSing"}:
				{WFView id="selectBridge"}{WFView id="bridgeuid"}{WFShowErrors id="bridgeuid"}
			</div>
			<div class="AdminNoteEditItem">
				{SssSBla value="AdminNoteDate"}:
				{WFView id="date"}{WFShowErrors id="date"}
			</div>
			<div class="AdminNoteEditItem">
				{WFView id="karma"}{SssSBla value="AdminNoteKarma"}
				{WFShowErrors id="karma"}
			</div>
			<div class="AdminNoteEditItem">
				{WFView id="publish"} {SssSBla value="AdminNotePublish"}
				{WFShowErrors id="publish"}
			</div><br />
			<div class="AdminNoteEditItem">
				{SssSBla value="AdminNoteHandle"}:
				{WFView id="handle"}{WFShowErrors id="handle"}
			</div><br />
			<div class="AdminNoteEditItem">
				{SssSBla value="AdminNoteMediaUID"}:
				{WFView id="mediauid"}{WFShowErrors id="mediauid"}
			</div>
			<div class="AdminNoteEditItem">
				{SssSBla value="AdminNoteUrl1"}:
				{WFView id="url1"}{WFShowErrors id="url1"}
			</div>
			<div class="AdminNoteEditItem">
				{SssSBla value="AdminNoteChildren"}:
				{WFView id="children"}{WFShowErrors id="children"}
			</div></td>
		</tr>
	</table>
				<div class="buttonrow">
		{WFView id="saveNew"}{WFView id="save"}<br /><br />{WFView id="deleteObj"}
	</div>
{/WFViewBlock}
{literal}<script>
function updateBridge() {var oBridge=document.getElementById('bridgeuid');var oSelectBridge=document.getElementById('selectBridge');if(!oBridge||!oSelectBridge)alert('Error switching bridge');else oBridge.value=oSelectBridge.value;};
function updateCountry() {var oCountry=document.getElementById('country');var oSelectCountry=document.getElementById('selectCountry');if(!oCountry||!oSelectCountry)alert('Error switching country');else oCountry.value=oSelectCountry.value;};
function updateLang() {var oLang=document.getElementById('lang');var oSelectLang=document.getElementById('selectLang');if(!oLang||!oSelectLang)alert('Error switching language');else oLang.value=oSelectLang.value;};
function showHelp(forThis){var oDialog=document.getElementById('helpDialog');var oHelp=document.getElementById('helpModule');if(!oDialog||!oHelp)alert('Error showing help');else{oHelp.invocationPath='help/note/'+forThis;oDialog.show();}};
</script>{/literal}
</div>{* end form-container *}
