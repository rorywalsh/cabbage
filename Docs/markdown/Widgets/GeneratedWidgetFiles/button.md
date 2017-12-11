# Button

Button creates a button on screen that can be used for a whole range of different tasks. The "channel" string identifies the channel on which Cabbage will communicate with Csound. "OnCaption" and "OffCaption" determine the strings that will appear on the button as users toggle between two states, i.e., 0 or 1. By default these captions are set to "On" and "Off" but the user can specify any strings they wish. 

<big></pre>
button [latched](#latched)(val), 
[active](#active)(val), 
[alpha](#alpha)(val), 
[bounds](#bounds)(x, y, width, height), 
[caption](#caption)("string"), 
[channel](#channel)("chan"), 
[colour:0](#colour_0)("colour"), 
[colour:1](#colour_1)("colour"), 
[fontcolour:0](#fontcolour_0)("colour"), 
[fontcolour:1](#fontcolour_1)("colour"), 
[identchannel](#identchannel)("channel"), 
[popup](#popup)(val), 
[rotate](#rotate)(radians, pivotx, pivoty), 
[svgfile](#svgfile)("type", "file"), 
[text](#text)("name"), 
[value](#value)(val), 
[visible](#visible)(val), 
[widgetarray](#widgetarray)("chan", number), 

</pre></big>

<!--(End of syntax)/-->

##Identifiers

### Specific Identifiers

{! ./markdown/Widgets/Properties/latched.md !} 

### Common Identifiers
{! ./markdown/Widgets/Properties/active.md !}

{! ./markdown/Widgets/Properties/alpha.md !} 

{! ./markdown/Widgets/Properties/bounds.md !} 

{! ./markdown/Widgets/Properties/caption.md !} 

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

{! ./markdown/Widgets/Properties/value.md !} 

{! ./markdown/Widgets/Properties/visible.md !} 

{! ./markdown/Widgets/Properties/widgetarray.md !} 

<!--(End of identifiers)/-->

>colour:1() and fontcolour:1() can be set using colour() and fontcolour(). However, it's recommended that you use the numerated colour identifiers in order to make your code as readable a possible. 

##Example
```csharp
<Cabbage>
form caption("Button Example") size(400, 300), colour(20, 20, 20), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage")
groupbox bounds(8, 110, 367, 177), text("Identifiers Updates")
button bounds(116, 38, 150, 50), channel("button1"), text("Enable Tone", "Disable Tone"),
button bounds(20, 140, 71, 40), channel("changeAttributes"), text("Change Attributes", "Change Attributes") 
button bounds(110, 140, 165, 62) identchannel("buttonIdent")
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

                instr 2
                    kUpdate init 0
                    iCnt init 0
                    if changed:k(chnget:k("changeAttributes")) == 1 then
                        event "i", "ChangeAttributes", 0, 1, kUpdate
                        kUpdate = (kUpdate < 6 ? kUpdate+1 : 0)
                    endif
                endin

                instr ChangeAttributes
                    SIdentifier init ""
    SIdent sprintf "pos(%d, 140) ", 100 + rnd(100)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "size(%d, %d) ", abs(rnd(200))+40, abs(rnd(100)+50)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "colour:0(%d, 255, 255) ", rnd(255)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "colour:1(%d, 0, 255) ", rnd(255)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "fontcolour:0(%d, 255, 0) ", rnd(255)
    SIdentifier strcat SIdentifier, SIdent
    SIdent sprintf "fontcolour:1(%d, %d, 255) ", rnd(255), rnd(255)
    SIdentifier strcat SIdentifier, SIdent   
    SIdent sprintf "text(\"TextOff %f\", \"TextOn %\") ", rnd(100), rnd(100)
    SIdentifier strcat SIdentifier, SIdent

                    chnset SIdentifier, "buttonIdent"     
                    prints SIdentifier         
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

![Button](../images/buttonExample.png)