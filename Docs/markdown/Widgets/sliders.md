# Sliders

Slider can be used to create an on-screen slider. Data can be sent to Csound on the channel specified through the channel string. Sliders can be rotary, vertical, horizontal, frange-based, or number boxes, and can react in both a linear and non-linear fashion. 

<big></pre>
slider WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/max.md !}  

{! ./markdown/Widgets/Properties/markerColour.md !}  

{! ./markdown/Widgets/Properties/markerThickness.md !}  

{! ./markdown/Widgets/Properties/markerStart.md !} 

{! ./markdown/Widgets/Properties/markerEnd.md !}  

{! ./markdown/Widgets/Properties/min.md !}  

{! ./markdown/Widgets/Properties/outlineColour.md !}  

{! ./markdown/Widgets/Properties/popupText.md !} Not available to nslider 

{! ./markdown/Widgets/Properties/popupPrefix.md !} Not available to nslider 

{! ./markdown/Widgets/Properties/popupPostfix.md !} Although nslider's don't use popup bubbles, this identifier will set a value postfix. 

{! ./markdown/Widgets/Properties/range.md !}   

{! ./markdown/Widgets/Properties/filmStrip.md !}   

{! ./markdown/Widgets/Properties/imgFile_slider.md !} 

{! ./markdown/Widgets/Properties/sliderBounds.md !}   

{! ./markdown/Widgets/Properties/textColour.md !}   

{! ./markdown/Widgets/Properties/text_slider.md !} 

{! ./markdown/Widgets/Properties/trackerThickness.md !} Not available for rsliders

{! ./markdown/Widgets/Properties/trackerInsideRadius.md !} Only available to rsliders

{! ./markdown/Widgets/Properties/trackerOutsideRadius.md !} Only available to rsliders

{! ./markdown/Widgets/Properties/textBoxOutlineColour.md !}  

{! ./markdown/Widgets/Properties/textBoxColour.md !}  

{! ./markdown/Widgets/Properties/trackerColour.md !} 

{! ./markdown/Widgets/Properties/valueTextBox.md !} 

{! ./markdown/Widgets/Properties/valueTextBoxBounds.md !} 

{! ./markdown/Widgets/Properties/velocity.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md  !}

{! ./markdown/Widgets/Properties/alpha.md !}

{! ./markdown/Widgets/Properties/automatable.md !} 

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/channel.md !}  

{! ./markdown/Widgets/Properties/colour.md !} 

{! ./markdown/Widgets/Properties/fontColour.md !} 

{! ./markdown/Widgets/Properties/identChannel.md  !}

{! ./markdown/Widgets/Properties/popup.md !} Not available to nslider

{! ./markdown/Widgets/Properties/presetIgnore.md !} 

{! ./markdown/Widgets/Properties/valuePrefix.md !}

{! ./markdown/Widgets/Properties/valuePostfix.md !}

{! ./markdown/Widgets/Properties/rotate.md  !}

{! ./markdown/Widgets/Properties/visible.md  !}

{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/widgetArray.md !}

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
label bounds(8, 6, 368, 20), text("Basic Usage"), fontColour("black")
hslider bounds(8, 38, 369, 50), channel("gain"), text("Gain") range(0, 1, 0, 1, 0.001) fontColour(91, 46, 46, 255) textColour(29, 29, 29, 255)
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
rslider bounds(70, 140, 41, 119) channel("rsliderChannel"), identChannel("widgetIdent"), range(0, 1, 0, 1, 0.001) 
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
