# Group Box

Groupbox creates a container for other GUI widgets. They do not communicate with Csound but can be useful for organising widgets into panels.

<big></pre>
groupbox WIDGET_SYNTAX
</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/lineThickness.md !}

{! ./markdown/Widgets/Properties/outlineThickness.md !}  

{! ./markdown/Widgets/Properties/text.md !}

{! ./markdown/Widgets/Properties/popup.md !} 

**plant("name")** [DEPRECATED] Sets the name of the plant. No two plants can have the same name. See [Plants](./plants.md)

**popup(val)** [DEPRECATED] If the groupbox is a plant, a value of 1 will cause the groupbox to open in a new window when show(1) is called. popup() should always be used in conjunction with the show() identifier. See [Plants](./plants.md)

**show(val)** [DEPRECATED] A value of 1 will cause the popup plant to show. A value of 0 will close the popup plant. See [Plants](./plants.md)

### Common Identifiers

{! ./markdown/Widgets/Properties/alpha.md !}  

{! ./markdown/Widgets/Properties/active.md !}  

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/colour.md !}  

{! ./markdown/Widgets/Properties/fontColour.md !}  

{! ./markdown/Widgets/Properties/channel.md !} 

{! ./markdown/Widgets/Properties/identChannel.md !}  

{! ./markdown/Widgets/Properties/imgFile.md !} 

{! ./markdown/Widgets/Properties/rotate.md !}  

{! ./markdown/Widgets/Properties/parent.md !} 

{! ./markdown/Widgets/Properties/visible.md !}  

{! ./markdown/Widgets/Properties/toFront.md !} 

{! ./markdown/Widgets/Properties/moveBehind.md !} 

{! ./markdown/Widgets/Properties/widgetArray.md !}  

<!--(End of identifiers)/-->

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form caption("Groupbox Example") size(380, 440), guiMode("queue"), colour(2, 145, 209) pluginId("def1")
groupbox bounds(6, 8, 368, 131) channel("group1"), fontColour(220, 220, 220), outlineColour(255, 255, 255), text("ADSR Envelope")colour(0, 0, 0, 0)
rslider bounds(16, 38, 86, 90), channel("att"), range(0, 1, 0.01, 1, 0.001), text("Att.")
rslider bounds(102, 38, 86, 90), channel("dec"), range(0, 1, 0.4, 1, 0.001), text("Dec.")
rslider bounds(190, 38, 86, 90), channel("sus"), range(0, 1, 0.7, 1, 0.001), text("Sus.")
rslider bounds(278, 38, 86, 90), channel("rel"), range(0, 1, 0.8, 1, 0.001), text("Rel.")

texteditor bounds(6, 252, 368, 175) channel("infoText"), readOnly(1), wrap(1), scrollbars(1)
keyboard bounds(6, 154, 368, 80) channel("keyboard")

</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key=4 --midi-velocity-amp=5
</CsOptions>e
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1


; Rory Walsh 2021 
;
; License: CC0 1.0 Universal
; You can copy, modify, and distribute this work, 
; even for commercial purposes, all without asking permission. 

instr 1

    SText  = "A groupbox is a basic container for other widgets. It can be part the top-level component in a group (i.e, plant), but in most case is static.\n\nIt's Z index will determine if it sits in front or behind other widgets. Therefore it should b declared before any of the widgets that fit on it\n\nIt does not send any information to Csound, but you can control aspects of a groupbox from Csound by sending identifier data to its channel."
    cabbageSet "infoText", "text", SText

endin
                

</CsInstruments>
<CsScore>
;starts instrument 1 and runs it for a week
i1 0 z
</CsScore>
</CsoundSynthesizer>

```
<!--(End Widget Example)/-->

