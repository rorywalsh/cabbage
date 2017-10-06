# Text Editor

Texteditor creates a single line text editor that can be used to send strings to Csound. Hitting return when in single line mode will send the string to Csound on a named string channel while pressing the up and down buttons when the texteditor is in focus will toggle through the previous strings that have been sent. When in "multiline" mode, press command and return to send the string data to Csound. 

```csharp
texteditor bounds(x, y, width, height), channel("channel"), \
text("text"), colour("colour"), fontcolour("colour"), alpha(val), mode("mode")\
visible(val), rotate(radians, pivotx, pivoty), widgetarray("chan", number) \ popuptext("text"), active(val)
```
<!--(End of syntax)/-->

##Identifiers
### Specific Identifiers

{! ./markdown/Widgets/Properties/mode_texteditor.md !} 

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

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/widgetarray.md !}  

<!--(End of identifiers)/-->

##Example

<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
texteditor bounds(10, 10, 200, 20), colour(30, 30, 30), fontcolour("yellow"), text("I'm a text editor!...")
</Cabbage>

![](../images/texteditorExample.png)