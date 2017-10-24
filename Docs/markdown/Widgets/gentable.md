# GenTable

Gentable displays the contents of a function table, multiple tables, or files. Function tables that have their identifier active() set to one, and use a negative GEN05, GEN07 or GEN02 can be edited by clicking and dragging on the table. Only tables of size less or equal to 16384 points can be manually edited. If you wish to save any tables that you create you can use one of Csound's built-in table saving opcode "ftsave". If you need only display the contents of a static sound file from disk, use soundfiler as it will be fast and use less CPU. If on the other hand you wish to create some user editable envelopes, or display FFT or live waveform data use table. 


```csharp
gentable bounds(x, y, width, height), tableNumber(1, 2, ...), \
tablecolour:N("colour1"), amprange(min, max, tablenumber, y-quantise),\
tablegridcolour("colour"), tablebackgroundcolour("colour"), samplerange(min, max),\
zoom(val), outlinethickness(val), identchannel("channel"), \
alpha(val), visible(val), scrubberposition(val), \
rotate(radians, pivotx, pivoty), widgetarray("chan", number), active(val)
```
<!--(End of syntax)/-->

##Identifiers

### Specific Identifiers

{! ./markdown/Widgets/Properties/amprange.md !} 

{! ./markdown/Widgets/Properties/bounds.md !} 

{! ./markdown/Widgets/Properties/tablenumber.md !} 

{! ./markdown/Widgets/Properties/tablecolour.md !} 

{! ./markdown/Widgets/Properties/tablebackgroundcolour.md !} 

{! ./markdown/Widgets/Properties/tablegridcolour.md !} 

{! ./markdown/Widgets/Properties/file_gentable.md !} 

{! ./markdown/Widgets/Properties/scrubberposition_gentable.md !} 

{! ./markdown/Widgets/Properties/samplerange.md !} 

{! ./markdown/Widgets/Properties/samplerange.md !} 

{! ./markdown/Widgets/Properties/outlinethickness.md !}  With regards to gentable, this sets the thickness of the waveform's outline. If you are drawing spectrograms setting this to 0 will speed up the process and reduce CPU drain. 

### Common Identifiers

{! ./markdown/Widgets/Properties/identchannel.md !} 

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/rotate.md !} 

{! ./markdown/Widgets/Properties/rotate.md !} 

{! ./markdown/Widgets/Properties/active.md !} 
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