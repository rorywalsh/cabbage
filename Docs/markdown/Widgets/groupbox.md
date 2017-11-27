# Group Box

Groupbox creates a container for other GUI widgets. They do not communicate with Csound but can be useful for organising widgets into panels.

```csharp
groupbox bounds(x, y, width, height), text("Caption"), \
colour("colour"), fontcolour("colour"), linethinckness(value), plant("name"), \
popup(val), child(val), visible(val), alpha(val), show(val), \
identchannel("channel"), rotate(radians, pivotx, pivoty), outlinethickness(val) \
widgetarray("chan", number), active(val), svgfile("filename")
```
<!--(End of syntax)/-->

##Identifiers

### Specific Identifiers

{! ./markdown/Widgets/Properties/linethickess.md !}

{! ./markdown/Widgets/Properties/outlinethickness.md !}  

{! ./markdown/Widgets/Properties/text_groupbox.md !}

{! ./markdown/Widgets/Properties/popup.md !} 

**plant("name")** [DEPRECATED] Sets the name of the plant. No two plants can have the same name. See [Plants](./plants.md)

**popup(val)** [DEPRECATED] If the groupbox is a plant, a value of 1 will cause the groupbox to open in a new window when show(1) is called. popup() should always be used in conjunction with the show() identifier. See [Plants](./plants.md)

**show(val)** [DEPRECATED] A value of 1 will cause the popup plant to show. A value of 0 will close the popup plant. See [Plants](./plants.md)

### Common Identifiers

{! ./markdown/Widgets/Properties/alpha.md !}  

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/fontcolour.md !}   

{! ./markdown/Widgets/Properties/identchannel.md !}  

{! ./markdown/Widgets/Properties/rotate.md !}  

{! ./markdown/Widgets/Properties/svgfile.md !}  

{! ./markdown/Widgets/Properties/visible.md !}  

{! ./markdown/Widgets/Properties/widgetarray.md !}  

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
groupbox bounds(42, 34, 180, 94), text("3 Sliders")
rslider bounds(56, 64, 50, 50), channel("rslider"), range(0, 1, 0)
rslider bounds(108, 64, 50, 50), channel("rslider"), range(0, 1, 0)
rslider bounds(160, 64, 50, 50), channel("rslider"), range(0, 1, 0)
</Cabbage>
```

![](../images/groupBoxExample.png)
