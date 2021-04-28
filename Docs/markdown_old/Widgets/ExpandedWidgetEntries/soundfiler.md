# Soundfiler

Soundfiler will display the contents of a sound file, or a function table containing a sound file (GEN01 tables). This is a fast waveform viewer that allows users to select regions of the waveform. Unlike the table widget, soundfiler can only display one waveform at a time. 


<big></pre>
soundfiler [channel](#channel_soundfiler)("start", "length"), 
[file](#file_soundfiler)("filename"), 
[scrubberposition](#scrubberposition)(val), 
[showscrubber](#showscrubber)(val), 
[tablenumber](#tablenumber)(val), 
[zoom](#zoom)(val), 
[samplerange](#samplerange)(min, max), 
[scrollbars](#scrollbars)(val), 
[active](#active)(val), 
[alpha](#alpha)(val), 
[bounds](#bounds)(x, y, width, height), 
[colour](#colour)("colour"), 
[identchannel](#identchannel)("channel"), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[visible](#visible)(val), 
[tofront](#tofront)(), 
[widgetarray](#widgetarray)("chan", number), 

</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/channel_soundfiler.md  !}

{! ./markdown/Widgets/Properties/file_soundfiler.md !} 

{! ./markdown/Widgets/Properties/scrubberposition.md !} 

{! ./markdown/Widgets/Properties/showscrubber.md !} 

{! ./markdown/Widgets/Properties/tablenumber.md  !}

{! ./markdown/Widgets/Properties/zoom.md !}  

{! ./markdown/Widgets/Properties/samplerange.md !}  

{! ./markdown/Widgets/Properties/scrollbars.md !}  

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !} 

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/bounds.md !}

{! ./markdown/Widgets/Properties/colour.md !} 

{! ./markdown/Widgets/Properties/identchannel.md !}  

{! ./markdown/Widgets/Properties/rotate.md !} 

{! ./markdown/Widgets/Properties/visible.md  !}

{! ./markdown/Widgets/Properties/tofront.md !} 

{! ./markdown/Widgets/Properties/widgetarray.md !}  

<!--(End of identifiers)/-->
![](../images/soundfiler.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form size(400, 500), caption("Soundfiler"), size(400, 300), colour(220, 220, 220), pluginID("def1")
soundfiler bounds(10,10, 380, 200), identchannel("soundfilerIdent")
filebutton bounds(10, 215, 100, 30), channel("openFile"), text("Browse")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --displays
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

seed 0 

;basic usage
instr 1
    SFile chnget "openFile"
    if changed(SFile) == 1 then
        SIdentString sprintfk "file(\"%s\")", SFile
        chnset SIdentString, "soundfilerIdent"
    endif
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
</CsScore>
</CsoundSynthesizer>
```
<!--(End Widget Example)/-->