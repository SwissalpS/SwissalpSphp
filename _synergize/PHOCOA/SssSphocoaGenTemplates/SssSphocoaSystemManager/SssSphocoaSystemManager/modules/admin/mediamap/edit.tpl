{* vim: set expandtab tabstop=4 shiftwidth=4 syntax=smarty: *}
<h2>Mediamap</h2>
<div class="form-container">
{WFView id="statusMessage"}
{WFShowErrors id=editMediamapForm}

{WFViewBlock id="editMediamapForm"}
    <fieldset>
    <legend>Mediamap Detail</legend>

                        {WFView id="mediauid"}
                                <div>
                <label for="url">url:</label>
                {WFView id="url"}{WFShowErrors id="url"}
            </div>
                                <div>
                <label for="type">type:</label>
                {WFView id="type"}{WFShowErrors id="type"}
            </div>
                                <div>
                <label for="width">width:</label>
                {WFView id="width"}{WFShowErrors id="width"}
            </div>
                                <div>
                <label for="height">height:</label>
                {WFView id="height"}{WFShowErrors id="height"}
            </div>
                                <div>
                <label for="cssclass">cssclass:</label>
                {WFView id="cssclass"}{WFShowErrors id="cssclass"}
            </div>
                                <div>
                <label for="mime">mime:</label>
                {WFView id="mime"}{WFShowErrors id="mime"}
            </div>
                                <div>
                <label for="karma">karma:</label>
                {WFView id="karma"}{WFShowErrors id="karma"}
            </div>
                <div class="buttonrow">
        {WFView id="saveNew"}{WFView id="save"}{WFView id="deleteObj"}
    </div>
    </fieldset>
{/WFViewBlock}
</div>{* end form-container *}