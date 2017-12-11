# List box

Listbox creates a list of custom items. Similary to a combobox in functionality, listboxes can be used for much longer lists that users can scroll through. Double-clicking on a listbox item will send a channel message to Csound. 


<big></pre>
listbox [align](#align)("position"), 
[channeltype](#channel_type)("type"), 
[file](#file_combobox)("filename"), 
[highlightcolour](#highlightcolour)("colour"), 
[items](#items)("item1", "item2", ...), 
[populate](#populate_combobox)("filetype", "dir"), 
[active](#active)(val), 
[alpha](#alpha)(val), 
[bounds](#bounds)(x, y, width, height), 
[caption](#caption)("string"), 
[channel](#channel)("chan"), 
[colour](#colour)("colour"), 
[fontcolour](#fontcolour)("colour"), 
[identchannel](#identchannel)("channel"), 
[popup](#popup)(val), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[value](#value)(val), 
[visible](#visible)(val), 
[widgetarray](#widgetarray)("chan", number), 

</pre></big>

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