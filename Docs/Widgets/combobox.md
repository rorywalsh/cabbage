# Combo Box

Combobox creates a drop-down list of items which users can choose from. Once the user selects an item, the index of their selection will be sent to Csound on a channel named by the channel string. The default value is 0.


```csharp
combobox bounds(x, y, width, height), channel("chan"), align("position")\
value(val), items("item1", "item2", ...), file("filename.txt"), populate("filetype", "dir"),\
channeltype("type"), colour("colour"), fontcolour("colour"), \
identchannel("channel"), alpha(val), visible(val), \
rotate(radians, pivotx, pivoty), widgetarray("chan", number), popuptext("text") \
active(val)
```
<!--(End of syntax)/-->

## Identifiers

### Specific Identifiers

{% include "./Properties/align.md" %}

{% include "./Properties/channel_type.md" %}

{% include "./Properties/file_combobox.md" %}

{% include "./Properties/items.md" %}

{% include "./Properties/populate_combobox.md" %}


### Common Identifiers
{% include "./Properties/active.md" %}  

{% include "./Properties/alpha.md" %} 

{% include "./Properties/bounds.md" %}  

{% include "./Properties/channel.md" %}

{% include "./Properties/colour.md" %}  

{% include "./Properties/fontcolour.md" %} 

{% include "./Properties/identchannel.md" %} 

{% include "./Properties/popup.md" %} 

{% include "./Properties/rotate.md" %}  

{% include "./Properties/value.md" %} Comboboxes indices start at 1, not 0. If you set the initial value to 0 the combobox will not show any items.  

{% include "./Properties/visible.md" %}  

{% include "./Properties/widgetarray.md" %} 

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
combobox bounds(20, 16, 100, 30), channel("combobox"), items("Item 1", "Item 2", "Item 3"), colour("yellow"), fontcolour("black")
</Cabbage>
```

![](../images/comboboxExample.png)