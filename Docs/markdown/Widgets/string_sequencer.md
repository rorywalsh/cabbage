# String Sequencer

The stringsequencer widget is string based sequencer that can be used to send strings to Csound at a particular BPM. It takes the style of an old-school tracker, but can be adapted to a wide range of uses. The "channel" identifier takes at least 2 strings. The first will be a channel that transmits the current beat number to Csound. This is a bi-directional channel that users can send data to the widget on. For example, users may wish to set the current beat at some point. The next channels represent the tracks that appear in the widget, from left to right. Details of this widget's identifiers are given below.

```csharp
stringsequencer bounds(x, y, width, height), channel("beatChan", "chan1", "chan2", "chan3"),
colour("colour"), fontcolour("colour"), bpm(val), numberofsteps(val), shownumbers(val), cellwidth(val),
cellheight(val), identchannel("channel"), alpha(val), visible(val), text("Track1", "Track2", "Track3")
rotate(radians, pivotx, pivoty), active(val)
```
<!--(End of syntax)/-->

##Identifiers

### Specific Identifiers

{! ./markdown/Widgets/Properties/bpm.md !}

{! ./markdown/Widgets/Properties/numberofsteps.md !}

{! ./markdown/Widgets/Properties/shownumbers.md !}

{! ./markdown/Widgets/Properties/cellwidth.md !}

{! ./markdown/Widgets/Properties/cellheight.md !}

{! ./markdown/Widgets/Properties/bpm.md !}
### Common Identifiers
{! ./markdown/Widgets/Properties/active.md !}

{! ./markdown/Widgets/Properties/alpha.md !}

{! ./markdown/Widgets/Properties/bounds.md !}

{! ./markdown/Widgets/Properties/channel.md !}

{! ./markdown/Widgets/Properties/colour.md !}

{! ./markdown/Widgets/Properties/fontcolour.md !}

{! ./markdown/Widgets/Properties/fontcolour.md !} In the case of a stringsequencer it will determine the colour of the numbers corresponding to each step. 

{! ./markdown/Widgets/Properties/identchannel.md !}

{! ./markdown/Widgets/Properties/rotate.md !}

{! ./markdown/Widgets/Properties/text.md !}

{! ./markdown/Widgets/Properties/visible.md !}

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
button bounds(20, 16, 100, 30), channel("button"),  text("Push me"), fontcolour("white")
infobutton bounds(120, 16, 100, 30), channel("button"),  file("README.txt"), text("Info")
filebutton bounds(220, 16, 100, 30), channel("button"),  populate("*.wav", ""), text("Browse")
</Cabbage>
```

![Button](../images/buttonExample.png)
