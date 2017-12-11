# Button (Information button)

Infobuttons can be used to open html files in the system's default web browser. When clicked, the file passed to the file() identifier will be opened, if it is a valid file. This widget can be useful for providing help files or any other additional information about your instruments. 

```csharp
infobutton bounds(x, y, width, height), text("name"), \
colour("colour"), fontcolour("colour") file("file name"), \
identchannel("chan"), alpha(val), visible(val), \
rotate(radians, pivotx, pivoty), widgetarray("chan", number), \
popuptext("text"), active(val), svgfile("type", "file")
```
<!--(End of syntax)/-->

## Identifiers

### Specific Identifiers

{! ./markdown/Widgets/Properties/file_info_button.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !} 

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/bounds.md !} 

{! ./markdown/Widgets/Properties/colour_0.md !} 

{! ./markdown/Widgets/Properties/colour_1.md !} 

{! ./markdown/Widgets/Properties/fontcolour_0.md !} 

{! ./markdown/Widgets/Properties/fontcolour_1.md !} 

{! ./markdown/Widgets/Properties/identchannel.md !} 

{! ./markdown/Widgets/Properties/popup.md !} 

{! ./markdown/Widgets/Properties/svgfile.md !} 

{! ./markdown/Widgets/Properties/text.md !} 

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/widgetarray.md !} 


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

![](../images/buttonExample.png)