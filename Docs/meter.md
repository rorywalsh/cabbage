# Meter

Meters can be used to create any kind of horizontal or vertical metering bar, such as VU meters. 


```csharp
vmeter bounds(x, y, width, height), overlaycolour("colour"), channel("chan") \
metercolour:N("colour"), align("position"), identchannel("channel"), \
alpha(val), visible(val), rotate(radians, pivotx, pivoty), corners(val)\
widgetarray("chan", number), active(val), outlinethickness(val)
```
<!--(End of syntax)/-->

##Identifiers

**bounds(x, y, width, height)** integer values that set position and size on screen(in pixels). 

**overlaycolour("colour")** This sets the colour of the meter overlay. The meter overlay draws a rectangle on top of the meter's gradient fill to give the impression that the meter is moving up and down. This colour shouldn't have any transparency. Any CSS or HTML colour string can be passed to this identifier. The colour identifier can also be passed an RBG, or RGBA value. All channel values must be between 0 and 255. For instance colour(0, 0, 255) will create a blue, while colour(0, 255, 0, 255) will create a green with an alpha channel set to full.  

**outlinethickness** Set the thickess of the meter's border. 

**outlinecolour("colour")** Sets the colour of the border if outlinethickness is greater than 1.

**channel("chan")** "chan" is the name of the channel that Cabbage will communicate with Csound on. Meter expect normalised values between 0 and 1.

**identchannel("channel")** Sets the channel which messages from Csound are sent to the widget on. When used with a chnset opcode users can override widget attributes. See [Identifier channels](./identchannels.md)  

**alpha(val)** A value between 0 and 1 will set the alpha blend value for the entire component. Can be useful if you need to fade widgets in and out. 

**visible(val)** A value of 0 will cause the widget to become invisible. Widgets have their visibility set to 1 by default. 

**rotate(radians, pivotx, pivoty)** Rotates the widget by a number of radians(2xPI=full rotation). pivotx and pivoty will determine the rotation pivot points, where 0, 0 represents the component's top-left position. 

**widgetarray("chan", number)** Creates an number of widgets in a single pass. See [Widget arrays](./widget_arrays.md)

**active(val)** Will deactivate a control if 0 is passed. Controls which are deactivate can still be updated from Csound.

**corners(val)** Sets the radius size of the widget's corners.


##Slider types:

* *rslider*, a standard rotary or knob slider

* *hslider*, a standard horizontal slider

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form caption("Meters") size(400, 300), colour(20, 20, 20)
vmeter bounds(10, 8, 35, 198) channel("vMeter1") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 
vmeter bounds(50, 8, 35, 198) channel("vMeter2") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 
vmeter bounds(90, 8, 35, 198) channel("vMeter3") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 
vmeter bounds(130, 8, 35, 198) channel("vMeter4") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 

hmeter bounds(170, 10, 180, 35) channel("hMeter1") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 
hmeter bounds(170, 50, 180, 35) channel("hMeter2") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 
hmeter bounds(170, 90, 180, 35) channel("hMeter3") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 
hmeter bounds(170, 130, 180, 35) channel("hMeter4") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 
</Cabbage>
```

![](images/metersEample.png)