# Image

Image creates a static shape or graphic. It can be used to show pictures or it can be used to draw simple shapes. If you wish to display a picture you must pass the file name to the file() identifier. For convenience, the file passed to file() should be in the same directory as your Cabbage instrument. 

```csharp
image bounds(x, y, width, height), colour("colour"), channel("chan")\
file("file name"), shape("type"), outlinecolour("colour"), \
outlinethickness(thickness), plant("name"), identchannel("channel"), \
visible(val), alpha(val), rotate(radians, pivotx, pivoty), corners(val) \
widgetarray("chan", number), popuptext("text"), active(val)
```
<!--(End of syntax)/-->
##Identifiers 

**bounds(x, y, width, height)** integer values that set position and size on screen(in pixels). 

**file("filename")** "filename" is the name of the image file to be displayed on the widget. If a full file path is not given, file() will search  in the current directory, i.e., the directory that contains the csd file that is open. It is best to keep all files in the same directory as your csd file, but if you wish to keep them in a sperate folder you can pass a full path to the file() identifier. Cabbage will support most file types, and will also display SVG files. 

>Try to avoid full path names at all costs. They will work fine on a local machine, but will not be valid on another machine.

**shape("shape")** Sets the shape of the image. Default is "square" but users can use "circle" also. Note that by settings the corners to a high value one can also create elliptical shapes.   

**channel("chan")** "chan" is the name of the channel that Cabbage will communicate with Csound on. A left-mouse click on an image will send a 1 to the channel passed to channel(). Subsequent clicks will toggle between 0 and 1. 

**colour("colour")** This sets the colour of the image if no file name is given with the file identifier. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full.  

**outlinecolour("colour")** This sets the outline colour of the image/shape. See above for details on valid colours. .

**outlinethickness(thickness)** This sets the line thickness in pixels.

**plant("name")** Sets the name of the plant. No two plants can have the same name. See [Plants](./plants.md)

**identchannel("channel")** Sets the channel which messages from Csound are sent to the widget on. When used with a chnset opcode users can override widget attributes. See [Identifier channels](./identchannels.md) 

**alpha(val)** A value between 0 and 1 will set the alpha blend value for the entire component. Can be useful if you need to fade widgets in and out. 

**visible(val)** A value of 0 will cause the widget to become invisible. Widgets have their visibility set to 1 by default. 

**rotate(radians, pivotx, pivoty)** Rotates the widget by a number of radians(2xPI=full rotation). pivotx and pivoty will determine the rotation pivot points, where 0, 0 represents the component's top-left position. 

**widgetarray("chan", number)** Creates an number of widgets in a single pass. See [Widget arrays](./widget_arrays.md)

**active(val)** Will deactivate a control if 0 is passed. Controls which are deactivate can still be updated from Csound.

**corners(val)** Sets the radius size of the widget's corners.
<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
image bounds(58, 4, 257, 183), shape("ellipse"), colour("purple")  
image bounds(84, 22, 200, 150), file("/home/rory/sourcecode/cabbageaudio/cabbage/Images/logo_cabbage_sw_no_text.png"),  
</Cabbage>
```

![](images/imageExample.png)