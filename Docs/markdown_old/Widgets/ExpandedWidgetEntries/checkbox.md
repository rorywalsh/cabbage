# Check Box

Checkbox creates a checkbox which functions like a button, but the associated caption will not change when the user checks it. As with all widgets capable of sending data to Csound, the channel string is the channel on which the widget will communicate with Csound. 


<big></pre>
checkbox [corners](#checkbox_corner)(val), 
[radiogroup](#radiogroup)(val), 
[active](#active)(val), 
[automatable](#automatable)(val), 
[alpha](#alpha)(val), 
[bounds](#bounds)(x, y, width, height), 
[channel](#channel)("chan"), 
[colour:0](#colour_0)("colour"), 
[colour:1](#colour_1)("colour"), 
[fontcolour](#fontcolour)("colour"), 
[identchannel](#identchannel)("channel"), 
[popup](#popup)(val), 
[presetIgnore](#presetignore)(val), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[shape](#shape)("shape"), 
[text](#text)("name"), 
[value](#value)(val), 
[visible](#visible)(val), 
[tofront](#tofront)(), 
[widgetarray](#widgetarray)("chan", number), 

</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/checkbox_corner.md !} 

{! ./markdown/Widgets/Properties/radiogroup.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !}

{! ./markdown/Widgets/Properties/automatable.md !}

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/bounds.md !} 

{! ./markdown/Widgets/Properties/channel.md !} 

{! ./markdown/Widgets/Properties/colour_0.md !} 

{! ./markdown/Widgets/Properties/colour_1.md !} 

>You can create an invisible checkbox by setting the colour to something with an alpha value of 0. Images can then be placed under the checkbox to create an image button. See the PVSampler instrument as an example of this. 

{! ./markdown/Widgets/Properties/fontcolour.md !} 

{! ./markdown/Widgets/Properties/identchannel.md !} 

{! ./markdown/Widgets/Properties/popup.md !} 

{! ./markdown/Widgets/Properties/presetignore.md !} 

{! ./markdown/Widgets/Properties/rotate.md !} 

{! ./markdown/Widgets/Properties/shape.md !} 

{! ./markdown/Widgets/Properties/text.md !}

{! ./markdown/Widgets/Properties/value.md !} 

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/tofront.md !} 

{! ./markdown/Widgets/Properties/widgetarray.md !} 

<!--(End of identifiers)/-->

>colour:1() can be set using colour(). However, it's recommended that you use the numerated colour identifiers in order to make your code more readable. 

![Button](../images/checkbox.gif)

## Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Checkbox Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
checkbox bounds(116, 38, 150, 50), channel("checkbutton1"), text("Enable Tone", "Disable Tone"),
checkbox bounds(110, 140, 165, 62) identchannel("widgetIdent")
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
    aTone oscili chnget:k("checkbutton1"), 300
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
	SIdent sprintf "corners(%d) ", rnd(100)/80
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "alpha(%f) ", 50 + rnd(50)/50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "pos(%d, 140) ", 100 + rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "size(%d, %d) ", abs(rnd(200))+40, abs(rnd(100))+50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "colour:0(%d, 255, 255) ", rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "colour:1(%d, 0, 255) ", rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "fontcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
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


