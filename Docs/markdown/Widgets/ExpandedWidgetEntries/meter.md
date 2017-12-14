# Meter

Meters can be used to create any kind of horizontal or vertical metering bar, such as VU meters. 


<big></pre>
meter [corners](#corners)(val), 
[outlinecolour](#outlinecolour)("colour"), 
[outlinethickness](#outlinethickness)(value), 
[overlaycolour](#overlaycolour)("colour"), 
[metercolour:N](#metercolour)("colour"), 
[active](#active)(val), 
[alpha](#alpha)(val), 
[bounds](#bounds)(x, y, width, height), 
[channel](#channel)("chan"), 
[identchannel](#identchannel)("channel"), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[visible](#visible)(val), 
[widgetarray](#widgetarray)("chan", number), 

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
form caption("Meter example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
vmeter bounds(116, 32, 35, 80) channel("vMeter1") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(2) 
vmeter bounds(156, 32, 35, 80) channel("vMeter2") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(2) 
vmeter bounds(196, 32, 35, 80) channel("vMeter3") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(2) 
vmeter bounds(236, 32, 35, 80) channel("vMeter4") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(2)
groupbox bounds(8, 118, 380, 177), text("Randomly Updated Identifiers")
vmeter bounds(142, 140, 46, 145), channel("meterTest") identchannel("widgetIdent")
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
	chnset abs(randi:k(1.1, 100, 2)), "vMeter1"
	chnset abs(randi:k(1.1, 100, 2)), "vMeter2"
	chnset abs(randi:k(1.1, 100, 2)), "vMeter3"
	chnset abs(randi:k(1.1, 100, 2)), "vMeter4"
endin

;WIDGET_ADVANCED_USAGE

                instr 2
                    if metro(1) == 1 then
                        event "i", "ChangeAttributes", 0, 1
                    endif
                endin

                instr ChangeAttributes
                    SIdentifier init ""
    SIdent sprintf "corners(%d) ", rnd(100)/80
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "outlinecolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
    SIdent sprintf "outlinethickness(%f) ", rnd(100)/50
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "overlaycolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
    SIdent sprintf "metercolour:0(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
    SIdent sprintf "alpha(%f) ", rnd(100)/100
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "pos(%d, 140) ", 100 + rnd(100)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "size(%d, %d) ", abs(rnd(200))+40, abs(rnd(100))+50
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "visible(%d) ", (rnd(100) > 80 ? 0 : 1)
    SIdentifier strcat SIdentifier, SIdent

                    chnset SIdentifier, "widgetIdent"           
                endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
</CsScore>
</CsoundSynthesizer>
```

![](../images/metersEample.png)