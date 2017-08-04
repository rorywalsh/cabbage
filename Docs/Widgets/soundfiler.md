# Soundfiler

Soundfiler will display the contents of a sound file, or a function table containing a sound file (GEN01 tables). This is a fast waveform viewer that allows users to select regions of the waveform. Unlike the table widget, soundfiler can only display one waveform at a time. 


```csharp
soundfiler bounds(x, y, width, height), channel("start", "end"), \
colour("colour1"), zoom(val), tablenumber(val), file("filename"), \
scrubberpos(val), identchannel("channel"), alpha(val), visible(val), \
rotate(radians, pivotx, pivoty), widgetarray("chan", number), active(val)
```
<!--(End of syntax)/-->

##Identifiers

### Specific Identifiers

{% include "./Properties/channel_soundfiler.md"  %}

{% include "./Properties/file_soundfiler.md" %}

{% include "./Properties/scrubberposition.md" %}

{% include "./Properties/tablenumber.md"  %}

{% include "./Properties/zoom.md" %} 

### Common Identifiers

{% include "./Properties/active.md" %}

{% include "./Properties/alpha.md" %}

{% include "./Properties/bounds.md"  %}

{% include "./Properties/colour.md" %}

{% include "./Properties/identchannel.md" %} 

{% include "./Properties/rotate.md" %}

{% include "./Properties/visible.md"  %}

{% include "./Properties/Widget_array_property.md" %} 

<!--(End of identifiers)/-->

##Example

```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
soundfiler bounds(10,10, 380, 200), file("/home/rory/JazzStems/06_Piano.wav")
</Cabbage>
```
![](../images/soundfilerExample.png)