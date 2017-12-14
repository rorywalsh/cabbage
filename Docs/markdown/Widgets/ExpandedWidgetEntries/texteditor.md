# Text Editor

Texteditor creates a single line text editor that can be used to send strings to Csound. Hitting return when in single line mode will send the string to Csound on a named string channel while pressing the up and down buttons when the texteditor is in focus will toggle through the previous strings that have been sent. When in "multiline" mode, press command and return to send the string data to Csound. 

<big></pre>
texteditor [wrap](#wrap)(val), 
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

{! ./markdown/Widgets/Properties/wrap.md !} 

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

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/widgetarray.md !}  

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form caption("File Button Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
label bounds(12, 36, 297, 19), text("Enter some text and hit enter"), align("left") fontcolour(84, 83, 83, 255)
texteditor bounds(12, 58, 152, 24), channel("filebutton1"), text("") value(0) file("/Users/walshr/sourcecode/cabbage/Examples/Widgets/Sliders.csd")
texteditor bounds(146, 140, 68, 127) identchannel("widgetIdent")
label bounds(192, 60, 192, 21), text(""), identchannel("labelIdent")

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
SFile chnget "filebutton1" 
    if changed(SFile)== 1 then
        printf "%s\n", k(1), SFile
        SMessage sprintfk "text(\"%s\") ", SFile
        chnset SMessage, "labelIdent"
    endif
endin

;WIDGET_ADVANCED_USAGE

                instr 2
                    if metro(1) == 1 then
                        event "i", "ChangeAttributes", 0, 1
                    endif
                endin

                instr ChangeAttributes
                    SIdentifier init ""
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

![](../images/texteditorExample.png)