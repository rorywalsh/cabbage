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

{! ./markdown/Widgets/Properties/align.md !} 

{! ./markdown/Widgets/Properties/channel_type.md !} 

{! ./markdown/Widgets/Properties/file_combobox.md !} 

{! ./markdown/Widgets/Properties/items.md !} 

{! ./markdown/Widgets/Properties/populate_combobox.md !} 


### Common Identifiers
{! ./markdown/Widgets/Properties/active.md !}   

{! ./markdown/Widgets/Properties/alpha.md !}  

{! ./markdown/Widgets/Properties/bounds.md !}   

{! ./markdown/Widgets/Properties/channel.md !} 

{! ./markdown/Widgets/Properties/colour.md !}   

{! ./markdown/Widgets/Properties/fontcolour.md !}  

{! ./markdown/Widgets/Properties/identchannel.md !}  

{! ./markdown/Widgets/Properties/popup.md !}  

{! ./markdown/Widgets/Properties/rotate.md !}   

{! ./markdown/Widgets/Properties/value.md !}  Comboboxes indices start at 1, not 0. If you set the initial value to 0 the combobox will not show any items.  

{! ./markdown/Widgets/Properties/visible.md !}   

{! ./markdown/Widgets/Properties/widgetarray.md !}  

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
combobox bounds(20, 16, 100, 30), channel("combobox"), items("Item 1", "Item 2", "Item 3"), colour("yellow"), fontcolour("black")
</Cabbage>
```

![](../images/comboboxExample.png)