# cabbageGetValue

Gets the current value of a widget, for example the current value of a `slider`, `combobox`, `button`, etc. You can add an optional trigger output argument. This will send a trigger signal of 1 whenever the value has changed.

### Syntax

<pre>kValue <b>cabbageGetValue</b> SChannel</pre>

<pre>SValue <b>cabbageGetValue</b> SChannel</pre>

<pre>iValue <b>cabbageGetValue</b> SChannel</pre>

<pre>kValue [, kTrig] <b>cabbageGetValue</b> SChannel</pre>

<pre>SValue [, kTrig] <b>cabbageGetValue</b> SChannel</pre>


### Example

<pre>kSliderValue, kSliderHasUpdated <b>cabbageGetValue</b> "freq"</pre>

`kSliderValue` will always hold the current slider value, while `kSliderHasUpdated` will send a one each time the value of the slider changes.
