<a name="channel"><h3 style="padding-top: 40px; margin-top: 40px;"></h3></a>
_____________________________
**channel("chan")** or `channel("chan1", "chan2")` in the case of widgets that accept two channels such as xypad, soundfiler and range widgets. `channel()` accepts a string/s that names the channel/s that Cabbage will communicate with Csound on. The current value of this widget can be retrieved in Csound using a `chnget`, or a `cabbageGetValue` opcode. Its value can be set using the `cabbageSet`, or `cabbageSetValue` opcodes. `hrange`, `vrange`, `xypad`, and `soundfiler` all take two channels:<br>

<b>hrange, vrange</b> : `channel("min", "max")` - min and max values

<b>xypad</b> : `channel("x", "y")` - x and y values

<b>soundfiler</b> : `channel("start", "length")` - start time and length of user selection, in samples

> Channels named should start with a letter and cannot have any white spaces. Note that all widgets should have a unique channel name. 

