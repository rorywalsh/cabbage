# Group Box

Groupbox creates a container for other GUI widgets. They do not communicate with Csound but can be useful for organising widgets into panels.

```csharp
groupbox bounds(x, y, width, height), text("Caption"), \
colour("colour"), fontcolour("colour"), line(value), plant("name"), \
popup(val), child(val), visible(val), alpha(val), show(val), \
identchannel("channel"), rotate(radians, pivotx, pivoty), \
widgetarray("chan", number), active(val), svgfile("filename")
```
<!--(End of syntax)/-->

##Identifiers
**bounds(x, y, width, height)** integer values that set position and size on screen(in pixels)

**text("caption")** "caption" will be the string to appear on the group box

**colour("colour")** This sets the colour of the groupbox. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full.  

**fontcolour("colour")** Sets the colour of the font to appear on the groupbox. See above for details on valid colours. .

**linethickess(value)** Sets the thickness of the line that appears on a groupbox. Set to 0 to disable the line. 

**plant("name")** Sets the name of the plant. No two plants can have the same name. See [Plants](./plants.md)

**alpha(val)** A value between 0 and 1 will set the alpha blend value for the entire component. Can be useful if you need to fade widgets in and out. 

**visible(val)** A value of 0 will cause the widget to become invisible. Widgets have their visibility set to 1 by default. 

**popup(val)** (Deprecated) If the groupbox is a plant, a value of 1 will cause the groupbox to open in a new window when show(1) is called. popup() should always be used in conjunction with the show() identifier. See [Plants](./plants.md)

**show(val)** A value of 1 will cause the popup plant to show. A value of 0 will close the popup plant. See [Plants](./plants.md)

**child(0)** A value of 1 will cause the popup plant to be a child of the main form. By default, popup plants are not children of the main form and can therefore appear anywhere on screen. If the popup plant is a child of the main form, it can only appear within the bounds of the main form. 

**identchannel("channel")** Sets the channel which messages from Csound are sent to the widget on. When used with a chnset opcode users can override widget attributes. See [Identifier channels](./identchannels.md)  

**rotate(radians, pivotx, pivoty)** Rotates the widget by a number of radians(2xPI=full rotation). pivotx and pivoty will determine the rotation pivot points, where 0, 0 represents the component's top-left position. 

**widgetarray("chan", number)** Creates an number of widgets in a single pass. See [Widget arrays](./widget_arrays.md)

**active(val)** Will deactivate a control if 0 is passed. Controls which are deactivate can still be updated from Csound.

**svgfile("filename")** Use this identifier to set a unique .svg file to use instead of the default look and feel. Note, the same affect can also be achieved using an image widget. 
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

![](images/groupBoxExample.png)