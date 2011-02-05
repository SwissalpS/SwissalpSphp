{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
<h2>{SssSBla value="PermsSing"}</h2>
<div class="form-container">
{WFView id="statusMessage"}
{WFShowErrors id=editPermissionsForm}

{WFViewBlock id="editPermissionsForm"}
    <fieldset>
    <legend>{SssSBla value="PermsSing"} {SssSBla value="SharedDetail"}</legend>

                        {WFView id="uid"}
                                <div>
                <label for="domain">{SssSBla value="AdminPermsDomain"}:</label>
                {WFView id="domain"}{WFShowErrors id="domain"}
            </div>
                                <div>
                <label for="hash">{SssSBla value="AdminPermsHash"}:</label>
                {WFView id="hash"}{WFShowErrors id="hash"}
                {* WFViewBlock id="hashBoxGroup" *}
                	{WFView id="isSuperUser"}
                	{WFView id="mayRead"}
                	{WFView id="mayWrite"}
                	{WFView id="mayAdmin"}
                {* /WFViewBlock *}
            </div>
{literal}<script>function reCalcHash(){var tf_hash=document.getElementById("hash");var cb_SU=document.getElementById("isSuperUser");var cb_MR=document.getElementById("mayRead");var cb_MW=document.getElementById("mayWrite");var cb_MA=document.getElementById("mayAdmin");var hash=((cb_SU.checked)?parseFloat(cb_SU.value):0.0)+((cb_MR.checked)?parseFloat(cb_MR.value):0.0)+((cb_MW.checked)?parseFloat(cb_MW.value):0.0)+((cb_MA.checked)?parseFloat(cb_MA.value):0.0);tf_hash.value=hash;};function updateCheckboxes(){var tf_hash=document.getElementById("hash");var cb_SU=document.getElementById("isSuperUser");var cb_MR=document.getElementById("mayRead");var cb_MW=document.getElementById("mayWrite");var cb_MA=document.getElementById("mayAdmin");var hash=parseFloat(tf_hash.value);cb_SU.checked=(cb_SU.value<=hash);cb_MR.checked=(hash&cb_MR.value);cb_MW.checked=(hash&cb_MW.value);cb_MA.checked=(hash&cb_MA.value);};updateCheckboxes();</script>{/literal}
                <div class="buttonrow">
        {WFView id="saveNew"}{WFView id="save"}{WFView id="deleteObj"}
    </div>
    </fieldset>
{/WFViewBlock}
</div>{* end form-container *}
