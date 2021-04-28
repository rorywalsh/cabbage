# cabbageSetValue

Sets the current numeric value of a widget, for example the current value of a `slider`, `combobox`, `button`, etc. If you add an optional trigger input argument, updates will only be sent to the widget when the trigger variable is 1.

### Syntax

<pre><b>cabbageGetValue</b> SChannel, kValue [, kTrig]</pre>


### Example

<pre><b>cabbageSetValue</b> "freq", random:k(1, 100)</pre>

In the above code, the widget with channel "freq" will be updated on each k-cycle with a random value between 0 and 100.  
