# Check Box

Checkbox creates a checkbox which functions like a button, but the associated caption will not change when the user checks it. As with all widgets capable of sending data to Csound, the channel string is the channel on which the widget will communicate with Csound. 


```csharp
checkbox bounds(x, y, width, height), channel("chan"), \
text("name"), value(val), colour:0("colour"), \
colour:1("colour"), shape("shape"), fontcolour("colour"), \
identchannel("channel"), alpha(val), visible(val), corners(val), \
rotate(radians, pivotx, pivoty), widgetarray("chan", number), popuptext("text") \
active(val)
```
<!--(End of syntax)/-->

## Identifiers
### Specific Identifiers

{% include "./Properties/checkbox_corner.md" %}

### Common Identifiers

{% include "./Properties/active.md" %}

{% include "./Properties/alpha.md" %}

{% include "./Properties/bounds.md" %}

{% include "./Properties/channel.md" %}

{% include "./Properties/colour_0.md" %}

{% include "./Properties/colour_1.md" %}

>You can create an invisible checkbox by setting the colour to something with an alpha value of 0. Images can then be placed under the checkbox to create an image button. See the PVSampler instrument as an example of this. 

{% include "./Properties/fontcolour.md" %}

{% include "./Properties/identchannel.md" %}

{% include "./Properties/popup.md" %}

{% include "./Properties/rotate.md" %}

{% include "./Properties/shape.md" %}

{% include "./Properties/text.md" %}

{% include "./Properties/value.md" %}

{% include "./Properties/visible.md" %}

{% include "./Properties/Widget_array_property.md" %}

<!--(End of identifiers)/-->

>colour:1() can be set using colour(). However, it's recommended that you use the numerated colour identifiers in order to make your code more readable. 

## Example
```csharp
<Cabbage>
form size(400, 500), caption("Untitled"), pluginID("plu1"), colour(39, 40, 34)
checkbox bounds(20, 16, 150, 30), channel("combobox"), items("Check me out!"), colour("lime"), fontcolour("white")
</Cabbage>
```
![](../images/checkboxExample.png)
