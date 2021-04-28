# Keyboard

Keyboard will display a keyboard that will send MIDI information to your Csound instrument. This component can be used together with a hardware controller. Pressing keys on the actual MIDI keyboard will cause the on-screen keys to react up. 

<big></pre>
keyboard [keyseperatorcolour](#keyseparatorcolour)("colour"), 
[mouseoverkeycolour](#mouseoverkeycolour)("colour"), 
[keypressbaseoctave](#keypressbaseoctave)(val), 
[whitenotecolour](#whitenotecolour)("colour"), 
[blacknotecolour](#blacknotecolour)("colour"), 
[arrowbackgroundcolour](#arrowbackgroundcolour)("colour"), 
[keysdowncolour](#keydowncolour)("colour"), 
[arrowcolour](#arrowcolour)("colour"), 
[keywidth](#keywidth)(val), 
[middlec](#middlec)(val), 
[scrollbars](#scrollbars)(val), 
[value](#value_keyboard)(note), 
[bounds](#bounds)(x, y, width, height), 
[identchannel](#identchannel)("channel"), 
[tofront](#tofront)(), 
[visible](#visible)(val), 

</pre></big>

### Specific Identifiers

{! ./markdown/Widgets/Properties/keyseparatorcolour.md !} 

{! ./markdown/Widgets/Properties/mouseoverkeycolour.md !}  

{! ./markdown/Widgets/Properties/keypressbaseoctave.md !}  

{! ./markdown/Widgets/Properties/whitenotecolour.md !}  

{! ./markdown/Widgets/Properties/blacknotecolour.md !} 

{! ./markdown/Widgets/Properties/arrowbackgroundcolour.md !} 

{! ./markdown/Widgets/Properties/keydowncolour.md !} 

{! ./markdown/Widgets/Properties/arrowcolour.md !} 

{! ./markdown/Widgets/Properties/keywidth.md !} 

{! ./markdown/Widgets/Properties/middlec.md !}

{! ./markdown/Widgets/Properties/scrollbars.md !}

{! ./markdown/Widgets/Properties/value_keyboard.md !}

### Common Identifiers

{! ./markdown/Widgets/Properties/bounds.md !}  

{! ./markdown/Widgets/Properties/identchannel.md !}  

{! ./markdown/Widgets/Properties/tofront.md !} 

{! ./markdown/Widgets/Properties/visible.md !}  


<!--(End of identifiers)/-->

>The keyboard can be played at different velocities depending on where you click on the key with your mouse. Clicking at the top of the key will cause a quieter velocity while clicking on the bottom will cause the note to sound with full velocity. If you wish to play the keyboard like a nutjob in standalone mode, make sure to pass '-m0d' to the CsOptions(disable console messages). The keyboard widget is only provided as a quick and easy means of testing plugins in Cabbage. Treating it as anything more than that could result in severe disappointment!  

![](../images/keyboard.gif)

##Example
<!--(Widget Example)/-->
```csharp
<Cabbage>
form size(400, 300), caption("Keyboard"), pluginID("plu1")
keyboard bounds(10, 10, 385, 160), identchannel("widgetIdent")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key=4
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

;basic usage
instr 1 
a1 oscil 1, cpsmidinn(p4)
outs a1, a1
endin        

;WIDGET_ADVANCED_USAGE

instr 2
    if metro(1) == 1 then
        event "i", "ChangeAttributes", 0, 1
    endif
endin

instr ChangeAttributes
    SIdentifier init ""
	SIdent sprintf "keyseperatorcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "mouseoverkeycolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "keypressbaseoctave(%d) ", rnd(10)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "whitenotecolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "blacknotecolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "arrowbackgroundcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "keydowncolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "arrowcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "keywidth(%d) ", 20+rnd(20)
	SIdentifier strcat SIdentifier, SIdent
    ;send identifier string to Cabbage
    chnset SIdentifier, "widgetIdent"           
endin
                

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
i2 0 z
</CsScore>
</CsoundSynthesizer>
```
<!--(Widget Example)/-->
