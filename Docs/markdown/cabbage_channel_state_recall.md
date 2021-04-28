# cabbageChannelStateRecall

This opcode will read a previously saved state JSON file and update all channels accordingly. It can be used to with the channelStateSave opcode to create a robust preset system. 

### Syntax

<pre>iRes <b>channelStateRecall</b> SFilename
kRes <b>channelStateRecall</b> SFilename SIgnoreChannels[]</pre>

#### Initialization

* `iRes` -- 1 for success, 0 for fail
* `SFilename` -- filename

#### Performance

* `kRes` -- 1 for success, 0 for fail
* `SFilename` -- filename
* `SIgnoreChannels[]` -- optional array of channels that should not be overwritten by recall. In some frontends, updating a channel that is use to trigger a state recall will cause an endless loop. In the example below, the channels triggerSave and recallCombo are ignored as they are used to control saving and recalling of presets. 

### Example

```csharp
<Cabbage>
form caption("Presets") size(370, 280), colour(58, 110, 182), pluginid("fsad")
keyboard bounds(10, 90, 345, 95)
rslider bounds(12, 8, 85, 79), channel("att"), range(0, 1, 0.01), text("Att.")
rslider bounds(98, 8, 85, 79), channel("dec"), range(0, 1, 0.4), text("Dec.")
rslider bounds(184, 8, 85, 79), channel("sus"), range(0, 1, 0.7), text("Sus.")
rslider bounds(270, 8, 85, 79), channel("rel"), range(0, 1, 0.8), text("Rel.")
combobox bounds(92, 192, 226, 25), populate("*.txt", "."), channel("recallCombo"), channeltype("string"), identchannel("recallComboIdent")
button bounds(10, 192, 80, 40), channel("triggerSave"), text("Save state")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5 --opcode-lib=C:\Users\rory\sourcecode\csoundPlugins\build\Debug\channelStateOpcodes.dll
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs = 1

gkFileNumber init 10

instr 1
   kEnv madsr chnget:i("att"), chnget:i("dec"), chnget:i("sus"), chnget:i("rel")
   aOut vco2 kEnv*p5, p4
   outs aOut, aOut
endin

instr 10
   SFilename chnget "recallCombo"
   if changed2:k(SFilename) == 1 then
      SIgnoreChannels[] init 2
      SIgnoreChannels[0] = "triggerSave"
      SIgnoreChannels[1] = "recallCombo"
      kOk = channelStateRecall:k(SFilename, SIgnoreChannels)
   endif

   if changed:k(chnget:k("triggerSave")) == 1 then
      SPath = chnget:S("CSD_PATH")
      SFilename = sprintfk:S("PresetTest%d.txt", gkFileNumber)
      printks "%s", 0, SFilename
      kOk = channelStateSave:k(SFilename)
      gkFileNumber+=1
      chnset "refreshfiles()", "recallComboIdent"
    endif
endin


</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i10 0 z
</CsScore>
</CsoundSynthesizer>
```

