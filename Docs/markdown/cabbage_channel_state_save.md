# cabbageChannelStateSave

This opcode will save the value of each channel declared in a Csound orchestra to a JSON file on disk. It can be used with the channelStateRecall opcode to create a robust preset system.
Syntax

<blockquote style="font-style:italic;border-left:10px solid #93d200;color:rgb(3, 147, 210);padding:1px;padding-left:10px;margin-top:0px;margin-bottom:1px;border-left-width:0.25rem"> Added in Cabbage v2.5.44</blockquote>

### Syntax

<pre>iRes <b>channelStateSave</b> SFilename
kRes <b>channelStateSave</b> SFilename</pre>

#### Initialization

* `iRes` -- 1 for success, 0 for fail
* `SFilename` -- filename

#### Performance

* `kRes` -- 1 for success, 0 for fail
* `SFilename` -- filename

### Example

This example shows channelStateSave being used to save the current states of a all Cabbage widgets to disk. The states can be recalled using the channelStateRecall opcode.

```csharp
<Cabbage>
form caption("Presets") size(370, 280), guiMode("queue"), colour(58, 110, 182), pluginId("fsad")
keyboard bounds(10, 90, 345, 95)
rslider bounds(12, 8, 85, 79), channel("att"), range(0, 1, 0.01), text("Att.")
rslider bounds(98, 8, 85, 79), channel("dec"), range(0, 1, 0.4), text("Dec.")
rslider bounds(184, 8, 85, 79), channel("sus"), range(0, 1, 0.7), text("Sus.")
rslider bounds(270, 8, 85, 79), channel("rel"), range(0, 1, 0.8), text("Rel.")
combobox bounds(92, 192, 226, 25), populate("*.pre", "."), channel("recallCombo"), channelType("string")
button bounds(10, 192, 80, 40), channel("triggerSave"), text("Save state")
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

gkFileNumber init 10


instr 1
   kEnv madsr cabbageGetValue:i("att"), cabbageGetValue:i("dec"), cabbageGetValue:i("sus"), cabbageGetValue:i("rel")
   aOut vco2 kEnv*p5, p4
   outs aOut, aOut
endin

instr 10
   SFilename, kTrig cabbageGetValue "recallCombo"
   if kTrig == 1 then
      SIgnoreChannels[] init 2
      SIgnoreChannels[0] = "triggerSave"
      SIgnoreChannels[1] = "recallCombo"
      kOk = cabbageChannelStateRecall:k(SFilename, SIgnoreChannels)
      cabbageSetValue "att", cabbageGetValue:k("att")
      cabbageSetValue "dec", cabbageGetValue:k("dec")
      cabbageSetValue "sus", cabbageGetValue:k("sus")
      cabbageSetValue "rel", cabbageGetValue:k("rel")
   endif

   if changed:k(chnget:k("triggerSave")) == 1 then
      SPath = chnget:S("CSD_PATH")
      SFilename = sprintfk:S("PresetTest%d.pre", gkFileNumber)
      printks "%s", 0, SFilename
      kOk = cabbageChannelStateSave:k(SFilename)
      gkFileNumber+=1
      cabbageSet 1, "recallCombo", "refreshFiles(1)" 
    endif
endin


</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i10 0 z
</CsScore>
</CsoundSynthesizer>
```

> Added in Cabbage v2.5.43