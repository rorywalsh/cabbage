# cabbageGetStateValue

Will retrieve a value from a plugin's internal state object. Can retrieve scalars and arrays. 

### Syntax

<pre>iVal <b>getStateValue</b> SKeyName</pre>
<pre>kVale <b>getStateValue</b> SKeyName</pre>
<pre>SVal <b>getStateValue</b> SKeyName</pre>
<pre>iVal[] <b>getStateValue</b> SKeyName</pre>
<pre>kVal[] <b>getStateValue</b> SKeyName</pre>
<pre>SVal[] <b>getStateValue</b> SKeyName</pre>

### Example

<pre>SVal[] <b>getStateValue</b> "stringList"
<b>printarray</b> SVal, 1, "%s"
SVal[] <b>getStateValue</b> "stringList"
<b>printarray</b> SVal, 1, "%s"</pre>



