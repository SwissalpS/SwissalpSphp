{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
<h2>{SssSBla value="BlaPlur"}</h2>
<p><a href="{WFURL action="edit"}">Add a new Bla000.</a></p>

<h3>{SssSBla value="BlaPlur"} {SssSBla value="SharedSearch"}</h3>
<div class="form-container">
{WFViewBlock id="searchBla000Form"}
    {WFView id="paginatorState"}
    <fieldset>
        <p><label for="query">{SssSBla value="AdminBlaEnterPartialUID"}:</label></p>
		<div>
			{WFView id="query"} {WFView id="search"} {WFView id="clear"}
		</div>
    </fieldset>
{/WFViewBlock}
</div>{* end form-container *}

<p>{WFView id="paginatorPageInfo"} {WFView id="paginatorNavigation"}</p>

<table border="0" cellspacing="0" cellpadding="5" class="datagrid">
{section name=items loop=$__module->valueForKeyPath('Bla000.arrangedObjectCount')}
    {if $smarty.section.items.first}
    <tr>
        <th>uid</th>
        <th></th>
        <th>{SssSBla value="Bla000Comment"}</th>
    </tr>
    {/if}
    <tr>
        <td>{WFView id="uid"}</td>
        <td>{WFView id="editLink"} {WFView id="deleteLink"}</td>
        <td>{WFView id="comment"}</td>
    </tr>
{sectionelse}
    <tr><td>{SssSBla value="AdminBlaNoneFound"}.</td></tr>
{/section}
</table>

<script>
{literal}
Event.observe(window, 'load', function() { document.forms.searchBla000Form.query.focus(); });
{/literal}
</script>
