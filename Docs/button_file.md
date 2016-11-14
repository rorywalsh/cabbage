# Button (File button)

A filebutton can be used to select files from disk. The filename selected, or created will be sent to Csound on the named channel as a string message. Filebuttons can also be used to select snapshot files for Cabbage to save parameter presets. See the populate identifier below. 

```csharp
filebutton bounds(x, y, width, height), channel("chan"), \
text("offCaption"), populate("filetype", "dir"), value(val), \
colour("colour"), fontcolour("colour"), identchannel("channel"), \
alpha(val), visible(val), mode("mode"), rotate(radians, pivotx, pivoty), \
widgetarray("chan", number), popuptext("text"), active(val), svgfile("type", "file")
```
<!--(End of syntax)/-->

##Identifiers
**bounds(x, y, width, height)** integer values that set position and size on screen(in pixels)

**text("name")** "name" will be the text that appears on the top of the info button.  

**colour("colour")** This sets the colour of the button. filebuttons only have a single state, therefore they don't need a unique on and off colour. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full.  

**fontcolour("colour")** Sets the colour of the text that appears with the slider. See above for details on valid colours.

**identchannel("channel")** Sets the channel which messages from Csound are sent to the widget on. When used with a chnset opcode users can override widget attributes. See [Identifier channels](./identchannels.md) 

**rotate(radians, pivotx, pivoty)** Rotates the widget by a number of radians(2xPI=full rotation). pivotx and pivoty will determine the rotation pivot points, where 0, 0 represents the component's top-left position. 

**alpha(val)** A value between 0 and 1 will set the alpha blend value for the entire component. Can be useful if you need to fade widgets in and out. 

**visible(val)** A value of 0 will cause the widget to become invisible. Widgets have their visibility set to 1 by default. 

**populate("filetype", "dir")** Sets the type of file to search for, and the initial directory to look in. For example, to set the file type to wave files use "*.wav" as the filetype string. If using the filebutton to record parameter snapshots, you must specify a filetype of "\*.snaps", and a mode type of snapshot. If no directory is specified, the file browser will open in the current working directory.  

**mode("mode")** Set how the button will behave when pressed. Valid modes are:

* *file*, Default. Will let the browser to select a file
* *directory*, Will let the browser to select a folder/directory
* *snapshot*, Will stop the file browser from opening, and instead will write a preset file to disk with a default file name. See [Presets](./presets.md) 

**widgetarray("chan", number)** Creates an number of widgets in a single pass. See [Widget arrays](./widget_arrays.md)

**popuptext("text")** Displays a popup text box when a user hovers over the widget.

**active(val)** Will deactivate a control if 0 is passed. Controls which are deactivate can still be updated from Csound.

**svgfile("type", "file")** Use this identifier to set a unique .svg file to use instead of the default look and feel. "type" should be one of the following:

- "on" : sets the button image when it is on
- "off" : sets the button image when it is off

For more information see [Using SVGs](./using_svgs.md)

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
button bounds(20, 16, 100, 30), channel("button"),  text("Push me"), fontcolour("white")
infobutton bounds(120, 16, 100, 30), channel("button"),  file("README.txt"), text("Info")
filebutton bounds(220, 16, 100, 30), channel("button"),  populate("*.wav", ""), text("Browse")
</Cabbage>
```


![](images/buttonExample.png)