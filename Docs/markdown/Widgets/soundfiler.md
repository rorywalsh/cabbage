# Soundfiler

Soundfiler will display the contents of a sound file, or a function table containing a sound file (GEN01 tables). This is a fast waveform viewer that allows users to select regions of the waveform. Unlike the table widget, soundfiler can only display one waveform at a time. 


<big></pre>
soundfiler WIDGET_SYNTAX
</pre></big>

<!--(End of syntax)/-->

##Identifiers

### Specific Identifiers

{! ./markdown/Widgets/Properties/channel_soundfiler.md  !}

{! ./markdown/Widgets/Properties/file_soundfiler.md !} 

{! ./markdown/Widgets/Properties/scrubberposition.md !} 

{! ./markdown/Widgets/Properties/tablenumber.md  !}

{! ./markdown/Widgets/Properties/zoom.md !}  

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !} 

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/bounds.md !}

{! ./markdown/Widgets/Properties/colour.md !} 

{! ./markdown/Widgets/Properties/identchannel.md !}  

{! ./markdown/Widgets/Properties/rotate.md !} 

{! ./markdown/Widgets/Properties/visible.md  !}

{! ./markdown/Widgets/Properties/widgetarray.md !}  

<!--(End of identifiers)/-->

##Example

```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
soundfiler bounds(10,10, 380, 200), file("/home/rory/JazzStems/06_Piano.wav")
</Cabbage>
```
![](../images/soundfilerExample.png)