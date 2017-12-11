# Endless encoder

Encoder creates an endless rotary encoder similar to those found in certain well-known hardware samplers. Unlike regular sliders, *encoder* doesn't have a start and stop point. One may set a starting point, but after this the user can drag up or down as long as they like. By setting a very small slider increment these widgets can offer a level of fine grain control not possible with regular sliders. 

<big></pre>
encoder WIDGET_SYNTAX
</pre></big>
<!--(End of syntax)/-->

##Identifiers

### Specific Identifiers

{! ./markdown/Widgets/Properties/increment.md !} 

{! ./markdown/Widgets/Properties/max.md !} 

{! ./markdown/Widgets/Properties/min.md !} 

{! ./markdown/Widgets/Properties/outlinecolour.md !} 

{! ./markdown/Widgets/Properties/trackercolour.md !} 

{! ./markdown/Widgets/Properties/textbox.md !} 

{! ./markdown/Widgets/Properties/textcolour.md !} 

{! ./markdown/Widgets/Properties/text_slider.md !} 

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

{! ./markdown/Widgets/Properties/svgfile.md !} 

{! ./markdown/Widgets/Properties/value.md !}  

{! ./markdown/Widgets/Properties/visible.md !}  

{! ./markdown/Widgets/Properties/widgetarray.md !}  

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form size(400, 200), caption("Untitled"), pluginID("plu1"), colour(57, 110, 185)
encoder bounds(12, 8, 105, 105), channel("vslider1"), value(100), sliderincr(0.01), text("Encoder"), colour("yellow")
rslider bounds(140, 8, 50, 50), text("R-Slider"), channel("rslider1"), range(0, 1, 0)
rslider bounds(140, 62, 50, 50), channel("rslider2"), range(0, 1, .5), colour("lime"), trackercolour("pink")
hslider bounds(196, 8, 200, 30), channel("hslider1"), range(0, 1, .2), trackercolour("red"), textbox(1)
hslider bounds(196, 42, 200, 30), text("H-Slider"), channel("hslider2"), range(0, 1, 1)
hslider bounds(196, 42, 200, 30), text("H-Slider"), channel("hslider2"), range(0, 1, 1)
</Cabbage>
```

![Encoder](../images/encoder.png)