# Meter

Meters can be used to create any kind of horizontal or vertical metering bar, such as VU meters. 


<big></pre>
meter WIDGET_SYNTAX
</pre></big>
<!--(End of syntax)/-->

##Identifiers

### Specific Identifiers

{! ./markdown/Widgets/Properties/corners.md !} 

{! ./markdown/Widgets/Properties/outlinecolour.md !}  

{! ./markdown/Widgets/Properties/outlinethickness.md !}  

{! ./markdown/Widgets/Properties/overlaycolour.md !} 

{! ./markdown/Widgets/Properties/metercolour.md !} 


### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/bounds.md !}   

{! ./markdown/Widgets/Properties/channel.md !}  Meter expect normalised values between 0 and 1.
 
{! ./markdown/Widgets/Properties/identchannel.md !}  

{! ./markdown/Widgets/Properties/rotate.md !}    

{! ./markdown/Widgets/Properties/visible.md !}  
 
{! ./markdown/Widgets/Properties/widgetarray.md !}  

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