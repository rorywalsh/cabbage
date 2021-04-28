# Endless encoder

Encoder creates an endless rotary encoder similar to those found in certain well-known hardware samplers. Unlike regular sliders, *encoder* doesn't have a start and stop point. One may set a starting point, but after this the user can drag up or down as long as they like. By setting a very small slider increment these widgets can offer a level of fine grain control not possible with regular sliders. 

<big></pre>
encoder [increment](#increment)(val), 
[max](#max)(val), 
[min](#min)(val), 
[outlinecolour](#outlinecolour)("colour"), 
[trackercolour](#trackercolour)("colour"), 
[textbox](#textbox)(on/off), 
[textcolour](#textcolour)("colour"), 
[text](#text_slider)("name"), 
[active](#active)(val), 
[alpha](#alpha)(val), 
[automatable](#automatable)(val), 
[bounds](#bounds)(x, y, width, height), 
[caption](#caption)("string"), 
[channel](#channel)("chan"), 
[colour](#colour)("colour"), 
[fontcolour](#fontcolour)("colour"), 
[identchannel](#identchannel)("channel"), 
[popup](#popup)(val), 
[presetIgnore](#presetignore)(val), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[imgfile](#svgfile)("type", "file"), 
[value](#value)(val), 
[valueprefix](#valueprefix)("valueprefix"), 
[valuepostfix](#valuepostfix)("valuepostfix"), 
[visible](#visible)(val), 
[tofront](#tofront)(), 
[widgetarray](#widgetarray)("chan", number), 

</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/increment.md !} 

{! ./markdown/Widgets/Properties/max.md !} 

{! ./markdown/Widgets/Properties/min.md !} 

{! ./markdown/Widgets/Properties/outlinecolour.md !} 

{! ./markdown/Widgets/Properties/trackercolour.md !} 

{! ./markdown/Widgets/Properties/textbox.md !} 

{! ./markdown/Widgets/Properties/textcolour.md !} 

{! ./markdown/Widgets/Properties/text_slider.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !} 

{! ./markdown/Widgets/Properties/alpha.md !}

{! ./markdown/Widgets/Properties/automatable.md !}

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/caption.md !}  

{! ./markdown/Widgets/Properties/channel.md !}  

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/fontcolour.md !}   

{! ./markdown/Widgets/Properties/identchannel.md !}  

{! ./markdown/Widgets/Properties/popup.md !}

{! ./markdown/Widgets/Properties/presetignore.md !} 

{! ./markdown/Widgets/Properties/rotate.md !}  

{! ./markdown/Widgets/Properties/svgfile.md !} 

{! ./markdown/Widgets/Properties/value.md !}

{! ./markdown/Widgets/Properties/valueprefix.md !}

{! ./markdown/Widgets/Properties/valuepostfix.md !}

{! ./markdown/Widgets/Properties/visible.md !}  

{! ./markdown/Widgets/Properties/tofront.md !} 

{! ./markdown/Widgets/Properties/widgetarray.md !}  

<!--(End of identifiers)/-->

![Button](../images/encoder.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Encoder Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
encoder bounds(8, 38, 369, 50), channel("gain"), text("Gain") min(0), max(1), increment(.01) fontcolour(91, 46, 46, 255) textcolour(29, 29, 29, 255)
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
encoder bounds(70, 140, 41, 119) identchannel("widgetIdent"),  
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
	SIdent sprintf "trackercolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "textcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "text(\"TextOff %f\") ", rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "alpha(%f) ", 50 + rnd(50)/50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "pos(%d, 140) ", 100 + rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "size(%d, %d) ", abs(rnd(200))+40, abs(rnd(100))+50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "caption(\"Text%d\") ", rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "colour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "fontcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "value(%f) ", rnd(1)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "visible(%d) ", (rnd(100) > 80 ? 0 : 1)
	SIdentifier strcat SIdentifier, SIdent
    ;send identifier string to Cabbage
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
<!--(Widget Example)/-->
