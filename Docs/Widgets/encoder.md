# Endless encoder

Encoder creates an endless rotary encoder similar to those found in certain well-known hardware samplers. Unlike regular sliders, *encoder* doesn't have a start and stop point. One may set a starting point, but after this the user can drag up or down as long as they like. By setting a very small slider increment these widgets can offer a level of fine grain control not possible with regular sliders. 

```csharp
encoder bounds(x, y, width, height), channel("chan"), min(val), max(val)\
text("name"), textbox(val), value(val), increment(val), textcolour("colour"), fontcolour("colour"), \
trackercolour("colour"), outlinecolour("colour"), identchannel("channel"), alpha(val), \
visible(val), caption("caption"), rotate(radians, pivotx, pivoty),\
widgetarray("chan", number), popuptext("text") active(val)
```
<!--(End of syntax)/-->

##Identifiers

### Specific Identifiers

{% include "./Properties/increment.md" %}

{% include "./Properties/max.md" %}

{% include "./Properties/min.md" %}

{% include "./Properties/outlinecolour.md" %}

{% include "./Properties/trackercolour.md" %}

{% include "./Properties/textbox.md" %}

{% include "./Properties/textcolour.md" %}

{% include "./Properties/text_slider.md" %}

### Common Identifiers

{% include "./Properties/active.md" %} 

{% include "./Properties/alpha.md" %} 

{% include "./Properties/bounds.md" %} 

{% include "./Properties/caption.md" %} 

{% include "./Properties/channel.md" %} 

{% include "./Properties/colour.md" %} 

{% include "./Properties/fontcolour.md" %}  

{% include "./Properties/identchannel.md" %} 

{% include "./Properties/popup.md" %} 

{% include "./Properties/rotate.md" %} 

{% include "./Properties/svgfile.md" %}

{% include "./Properties/value.md" %} 

{% include "./Properties/visible.md" %} 

{% include "./Properties/Widget_array_property.md" %} 

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