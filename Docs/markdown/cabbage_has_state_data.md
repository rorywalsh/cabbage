# cabbageHasStateData

Will read the JSON data from a plugin's internal state and output it as a string. 

<blockquote style="font-style:italic;border-left:10px solid #93d200;color:rgb(3, 147, 210);padding:1px;padding-left:10px;margin-top:0px;margin-bottom:1px;border-left-width:0.25rem"> Added in Cabbage v2.5.44</blockquote>

### Syntax

<pre>kRes <b>cabbageHasStateData</b></pre>

#### Initialization



<blockquote style="font-style:italic;border-left:10px solid #93d200;color:rgb(3, 147, 210);padding:1px;padding-left:10px;margin-top:0px;margin-bottom:1px;border-left-width:0.25rem"> Due to how and when hosts load saved session data, this opcodes works at perf-time (k-rate) only. This means the opcode will most likely return 0 in the first few k-cycles.</blockquote>

* `kRes` -- returns 1 if state data is accessible, or 0 if not. If there is no state data in the loaded session the result will be 0, until some state data has been saved. 

### Example

```csharp
<Cabbage>
form caption("Test") size(350, 300), guiMode("queue"), colour(58, 110, 182), pluginId("sfi1")
rslider bounds(20, 8, 60, 60) range(0, 1, 0, 1, 0.001), channel("gain"), text("Gain")
button bounds(100, 10, 80, 30), channel("saveButton"), text("Save"), latched(0)
button bounds(200, 10, 80, 30), channel("showButton"), text("Show"), latched(0)
csoundoutput bounds(14, 60, 300, 200) channel("csoundoutput1"), fontColour(147, 210, 0)
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
 
/* I'm using latched buttons here, if I use normal buttons their value could be
1 when the session is saved, as their default value is 0 it will cause a new state
value to be written when the session loads, thus overwriting the previous one! */ 


instr 1
    kHaveState = cabbageHasStateData()  
  
    if changed:k(kHaveState) == 1 then 
            printks "State found", 0
    endif
 
    kButtonSave, kTrig1 cabbageGetValue "saveButton"   
    kButtonShow, kTrig2 cabbageGetValue "showButton" 

    if kTrig1 == 1 && kButtonSave == 1 then
        cabbageSetStateValue "key1", cabbageGetValue:k("gain")
    endif

    if kTrig2 == 1 && kButtonShow == 1 then
        kData cabbageGetStateValue "key1" 
        printk 0, kData
    endif
endin

</CsInstruments>
<CsScore>
i1 0 z
</CsScore>
</CsoundSynthesizer>
```
