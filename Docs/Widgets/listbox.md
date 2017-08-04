# List box

Listbox creates a list of custom items. Similary to a combobox in functionality, listboxes can be used for much longer lists that users can scroll through. Double-clicking on a listbox item will send a channel message to Csound. 


```csharp
listbox bounds(x, y, width, height), channel("chan"), highlightcolour("colour") \
value(val), items("item1", "item2", ...), populate("filetype", "dir"),\
channeltype("type"), colour("colour"), fontcolour("colour"), align("position"), \
identchannel("channel"), alpha(val), file("filename.txt"), visible(val), caption("caption"), \
rotate(radians, pivotx, pivoty), widgetarray("chan", number), popuptext("text") \
active(val)
```
<!--(End of syntax)/-->

## Identifiers

### Specific Identifiers

{% include "./Properties/align.md" %}

{% include "./Properties/channel_type.md" %}

{% include "./Properties/file_combobox.md" %}

{% include "./Properties/highlightcolour.md" %}

{% include "./Properties/items.md" %} 

{% include "./Properties/populate_combobox.md" %}

### Common Identifiers

{% include "./Properties/active.md" %} 

{% include "./Properties/alpha.md" %} 

{% include "./Properties/bounds.md" %} 

{% include "./Properties/caption.md" %} 

{% include "./Properties/channel.md" %} 

{% include "./Properties/colour.md" %} 

{% include "./Properties/fontcolour.md" %}  

{% include "./Properties/identchannel.md" %} 

{% include "./Properties/popup.md" %} 

{% include "./Properties/rotate.md" %} 

{% include "./Properties/value.md" %}  

{% include "./Properties/visible.md" %} 

{% include "./Properties/widgetarray.md" %} 

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
listbox bounds(10, 16, 300, 200), channel("comps"), file("Compositions.txt"), colour("yellow"), fontcolour("black")
</Cabbage>
```

![](../images/listbox.png)