<Cabbage>
form caption("Texteditor Example") size(400, 300), colour(220, 220, 220), pluginid("TxE1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
groupbox bounds(8, 110, 380, 177), text("Randomly Updated Identifiers")
label bounds(12, 36, 297, 19), text("Enter some text and hit enter"), align("left") fontcolour(84, 83, 83, 255)
texteditor bounds(12, 58, 152, 24), channel("textEditor1"), text("") value(0) file("/Users/walshr/sourcecode/cabbage/Examples/Widgets/Sliders.csd")
texteditor bounds(146, 140, 68, 127) identchannel("widgetIdent")
label bounds(192, 60, 192, 21), text(""), identchannel("labelIdent")

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
SFile chnget "textEditor1" 
    if changed(SFile)== 1 then
        printf "%s\n", k(1), SFile
        SMessage sprintfk "text(\"%s\") ", SFile
        chnset SMessage, "labelIdent"
    endif
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
<!--End Widget Example)/-->
