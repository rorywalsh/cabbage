# Event Sequencer

The eventsequencer widget is an event based sequencer that can be used to send score events to Csound. Each cell in the matrix represents a score event that will be sent to Csound. The associated "channel" sets the current position of the steps and is controlled by Csound using the `chnset` opcode. 

<big></pre>
eventsequencer WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/celldata.md !}

{! ./markdown/Widgets/Properties/cellheight.md !}

{! ./markdown/Widgets/Properties/cellwidth.md !}

{! ./markdown/Widgets/Properties/colprefix.md !}

{! ./markdown/Widgets/Properties/orientation.md !}

{! ./markdown/Widgets/Properties/rowprefix.md !}

{! ./markdown/Widgets/Properties/shownumbers.md !}

{! ./markdown/Widgets/Properties/matrixsize.md !}

### Common Identifiers
{! ./markdown/Widgets/Properties/active.md !}

{! ./markdown/Widgets/Properties/alpha.md !}

{! ./markdown/Widgets/Properties/bounds.md !}

{! ./markdown/Widgets/Properties/channel.md !}

{! ./markdown/Widgets/Properties/colour.md !}

{! ./markdown/Widgets/Properties/fontcolour.md !}

{! ./markdown/Widgets/Properties/fontcolour.md !} In the case of a eventsequencer it will determine the colour of the numbers corresponding to each step. 

{! ./markdown/Widgets/Properties/identchannel.md !}

{! ./markdown/Widgets/Properties/rotate.md !}

{! ./markdown/Widgets/Properties/text.md !} In the case of an envetsequencer widget, each string will appear either above or to the left of the matrix, depening on the orientation. 

{! ./markdown/Widgets/Properties/visible.md !}

<!--(End of identifiers)/-->

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("String Sequencer") size(400, 400), pluginID("def1")
stringsequencer bounds(10, 10, 300, 320), channels("step", "track1", "track2", "track3", "track4"), active(0), identchannel("trackerIdent"), textcolour(200, 200, 200), highlightcolour(60, 60, 60) outlinecolour(80,80,80), bpm(180), fontcolour("white") backgroundcolour(20, 20, 20) showstepnumbers(4), numberofsteps(16)
rslider bounds(314, 10, 70, 70) channel("bpm") range(10, 300, 180, 1, 0.001) text("BPM") 
button bounds(314, 82, 70, 27) channel("startStop") text("Start", "Stop") 
rslider bounds(316, 112, 70, 70) channel("position") range(0, 15, 0, 1, 1) 
</Cabbage>
```
<!--(End Widget Example)/-->
![eventsequencer](../images/eventsequencer.gif)
