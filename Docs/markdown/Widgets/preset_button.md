# Preset Button

A filebutton can be used to select files from disk. The filename selected, or created will be sent to Csound on the named channel as a string message. Filebuttons can also be used to select snapshot files for Cabbage to save parameter presets. See the populate identifier below. 

<video width="800" height="600" controls>
<source src="../../images/docs/filebutton.mp4">
</video> 

<big></pre>
presetbutton WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/populate.md !} 

{! ./markdown/Widgets/Properties/mode.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !} 

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/bounds.md !} 

{! ./markdown/Widgets/Properties/channel.md !} 

{! ./markdown/Widgets/Properties/corners.md !} 

{! ./markdown/Widgets/Properties/colour_0.md !} 

{! ./markdown/Widgets/Properties/colour_1.md !} 

{! ./markdown/Widgets/Properties/fontColour_0.md !} 

{! ./markdown/Widgets/Properties/fontColour_1.md !} 

{! ./markdown/Widgets/Properties/identChannel.md !} 

{! ./markdown/Widgets/Properties/parent.md !} 

{! ./markdown/Widgets/Properties/popup.md !} 

{! ./markdown/Widgets/Properties/rotate.md !} 

{! ./markdown/Widgets/Properties/imgFile.md !} 

{! ./markdown/Widgets/Properties/text.md !} 

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/moveBehind.md !} 

{! ./markdown/Widgets/Properties/widgetArray.md !} 

<!--(End of identifiers)/-->
>colour:1() and fontColour:1() can be set using colour() and fontColour(). However, it's recommended that you use the numerated colour identifiers in order to make your code as readable a possible. 

##Example

```csharp
<Cabbage>
form caption("Button Example") size(380, 500), guiMode("queue"), colour(2, 145, 209) pluginId("def1")

filebutton bounds(16, 12, 117, 40) channel("loadfile"), text("Load Sample"), corners(5)
button bounds(146, 12, 80, 40) channel("play"), text("Play", "Stop"), corners(5)
button bounds(242, 12, 117, 40) channel("randomSpeed"), text("Random Speed"), corners(5)

texteditor bounds(18, 256, 341, 208) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)
soundfiler bounds(18, 76, 342, 157), channel("soundfiler1"), showScrubber(0), colour(147, 210, 0), tableBackgroundColour(0, 0, 0, 0)

label bounds(18, 230, 341, 24) visible(0), channel("warningLabel"), fontColour("white"), text("Plase load a sample first..."), colour(147, 210, 0)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>e
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1


; Rory Walsh 2021 
;
; License: CC0 1.0 Universal
; You can copy, modify, and distribute this file, 
; even for commercial purposes, all without asking permission. 


instr 1

    SText  = "This instrument shows an example of how file buttons can be used in Cabbage. A file button will launch a browser dialogue when clicked. The user selects a file or directory, depending on the mode. When they click Ok, it will return the name of the file or directory they just selected.\n\nIn the example above, a soundfiler widget is used to display the selected file"
    cabbageSet "infoText", "text", SText

    ;load sample to soundfiler - and dismiss wanring if it is showing
    SFilename, kLoadFile cabbageGetValue "loadfile"
    cabbageSet kLoadFile, "soundfiler1", "file", SFilename
    cabbageSet kLoadFile, "warningLabel", "visible", 0

    ;trigger playback of sample
    kPlayState, kPlayTrig cabbageGetValue "play"
    if kPlayTrig == 1 then
        if kPlayState == 1 then
            event "i", "SamplePlayback", 0, -1
        else
            turnoff2 nstrnum("SamplePlayback"), 0, 0
        endif
    endif


endin

instr SamplePlayback

    SFile cabbageGetValue "loadfile"
    
    if strlen(SFile) == 0 then    
        cabbageSet "warningLabel", "visible", 1
        turnoff        
    else    
        kRandomSpeed init 1
        kRandom, kTrig cabbageGetValue "randomSpeed"    
        kRandomSpeed = (kTrig == 1 ? random:k(-2, 2) : kRandomSpeed)
        a1, a2 diskin2 SFile, kRandomSpeed, 0, 1
        outs a1, a2
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
