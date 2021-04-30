# cabbageWriteStateData

Writes a JSON string to a plugin's internal state. This opcode is only used if you wish to set a lot of data in a single operation. It is more practical to simply call `cabbageSetStateValue` whenever you wish to add or modify internal state data.

### Syntax

<pre><b>cabbageWriteStateData</b> SData </pre>

#### Initialization

* `SData` -- a JSON string

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
    cabbageWriteStateData "{\"happy\": true, \"pi\": 1.141, \"test\": \"hello\", \"list\":[1, 0, 2, 3, 4, 5, 2, 3, 6],\"stringList\":[\"hi\", \"who\", \"goes\", \"there\"]"
endin

</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>
```


