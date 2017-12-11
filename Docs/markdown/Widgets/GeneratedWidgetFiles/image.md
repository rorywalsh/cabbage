# Image

Image creates a static shape or graphic. It can be used to show pictures or it can be used to draw simple shapes. If you wish to display a picture you must pass the file name to the file() identifier. For convenience, the file passed to file() should be in the same directory as your Cabbage instrument.

<big></pre>
image [corners](#corners)(val), 
[file](#file_image)("filename"), 
[outlinecolour](#outlinecolour)("colour"), 
[outlinethickness](#outlinethickness)(value), 
[popup](#popup)(val), 
[shape](#shape)("shape"), 
[active](#active)(val), 
[alpha](#alpha)(val), 
[bounds](#bounds)(x, y, width, height), 
[channel](#channel)("chan"), 
[colour](#colour)("colour"), 
[identchannel](#identchannel)("channel"), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[visible](#visible)(val), 
[widgetarray](#widgetarray)("chan", number), 

</pre></big>

<!--(End of syntax)/-->
##Identifiers

### Specific Identifiers

{! ./markdown/Widgets/Properties/corners.md !}

{! ./markdown/Widgets/Properties/file_image.md !}

{! ./markdown/Widgets/Properties/outlinecolour.md !}

{! ./markdown/Widgets/Properties/outlinethickness.md !}

{! ./markdown/Widgets/Properties/popup.md !} 

{! ./markdown/Widgets/Properties/shape.md !} . Note that by settings the corners to a high value one can also create elliptical shapes.  

**plant("name")** [DEPRECATED] Sets the name of the plant. No two plants can have the same name. See [Plants](./plants.md)

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/alpha.md !}  

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/channel.md !}  A left-mouse click on an image will send a 1 to the channel passed to channel(). Subsequent clicks will toggle between 0 and 1.

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/identchannel.md !}  

{! ./markdown/Widgets/Properties/rotate.md !}  

{! ./markdown/Widgets/Properties/visible.md !}  

{! ./markdown/Widgets/Properties/widgetarray.md !}  

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
image bounds(58, 4, 257, 183), shape("ellipse"), colour("purple")  
image bounds(84, 22, 200, 150), file("/home/rory/sourcecode/cabbageaudio/cabbage/Images/logo_cabbage_sw_no_text.png"),  
</Cabbage>
```

![](../images/imageExample.png)
