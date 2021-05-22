# Range Sliders

Create an on-screen slider with a dragable, and user selectable range. Data can be sent to Csound on the channel specified through the channel string.  

<video width="800" height="600" controls>
<source src="../../images/docs/range.mp4">
</video> 

<big></pre>
hrange WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/channel.md !}   

{! ./markdown/Widgets/Properties/max.md !} 

{! ./markdown/Widgets/Properties/min.md !} 

{! ./markdown/Widgets/Properties/outlineColour.md !} 

{! ./markdown/Widgets/Properties/range.md !} 

{! ./markdown/Widgets/Properties/text.md !} 

{! ./markdown/Widgets/Properties/textBox.md !} 

{! ./markdown/Widgets/Properties/textColour.md !} 

{! ./markdown/Widgets/Properties/trackerThickness.md !} 

{! ./markdown/Widgets/Properties/trackerColour.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/alpha.md !}

{! ./markdown/Widgets/Properties/automatable.md !}

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/caption.md !}  

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/fontColour.md !}   

{! ./markdown/Widgets/Properties/identChannel.md !}  

{! ./markdown/Widgets/Properties/popup.md !}

{! ./markdown/Widgets/Properties/presetIgnore.md !} 

{! ./markdown/Widgets/Properties/valuePrefix.md !}

{! ./markdown/Widgets/Properties/valuePostfix.md !}

{! ./markdown/Widgets/Properties/rotate.md !}  

{! ./markdown/Widgets/Properties/visible.md !}  

{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/widgetArray.md !}
 
<!--(End of identifiers)/-->

>Make sure to use two unique channel names when using hslider2 and vslider2, otherwise min and max will be set to the same value. 


##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("HRange Example") size(380, 500), guiMode("queue"), colour(2, 145, 209) pluginId("def1")
texteditor bounds(14, 260, 352, 228) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)

hrange bounds(22, 20, 336, 40) channel("rangeMin1", "rangeMax1") range(0, 1000, 100:200, 1, 0.001)
hrange bounds(22, 60, 336, 40) channel("rangeMin2", "rangeMax2") range(0, 1000, 200:300, 1, 0.001)
hrange bounds(22, 100, 336, 40) channel("rangeMin3", "rangeMax3") range(0, 1000, 400:500, 1, 0.001)
hrange bounds(22, 140, 336, 40) channel("rangeMin4", "rangeMax4") range(0, 1000, 600:700, 1, 0.001)
hrange bounds(22, 180, 336, 40) channel("rangeMin5", "rangeMax5") range(0, 1000, 800:900, 1, 0.001)
button bounds(278, 224, 78, 28) channel("agitate"), text("Agitate", "Stop")
combobox bounds(22, 224, 100, 30) channel("source"), items("Noise", "Pulse Wave", "Blurred Sample")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d 
</CsOptions>e
<CsInstruments>
; Initialize the global variables. 
ksmps = 16
nchnls = 2
0dbfs = 1

; Rory Walsh 2021 
;
; License: CC0 1.0 Universal
; You can copy, modify, and distribute this file, 
; even for commercial purposes, all without asking permission. 

instr 99

    SText  = "A form represents an instrument plugin window. All widgets are positioned relative to the top left of the form, i.e., position 0,0.\n\nWhen declaring a form, you can also set some useful attributes of your plugin such as the pluginId and plugin delay compensation (latency)."
    cabbageSet "infoText", "text", SText
    
endin

instr 1
    kMin cabbageGetValue sprintf("rangeMin%d", p4)
    kMax cabbageGetValue sprintf("rangeMax%d", p4)
    aSource init 0 
    if cabbageGetValue:k("source") == 1 then
        aSource pinker
    elseif cabbageGetValue:k("source") == 2 then
        aSource buzz .6, 50, 20, -1
    else
        a1, a2 diskin2 "Guitar3.wav", .4, 0, 1
        fftIn pvsanal	(a1+a2)*.5, 1024, 1024/4, 1024, 1
        fftBlur	pvsblur	fftIn, .5, 2
        aSource pvsynth	fftBlur
    endif
    
    aBp butterbp aSource, port(kMin+(kMax-kMin/2), .2), kMax-kMin
    outs aBp, aBp

    if cabbageGetValue:k("agitate") == 1 then
        kFreq = abs(jspline(1000-400, .1, .3))
        kRange = 100+abs(jspline(400, .2, .3))
        cabbageSet metro(10), sprintf("rangeMin%d", p4), "minValue", kFreq
        cabbageSet metro(10), sprintf("rangeMin%d", p4), "maxValue", kFreq+kRange
    endif

endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 z 1
i1 0 z 2
i1 0 z 3
i1 0 z 4
i1 0 z 5
i99 0 z

</CsScore>
</CsoundSynthesizer>

```
<!--(Widget Example)/-->

