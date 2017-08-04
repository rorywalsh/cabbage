# Form

Form creates the main application window. pluginid() is the only required identifier. The default values for size are 600x300. 

```csharp
form caption("title"), size(Width, Height), pluginid("plug"), \
colour("colour"), guirefresh(val), svgpath("path")
```
<!--(End of syntax)/-->
##Identifiers

### Specific Identifiers
{% include "./Properties/caption.md" %}

{% include "./Properties/size.md" %}

{% include "./Properties/pluginid.md" %}

{% include "./Properties/guirefresh.md" %}    

{% include "./Properties/svgpath.md" %} 

### Common Identifiers

{% include "./Properties/colour.md" %}    

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
</Cabbage>
```

![](../images/formExample.png)