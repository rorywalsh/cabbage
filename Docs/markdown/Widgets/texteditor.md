# Text Editor

Texteditor creates a text editor that can be used to send strings to Csound. Hitting return when in single line mode will send the string to Csound on a named string channel while pressing the up and down buttons when the texteditor is in focus will toggle through the previous strings that have been sent. When in "multiline" mode, press command and return to send the string data to Csound. 

<big></pre>
texteditor WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/wrap.md !} 

{! ./markdown/Widgets/Properties/readOnly.md !} 

{! ./markdown/Widgets/Properties/doubleClickTogglesEdit.md !} 

{! ./markdown/Widgets/Properties/caretColour.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !} 

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/bounds.md !} 
 
{! ./markdown/Widgets/Properties/channel.md !} 

{! ./markdown/Widgets/Properties/colour.md !} 

{! ./markdown/Widgets/Properties/fontColour.md !}   

{! ./markdown/Widgets/Properties/identChannel.md !} 

{! ./markdown/Widgets/Properties/popup.md !} 

{! ./markdown/Widgets/Properties/rotate.md !} 

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/widgetArray.md !}  

<!--(End of identifiers)/-->

![](../images/texteditor.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Texteditor Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontColour("black")
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
label bounds(12, 36, 297, 19), text("Enter some text and hit enter"), align("left") fontColour(84, 83, 83, 255)
texteditor bounds(12, 58, 152, 24), channel("textEditor1"), text("") value(0) file("/Users/walshr/sourcecode/cabbage/Examples/Widgets/Sliders.csd")
texteditor bounds(146, 140, 68, 127) identChannel("widgetIdent")
label bounds(192, 60, 192, 21), text(""), identChannel("labelIdent")

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
SFile chnget "textEditor1" 
    if changed(SFile)== 1 then
        printf "%s\n", k(1), SFile
        SMessage sprintfk "text(\"%s\") ", SFile
        chnset SMessage, "labelIdent"
    endif
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
<!--End Widget Example)/-->
