# Sliders

Slider can be used to create an on-screen slider. Data can be sent to Csound on the channel specified through the channel string. Sliders can be rotary, vertical, horizontal, frange-based, or number boxes, and can react in both a linear and non-linear fashion. 

<big></pre>
slider [max](#max)(val), 
[min](#min)(val), 
[outlinecolour](#outlinecolour)("colour"), 
[popuptext](#popuptext)("text"), 
[popuptext](#popupprefix)("text"), 
[popuppostfix](#popuppostfix)("text"), 
[range](#range)(min, max, value, skew, incr), 
[svgfile](#svgfile_slider)("type", "file"), 
[textcolour](#textcolour)("colour"), 
[text](#text_slider)("name"), 
[trackerthickness](#trackerthickness)(val), 
[textboxoutlinecolour](#textboxoutlinecolour)("colour"), 
[textboxcolour](#textboxcolour)("colour"), 
[trackercolour](#trackercolour)("colour"), 
[valuetextbox](#valuetextbox)(val), 
[velocity](#velocity)(val), 
[active](#active)(val), 
[alpha](#alpha)(val), 
[bounds](#bounds)(x, y, width, height), 
[channel](#channel)("chan"), 
[colour](#colour)("colour"), 
[fontcolour](#fontcolour)("colour"), 
[identchannel](#identchannel)("channel"), 
[popup](#popup)(val), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[visible](#visible)(val), 
[widgetarray](#widgetarray)("chan", number), 

</pre></big>

<!--(End of syntax)/-->

##Identifiers
### Specific Identifiers

{! ./markdown/Widgets/Properties/max.md !}  

{! ./markdown/Widgets/Properties/min.md !}  

{! ./markdown/Widgets/Properties/outlinecolour.md !}  

{! ./markdown/Widgets/Properties/popuptext.md !} Not available to nslider 

{! ./markdown/Widgets/Properties/popupprefix.md !} Not available to nslider 

{! ./markdown/Widgets/Properties/popuppostfix.md !} Although nslider's don't use popup bubbles, this identifier will set a value postfix. 

{! ./markdown/Widgets/Properties/range.md !}   

{! ./markdown/Widgets/Properties/svgfile_slider.md !}   

{! ./markdown/Widgets/Properties/textcolour.md !}   

{! ./markdown/Widgets/Properties/text_slider.md !} 

{! ./markdown/Widgets/Properties/trackerthickness.md !} 

{! ./markdown/Widgets/Properties/textboxoutlinecolour.md !}  

{! ./markdown/Widgets/Properties/textboxcolour.md !}  

{! ./markdown/Widgets/Properties/trackercolour.md !} 

{! ./markdown/Widgets/Properties/valuetextbox.md !} 

{! ./markdown/Widgets/Properties/velocity.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md  !}

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/channel.md !}  

{! ./markdown/Widgets/Properties/colour.md !} 

{! ./markdown/Widgets/Properties/fontcolour.md !} 

{! ./markdown/Widgets/Properties/identchannel.md  !}

{! ./markdown/Widgets/Properties/popup.md !} Not available to nslider 

{! ./markdown/Widgets/Properties/rotate.md  !}

{! ./markdown/Widgets/Properties/visible.md  !}

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

##Example

```csharp
<Cabbage>
form caption("Button Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
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

                instr 2
                    if metro(1) == 1 then
                        event "i", "ChangeAttributes", 0, 1
                    endif
                endin

                instr ChangeAttributes
                    SIdentifier init ""
    SIdent sprintf "outlinecolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
    SIdent sprintf "popuptext(\"Popup Text %d\") ", rnd(1000)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "textcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
    SIdent sprintf "text(\"TextOff %f\") ", rnd(100)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "trackerthickness(%f) ", rnd(10)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "textboxoutlinecolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
    SIdent sprintf "textboxcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
    SIdent sprintf "trackercolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
    SIdent sprintf "alpha(%f) ", rnd(100)/100
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "pos(%d, 140) ", 100 + rnd(100)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "size(%d, %d) ", abs(rnd(200))+40, abs(rnd(100))+50
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "colour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
    SIdent sprintf "fontcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
    SIdent sprintf "visible(%d) ", (rnd(100) > 80 ? 0 : 1)
    SIdentifier strcat SIdentifier, SIdent

                    chnset SIdentifier, "widgetIdent"           
                endin

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

![](../images/sliderExample.png)
