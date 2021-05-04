# cabbageCreate

This opcode can be used to create widgets directly within a Csound orchestra.  

### Syntax

<pre><b>cabbageSetValue</b> SChannel, kValue [, kTrig]</pre>
<pre><b>cabbageSetValue</b> SChannel, iValue</pre>

#### Initialization

* `iValue` -- widget value
* `SChannel` -- widget's channel name

#### Performance

* `kValue` -- widget value
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
