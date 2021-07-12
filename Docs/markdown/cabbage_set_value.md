# cabbageSetValue

Sets the current numeric value of a widget, for example the current value of a `slider`, `combobox`, `button`, etc. If you add an optional trigger input argument, updates will only be sent to the widget when the trigger variable is 1.

<blockquote style="font-style:italic;border-left:10px solid #93d200;color:rgb(3, 147, 210);padding:1px;padding-left:10px;margin-top:0px;margin-bottom:1px;border-left-width:0.25rem"> Added in Cabbage v2.5.44</blockquote>

### Syntax

<pre><b>cabbageSetValue</b> SChannel, kValue [, kTrig]</pre>
<pre><b>cabbageSetValue</b> SChannel, iValue</pre>
<pre><b>cabbageSetValue</b> SChannel, SValue [, kTrig]</pre>
<pre><b>cabbageSetValue</b> SChannel, SValue</pre>

> Note that you must be explicit with string types as to the rate of the opcode. If you want to update strings at k-rate, you must include a trigger variable and you must wrap your string value in a `sprintfk` opcode, i.e, `cabbageSetValue "combo", sprintfk("%s", "3c"), 1`. Failure to do so will result in the i-time version of cabbageSetValue for strings being called.  

#### Initialization

* `i/S/Value` -- widget value
* `SChannel` -- widget's channel name

#### Performance

* `k/S/Value` -- widget value
* `kTrig` -- an optional trigger. Updates will only be sent when this is equal to 1


### Example

```csharp
<Cabbage>
form caption("Test") size(350, 200), guiMode("queue"), colour(58, 110, 182), pluginId("sfi1")
rslider bounds(20, 8, 60, 60) range(0, 1, 0, 1, 0.001), channel("gain"), text("Gain")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
    cabbageSetValue "gain", random:k(0, 1)
endin


</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>
```
