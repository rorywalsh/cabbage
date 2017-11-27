# Form

Form creates the main application window. pluginid() is the only required identifier. The default values for size are 600x300. 

```csharp
form caption("title"), size(Width, Height), pluginid("plug"), \
colour("colour"), guirefresh(val), import("file1", "file2"), bundle("./folder1", "file1.txt", etc)
```
<!--(End of syntax)/-->
##Identifiers

### Specific Identifiers
{! ./markdown/Widgets/Properties/caption.md !} 

{! ./markdown/Widgets/Properties/size.md !} 

{! ./markdown/Widgets/Properties/pluginid.md !} 

{! ./markdown/Widgets/Properties/guirefresh.md !}     

{! ./markdown/Widgets/Properties/import.md !}  

{! ./markdown/Widgets/Properties/bundle.md !}  

### Common Identifiers

{! ./markdown/Widgets/Properties/colour.md !}     

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
</Cabbage>
```

![](../images/formExample.png)