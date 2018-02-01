<Cabbage>
form caption("HRange Example") size(400, 300), colour(220, 220, 220), pluginid("def1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
hrange bounds(120, 34, 160, 40), channel("hrangeL", "hrangeR"), colour(123, 34, 143), range(100, 1000, 200:300, 1, .01)
hrange bounds(110, 140, 162, 32), channel("range1", "range2"), identchannel("widgetIdent"), channel colour(123, 34, 143), corners(40), range(0, 1, .24:.75, 1 0.001)
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
    aTone1 oscili .25, chnget:k("hrangeL")
    aTone2 oscili .25, chnget:k("hrangeR")
    outs aTone1+aTone2, aTone1+aTone2    
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
	SIdent sprintf "text(\"TextOff %f\", \"TextOn %f\") ", rnd(100), rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "textcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "trackerthickness(%f) ", rnd(10)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "trackercolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "alpha(%f) ", 50 + rnd(50)/50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "pos(%d, 140) ", 100 + rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "size(%d, %d) ", abs(rnd(200))+40, abs(rnd(100))+50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "caption(\"Text%d\") ", rnd(100)
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

