# Signal Display

Displays a graphical representation of a signal. Must be used with the **display** or **dispfft** opcodes in Csound. 

<big></pre>
signaldisplay [backgroundcolour](#backgroundcolour)("colour"), 
[displaytype](#displaytype)("type"), 
[signalvariable](#signalvariable)("variables"), 
[updaterate](#updaterate)(val), 
[zoom](#zoom)(val), 
[active](#active)(val), 
[alpha](#alpha)(val), 
[bounds](#bounds)(x, y, width, height), 
[colour](#colour)("colour"), 
[identchannel](#identchannel)("channel"), 
[visible](#visible)(val), 

</pre></big>

<!--(End of syntax)/-->

##Identifiers
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

##Example
```csharp
<Cabbage>
form size(520, 350), caption("SignalDisplay")
signaldisplay bounds(10, 0, 500, 300), colour("cornflowerblue"), signavariable("aOutput")
button bounds(12, 304, 120, 25), channel("displaytype"), text("Spectroscope", "Spectrogram")
</Cabbage>
```

![](../images/fftdisplayExample.gif)