# cabbageGetWidgetChannels

This opcode will return an array of widget channel names. An optional string arugment can be used to filter the widget channels, based on identifiers that have been set. This opcode runs at i-time only. 

<blockquote style="font-style:italic;border-left:10px solid #93d200;color:rgb(3, 147, 210);padding:1px;padding-left:10px;margin-top:0px;margin-bottom:1px;border-left-width:0.25rem"> Added in Cabbage v2.6.9</blockquote>

### Syntax

<pre>SChannels[] <b>cabbageGetWidgetChannels</b> [SIdentifierFilter]</pre>

#### Initialization

* `SIdentifierFiler` -- optional filter in the form of Cabbage identifier syntax. For example, if this is set to `automatable(1)`, only widgets with `automatable(1)` will be added to the output array.  
* `SChannels[]` -- a string array of widget channel names

### Example

```csharp
<Cabbage>
form caption("Presets") size(370, 280), guiMode("queue"), colour(58, 110, 182), pluginId("MPre")
keyboard bounds(10, 90, 345, 95)
rslider bounds(12, 8, 85, 79), channel("att"), range(0, 1, 0.01), text("Att.")
rslider bounds(98, 8, 85, 79), channel("dec"), range(0, 1, 0.4), text("Dec.")
rslider bounds(184, 8, 85, 79), channel("sus"), range(0, 1, 0.7), text("Sus.")
rslider bounds(270, 8, 85, 79), channel("rel"), range(0, 1, 0.8), text("Rel.")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs = 1

;instrument will be triggered by keyboard widget
instr 1
    SNames[] cabbageGetWidgetChannels "automatable(1)"
    prints "\nChannels with automatable() set to 1\n"
    printarray SNames
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i1 0 z
</CsScore>
</CsoundSynthesizer>
```
