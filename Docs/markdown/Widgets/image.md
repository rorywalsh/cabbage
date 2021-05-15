# Image

Image creates a static shape or graphic. It can be used to show pictures or it can be used to draw simple shapes. If you wish to display a picture you must pass the file name to the file() identifier. For convenience, the file passed to file() should be in the same directory as your Cabbage instrument.

<big></pre>
image WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/corners.md !}

{! ./markdown/Widgets/Properties/crop.md !}

{! ./markdown/Widgets/Properties/file_image.md !}

{! ./markdown/Widgets/Properties/outlineColour.md !}

{! ./markdown/Widgets/Properties/outlineThickness.md !}

{! ./markdown/Widgets/Properties/popup.md !} 

{! ./markdown/Widgets/Properties/shape.md !} . Note that by settings the corners to a high value one can also create elliptical shapes.  

**plant("name")** [DEPRECATED] Sets the name of the plant. No two plants can have the same name. See [Plants](./plants.md)

### Common Identifiers

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/alpha.md !}  

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/channel.md !}  A left-mouse click on an image will send a 1 to the channel passed to channel(). Subsequent clicks will toggle between 0 and 1.

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/identChannel.md !}  

{! ./markdown/Widgets/Properties/rotate.md !}  

{! ./markdown/Widgets/Properties/visible.md !}  

{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/widgetArray.md !}  

<!--(End of identifiers)/-->

![](../images/image.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Image Example") size(380,440), guiMode("queue"), colour(2, 145, 209) pluginId("def1")
texteditor bounds(18, 280, 341, 150) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)

image bounds(72, 22, 234, 234) channel("image1"), file("cabbage.png")
image bounds(42, 26, 280, 190) channel("image2"), file("headphones.png")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 16
nchnls = 2
0dbfs = 1


; Rory Walsh 2021 
;
; License: CC0 1.0 Universal
; You can copy, modify, and distribute this file, 
; even for commercial purposes, all without asking permission. 

instr 99

    SText  = "An image widget can be used to display images. PNGs, JPEGSs and basic SVGs are supported. As with all Cabbage widgets, an image widget's attributes can be updated in real time by sending updated identifiers data to its channel. Images can also as as buttons, where a mouse click will toggle between 0 and 1.\n\nIn this example the RMS value from a simple loop is used to drive the size of the Cabbage."  
    cabbageSet "infoText", "text", SText
    
endin

instr 1    
    a1, a2 diskin2 "beat.ogg", 1, 0, 1
    outs a1, a2
    
    kRms rms a1+a2, 20
    cabbageSet metro(10), "image1", "bounds", 72, 22, 234+(kRms*10), 234+(kRms*10)

endin
              


</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
i99 0 0
i1 1 z
</CsScore>
</CsoundSynthesizer>

```
<!--(End Widget Example)/-->
