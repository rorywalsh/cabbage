# Button (File button)

A filebutton can be used to select files from disk. The filename selected, or created will be sent to Csound on the named channel as a string message. Filebuttons can also be used to select snapshot files for Cabbage to save parameter presets. See the populate identifier below. 

<big></pre>
filebutton WIDGET_SYNTAX
</pre></big>

<!--(End of syntax)/-->

##Identifiers

### Specific Identifiers

{! ./markdown/Widgets/Properties/file_populate.md !} 

{! ./markdown/Widgets/Properties/mode.md !} 

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !} 

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/bounds.md !} 

{! ./markdown/Widgets/Properties/channel.md !} 

{! ./markdown/Widgets/Properties/colour_0.md !} 

{! ./markdown/Widgets/Properties/colour_1.md !} 

{! ./markdown/Widgets/Properties/fontcolour_0.md !} 

{! ./markdown/Widgets/Properties/fontcolour_1.md !} 

{! ./markdown/Widgets/Properties/identchannel.md !} 

{! ./markdown/Widgets/Properties/popup.md !} 

{! ./markdown/Widgets/Properties/rotate.md !} 

{! ./markdown/Widgets/Properties/svgfile.md !} 

{! ./markdown/Widgets/Properties/text.md !} 

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/widgetarray.md !} 

<!--(End of identifiers)/-->
>colour:1() and fontcolour:1() can be set using colour() and fontcolour(). However, it's recommended that you use the numerated colour identifiers in order to make your code as readable a possible. 

##Example
```csharp
<Cabbage>
form caption("File Button Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
filebutton bounds(108, 30, 150, 50), channel("filebutton1"), text("Browsse", "Browsse") value(0) file("/Users/walshr/sourcecode/cabbage/Examples/Widgets/Sliders.csd")
button bounds(146, 140, 68, 127) identchannel("widgetIdent")
texteditor bounds(10, 84, 379, 21), text(""), identchannel("editorIdent")
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
        SMessage sprintfk "text(\"Selected File:%s\") ", SFile
        chnset SMessage, "editorIdent"
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


![](../images/buttonExample.png)