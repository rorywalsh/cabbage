# vrange

Create an on-screen slider with a dragable, and user selectable range. Data can be sent to Csound on the channel specified through the channel string.  


<big></pre>
vrange [channel](#channel_range)("min", "max"), 
[max](#max)(val), 
[min](#min)(val), 
[outlinecolour](#outlinecolour)("colour"), 
[range](#range)(min, max, value, skew, incr), 
[text](#text)("name"), 
[textbox](#textbox)(on/off), 
[textcolour](#textcolour)("colour"), 
[trackerthickness](#trackerthickness)(val), 
[trackercolour](#trackercolour)("colour"), 
[active](#active)(val), 
[alpha](#alpha)(val), 
[bounds](#bounds)(x, y, width, height), 
[colour](#colour)("colour"), 
[fontcolour](#fontcolour)("colour"), 
[identchannel](#identchannel)("channel"), 
[popup](#popup)(val), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[visible](#visible)(val), 
[widgetarray](#widgetarray)("chan", number), 

</pre></big>

<!--(End of syntax)/-->

##Identifiers

### Specific Identifiers

{! ./markdown/Widgets/Properties/channel_range.md !}   

{! ./markdown/Widgets/Properties/max.md !} 

{! ./markdown/Widgets/Properties/min.md !}  

{! ./markdown/Widgets/Properties/outlinecolour.md !} 

{! ./markdown/Widgets/Properties/range.md !} 

{! ./markdown/Widgets/Properties/text.md !} 

{! ./markdown/Widgets/Properties/textbox.md !} 

{! ./markdown/Widgets/Properties/textcolour.md !} 

{! ./markdown/Widgets/Properties/trackerthickness.md !} 

{! ./markdown/Widgets/Properties/trackercolour.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !} 

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/bounds.md !} 



{! ./markdown/Widgets/Properties/colour.md !} 

{! ./markdown/Widgets/Properties/fontcolour.md !}   

{! ./markdown/Widgets/Properties/identchannel.md !} 

{! ./markdown/Widgets/Properties/popup.md !} 

{! ./markdown/Widgets/Properties/rotate.md !} 

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/widgetarray.md !}  
 
<!--(End of identifiers)/-->

>Make sure to use two unique channel names when using hslider2 and vslider2, otherwise min and max will be set to the same value. 

##Example

```csharp
<Cabbage>
form caption("Test") size(615, 480), pluginID("add1"), guirefresh(1) 
hrange bounds(92, 16, 400, 30), channel("rangeone1", "rangetwo1"), range(-2000, 100, -100:100, 1, .001)
hrange bounds(92, 50, 400, 30), channel("rangeone2", "rangetwo2"), range(-2000, 100, -300:200, 1, .001)
vrange bounds(16, 16, 36, 119), channel("vrange1", "vrange2"), range(0, 300, 10:200, 1, 1)
hrange bounds(92, 84, 400, 30) channel("rangeone2", "rangetwo2"), range(-2000, 100, -1000:-100, 1, .001)
vrange bounds(54, 16, 36, 119), channel("vrange1", "vrange2"), range(0, 300, 0:100, 1, 1)
</Cabbage>
```

![](../images/rangeSliders.png)
