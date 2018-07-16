<Cabbage>
form size(400, 300), caption("Keyboard"), pluginid("Kbrd")
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
	SIdent sprintf "mouseoeverkeycolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "whitenotecolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "blacknotecolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "arrowbackgroundcolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
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
