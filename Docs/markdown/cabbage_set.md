# cabbageSet

Sets a widget's attribute identifier. Identifiers can be updated by a single string argument, or by passing multiple arguments that comprise the identifiers. If used at perf-time you must pass a trigger signal as the first argument. The i-rate version of this opcode does not need a trigger argument. Multiple identifers can also be updated by passing entire identifier strings   

### Syntax

<pre><b>cabbageSet</b> kTrig, SChannel, SIdentifier, XArgs</pre>
<pre><b>cabbageSet</b> kTrig, SChannel, SIdentifierString</pre>
<pre><b>cabbageSet</b> SChannel, SIdentifierString</pre>


### Example

<pre><b>cabbageSet</b> metro(1), "image1", "colour", random:k(0, 255), random:k(0, 255), random:k(0, 255)
<b>cabbageSet</b> metro(1), "image2", "bounds(10, 10, 100, 100)"</pre>

In the above code, the widget with channel "image1" will have its colour update every second, and the widget with channel "image2" will have it position set every 1 second. It is also possible to use `sprintfk` with these opcodes. The two lines of code below achieve the same effect. One uses arguments, and the other a `sprintfk`.

<pre><b>cabbageSet</b> metro(1), "image1", "bounds", 10, 10, random:k(0, 255), random:k(0, 255)
<b>cabbageSet</b> metro(1), "image2", <b>sprintfk<b>("bounds(10, 10, %d, %d)", random:k(0, 255), random:k(0, 255))</pre>

