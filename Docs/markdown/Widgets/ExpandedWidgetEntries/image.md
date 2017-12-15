# Image

Image creates a static shape or graphic. It can be used to show pictures or it can be used to draw simple shapes. If you wish to display a picture you must pass the file name to the file() identifier. For convenience, the file passed to file() should be in the same directory as your Cabbage instrument.

<big></pre>
image [corners](#corners)(val), 
[file](#file_image)("filename"), 
[outlinecolour](#outlinecolour)("colour"), 
[outlinethickness](#outlinethickness)(value), 
[popup](#popup)(val), 
[shape](#shape)("shape"), 
[active](#active)(val), 
[alpha](#alpha)(val), 
[bounds](#bounds)(x, y, width, height), 
[channel](#channel)("chan"), 
[colour](#colour)("colour"), 
[identchannel](#identchannel)("channel"), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[visible](#visible)(val), 
[widgetarray](#widgetarray)("chan", number), 

</pre></big>

<!--(End of syntax)/-->
##Identifiers

### Specific Identifiers

{! ./markdown/Widgets/Properties/corners.md !}

{! ./markdown/Widgets/Properties/file_image.md !}

{! ./markdown/Widgets/Properties/outlinecolour.md !}

{! ./markdown/Widgets/Properties/outlinethickness.md !}

{! ./markdown/Widgets/Properties/popup.md !} 

{! ./markdown/Widgets/Properties/shape.md !} . Note that by settings the corners to a high value one can also create elliptical shapes.  

**plant("name")** [DEPRECATED] Sets the name of the plant. No two plants can have the same name. See [Plants](./plants.md)

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/alpha.md !}  

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/channel.md !}  A left-mouse click on an image will send a 1 to the channel passed to channel(). Subsequent clicks will toggle between 0 and 1.

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/identchannel.md !}  

{! ./markdown/Widgets/Properties/rotate.md !}  

{! ./markdown/Widgets/Properties/visible.md !}  

{! ./markdown/Widgets/Properties/widgetarray.md !}  

<!--(End of identifiers)/-->

##Example
```csharp
<Cabbage>
form caption("Image Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
label bounds(6, 26, 380, 18) text("Image: click to send info to Csound") fontcolour(118, 118, 118, 255)
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")

image bounds(128, 46, 122, 57), channel("image1"), colour(123, 34, 143), corners(40)
image bounds(110, 140, 165, 62) identchannel("widgetIdent")
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
    aTone oscili chnget:k("image1"), 300
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
    SIdent sprintf "outlinecolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent  
    SIdent sprintf "outlinethickness(%f) ", rnd(100)/50
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "alpha(%f) ", rnd(100)/100
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "pos(%d, 140) ", 100 + rnd(100)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "size(%d, %d) ", abs(rnd(200))+40, abs(rnd(100))+50
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "colour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
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

![](../images/imageExample.png)
