# cabbageSetStateValue

Sets a value in the plugin's internal state object. Can set scalars or arrays. 

### Syntax

<pre><b>cabbageSetStateValue</b> SKeyName, SData</pre>
<pre><b>cabbageSetStateValue</b> SKeyName, kData</pre>
<pre><b>cabbageSetStateValue</b> SKeyName, iData</pre>
<pre><b>cabbageSetStateValue</b> SKeyName, SData[]</pre>
<pre><b>cabbageSetStateValue</b> SKeyName, kData[]</pre>

### Example

<pre><b>cabbageSetStateValue</b> "freq", <b>chnget:i</b>("sliderValue")
<b>cabbageSetStateValue</b> "pi", <b>chnget:i</b>("sliderValue")
SArr[] <b>fillarray</b> "1", "2", "3", "4"
<b>cabbageSetStateValue</b> "stringList", SArr</pre>





