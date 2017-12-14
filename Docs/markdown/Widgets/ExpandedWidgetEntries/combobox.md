# Combo Box

Combobox creates a drop-down list of items which users can choose from. Once the user selects an item, the index of their selection will be sent to Csound on a channel named by the channel string. The default value is 0.


<big></pre>
combobox [align](#align)("position"), 
[channeltype](#channel_type)("type"), 
[file](#file_combobox)("filename"), 
[items](#items)("item1", "item2", ...), 
[populate](#populate_combobox)("filetype", "dir"), 
[active](#active)(val), 
[alpha](#alpha)(val), 
[bounds](#bounds)(x, y, width, height), 
[channel](#channel)("chan"), 
[colour](#colour)("colour"), 
[fontcolour](#fontcolour)("colour"), 
[identchannel](#identchannel)("channel"), 
[popup](#popup)(val), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[value](#value)(val), 
[visible](#visible)(val), 
[widgetarray](#widgetarray)("chan", number), 

</pre></big>

<!--(End of syntax)/-->

## Identifiers

### Specific Identifiers

{! ./markdown/Widgets/Properties/align.md !} 

{! ./markdown/Widgets/Properties/channel_type.md !} 

{! ./markdown/Widgets/Properties/file_combobox.md !} 

{! ./markdown/Widgets/Properties/items.md !} 

{! ./markdown/Widgets/Properties/populate_combobox.md !} 


### Common Identifiers
{! ./markdown/Widgets/Properties/active.md !}   

{! ./markdown/Widgets/Properties/alpha.md !}  

{! ./markdown/Widgets/Properties/bounds.md !}   

{! ./markdown/Widgets/Properties/channel.md !} 

{! ./markdown/Widgets/Properties/colour.md !}   

{! ./markdown/Widgets/Properties/fontcolour.md !}  

{! ./markdown/Widgets/Properties/identchannel.md !}  

{! ./markdown/Widgets/Properties/popup.md !}  

{! ./markdown/Widgets/Properties/rotate.md !}   

{! ./markdown/Widgets/Properties/value.md !}  Comboboxes indices start at 1, not 0. If you set the initial value to 0 the combobox will not show any items.  

{! ./markdown/Widgets/Properties/visible.md !}   

{! ./markdown/Widgets/Properties/widgetarray.md !}  

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form caption("Checkbox Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
combobox bounds(36, 38, 150, 50), channel("combobox1"), items("100Hz", "200Hz", "300Hz")
combobox bounds(200, 38, 150, 50), channel("combobox2"), populate("*.*", ".")
combobox bounds(110, 140, 165, 62) identchannel("widgetIdent")
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
    iChoice = rnd(300)
    if iChoice > 0 && iChoice < 0 then
        SIdentifier strcat SIdentifier, "align(\"left\") "
    elseif iChoice > 100 && iChoice < 200 then
        SIdentifier strcat SIdentifier, "align(\"centre\") "
    else
        SIdentifier strcat SIdentifier, "align(\"right\") "
    endif
    SIdent sprintf "items(\"Text%d\", \"Text%d\", \"Text%d\", "Text%d\") ", rnd(100), rnd(100), rnd(100), rnd(100)
    SIdentifier strcat SIdentifier, SIdent
    SIdentifier strcat SIdentifier, "populate(\"*.*\", \".\") "
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
    SIdent sprintf "value(%f) ", rnd(1)
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

![](../images/comboboxExample.png)