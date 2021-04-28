<Cabbage>
form caption("Button Example") size(400, 300), colour(220, 220, 220), pluginID("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontColour("black")
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
optionbutton bounds(116, 38, 150, 50), channel("button1"), text("Sine", "Square", "Triangle", "Saw"),
button bounds(110, 140, 165, 62) identChannel("widgetIdent")
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
    if metro(1) == 1 then
        event "i", "ChangeAttributes", 0, 1
    endif
endin

instr ChangeAttributes
    SIdentifier init ""
	SIdent sprintf "alpha(%f) ", 50 + rnd(50)/50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "pos(%d, 140) ", 100 + rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "size(%d, %d) ", abs(rnd(200))+40, abs(rnd(100))+50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "caption(\"Text%d\") ", rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "colour:0(%d, 255, 255) ", rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "colour:1(%d, 0, 255) ", rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "fontColour:0(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "fontColour:1(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent   
	SIdent sprintf "outlineColour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "outlineColour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "outlineThickness(%f) ", rnd(100)/50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "text(\"TextOff %f\", \"TextOn %f\") ", rnd(100), rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "value(%f) ", rnd(1)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "visible(%d) ", (rnd(100) > 80 ? 0 : 1)
	SIdentifier strcat SIdentifier, SIdent
    ;send identifier string to Cabbage
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
