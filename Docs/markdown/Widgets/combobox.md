# Combo Box

Combobox creates a drop-down list of items which users can choose from. Once the user selects an item, the index of their selection will be sent to Csound on a channel named by the channel string. The default value is 0. The first item in the combobox will be seen as a 1, while 0 refers to no current selection.

A combobox can also be set up a work with strings. In this case Csound will receive the currently selected string instead of the current index. Note that using a combobox in this way renders it useless in terms of host automation. Host automation can only be achieved when a combobox is sending the current index, rather than the current string.  


<big></pre>
combobox WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/align.md !} 

{! ./markdown/Widgets/Properties/channel_type.md !} 

{! ./markdown/Widgets/Properties/file_combobox.md !} 

{! ./markdown/Widgets/Properties/items.md !} 

{! ./markdown/Widgets/Properties/populate_combobox.md !} 


### Common Identifiers
{! ./markdown/Widgets/Properties/active.md !}   

{! ./markdown/Widgets/Properties/alpha.md !}

{! ./markdown/Widgets/Properties/automatable.md !} 

{! ./markdown/Widgets/Properties/bounds.md !}   

{! ./markdown/Widgets/Properties/channel.md !} 

{! ./markdown/Widgets/Properties/colour.md !}   

{! ./markdown/Widgets/Properties/fontColour.md !}  

{! ./markdown/Widgets/Properties/identChannel.md !}  

{! ./markdown/Widgets/Properties/popup.md !}  

{! ./markdown/Widgets/Properties/presetIgnore.md !} 

{! ./markdown/Widgets/Properties/protectedItems.md !} 

{! ./markdown/Widgets/Properties/rotate.md !}   

{! ./markdown/Widgets/Properties/value.md !}  Comboboxes indices start at 1, not 0. If you set the initial value to 0 the combobox will not show any items.

{! ./markdown/Widgets/Properties/visible.md !}   

{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/widgetArray.md !}  

<!--(End of identifiers)/-->

![Button](../images/combobox.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Combobox Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontColour("black")
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
combobox bounds(36, 38, 150, 50), channel("combobox1"), items("100Hz", "200Hz", "300Hz")
combobox bounds(200, 38, 150, 50), channel("combobox2"), populate("*.*", ".")
combobox bounds(110, 140, 165, 62) identChannel("widgetIdent")
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
