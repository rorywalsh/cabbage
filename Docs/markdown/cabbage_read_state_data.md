# cabbageReadStateData

Will read the JSON data from a plugin's internal state and output it as a string. 

### Syntax

<pre>SData <b>cabbageReadStateData</b></pre>

#### Initialization

* `SData` -- the plugin's internal state data as a JSON string

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
    prints cabbageReadStateData()
endin

</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>
```

> Added in Cabbage v2.5.44