# Text Box

Textbox will display the contents of a text file. This can be useful for placing instructions and information directly on to an instrument.  


```csharp
textbox bounds(x, y, width, height), file("filename"), \
colour("colour"), fontcolour("colour"), wrap(val), alpha(val), \
visible(val), rotate(radians, pivotx, pivoty), widgetarray("chan", number) \
popuptext("text"), active(val)
```
<!--(End of syntax)/-->

##Identifiers
**bounds(x, y, width, height)** integer values that set position and size on screen(in pixels)

**colour("colour")** This sets the colour of the image if a file name is not passed to file. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full.  

**fontcolour("colour")** Sets the colour of the text that appears with the slider. See above for details on valid colours. .

**file("file name")** Set the file that will be opened. This file must reside in the same directory as the current .csd file. Do not pass a full path. Cabbage only needs the name and extension, for example file("help.txt"). 

**identchannel("channel")** Sets the channel which messages from Csound are sent to the widget on. When used with a chnset opcode users can override widget attributes. See [Identifier channels](./identchannels.md) 

**wrap(val)** Turns text wrapping on of off. This is set to 0 by default, so no wrapping of text is done. 

**alpha(val)** A value between 0 and 1 will set the alpha blend value for the entire component. Can be useful if you need to fade widgets in and out. 

**visible(val)** A value of 0 will cause the widget to become invisible. Widgets have their visibility set to 1 by default. 

**rotate(radians, pivotx, pivoty)** Rotates the widget by a number of radians(2xPI=full rotation). pivotx and pivoty will determine the rotation pivot points, where 0, 0 represents the component's top-left position. 

**widgetarray("chan", number)** Creates an number of widgets in a single pass. See [Widget arrays](./widget_arrays.md)

**popuptext("text")** Displays a popup text box when a user hovers over the widget.

**active(val)** Will deactivate a control if 0 is passed. Controls which are deactivate can still be updated from Csound.

<!--(End of identifiers)/-->

##Example

```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
textbox bounds(10, 10, 380, 450), colour(30, 30, 30), fontcolour("yellow"), file("test.csd")
</Cabbage>
```

![](images/textboxExample.png)