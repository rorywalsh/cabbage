# Sliders

Slider can be used to create an on-screen slider. Data can be sent to Csound on the channel specified through the channel string. Sliders can be rotary, vertical, horizontal, frange-based, or number boxes, and can react in both a linear and non-linear fashion. 

<big></pre>
slider WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/max.md !}  

{! ./markdown/Widgets/Properties/markercolour.md !}  

{! ./markdown/Widgets/Properties/markerthickness.md !}  

{! ./markdown/Widgets/Properties/markerstart.md !} 

{! ./markdown/Widgets/Properties/markerend.md !}  

{! ./markdown/Widgets/Properties/min.md !}  

{! ./markdown/Widgets/Properties/outlinecolour.md !}  

{! ./markdown/Widgets/Properties/popuptext.md !} Not available to nslider 

{! ./markdown/Widgets/Properties/popupprefix.md !} Not available to nslider 

{! ./markdown/Widgets/Properties/popuppostfix.md !} Although nslider's don't use popup bubbles, this identifier will set a value postfix. 

{! ./markdown/Widgets/Properties/range.md !}   

{! ./markdown/Widgets/Properties/filmstrip.md !}   

{! ./markdown/Widgets/Properties/imgfile_slider.md !} 

{! ./markdown/Widgets/Properties/sliderbounds.md !}   

{! ./markdown/Widgets/Properties/textcolour.md !}   

{! ./markdown/Widgets/Properties/text_slider.md !} 

{! ./markdown/Widgets/Properties/trackerthickness.md !} Not available for rsliders

{! ./markdown/Widgets/Properties/trackerinsideradius.md !} Only available to rsliders

{! ./markdown/Widgets/Properties/trackeroutsideradius.md !} Only available to rsliders

{! ./markdown/Widgets/Properties/textboxoutlinecolour.md !}  

{! ./markdown/Widgets/Properties/textboxcolour.md !}  

{! ./markdown/Widgets/Properties/trackercolour.md !} 

{! ./markdown/Widgets/Properties/valuetextbox.md !} 

{! ./markdown/Widgets/Properties/valuetextboxbounds.md !} 

{! ./markdown/Widgets/Properties/velocity.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md  !}

{! ./markdown/Widgets/Properties/alpha.md !}

{! ./markdown/Widgets/Properties/automatable.md !} 

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/channel.md !}  

{! ./markdown/Widgets/Properties/colour.md !} 

{! ./markdown/Widgets/Properties/fontcolour.md !} 

{! ./markdown/Widgets/Properties/identchannel.md  !}

{! ./markdown/Widgets/Properties/popup.md !} Not available to nslider

{! ./markdown/Widgets/Properties/valueprefix.md !}

{! ./markdown/Widgets/Properties/valuepostfix.md !}

{! ./markdown/Widgets/Properties/rotate.md  !}

{! ./markdown/Widgets/Properties/visible.md  !}

{! ./markdown/Widgets/Properties/tofront.md !} 

{! ./markdown/Widgets/Properties/widgetarray.md !}

<!--(End of identifiers)/-->

##Slider types:

* *rslider*, a standard rotary or knob slider

* *hslider*, a standard horizontal slider

* *vslider*, a standard vertical slider

* *hslider2*, two value horizontal range slider (**deprecated, use hrange**)

* *vslider2*, two value vertical range slider (**deprecated, use vrange**)

* *nslider*, a number box slider in the style of Pure Data /MaxMSP


>Make sure to use two unique channel names when using hslider2 and vslider2, otherwise min and max will be set to the same value. 

![](../images/sliders.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Slider Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
hslider bounds(8, 38, 369, 50), channel("gain"), text("Gain") range(0, 1, 0, 1, 0.001) fontcolour(91, 46, 46, 255) textcolour(29, 29, 29, 255)
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
rslider bounds(70, 140, 41, 119) channel("rsliderChannel"), identchannel("widgetIdent"), range(0, 1, 0, 1, 0.001) 
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
    aTone oscili chnget:k("gain"), 300
    outs aTone, aTone    
endin

;WIDGET_ADVANCED_USAGE

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z
i2 0 z
</CsScore>
</CsoundSynthesizer>
```
<!--(End Widget Example)/-->
