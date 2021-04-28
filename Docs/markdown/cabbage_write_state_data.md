# cabbageWriteStateData

Will write JSON data to the plugins internal state, if it saved in a DAW session. You can choose to update or overwrite the existing data.

### Syntax

<pre><b>cabbageWriteStateData</b> iMode, SJSONData</pre>


### Example

<pre><b>cabbageWriteStateData 0, "{ \"happy\": true, \"pi\": 3.141}"</pre>

You only need to this opcode if you wish to write lots of JSON data to state in a single operation. In practice it's easier to simply call `cabbageSetStateValue`. 

