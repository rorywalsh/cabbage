# cabbageGet

Gets the widget identifier value, for example the current colour, size, position, etc. You can add an optional trigger output argument. This will send a trigger signal of 1 whenever the value has changed. 

### Syntax

<pre>kIdent <b>cabbageGet</b> SChannel, SIdentifier</pre>
<pre>SIdent <b>cabbageGet</b> SChannel, SIdentifier</pre>
<pre>kIdent [, kTrig] <b>cabbageGet</b> SChannel, SIdentifier</pre>
<pre>SIdent [, kTrig] <b>cabbageGet</b> SChannel, SIdentifier</pre>

<pre>kIdent[] <b>cabbageGet</b> SChannel, SIdentifier</pre>
<pre>SIdent[] <b>cabbageGet</b> SChannel, SIdentifier</pre>
<pre>kIdent[] [, kTrig] <b>cabbageGet</b> SChannel, SIdentifier</pre>
<pre>SIdent[] [, kTrig] <b>cabbageGet</b> SChannel, SIdentifier</pre>

<pre>kChannelValue <b>cabbageGet</b> SChannel</pre>
<pre>SChannelValue <b>cabbageGet</b> SChannel</pre>
<pre>kChannelValue [, kTrig] <b>cabbageGet</b> SChannel</pre>
<pre>SChannelValue [, kTrig] <b>cabbageGet</b> SChannel</pre>


### Example

<pre>kBounds[] <b>cabbageGet</b> "button1", "bounds"</pre>
<pre>printarray kBounds</pre>

In the above code, Csound will print the x, y, width and height values for the widget with the channel "button1". 

>Note that the versions of <b>cabbageSet</b> that only take a a single channel string are typically used to query [reserved channels](./macros_and_reserved_channels.md). 
