# OptionButton

Optionbutton creates a button on screen that can be used to toggle between a range of items. It functions much like a combobox box, but doesn't have a drop down menu. Clicking on the button will cause it to cycle through each of its text items, sending a numeric value to Csound in the process. 

<big></pre>
button [active](#active)(val), 
[alpha](#alpha)(val), 
[automatable](#automatable)(val), 
[bounds](#bounds)(x, y, width, height), 
[caption](#caption)("string"), 
[channel](#channel)("chan"), 
[colour:0](#colour_0)("colour"), 
[colour:1](#colour_1)("colour"), 
[fontcolour:0](#fontcolour_0)("colour"), 
[fontcolour:1](#fontcolour_1)("colour"), 
[identchannel](#identchannel)("channel"), 
[outlinecolour](#outlinecolour)("colour"), 
[outlinecolour](#outlinecolour)("colour"), 
[outlinethickness](#outlinethickness)(value), 
[popup](#popup)(val), 
[radiogroup](#radiogroup)(val), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[imgfile](#imgfile)("type", "file"), 
[text](#text)("name"), 
[value](#value)(val), 
[visible](#visible)(val), 
[tofront](#tofront)(), 
[widgetarray](#widgetarray)("chan", number), 

</pre></big>

### Specific Identifiers

### Common Identifiers
{! ./markdown/Widgets/Properties/active.md !}

{! ./markdown/Widgets/Properties/alpha.md !}

{! ./markdown/Widgets/Properties/automatable.md !} 

{! ./markdown/Widgets/Properties/bounds.md !} 

{! ./markdown/Widgets/Properties/caption.md !} 

{! ./markdown/Widgets/Properties/channel.md !} 

{! ./markdown/Widgets/Properties/colour_0.md !} 

{! ./markdown/Widgets/Properties/colour_1.md !} 

{! ./markdown/Widgets/Properties/fontcolour_0.md !} 

{! ./markdown/Widgets/Properties/fontcolour_1.md !} 

{! ./markdown/Widgets/Properties/identchannel.md !} 

{! ./markdown/Widgets/Properties/outlinecolour.md !} 

{! ./markdown/Widgets/Properties/outlinecolour.md !} 

{! ./markdown/Widgets/Properties/outlinethickness.md !} 

{! ./markdown/Widgets/Properties/popup.md !} 

{! ./markdown/Widgets/Properties/radiogroup.md !} 

{! ./markdown/Widgets/Properties/rotate.md !} 

{! ./markdown/Widgets/Properties/imgfile.md !} 

{! ./markdown/Widgets/Properties/text.md !}

{! ./markdown/Widgets/Properties/value.md !} 

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/tofront.md !} 

{! ./markdown/Widgets/Properties/widgetarray.md !} 

<!--(End of identifiers)/-->

>colour:1() and fontcolour:1() can be set using colour() and fontcolour(). However, it's recommended that you use the numerated colour identifiers in order to make your code as readable a possible. 

![Button](../images/button.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Button Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
optionbutton bounds(116, 38, 150, 50), channel("button1"), text("Sine", "Square", "Triangle", "Saw"),
button bounds(110, 140, 165, 62) identchannel("widgetIdent")
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
    aTone oscili chnget:k("button1"), 300
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
	SIdent sprintf "alpha(%f) ", 50 + rnd(50)/50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "pos(%d, 140) ", 100 + rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "size(%d, %d) ", abs(rnd(200))+40, abs(rnd(100))+50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "caption(\"Text%d\") ", rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "colour:0(%d, 255, 255) ", rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "colour:1(%d, 0, 255) ", rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "fontcolour:0(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "fontcolour:1(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent   
	SIdent sprintf "outlinecolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "outlinecolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "outlinethickness(%f) ", rnd(100)/50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "text(\"TextOff %f\", \"TextOn %f\") ", rnd(100), rnd(100)
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
<!--(End Widget Example)/-->
