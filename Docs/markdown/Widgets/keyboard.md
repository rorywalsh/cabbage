# Keyboard

Keyboard will display a keyboard that will send MIDI information to your Csound instrument. This component can be used together with a hardware controller. Pressing keys on the actual MIDI keyboard will cause the on-screen keys to react up. 

<video width="800" height="600" controls>
<source src="../../images/docs/keyboard.mp4">
</video> 

<big></pre>
keyboard WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/keySeparatorColour.md !} 

{! ./markdown/Widgets/Properties/mouseOverKeyColour.md !}  

{! ./markdown/Widgets/Properties/keypressBaseOctave.md !}  

{! ./markdown/Widgets/Properties/whiteNoteColour.md !}  

{! ./markdown/Widgets/Properties/blackNoteColour.md !} 

{! ./markdown/Widgets/Properties/arrowBackgroundColour.md !} 

{! ./markdown/Widgets/Properties/keyDownColour.md !} 

{! ./markdown/Widgets/Properties/arrowColour.md !} 

{! ./markdown/Widgets/Properties/keyWidth.md !} 

{! ./markdown/Widgets/Properties/middleC.md !}

{! ./markdown/Widgets/Properties/scrollbars.md !}

{! ./markdown/Widgets/Properties/value.md !}

### Common Identifiers

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/identChannel.md !}  

{! ./markdown/Widgets/Properties/channel.md !} 

{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/moveBehind.md !} 

{! ./markdown/Widgets/Properties/parent.md !} 

{! ./markdown/Widgets/Properties/visible.md !}  


<!--(End of identifiers)/-->

>The keyboard can be played at different velocities depending on where you click on the key with your mouse. Clicking at the top of the key will cause a quieter velocity while clicking on the bottom will cause the note to sound with full velocity. If you wish to play the keyboard like a nutjob in standalone mode, make sure to pass '-m0d' to the CsOptions(disable console messages). The keyboard widget is only provided as a quick and easy means of testing plugins in the Cabbage IDE. Treating it as anything more than that could result in severe disappointment!  


##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form size(465, 505), caption("Keyboard Example"), guiMode("queue"), pluginId("plu1")
keyboard bounds(6, 148, 445, 92)
groupbox bounds(6, 6, 223, 134), text("LFO"), colour(0,0,0,0), fontColour("white")
groupbox bounds(232, 6, 223, 134), text("Filter"), colour(0,0,0,0), fontColour("white")

rslider bounds(16, 32, 100, 100), channel("rate"), textColour("white"), range(0, 10, 1, 1, 0.01), text("rate")
rslider bounds(118, 32, 100, 100), channel("intensity"), textColour("white") range(0, 1, 0, 1, 0.01), text("intensity") 
rslider bounds(246, 32, 100, 100), channel("resonance"), textColour("white") range(0, 1, 0, 1, 0.01), text("res")
rslider bounds(348, 32, 100, 100), channel("cutoff"), textColour("white"), text("cutoff"), colour("white"), range(0, 22050, 10000)

checkbox bounds(10, 252, 65, 20), colour:1(147, 210, 0), channel("cutoffCheckbox"), radioGroup(99) value(1), text("cutoff"), 
checkbox bounds(74, 252, 58, 20), colour:1(147, 210, 0), channel("pitchCheckbox"), radioGroup(99) text("pitch"),

combobox bounds(284, 250, 100, 25), populate("*.snaps"), channel("combo1")
filebutton bounds(222, 250, 60, 25), text("Save", "Save"), populate("*.snaps"),mode("named preset") channel("filebutton1") value(0)
filebutton bounds(386, 250, 60, 25), text("Remove"), populate("*.snaps", "test"), mode("remove preset")

texteditor bounds(10, 288, 438, 209) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)

</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -m0d -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 64
nchnls = 2
0dbfs=1
  
; Rory Walsh 2021 
;
; License: CC0 1.0 Universal
; You can copy, modify, and distribute this file, 
; even for commercial purposes, all without asking permission. 


instr 99

    SText  = "A keyboard widget is a simple virtual keyboard that can be used to trigger instruments in Csound. By default it will trigger 'instr 1' on channel 1, 'instr 2' on channel 2 and so on. You can reassign the channels using the 'massign' opcodes in Csound.\n\nIt is set up to use midi note number 60 as C4 bu this can be changing. The keyboard is velocity sensitive. Pressing a key higher up the note will result in a quieter note.\n\nIn this instrument we are sending p4 and p5 to instrument 1. What those p-fields represent is set in the CsOptions section. In this case p4 is the MIDI note number, while p5 is an amplitude value between 0 and 1." 
    cabbageSet "infoText", "text", SText

endin

instr 1
    kDeClick madsr .1, 0, 1, .1
	kAmp cabbageGet "intensity"
	kFreq cabbageGet "rate"
	kRes cabbageGet "resonance"
	kPitch cabbageGet "pitchCheckbox"
	kCutoff cabbageGet "cutoff"
	kLfo lfo kAmp, kFreq, 5

	if kPitch==0 then
		aSig vco2 p5, p4
		aMoog moogladder aSig, (kAmp == 0 ? kCutoff : kLfo*kCutoff), kRes
	else
		aSig vco2 p5, (kAmp == 0 ? p4 : p4 -(1-kLfo*p4*.5))
		aMoog moogladder aSig, kCutoff, kRes
	endif

	outs aMoog*kDeClick, aMoog*kDeClick

endin


</CsInstruments>  
<CsScore>
f1 0 1024 10 1
i99 0 z
f0 3600
</CsScore>
</CsoundSynthesizer>
```
<!--(Widget Example)/-->
