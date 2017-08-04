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

### Specific Identifiers

{% include "./Properties/corners.md" %}

{% include "./Properties/outlinecolour.md" %} 

{% include "./Properties/outlinethickness.md" %} 

{% include "./Properties/overlaycolour.md" %}


### Common Identifiers

{% include "./Properties/active.md" %} 

{% include "./Properties/alpha.md" %}

{% include "./Properties/bounds.md" %}  

{% include "./Properties/channel.md" %} Meter expect normalised values between 0 and 1.
 
{% include "./Properties/identchannel.md" %} 

{% include "./Properties/rotate.md" %}   

{% include "./Properties/visible.md" %} 
 
{% include "./Properties/Widget_array_property.md" %} 

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

![](../images/metersEample.png)