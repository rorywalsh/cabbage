# vrange

Create an on-screen slider with a dragable, and user selectable range. Data can be sent to Csound on the channel specified through the channel string.  


```csharp
vrange bounds(x, y, width, height), channel("chan"), \
text("name"), textbox(val), range(min, max, value, skew, incr), \
min(val), max(val), textcolour("colour"), fontcolour("colour"), \
trackercolour("colour"), outlinecolour("colour"), trackerthickness(val)
identchannel("channel"), alpha(val), visible(val), caption("caption"), \
rotate(radians, pivotx, pivoty), widgetarray("chan", number), popuptext("text") \
active(val)
```
<!--(End of syntax)/-->

##Identifiers
**bounds(x, y, width, height)** integer values that set position and size on screen(in pixels). 

**channel("min", "max")** The names of the channels that Cabbage will communicate with Csound on. The current value of this widget#s min and max can be retrieved in Csound using a chnget opcode, or can be set using a chnset opcode.  

**range(min, max, value, skew, incr)** the first 2 parameters are required. The rest are optional. The first two parameters let you set the minimum value and the maximum value. The next parameter determines the initial value of the slider. For a range slider you must pass two values here separated by a colon. For example, your range() identifier might read range(0, 100, 20:60, 1, .1). The next parameter allows you to adjust the skew factor. Tweaking the skew factor can cause the slider to output values in a non linear fashion. A skew of 0.5 will cause the slider to output values in an exponential fashion. A skew of 1 is the default value, which causes the slider to behave is a typical linear form. The final parameter sets the incremental step size for the slider. If you need to have very fine control of steps you may wish to look at using an [Endless Encoder](./encoder.md) instead. 

**min(val)** min() will set the default minimum position of the minimum thumb controller. The absolute minimum is set using the range identifier. This identifier is not needed if you passed two values to the range identifier. See above. 

**max(val)** max() will set the default maximum position of the maximum thumb controller. The absolute maximum is set using the range identifier. See above. This identifier is not needed if you passed two values to the range identifier. See above.  

**text("name")** The string passed in for "name" will appear on a label beside the slider. This is useful for naming sliders.   

**textbox(on/off)** textbox takes a 0 or a 1. 1 will cause a text box to appear with the sliders values. Leaving this out will result in the numbers appearing automatically when you hover over the sliders with your mouse.

**colour("colour")** This sets the slider. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full. 

**textcolour("colour")** This set the colour of the text passed to text(). If you wish to change the colour of the numbers being displayed use fontcolour(). See below. See above for details on valid colours. 

**fontcolour("colour")** Sets the colour of the text used to display the slider's value when textbox is enable. See above for details on valid colours. .

**trackercolour("colour")** Sets the colour of the slider's tracker. This is the line that follows the slider when you move it. See above for details on valid colours. To disable the tracker you can set this colour to something with alpha 0, or set the tracker's thickness to 0. 

**trackerthickness(val)** Sets the thickness of the slider's tracker, 1 being full thickness. This is the line that follows the slider when you move it. To disable the tracker you can set its thickness to 0 or it's alpha colour channel to 0. 

**outlinecolour("colour")** Sets the colour of a rotary slider**s tracker outline. This is the line that is drawn around the rslider's tracker. If you don't wish to display the tracker outline set the colour to something with an alpha value of 0. See above for details on valid colours.  

**identchannel("channel")** Sets the channel which messages from Csound are sent to the widget on. When used with a chnset opcode users can override widget attributes. See [Identifier channels](./identchannels.md) 

**alpha(val)** A value between 0 and 1 will set the alpha blend value for the entire component. Can be useful if you need to fade widgets in and out. 

**visible(val)** A value of 0 will cause the widget to become invisible. Widgets have their visibility set to 1 by default. 

**caption("caption")** This identifier will place the widget within a groupbox. "caption" is the text that will appear on group box. This identifier is included for legacy purposes only. It is recommended that you use use your own groupbox component as it will give you far more control over how everything appears on your instrument. 

**rotate(radians, pivotx, pivoty)** Rotates the widget by a number of radians(2xPI=full rotation). pivotx and pivoty will determine the rotation pivot points, where 0, 0 represents the component's top-left position. 

**widgetarray("chan", number)** Creates an number of widgets in a single pass. See [Widget arrays](./widget_arrays.md)

**popuptext("text")** Sets the text that will appear in the popup text bubble when a user hovers over the widget. This can be used to override the defaul text which displays the channel name and the current value. 

**active(val)** Will deactivate a control if 0 is passed. Controls which are deactivate can still be updated from Csound.

<!--(End of identifiers)/-->

>Make sure to use two unique channel names when using hslider2 and vslider2, otherwise min and max will be set to the same value. 

##Example

```csharp
<Cabbage>
form caption("Test") size(615, 480), pluginID("add1"), guirefresh(1) 
hrange bounds(92, 16, 400, 30), channel("rangeone1", "rangetwo1"), range(-2000, 100, -100:100, 1, .001)
hrange bounds(92, 50, 400, 30), channel("rangeone2", "rangetwo2"), range(-2000, 100, -300:200, 1, .001)
vrange bounds(16, 16, 36, 119), channel("vrange1", "vrange2"), range(0, 300, 10:200, 1, 1)
hrange bounds(92, 84, 400, 30) channel("rangeone2", "rangetwo2"), range(-2000, 100, -1000:-100, 1, .001)
vrange bounds(54, 16, 36, 119), channel("vrange1", "vrange2"), range(0, 300, 0:100, 1, 1)
</Cabbage>
```

![](images/rangeSliders.png)
