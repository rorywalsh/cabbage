# Hrange

Create an on-screen slider with a dragable, and user selectable range. Data can be sent to Csound on the channel specified through the channel string.  


<big></pre>
hrange [channel](#channel_range)("min", "max"), 
[max](#max)(val), 
[min](#min)(val), 
[outlinecolour](#outlinecolour)("colour"), 
[range](#range)(min, max, value, skew, incr), 
[text](#text)("name"), 
[textbox](#textbox)(on/off), 
[textcolour](#textcolour)("colour"), 
[trackerthickness](#trackerthickness)(val), 
[trackercolour](#trackercolour)("colour"), 
[active](#active)(val), 
[alpha](#alpha)(val), 
[automatable](#automatable)(val), 
[bounds](#bounds)(x, y, width, height), 
[caption](#caption)("string"), 
[colour](#colour)("colour"), 
[fontcolour](#fontcolour)("colour"), 
[identchannel](#identchannel)("channel"), 
[popup](#popup)(val), 
[presetIgnore](#presetignore)(val), 
[valueprefix](#valueprefix)("valueprefix"), 
[valuepostfix](#valuepostfix)("valuepostfix"), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[visible](#visible)(val), 
[tofront](#tofront)(), 
[widgetarray](#widgetarray)("chan", number), 

</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/channel_range.md !}   

{! ./markdown/Widgets/Properties/max.md !} 

{! ./markdown/Widgets/Properties/min.md !} 

{! ./markdown/Widgets/Properties/outlinecolour.md !} 

{! ./markdown/Widgets/Properties/range.md !} 

{! ./markdown/Widgets/Properties/text.md !} 

{! ./markdown/Widgets/Properties/textbox.md !} 

{! ./markdown/Widgets/Properties/textcolour.md !} 

{! ./markdown/Widgets/Properties/trackerthickness.md !} 

{! ./markdown/Widgets/Properties/trackercolour.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/alpha.md !}

{! ./markdown/Widgets/Properties/automatable.md !}

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/caption.md !}  

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/fontcolour.md !}   

{! ./markdown/Widgets/Properties/identchannel.md !}  

{! ./markdown/Widgets/Properties/popup.md !}

{! ./markdown/Widgets/Properties/presetignore.md !} 

{! ./markdown/Widgets/Properties/valueprefix.md !}

{! ./markdown/Widgets/Properties/valuepostfix.md !}

{! ./markdown/Widgets/Properties/rotate.md !}  

{! ./markdown/Widgets/Properties/visible.md !}  

{! ./markdown/Widgets/Properties/tofront.md !} 

{! ./markdown/Widgets/Properties/widgetarray.md !}
 
<!--(End of identifiers)/-->

>Make sure to use two unique channel names when using hslider2 and vslider2, otherwise min and max will be set to the same value. 

![](../images/hrange.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("HRange Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
hrange bounds(120, 34, 160, 40), channel("hrangeL", "hrangeR"), colour(123, 34, 143), range(100, 1000, 200:300, 1, .01)
hrange bounds(110, 140, 162, 32), channel("range1", "range2"), identchannel("widgetIdent"), channel colour(123, 34, 143), corners(40), range(0, 1, .24:.75, 1 0.001)
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
    aTone1 oscili .25, chnget:k("hrangeL")
    aTone2 oscili .25, chnget:k("hrangeR")
    outs aTone1+aTone2, aTone1+aTone2    
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
	SIdent sprintf "text(\"TextOff %f\", \"TextOn %f\") ", rnd(100), rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "textcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "trackerthickness(%f) ", rnd(10)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "trackercolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
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

