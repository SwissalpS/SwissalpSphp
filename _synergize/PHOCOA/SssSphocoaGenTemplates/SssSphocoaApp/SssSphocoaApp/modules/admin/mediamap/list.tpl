{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
<h2>Mediamap</h2>
<p><a href="{WFURL action="edit"}">Add a new Mediamap.</a></p>

<h3>Mediamap Search</h3>
<div class="form-container">
{WFViewBlock id="searchMediamapForm"}
    {WFView id="paginatorState"}
    <fieldset>
        <p><label for="query">Enter a partial url:</label></p>
		<div>
			{WFView id="query"} {WFView id="search"} {WFView id="clear"}
		</div>
    </fieldset>
{/WFViewBlock}
</div>{* end form-container *}

<p>{WFView id="paginatorPageInfo"} {WFView id="paginatorNavigation"}</p>

<table border="0" cellspacing="0" cellpadding="5" class="datagrid">
{section name=items loop=$__module->valueForKeyPath('Mediamap.arrangedObjectCount')}
    {if $smarty.section.items.first}
    <tr>
        <th>Mediamap</th>
        <th></th>
    </tr>
    {/if}
    <tr>
        <td>{WFView id="url"}</td>
        <td>{WFView id="editLink"} {WFView id="deleteLink"}</td>
    </tr>
{sectionelse}
    <tr><td>No Mediamap(s) found.</td></tr>
{/section}
</table>

<script>
{literal}
Event.observe(window, 'load', function() { document.forms.searchMediamapForm.query.focus(); });
{/literal}
</script>
