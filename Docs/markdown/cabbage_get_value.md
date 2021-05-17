# cabbageGetValue

Gets the current value of a widget, for example the current value of a `slider`, `combobox`, `button`, etc. You can add an optional trigger output argument. This will send a trigger signal of 1 whenever the value has changed.

### Syntax

<pre>kValue <b>cabbageGetValue</b> SChannel</pre>
<pre>SValue <b>cabbageGetValue</b> SChannel</pre>
<pre>iValue <b>cabbageGetValue</b> SChannel</pre>
<pre>kValue [, kTrig] <b>cabbageGetValue</b> SChannel</pre>
<pre>SValue [, kTrig] <b>cabbageGetValue</b> SChannel</pre>

#### Initialization

* `iValue` -- widget value
* `SChannel` -- widget's channel name

#### Performance

* `kValue` -- widget value
* `kTrig` -- an optional trigger output. Will be 1 for a single k-cycle whenever the widget value has changed. Otherwise it will remain 0.  


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
    kSliderValue cabbageGetValue "gain"
    printk2 kSliderValue
endin

</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>
```
