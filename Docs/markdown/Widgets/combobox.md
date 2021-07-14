# Combo Box

Combobox creates a drop-down list of items which users can choose from. Once the user selects an item, the index of their selection will be sent to Csound on a channel named by the channel string. The default value is 0. The first item in the combobox will be seen as a 1, while 0 refers to no current selection.

A combobox can also be set up a work with strings. In this case Csound will receive the currently selected string instead of the current index. Note that using a combobox in this way renders it useless in terms of host automation. Host automation can only be achieved when a combobox is sending the current index, rather than the current string.  

<video width="800" height="600" controls>
<source src="../../images/docs/combobox.mp4">
</video> 

<big></pre>
combobox WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/align.md !} 

{! ./markdown/Widgets/Properties/channel_type.md !} 

{! ./markdown/Widgets/Properties/file.md !} 

{! ./markdown/Widgets/Properties/items.md !} 

{! ./markdown/Widgets/Properties/populate.md !} 

{! ./markdown/Widgets/Properties/sort.md !} 


### Common Identifiers
{! ./markdown/Widgets/Properties/active.md !}   

{! ./markdown/Widgets/Properties/alpha.md !}

{! ./markdown/Widgets/Properties/align.md !}  

{! ./markdown/Widgets/Properties/automatable.md !} 

{! ./markdown/Widgets/Properties/bounds.md !}   

{! ./markdown/Widgets/Properties/channel.md !} 

{! ./markdown/Widgets/Properties/colour.md !}   

{! ./markdown/Widgets/Properties/fontColour.md !}  

{! ./markdown/Widgets/Properties/identChannel.md !}  

{! ./markdown/Widgets/Properties/parent.md !} 

{! ./markdown/Widgets/Properties/popup.md !}  

{! ./markdown/Widgets/Properties/presetIgnore.md !} 

{! ./markdown/Widgets/Properties/protectedItems.md !} 

{! ./markdown/Widgets/Properties/rotate.md !}   

{! ./markdown/Widgets/Properties/value.md !}  Comboboxes indices start at 1, not 0. If you set the initial value to 0 the combobox will not show any items.

{! ./markdown/Widgets/Properties/visible.md !}   

{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/moveBehind.md !} 

{! ./markdown/Widgets/Properties/widgetArray.md !}  

<!--(End of identifiers)/-->

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Combobox Example") size(380, 500), guiMode("queue"), colour(2, 145, 209) pluginId("def1")

rslider bounds(12, 9, 86, 90), channel("att"), range(0, 1, 0.01, 1, 0.001), text("Att.")
rslider bounds(99, 9, 86, 90), channel("dec"), range(0, 1, 0.4, 1, 0.001), text("Dec.")
rslider bounds(187, 9, 86, 90), channel("sus"), range(0, 1, 0.7, 1, 0.001), text("Sus.")
rslider bounds(274, 9, 86, 90), channel("rel"), range(0, 1, 0.8, 1, 0.001), text("Rel.")

texteditor bounds(18, 256, 341, 208) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)
keyboard bounds(12, 104, 348, 80) channel("keyboard")
combobox bounds(260, 188, 100, 30) channel("waveform"), colour:0(147, 210, 0), corners(5), items("Saw", "Square", "Triangle")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key=4 --midi-velocity-amp=5
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

instr 99

    SText  = "An combobox used to toggle through different items. The widget will send its current index to is associated channel, starting from 1*.\n\nIn this example an combobox can be used to select a given waveform. We can't just pass these indices to the vco2 opcode. Instead we use them as indices to simple array that hold the appropriate vco2 waveform modes.\n\n [A combobox always start with index 1, not 0]"
    cabbageSet "infoText", "text", SText

endin

instr 1
    
    iVcoModes[] fillarray 0, 10, 12    
    iAtt cabbageGetValue "att"
    iDec cabbageGetValue "dec"
    iSus cabbageGetValue "sus"
    iRel cabbageGetValue "rel"
    kEnv madsr iAtt, iDec, iSus, iRel
    aVco vco2 kEnv*p5, cpsmidinn(p4), iVcoModes[cabbageGetValue:i("waveform")-1]
    outs aVco, aVco    
    
endin


</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
i99 0 z
</CsScore>
</CsoundSynthesizer>

```
<!--(End Widget Example)/-->
