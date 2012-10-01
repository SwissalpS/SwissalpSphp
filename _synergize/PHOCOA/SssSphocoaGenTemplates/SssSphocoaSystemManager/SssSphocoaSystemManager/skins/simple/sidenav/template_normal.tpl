<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		{$skinHead}
		{WFSkinCSSYAHOOBase hosted="local"}
		{WFSkinCSS file="default.css"}
		{* this messes with umlauts even tho it should guarantee that they show correctly*}<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<meta name="format-detection" content="telephone=no" />
	</head>
	<body class="yui-skin-sam">
		<div id="doc4" class="{$skinThemeVars.yuiTemplate}">
			<div id="hd">
				<a href="/"><img src="{$skinDirShared}/phocoa-logo.png" alt="PHOCOA - a php web framework" /></a>{WFSkinModuleView invocationPath="admin/panel/show"}
			</div>
			<div id="bd">
				<div id="yui-main">
					<div class="yui-b">
						{$skinBody}
					</div>
				</div>
				<div class="yui-b">
					{WFSkinModuleView invocationPath="menu/menu/mainMenu/0"}
				</div>
			</div>
			<div id="ft">
				{$skin->namedContent('copyright')}				{$skin->namedContent('copyright')} | {$skin->namedContent('logLink')}

			</div>
		</div>
	</body>
	<script>{literal}
	Event.observe(window, 'load', function() {
		// trick for iFon address bar hiding
		window.scrollTo(0,1);
	});
	{/literal}</script>
</html>
