# Csound Output Widget

Csoundoutput will display Csound output messages within your instrument**s GUI. This widget can be very useful when working in plugin mode and can be invaluable when it comes to de-slugging Cabbage instruments. 


```csharp
csoundoutput bounds(x, y, width, height), text("name"), \
colour("colour"), fontcolour("colour"), identchannel("channel"), \
visible(val), alpha(val), widgetarray("chan", number)
```
<!--(End of syntax)/-->
##Identifiers
**bounds(x, y, width, height)** integer values that set position and size on screen(in pixels)

**text("name")** "name" will be the text that appears on the top of the check box.  

**colour("colour")** This sets the colour of the background of the output console. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full.  

**fontcolour("colour")** Sets the colour of the text. See above for details on valid colours. 

**identchannel("channel")** Sets the channel which messages from Csound are sent to the widget on. When used with a chnset opcode users can override widget attributes. See [Identifier channels](./identchannels.md) 

**alpha(val)** A value between 0 and 1 will set the alpha blend value for the entire component. Can be useful if you need to fade widgets in and out. 

**visible(val)** A value of 0 will cause the widget to become invisible. Widgets have their visibility set to 1 by default. 

**widgetarray("chan", number)** Creates an number of widgets in a single pass. See [Widget arrays](./widget_arrays.md)

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
csoundoutput bounds(10, 10, 380, 400)
</Cabbage>
```
![](images/csoundOutputExample.png)