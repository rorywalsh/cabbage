# Signal Display

Displays a graphical representation of a signal. Must be used with the **display** or **dispfft** opcodes in Csound. 

<big></pre>
signaldisplay WIDGET_SYNTAX
</pre></big>

### Specific Identifiers
{! ./markdown/Widgets/Properties/backgroundcolour.md !}  

{! ./markdown/Widgets/Properties/displaytype.md !}  

{! ./markdown/Widgets/Properties/signalvariable.md !} 

{! ./markdown/Widgets/Properties/updaterate.md !} 

{! ./markdown/Widgets/Properties/zoom.md !} 

### Common Identifiers !}

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/alpha.md !}  

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/identchannel.md !}  

{! ./markdown/Widgets/Properties/visible.md !}  

<!--(End of identifiers)/-->

> To enable the use of the signaldisplay widget you must pass --displays to your CsOptions. See the SignalDisplay example for details. Also note that the Lissajous display is the most CPU expensive display option, followed by waveform. The spectral modes are the least CPU expensive.   

![](../images/signaledisplay.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Signaldisplay Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
signaldisplay bounds(8, 30, 380, 170), colour("lime"), backgroundcolour("black"), displaytype("waveform"), signalvariable("aSig")
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

giWave ftgen 1, 0, 4096, 10, 1, 1, 1, 1 
seed 0 
;basic usage
instr 1
    aSig randi 1, 200
    display	aSig, .5, 1   
endin      

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
;i2 0 z
</CsScore>
</CsoundSynthesizer>
```
<!--(End Widget Example)/-->
