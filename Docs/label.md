# Label

Labels can be used for placing text on-screen. 


```csharp
label bounds(x, y, width, height), text("text"), colour("colour"), channel("chan") \
fontcolour("colour"), align("position"), identchannel("channel"), \
alpha(val), visible(val), rotate(radians, pivotx, pivoty), corners(val)\
widgetarray("chan", number), active(val), fontstyle("style")
```
<!--(End of syntax)/-->

##Identifiers

**bounds(x, y, width, height)** integer values that set position and size on screen(in pixels). 

**text("text")** "text" will be the string to appear on the label

**align("position")** Aligns the label's text. "position" should be 'left', 'right' or 'centre'. 

**colour("colour")** This sets the background colour of the label. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full.  

**channel("chan")** "chan" is the name of the channel that Cabbage will communicate with Csound on. A left-mouse click on an a label will send a 1 to the channel passed to channel(). Subsequent clicks will toggle between 0 and 1. 

**fontcolour("colour")** This sets the colour of the image if no file name is given with the file identifier. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full.  

**identchannel("channel")** Sets the channel which messages from Csound are sent to the widget on. When used with a chnset opcode users can override widget attributes. See [Identifier channels](./identchannels.md)  

**alpha(val)** A value between 0 and 1 will set the alpha blend value for the entire component. Can be useful if you need to fade widgets in and out. 

**visible(val)** A value of 0 will cause the widget to become invisible. Widgets have their visibility set to 1 by default. 

**rotate(radians, pivotx, pivoty)** Rotates the widget by a number of radians(2xPI=full rotation). pivotx and pivoty will determine the rotation pivot points, where 0, 0 represents the component's top-left position. 

**widgetarray("chan", number)** Creates an number of widgets in a single pass. See [Widget arrays](./widget_arrays.md)

**active(val)** Will deactivate a control if 0 is passed. Controls which are deactivate can still be updated from Csound.

**corners(val)** Sets the radius size of the widget's corners.

**fontstyle("style")** Sets the style of the font. Valid styles are, "plain", "bold"(default), "bold italic", "italic", "underlined", "bold underlined", "italic underlined" and "bold italic underlined". 

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
label bounds(14, 10, 161, 52), text("Label"), colour("red"), fontcolour("pink")
label bounds(82, 44, 161, 52), text("Label"), colour(255, 255, 0, 0), fontcolour("green")
label bounds(162, 80, 161, 52), text("Label"), colour("green"), fontcolour("red")
label bounds(218, 118, 161, 52), text("Label"), colour(120, 40, 20, 0), fontcolour("yellow")
</Cabbage>
```

![](images/labelExample.png)