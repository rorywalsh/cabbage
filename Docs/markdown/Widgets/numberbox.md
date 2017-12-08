# Number Box (deprecated, use [nslider](./sliders.md) instead)

Number boxes can be used to create a PureData, Max/MSP style number box. Data can be sent to Csound on the channel specified through the channel string.


```csharp
numberbox bounds(x, y, width, height), channel("chan"), \
text("name"), textbox(val), range(min, max, value, skew, incr), \
min(val), max(val), textcolour("colour"), fontcolour("colour"), \
identchannel("channel"), alpha(val), visible(val), \
rotate(radians, pivotx, pivoty), widgetarray("chan", number), popuptext("text") \
active(val)
```
<!--(End of syntax)/-->

##Identifiers

### Specific Identifiers

{! ./markdown/Widgets/Properties/max.md !} 

{! ./markdown/Widgets/Properties/min.md !} 

{! ./markdown/Widgets/Properties/range.md !} 

{! ./markdown/Widgets/Properties/text_slider.md !} 

{! ./markdown/Widgets/Properties/textbox.md !} 

{! ./markdown/Widgets/Properties/textcolour.md !} 

{! ./markdown/Widgets/Properties/trackerthickness.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/alpha.md !}  

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/caption.md !}  

{! ./markdown/Widgets/Properties/channel.md !}  

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/fontcolour.md !}   

{! ./markdown/Widgets/Properties/identchannel.md !}  

{! ./markdown/Widgets/Properties/popup.md !}   

{! ./markdown/Widgets/Properties/rotate.md !}   

{! ./markdown/Widgets/Properties/visible.md !}  

{! ./markdown/Widgets/Properties/widgetarray.md !}  


<!--(End of identifiers)/-->

##Example

```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
numberbox bounds(12, 8, 50, 20), channel("vslider1"), range(0, 1, .6), colour("yellow"), textbox(1)
vslider bounds(48, 8, 30, 200), channel("vslider2"), range(0, 1, .6), trackercolour("purple")
rslider bounds(88, 8, 50, 50), text("R-Slider"), channel("rslider1"), range(0, 1, 0)
rslider bounds(88, 62, 50, 50), channel("rslider2"), range(0, 1, .5), colour("lime"), trackercolour("pink")
hslider bounds(144, 8, 200, 30), channel("hslider1"), range(0, 1, .2), trackercolour("red"), textbox(1)
hslider bounds(144, 42, 200, 30), text("H-Slider"), channel("hslider2"), range(0, 1, 1)
</Cabbage>
```

![](../images/numberboxExample.png)
