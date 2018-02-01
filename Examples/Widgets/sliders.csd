<Cabbage>
form caption("Slider Example") size(400, 300), colour(220, 220, 220), pluginid("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
hslider bounds(8, 38, 369, 50), channel("gain"), text("Gain") range(0, 1, 0, 1, 0.001) fontcolour(91, 46, 46, 255) textcolour(29, 29, 29, 255)
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
rslider bounds(70, 140, 41, 119) channel("rsliderChannel"), identchannel("widgetIdent"), range(0, 1, 0, 1, 0.001) 
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
    aTone oscili chnget:k("gain"), 300
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
	SIdent sprintf "outlinecolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "popuptext(\"Popup Text %d\") ", rnd(1000)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "textcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "text(\"TextOff %f\") ", rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "trackerthickness(%f) ", rnd(10)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "trackerinsideradius(%f) ", rnd(10)/10
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "trackeroutsideradius(%f) ", rnd(10)/10
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "textboxoutlinecolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "textboxcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "trackercolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "alpha(%f) ", 50 + rnd(50)/50
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
