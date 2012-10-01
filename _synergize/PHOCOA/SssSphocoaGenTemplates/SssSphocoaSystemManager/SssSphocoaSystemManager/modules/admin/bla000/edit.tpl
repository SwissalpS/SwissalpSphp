{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
<h2>{SssSBla value="BlaSing"}</h2>
<div class="form-container">
{WFView id="statusMessage"}
{WFShowErrors id=editBla000Form}

{WFViewBlock id="editBla000Form"}
	<table width="200" border="0" cellpadding="1" cellspacing="1">
        <tbody>
            <tr>
                <td>uid</td>
                <td>{WFView id="uid"}</td>
                <td>&nbsp;</td>
            </tr>
            <tr>
                <td>{SssSBla value="Bla000Comment"}</td>
                <td>{WFView id="comment"}</td>
                <td>{WFShowErrors id="comment"}</td>
            </tr>
            <tr>
                <td><a name="enTF"></a>{SssSBla value="Bla000LangEN"}</td>
                <td>{WFView id="en"}</td>
                <td><a href="#scratch">Jump2scratch</a>{WFShowErrors id="en"}</td>
            </tr>
            <tr>
                <td>{SssSBla value="Bla000LangDE"}</td>
                <td>{WFView id="de"}</td>
                <td>{WFShowErrors id="de"}</td>
            </tr>
            <tr>
                <td>{SssSBla value="Bla000LangFR"}</td>
                <td>{WFView id="fr"}</td>
                <td>{WFShowErrors id="fr"}</td>
            </tr>
            <tr>
                <td>{SssSBla value="Bla000LangIT"}</td>
                <td>{WFView id="it"}</td>
                <td>{WFShowErrors id="it"}</td>
            </tr>
            <tr>
                <td>{SssSBla value="Bla000LangRM"}</td>
                <td>{WFView id="rm"}</td>
                <td>{WFShowErrors id="rm"}</td>
            </tr>
            <tr>
            	<td><a name="scratch"></a>{SssSBla value="Bla000ScratchHTML"}</td>
                <td>{WFView id="scratchHTML"}</td>
                <td>&nbsp;</td>
            </tr>
        </tbody>
    </table>
    <div class="buttonrow">
        {WFView id="saveNew"}{WFView id="save"}{WFView id="deleteObj"}
    </div>
{/WFViewBlock}
</div>{* end form-container *}
