# Sliders

Slider can be used to create an on-screen slider. Data can be sent to Csound on the channel specified through the channel string. Sliders can be rotary, vertical, horizontal, frange-based, or number boxes, and can react in both a linear and non-linear fashion. 

<big></pre>
slider WIDGET_SYNTAX
</pre></big>

<!--(End of syntax)/-->

##Identifiers
### Specific Identifiers

{! ./markdown/Widgets/Properties/max.md !}  

{! ./markdown/Widgets/Properties/min.md !}  

{! ./markdown/Widgets/Properties/outlinecolour.md !}  

{! ./markdown/Widgets/Properties/popuptext.md !} Not available to nslider 

{! ./markdown/Widgets/Properties/popupprefix.md !} Not available to nslider 

{! ./markdown/Widgets/Properties/popuppostfix.md !} Although nslider's don't use popup bubbles, this identifier will set a value postfix. 

{! ./markdown/Widgets/Properties/range.md !}   

{! ./markdown/Widgets/Properties/svgfile_slider.md !}   

{! ./markdown/Widgets/Properties/textcolour.md !}   

{! ./markdown/Widgets/Properties/text_slider.md !} 

{! ./markdown/Widgets/Properties/trackerthickness.md !} 

{! ./markdown/Widgets/Properties/textboxoutlinecolour.md !}  

{! ./markdown/Widgets/Properties/textboxcolour.md !}  

{! ./markdown/Widgets/Properties/trackercolour.md !} 

{! ./markdown/Widgets/Properties/valuetextbox.md !} 

{! ./markdown/Widgets/Properties/velocity.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md  !}

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/channel.md !}  

{! ./markdown/Widgets/Properties/colour.md !} 

{! ./markdown/Widgets/Properties/fontcolour.md !} 

{! ./markdown/Widgets/Properties/identchannel.md  !}

{! ./markdown/Widgets/Properties/popup.md !} Not available to nslider 

{! ./markdown/Widgets/Properties/rotate.md  !}

{! ./markdown/Widgets/Properties/visible.md  !}

{! ./markdown/Widgets/Properties/widgetarray.md !} 

<!--(End of identifiers)/-->

##Slider types:

* *rslider*, a standard rotary or knob slider

* *hslider*, a standard horizontal slider

* *vslider*, a standard vertical slider

* *hslider2*, two value horizontal range slider (**deprecated, use hrange**)

* *vslider2*, two value vertical range slider (**deprecated, use vrange**)

* *nslider*, a number box slider in the style of Pure Data /MaxMSP


>Make sure to use two unique channel names when using hslider2 and vslider2, otherwise min and max will be set to the same value. 

##Example

```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
vslider bounds(12, 8, 35, 200), channel("vslider1"), range(0, 1, .6), colour("yellow"), textbox(1)
vslider bounds(48, 8, 30, 200), channel("vslider2"), range(0, 1, .6), trackercolour("purple")
rslider bounds(88, 8, 50, 50), text("R-Slider"), channel("rslider1"), range(0, 1, 0)
rslider bounds(88, 62, 50, 50), channel("rslider2"), range(0, 1, .5), colour("lime"), trackercolour("pink")
hslider bounds(144, 8, 200, 30), channel("hslider1"), range(0, 1, .2), trackercolour("red"), textbox(1)
hslider bounds(144, 42, 200, 30), text("H-Slider"), channel("hslider2"), range(0, 1, 1)
</Cabbage>
```

![](../images/sliderExample.png)
