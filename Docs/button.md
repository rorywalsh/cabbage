# Button

Button creates a button on screen that can be used for a whole range of different tasks. The "channel" string identifies the channel on which Cabbage will communicate with Csound. "OnCaption" and "OffCaption" determine the strings that will appear on the button as users toggle between two states, i.e., 0 or 1. By default these captions are set to "On" and "Off" but the user can specify any strings they wish. 

```csharp
button bounds(x, y, width, height), channel("chan"), text("offCaption","onCaption"), \
value(val), colour("colour"), fontcolour("colour"), latched(val), \
identchannel("channel"), alpha(val), visible(val), caption("caption"), \ 
rotate(radians, pivotx, pivoty), widgetarray("chan", number), popuptext("text") \
active(val), svgfile("type", "file")
```
<!--(End of syntax)/-->


##Identifiers

#include "./Properties/active.md"

#include "./Properties/alpha.md"

#include "./Properties/bounds.md"

#include "./Properties/caption.md"

#include "./Properties/channel.md"

#include "./Properties/colour_0.md"

#include "./Properties/colour_1.md"

#include "./Properties/fontcolour_0.md"

#include "./Properties/fontcolour_1.md"

#include "./Properties/identchannel.md"

#include "./Properties/latched.md"

#include "./Properties/popup.md"

#include "./Properties/rotate.md"

#include "./Properties/svgfile.md"

#include "./Properties/text.md"

#include "./Properties/value.md"

#include "./Properties/visible.md"

#include "./Properties/Widget_array_property.md"

<!--(End of identifiers)/-->

>colour:1() and fontcolour:1() can be set using colour() and fontcolour(). However, it's recommended that you use the numerated colour identifiers in order to make your code as readable a possible. 

##Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
button bounds(20, 16, 100, 30), channel("button"),  text("Push me"), fontcolour("white")
infobutton bounds(120, 16, 100, 30), channel("button"),  file("README.txt"), text("Info")
filebutton bounds(220, 16, 100, 30), channel("button"),  populate("*.wav", ""), text("Browse")
</Cabbage>
```

![Button](../images/buttonExample.png)