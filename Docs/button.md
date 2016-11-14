# Button

Button creates a button on screen that can be used for a whole range of different tasks. The "channel" string identifies the channel on which Cabbage will communicate with Csound. "OnCaption" and "OffCaption" determine the strings that will appear on the button as users toggle between two states, i.e., 0 or 1. By default these captions are set to "On" and "Off" but the user can specify any strings they wish. 

```csharp
button bounds(x, y, width, height), channel("chan"), text("offCaption","onCaption"), \
value(val), colour("colour"), fontcolour("colour"), latched(val), \
identchannel("channel"), alpha(val), visible(val), caption("caption"), \ 
rotate(radians, pivotx, pivoty), widgetarray("chan", number), popuptext("text") \
active(val), svgfile("type", "file")
```
<!--(End of syntax)/-->


##Identifiers

**bounds(x, y, width, height)** 
integer values that set position and size on screen(in pixels). 

**channel("chan")** 
"chan" is the name of the channel that Cabbage will communicate with Csound on. The current value of this widget can be retrieved in Csound using a chnget opcode, or can be set using a chnset opcode. 

**text("offCaption", "onCaption")**
The text identifier must be passed at least one string argument. This string will be the one that will appear on the button. If you pass two strings to text() the button will toggle between the two string each time it is pushed.  

**value(val)**
val sets the initial state of the widget

**colour:0("colour")**
This sets the colour of the button when it's value is 0. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full. 

**colour:1("colour")**
This sets the colour of the button when it's value is 1.  See above for details on valid colours. 

>You can create an invisible button by setting the colour to something with an alpha value of 0. Images can then be placed under the button to create an image button. 

**fontcolour:0("colour")**
Sets the colour of the text that appears on the button when it's in its off state. See above for details on valid colours. 

**fontcolour:1("colour")**
Sets the colour of the text that appears on the button when it's in its on state. See above for details on valid colours.

**latched(val)**
With latched set to 0 the button will send a message to Csound on the initial mouse press, and once again as soon as the user releases the button. Therefore the button will toggle between 0 and 1 on each click rather than needing two separate mouse clicks. 

**identchannel("channel")**
Sets the channel which messages from Csound are sent to the widget on. When used with a chnset opcode users can override widget attributes. See [Identifier channels](./identchannels.md)

**rotate(radians, pivotx, pivoty)**
Rotates the widget by a number of radians(2xPI=full rotation). pivotx and pivoty will determine the rotation pivot points, where 0, 0 represents the component's top-left position. 

**alpha(val)**
A value between 0 and 1 will set the alpha blend value for the entire component. Can be useful if you need to fade widgets in and out. 

**visible(val)**
A value of 0 will cause the widget to become invisible. Widgets have their visibility set to 1 by default. 

**caption("caption")**
This identifier will place the widget within a groupbox. "caption" is the text that will appear on group box. This identifier is included for legacy purposes only. It is recommended that you use use your own groupbox component as it will give you far more control over how everything appears on your instrument. 

**widgetarray("chan", number)** Creates an number of widgets in a single pass. See [Widget arrays](./widget_arrays.md)

**popuptext("text")** Displays a popup text box when a user hovers over the widget.

**active(val)** Will deactivate a control if 0 is passed. Controls which are deactivate can still be updated from Csound.

**svgfile("type", "file")** Use this identifier to set a unique .svg file to use instead of the default look and feel. "type" should be one of the following:

- "on" : sets the button image when it is on
- "off" : sets the button image when it is off

For more information see [Using SVGs](./using_svgs.md)

<!--(End of identifiers)/-->

>colour:1() and fontcolour:1() can be set using colour() and fontcolour(). However, it's recommended that you use the numerated colour identifiers in order to make your code as readable a possible. 

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
button bounds(20, 16, 100, 30), channel("button"),  text("Push me"), fontcolour("white")
infobutton bounds(120, 16, 100, 30), channel("button"),  file("README.txt"), text("Info")
filebutton bounds(220, 16, 100, 30), channel("button"),  populate("*.wav", ""), text("Browse")
</Cabbage>
```

![Button](images/buttonExample.png)