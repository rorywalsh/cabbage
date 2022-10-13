# cabbageReadStateData

Will read the JSON data from a plugin's internal state and output it as a string. 

<blockquote style="font-style:italic;border-left:10px solid #93d200;color:rgb(3, 147, 210);padding:1px;padding-left:10px;margin-top:0px;margin-bottom:1px;border-left-width:0.25rem"> Added in Cabbage v2.5.44</blockquote>

### Syntax

<pre>SData <b>cabbageReadStateData</b></pre>

#### Initialization


<blockquote style="font-style:italic;border-left:10px solid #93d200;color:rgb(3, 147, 210);padding:1px;padding-left:10px;margin-top:0px;margin-bottom:1px;border-left-width:0.25rem"> Although this opcode work at both init (i-time) and perf-time (k-rate), the state data might not be available in the first k-cycle. This is down to how hosts load saved session data. So if you are using this opcode for debug purposes, delay it by some cycles to be sure you get access to the data. </blockquote>

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
i1 1 z; delay start of instrument 1 to make sure the state data is there
</CsScore>
</CsoundSynthesizer>
```
