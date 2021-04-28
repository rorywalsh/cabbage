# OptionButton

Optionbutton creates a button on screen that can be used to toggle between a range of items. It functions much like a combobox box, but doesn't have a drop down menu. Clicking on the button will cause it to cycle through each of its text items, sending a numeric value to Csound in the process. 

<big></pre>
button WIDGET_SYNTAX
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

{! ./markdown/Widgets/Properties/fontColour_0.md !} 

{! ./markdown/Widgets/Properties/fontColour_1.md !} 

{! ./markdown/Widgets/Properties/identChannel.md !} 

{! ./markdown/Widgets/Properties/outlineColour.md !} 

{! ./markdown/Widgets/Properties/outlineColour.md !} 

{! ./markdown/Widgets/Properties/outlineThickness.md !} 

{! ./markdown/Widgets/Properties/popup.md !} 

{! ./markdown/Widgets/Properties/radioGroup.md !} 

{! ./markdown/Widgets/Properties/rotate.md !} 

{! ./markdown/Widgets/Properties/imgFile.md !} 

{! ./markdown/Widgets/Properties/text.md !}

{! ./markdown/Widgets/Properties/value.md !} 

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/widgetArray.md !} 

<!--(End of identifiers)/-->

>colour:1() and fontColour:1() can be set using colour() and fontColour(). However, it's recommended that you use the numerated colour identifiers in order to make your code as readable a possible. 

![Button](../images/button.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Button Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontColour("black")
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
optionbutton bounds(116, 38, 150, 50), channel("button1"), text("Sine", "Square", "Triangle", "Saw"),
button bounds(110, 140, 165, 62) identChannel("widgetIdent")
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
