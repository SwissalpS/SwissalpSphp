{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
<h2>{SssSBla value="NotePlur"}</h2>
<span style="padding-left:25px;">&nbsp;</span><div class="addNewNoteContainer" onMouseUp="document.location=this.children(0);">{WFView id="addNewNoteImage"}</div><div class="addNewNoteContainer" onMouseUp="SssSopenNoteDialog();">{WFView id="addNewNoteLink"}</div>

<h3>Notes Search</h3>
<div class="form-container">
{WFViewBlock id="searchNotesForm"}
    {WFView id="paginatorState"}
    <fieldset>
        <p><label for="query">{WFView id="adminBlaEditCriteria"}{WFView id="selectSearchCriteria"}:</label></p>
		<div>
			{WFView id="query"} {WFView id="search"} {WFView id="clear"}
		</div>
    </fieldset>
{/WFViewBlock}
</div>{* end form-container *}

<p>{WFView id="paginatorPageInfo"} {WFView id="paginatorNavigation"}</p>

<table border="0" cellspacing="0" cellpadding="5" class="datagrid">
{section name=items loop=$__module->valueForKeyPath('Notes.arrangedObjectCount')}
    {if $smarty.section.items.first}
    <tr>
        <th>{WFView id="paginatorSortName"}</th>
        <th>{WFView id="paginatorSortLang"}</th>
        <th>{WFView id="paginatorSortCountry"}</th>
        <th>{WFView id="paginatorSortNote"}</th>
        <th>{WFView id="paginatorSortBridge"}</th>
        <th>{WFView id="paginatorSortDate"}</th>
        <th>{WFView id="paginatorSortKarma"}</th>
        <th>{WFView id="paginatorSortPublish"}</th>
        <th></th>
    </tr>
    {/if}
    <tr>
        <td>{WFView id="name"}</td>
        <td>{WFView id="lang"}</td>
        <td>{WFView id="country"}</td>
        <td>{WFView id="note"}</td>
        <td>{WFView id="bridge"}</td>
        <td>{WFView id="date"}</td>
        <td>{WFView id="karma"}</td>
        <td>{WFView id="publish"}</td>
        <td>{WFView id="editLink"} {WFView id="deleteLink"}</td>
    </tr>
{sectionelse}
    <tr><td>{SssSBla value="NotesNoneFound"}</td></tr>
{/section}
</table>
{WFView id="newNoteDialog"}
<script>{literal}
function SssSopenNoteDialog(){
//PHOCOA.namespace('widgets.addNewNoteLink.events.click');
	var noteDlg=PHOCOA.runtime.getObject('newNoteDialog');
	//var noteDlg=document.getElementById('newNoteDialog');
	if(noteDlg)noteDlg.show();else alert('{/literal}{SssSBla value="SysPleaseWait4Page2Load" noDIV="true"}{literal}');}
{/literal}</script><noscript>{WFView id="noScriptAddNewNote"}</noscript>
{WFView id="zoomViewDialog"}

<script>
{literal}
Event.observe(window, 'load', function() { document.forms.searchNotesForm.query.focus(); });
{/literal}
</script>
