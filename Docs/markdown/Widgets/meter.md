# Meter

Meters can be used to create any kind of horizontal or vertical metering bar, such as VU meters. 


<big></pre>
meter WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/corners.md !} 

{! ./markdown/Widgets/Properties/outlineColour.md !}  

{! ./markdown/Widgets/Properties/outlineThickness.md !}  

{! ./markdown/Widgets/Properties/overlayColour.md !} 

{! ./markdown/Widgets/Properties/meterColour.md !} 


### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/bounds.md !}   

{! ./markdown/Widgets/Properties/channel.md !}  Meter expect normalised values between 0 and 1.
 
{! ./markdown/Widgets/Properties/identChannel.md !}  

{! ./markdown/Widgets/Properties/rotate.md !}    

{! ./markdown/Widgets/Properties/visible.md !}  
 
{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/widgetArray.md !}  

##Slider types:

* *rslider*, a standard rotary or knob slider

* *hslider*, a standard horizontal slider

<!--(End of identifiers)/-->
![](../images/meter.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Meter example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontColour("black")
vmeter bounds(116, 32, 35, 80) channel("vMeter1") value(0) overlayColour(70, 53, 53, 255) meterColour:0(0, 255, 0, 255) meterColour:1(0, 103, 171, 255) meterColour:2(23, 0, 123, 255) outlineThickness(2) 
vmeter bounds(156, 32, 35, 80) channel("vMeter2") value(0) overlayColour(70, 53, 53, 255) meterColour:0(0, 255, 0, 255) meterColour:1(0, 103, 171, 255) meterColour:2(23, 0, 123, 255) outlineThickness(2) 
vmeter bounds(196, 32, 35, 80) channel("vMeter3") value(0) overlayColour(70, 53, 53, 255) meterColour:0(0, 255, 0, 255) meterColour:1(0, 103, 171, 255) meterColour:2(23, 0, 123, 255) outlineThickness(2) 
vmeter bounds(236, 32, 35, 80) channel("vMeter4") value(0) overlayColour(70, 53, 53, 255) meterColour:0(0, 255, 0, 255) meterColour:1(0, 103, 171, 255) meterColour:2(23, 0, 123, 255) outlineThickness(2)
groupbox bounds(8, 118, 380, 177), text("Randomly Updated Identifiers")
vmeter bounds(142, 140, 46, 145), channel("meterTest") identChannel("widgetIdent")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

seed 0 
;basic usage
instr 1
    if metro(20) == 1 then
        chnset abs:k(randi:k(1.1, 100, 2)), "vMeter1"
        chnset abs:k(randi:k(1.1, 100, 2)), "vMeter2"
        chnset abs:k(randi:k(1.1, 100, 2)), "vMeter3"
        chnset abs:k(randi:k(1.1, 100, 2)), "vMeter4"
        chnset abs:k(randi:k(1.1, 100, 2)), "meterTest"
	endif
endin

;WIDGET_ADVANCED_USAGE

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i1 0 z
i2 0 z
</CsScore>
</CsoundSynthesizer>
```
<!--(End Widget Example)/-->
