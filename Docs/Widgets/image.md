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

### Specific Identifiers

{% include "./Properties/corners.md" %}

{% include "./Properties/file_image.md" %}

{% include "./Properties/outlinecolour.md" %}
 
{% include "./Properties/outlinethickness.md" %}

{% include "./Properties/shape.md" %}. Note that by settings the corners to a high value one can also create elliptical shapes.  

**plant("name")** [DEPRECATED] Sets the name of the plant. No two plants can have the same name. See [Plants](./plants.md)

### Common Identifiers

{% include "./Properties/active.md" %} 

{% include "./Properties/alpha.md" %} 

{% include "./Properties/bounds.md" %} 

{% include "./Properties/channel.md" %} A left-mouse click on an image will send a 1 to the channel passed to channel(). Subsequent clicks will toggle between 0 and 1. 

{% include "./Properties/colour.md" %} 

{% include "./Properties/identchannel.md" %} 

{% include "./Properties/rotate.md" %} 

{% include "./Properties/visible.md" %} 

{% include "./Properties/Widget_array_property.md" %} 

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