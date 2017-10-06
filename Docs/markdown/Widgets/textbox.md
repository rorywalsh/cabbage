# Text Box

Textbox will display the contents of a text file. This can be useful for placing instructions and information directly on to an instrument.  


```csharp
textbox bounds(x, y, width, height), file("filename"), \
colour("colour"), fontcolour("colour"), wrap(val), alpha(val), \
visible(val), rotate(radians, pivotx, pivoty), widgetarray("chan", number) \
popuptext("text"), active(val)
```
<!--(End of syntax)/-->

##Identifiers
### Specific Identifiers

{! ./markdown/Widgets/Properties/file_textbox.md !} 

{! ./markdown/Widgets/Properties/wrap.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/alpha.md !}  

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/fontcolour.md !} 

{! ./markdown/Widgets/Properties/identchannel.md !} 

{! ./markdown/Widgets/Properties/popup.md !} 

{! ./markdown/Widgets/Properties/rotate.md !}

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/widgetarray.md !} 

<!--(End of identifiers)/-->

##Example

```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
textbox bounds(10, 10, 380, 450), colour(30, 30, 30), fontcolour("yellow"), file("test.csd")
</Cabbage>
```

![](../images/textboxExample.png)