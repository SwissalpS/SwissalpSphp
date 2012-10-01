{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
<h2>{SssSBla value="PPermsPlur"}</h2>
<p><a href="{WFURL action="edit"}">{SssSBla value="SharedAddNew" noEdit="true"} {SssSBla value="PPermsSing" noEdit="true"}.</a></p>

<h3>{SssSBla value="SharedSearch"} {SssSBla value="PPermsPlur"}</h3>
<div class="form-container">
{WFViewBlock id="searchPermpresetsForm"}
    {WFView id="paginatorState"}
    <fieldset>
        <p><label for="query">{SssSBla value="SharedEnterPartial"} {SssSBla value="AdminPPermsName"}:</label></p>
		<div>
			{WFView id="query"} {WFView id="search"} {WFView id="clear"}
		</div>
    </fieldset>
{/WFViewBlock}
</div>{* end form-container *}

<p>{WFView id="paginatorPageInfo"} {WFView id="paginatorNavigation"}</p>

<table border="0" cellspacing="0" cellpadding="5" class="datagrid">
{section name=items loop=$__module->valueForKeyPath('Permpresets.arrangedObjectCount')}
    {if $smarty.section.items.first}
    <tr>
        <th>{SssSBla value="PPermsPlur"}</th>
        <th></th>
    </tr>
    {/if}
    <tr>
        <td>{WFView id="name"}</td>
        <td>{WFView id="editLink"} {WFView id="deleteLink"}</td>
    </tr>
{sectionelse}
    <tr><td>{SssSBla value="AdminPermsListNoPPermsFound"}.</td></tr>
{/section}
</table>

<script>
{literal}
Event.observe(window, 'load', function() { document.forms.searchPermpresetsForm.query.focus(); });
{/literal}
</script>
