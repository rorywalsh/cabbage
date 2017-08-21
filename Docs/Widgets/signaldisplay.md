# Signal Display

Displays a graphical representation of a signal. Must be used with the **display** or **dispfft** opcodes in Csound. 

```csharp
signaldisplay bounds(x, y, width, height), colour("colour"), groundcolour("colour"), \
signalvariable("a-rate variables"), displaytype("type"), identchannel("channel"), alpha(val), \
visible(val), active(val), zoom(val), updaterate(val)
```
<!--(End of syntax)/-->

##Identifiers
### Specific Identifiers
{% include "./Properties/backgroundcolour.md" %} 

{% include "./Properties/displaytype.md" %} 

{% include "./Properties/signalvariable.md" %}

{% include "./Properties/updaterate.md" %}

{% include "./Properties/zoom.md" %}

### Common Identifiers %}

{% include "./Properties/active.md" %} 

{% include "./Properties/alpha.md" %} 

{% include "./Properties/bounds.md" %} 

{% include "./Properties/colour.md" %} 

{% include "./Properties/identchannel.md" %} 

{% include "./Properties/visible.md" %} 

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