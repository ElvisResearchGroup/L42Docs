m4_include(`header.h')
m4 dnl
WConversion
m4_include(`postHeader.h')m4_dnl

m4_define(`url',`https://l42.is/libraries/')
m4_define(`combine', `$1$2')

OFloatClass(index)
<div class="rotate90"> Index of Libraries</div>
ContinueFloat
<ol>
WMenuItem(combine(url,`Time.xhtml'),`Time')
WMenuItem(combine(url, `GuiBuilder.xhtml'),`GuiBuilder')
WMenuItem(combine(url, `JavaServer.xhtml'),`JavaServer')
WMenuItem(combine(url, `Json.xhtml'),`Json')
WMenuItem(combine(url, `Process.xhtml'),`Process')
WMenuItem(combine(url, `Query.xhtml'),`Query')
WMenuItem(combine(url, `RawQuery.xhtml'),`RawQuery')
</ol>
CFloat
<p> Here you can find the documentation for the 42 modules and towels
</p>
m4_include(`footer.h')