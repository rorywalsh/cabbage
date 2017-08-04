# GenTable

Gentable displays the contents of a function table, multiple tables, or files. Function tables that have their identifier active() set to one, and use a negative GEN05, GEN07 or GEN02 can be edited by clicking and dragging on the table. Only tables of size less or equal to 16384 points can be manually edited. If you wish to save any tables that you create you can use one of Csound's built-in table saving opcode "ftsave". If you need only display the contents of a static sound file from disk, use soundfiler as it will be fast and use less CPU. If on the other hand you wish to create some user editable envelopes, or display FFT or live waveform data use table. 


```csharp
gentable bounds(x, y, width, height), tableNumber(1, 2, ...), \
tablecolour("colour1", "colour2", ...), amprange(min, max, tablenumber, y-quantise),\
tablegridcolour("colour"), tablebackgroundcolour("colour"), samplerange(min, max),\
zoom(val), outlinethickness(val), identchannel("channel"), \
alpha(val), visible(val), scrubberposition(val), \
rotate(radians, pivotx, pivoty), widgetarray("chan", number), active(val)
```
<!--(End of syntax)/-->

##Identifiers

### Specific Identifiers

{% include "./Properties/amprange.md" %}

{% include "./Properties/bounds.md" %}

{% include "./Properties/tablenumber.md" %}

{% include "./Properties/tablecolour.md" %}

{% include "./Properties/tablebackgroundcolour.md" %}

{% include "./Properties/tablegridcolour.md" %}

{% include "./Properties/file_gentable.md" %}

{% include "./Properties/scrubberposition_gentable.md" %}

{% include "./Properties/samplerange.md" %}

{% include "./Properties/samplerange.md" %}

{% include "./Properties/outlinethickness.md" %} With regards to gentable, this sets the thickness of the waveform's outline. If you are drawing spectrograms setting this to 0 will speed up the process and reduce CPU drain. 

### Common Identifiers

{% include "./Properties/identchannel.md" %}

{% include "./Properties/alpha.md" %}

{% include "./Properties/visible.md" %}

{% include "./Properties/rotate.md" %}

{% include "./Properties/rotate.md" %}

{% include "./Properties/active.md" %}
<!--(End of identifiers)/-->

>If you need to redraw tables quickly, make sure they don't use negative GEN routines as it will seriously slow down redrawing. 

##Example

```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
gentable bounds(10, 10, 380, 200), tablenumber(1, 2), tablecolour("yellow", "red"), tablegridcolour(60, 60,60)
</Cabbage>
```
![](../images/gentableExample.png)