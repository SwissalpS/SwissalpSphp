{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
<h2>{SssSBla value="PermsPlur"}</h2>
<p><a href="{WFURL action="edit"}">{SssSBla value="SharedAddNew" noEdit="true"} {SssSBla value="PermsSing" noEdit="true"}.</a></p>

<h3>{SssSBla value="SharedSearch"} {SssSBla value="PermsPlur"}</h3>
<div class="form-container">
{WFViewBlock id="searchPermissionsForm"}
    {WFView id="paginatorState"}
    <fieldset>
        <p><label for="query">{SssSBla value="SharedEnterPartial"} {SssSBla value="AdminPermsDomain"}:</label></p>
		<div>
			{WFView id="query"} {WFView id="search"} {WFView id="clear"}
		</div>
    </fieldset>
{/WFViewBlock}
</div>{* end form-container *}

<p>{WFView id="paginatorPageInfo"} {WFView id="paginatorNavigation"}</p>

<table border="0" cellspacing="0" cellpadding="5" class="datagrid">
{section name=items loop=$__module->valueForKeyPath('Permissions.arrangedObjectCount')}
    {if $smarty.section.items.first}
    <tr>
        <th>uid</th>
        <th>{SssSBla value="AdminPermsDomain"}</th>
        <th>{SssSBla value="AdminPermsHash"}</th>
    </tr>
    {/if}
    <tr>
        <td>{WFView id="uid"}</td>
        <td>{WFView id="domain"}</td>
        <td>{WFView id="hash"}</td>
        <td>{WFView id="editLink"} {WFView id="deleteLink"}</td>
    </tr>
{sectionelse}
    <tr><td>{SssSBla value="AdminPermsListNoPermsFound"}.</td></tr>
{/section}
</table>

<script>
{literal}
Event.observe(window, 'load', function() { document.forms.searchPermissionsForm.query.focus(); });
{/literal}
</script>
