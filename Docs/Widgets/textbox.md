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

{% include "./Properties/file_textbox.md" %}

{% include "./Properties/wrap.md" %}

### Common Identifiers

{% include "./Properties/active.md" %} 

{% include "./Properties/alpha.md" %} 

{% include "./Properties/bounds.md" %} 

{% include "./Properties/colour.md" %} 

{% include "./Properties/fontcolour.md" %}

{% include "./Properties/identchannel.md" %}

{% include "./Properties/popup.md" %}

{% include "./Properties/rotate.md"  %}

{% include "./Properties/visible.md" %}

{% include "./Properties/widgetarray.md" %}

<!--(End of identifiers)/-->

##Example

```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
textbox bounds(10, 10, 380, 450), colour(30, 30, 30), fontcolour("yellow"), file("test.csd")
</Cabbage>
```

![](../images/textboxExample.png)