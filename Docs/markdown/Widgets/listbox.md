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

{! ./markdown/Widgets/Properties/align.md !} 

{! ./markdown/Widgets/Properties/channel_type.md !} 

{! ./markdown/Widgets/Properties/file_combobox.md !} 

{! ./markdown/Widgets/Properties/highlightcolour.md !} 

{! ./markdown/Widgets/Properties/items.md !}  

{! ./markdown/Widgets/Properties/populate_combobox.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/alpha.md !}  

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/caption.md !}  

{! ./markdown/Widgets/Properties/channel.md !}  

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/fontcolour.md !}   

{! ./markdown/Widgets/Properties/identchannel.md !}  

{! ./markdown/Widgets/Properties/popup.md !}  

{! ./markdown/Widgets/Properties/rotate.md !}  

{! ./markdown/Widgets/Properties/value.md !}   

{! ./markdown/Widgets/Properties/visible.md !}  

{! ./markdown/Widgets/Properties/widgetarray.md !}  

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
listbox bounds(10, 16, 300, 200), channel("comps"), file("Compositions.txt"), colour("yellow"), fontcolour("black")
</Cabbage>
```

![](../images/listbox.png)