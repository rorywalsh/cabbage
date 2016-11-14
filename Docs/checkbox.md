# Check Box

Checkbox creates a checkbox which functions like a button, but the associated caption will not change when the user checks it. As with all widgets capable of sending data to Csound, the channel string is the channel on which the widget will communicate with Csound. 


```csharp
checkbox bounds(x, y, width, height), channel("chan"), \
text("name"), value(val), colour:0("colour"), \
colour:1("colour"), shape("shape"), fontcolour("colour"), \
identchannel("channel"), alpha(val), visible(val), corners(val), \
rotate(radians, pivotx, pivoty), widgetarray("chan", number), popuptext("text") \
active(val)
```
<!--(End of syntax)/-->

## Identifiers
**bounds(x, y, width, height)** integer values that set position and size on screen(in pixels). 

**channel("chan")** "chan" is the name of the channel that Cabbage will communicate with Csound on. The current value of this widget can be retrieved in Csound using a chnget opcode, or can be set using a chnset opcode. 

**text("name")** "name" will be the text that appears beside the checkbox.  

**value(val)** val sets the initial state of the control

**colour:0("colour")** This sets the colour of the LED when it is in its OFF state. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full.  

**colour:1("colour")** Sets the colour of the widget when it is in its ON state. See above for details on valid colours.

>You can create an invisible checkbox by setting the colour to something with an alpha value of 0. Images can then be placed under the checkbox to create an image button. See the PVSampler instrument as an example of this. 

**fontcolour("colour")** Sets the colour of the font to appear on the groupbox. See above for details on valid colours.

**shape("shape")** Sets the shape of the LED. Default is "square" but users can use "circle" also. 

**rotate(radians, pivotx, pivoty)** Rotates the widget by a number of radians(2xPI=full rotation). pivotx and pivoty will determine the rotation pivot points, where 0, 0 represents the component's top-left position. 

**identchannel("channel")** Sets the channel which messages from Csound are sent to the widget on. When used with a chnset opcode users can override widget attributes. See [Identifier channels](./identchannels.md) 

**alpha(val)** A value between 0 and 1 will set the alpha blend value for the entire component. Can be useful if you need to fade widgets in and out. 

**visible(val)** A value of 0 will cause the widget to become invisible. Widgets have their visibility set to 1 by default. 

**widgetarray("chan", number)** Creates an number of widgets in a single pass. See [Widget arrays](./widget_arrays.md)

**popuptext("text")** Displays a popup text box when a user hovers over the widget.

**active(val)** Will deactivate a control if 0 is passed. Controls which are deactivate can still be updated from Csound.

**corners(val)** Sets the radius size of the widget's corners.
<!--(End of identifiers)/-->

>colour:1() can be set using colour(). However, it's recommended that you use the numerated colour identifiers in order to make your code more readable. 

## Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
checkbox bounds(20, 16, 150, 30), channel("combobox"), items("Check me out!"), colour("lime"), fontcolour("white")
</Cabbage>
```
![](images/checkboxExample.png)
