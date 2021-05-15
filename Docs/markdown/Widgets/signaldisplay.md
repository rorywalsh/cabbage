# Signal Display

Displays a graphical representation of a signal. Must be used with the **display** or **dispfft** opcodes in Csound. You must pass a Csound a-rate variable to the `signalVariable()` identifier, and you must enable `--displays` in the Csound options. Please see the SignalDisplay example csd for more details. 

<big></pre>
signaldisplay WIDGET_SYNTAX
</pre></big>

### Specific Identifiers
{! ./markdown/Widgets/Properties/backgroundColour.md !}  

{! ./markdown/Widgets/Properties/displayType.md !}  

{! ./markdown/Widgets/Properties/signalVariable.md !} 

{! ./markdown/Widgets/Properties/updateRate.md !} 

{! ./markdown/Widgets/Properties/zoom.md !} 

### Common Identifiers !}

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/alpha.md !}  

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/identChannel.md !}  

{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/visible.md !}  

<!--(End of identifiers)/-->

> To enable the use of the signaldisplay widget you must pass --displays to your CsOptions. See the SignalDisplay example for details. Also note that the Lissajous display is the most CPU expensive display option, followed by waveform. The spectral modes are the least CPU expensive.   

![](../images/signaledisplay.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form size(380, 500), caption("SignalDisplay"), guiMode("queue"), pluginId("SigD")
signaldisplay bounds(10, 8, 362, 187), colour("white") displayType("waveform"), backgroundColour(147, 210, 0), zoom(-1), signalVariable("a1", "a2"), channel("display")
combobox bounds(254, 274, 120, 28), align("centre"), channel("displayCombo"), text("Waveform", "Spectroscope", "Spectrogram", "Lissajous")
keyboard bounds(8, 204, 368, 60)
combobox bounds(10, 272, 100, 30), align("centre"), channel("waveshape"), items("Sine", "Saw", "Square")
texteditor bounds(12, 314, 361, 163) channel("infoText"), wrap(1), text("A signaldisplay widget will display an audio signal in either the time/pressure, or time/frequency domain. In this example it will display a variety of different waveforms. This widget is works with the 'display' and 'dispfft' opcodes in Csound, and as such you must include the '--displays' flag in your CsOptions.", "", "You must also pass the names of the audio variables you wish to display to the signalVariable() identifier. In this example variables a1 and a2 are displayed.")
</Cabbage>
<CsoundSynthesizer> 
<CsOptions>
-n --displays -+rtmidi=NULL -M0 --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
;sr is set by the host
ksmps = 16
nchnls = 2
0dbfs=1


instr 1
a1, a2 init 0

if chnget:k("waveshape") == 1 then
	a1 oscili 1, p4
elseif chnget:k("waveshape") == 2 then
	a1 vco2 1, p4
elseif chnget:k("waveshape") == 3 then
	a1 vco2 1, p4, 10
endif
outs a1, a1


aLFO oscil .6, random:k(10, 100)
a2 oscil 1, p4*.9+aLFO

display	a1, .1, 1
dispfft a1, .1, 1024
display	a2, .1, 1
endin


instr 2

    SText  = "A signaldisplay widget will display an audio signal in either the time/pressure, or time/frequency domain. In this example it will display a variety of different waveforms. This widget is works with the 'display' and 'dispfft' opcodes in Csound, and as such you must include the '--displays' flag in your CsOptions.\n\nYou must also pass the names of the audio variables you wish to display to the signalVariable() identifier. In this example variables a1 and a2 are displayed."
    cabbageSet "infoText", "text", SText
    

    kDisplayType, kTrig cabbageGetValue "displayCombo"
    STypes[] init 4
    STypes[0] = "waveform"
    STypes[1] = "spectroscope"
    STypes[2] = "spectrogram"
    STypes[3] = "lissajous"
    cabbageSet kTrig, "display", "displayType", STypes[kDisplayType-1]
endin

</CsInstruments>
<CsScore>
;i1 0 3600
f0 3600
i2 0 3600
</CsScore>
</CsoundSynthesizer>

```
<!--(End Widget Example)/-->
