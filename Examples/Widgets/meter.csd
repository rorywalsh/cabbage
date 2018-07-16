<Cabbage>
form caption("Meter example") size(400, 300), colour(220, 220, 220), pluginid("Mtr1")
label bounds(8, 6, 368, 20), text("Basic Usage"), fontcolour("black")
vmeter bounds(116, 32, 35, 80) channel("vMeter1") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(2) 
vmeter bounds(156, 32, 35, 80) channel("vMeter2") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(2) 
vmeter bounds(196, 32, 35, 80) channel("vMeter3") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(2) 
vmeter bounds(236, 32, 35, 80) channel("vMeter4") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(2)
groupbox bounds(8, 118, 380, 177), text("Randomly Updated Identifiers")
vmeter bounds(142, 140, 46, 145), channel("meterTest") identchannel("widgetIdent")
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
    if metro(20) == 1 then
        chnset abs:k(randi:k(1.1, 100, 2)), "vMeter1"
        chnset abs:k(randi:k(1.1, 100, 2)), "vMeter2"
        chnset abs:k(randi:k(1.1, 100, 2)), "vMeter3"
        chnset abs:k(randi:k(1.1, 100, 2)), "vMeter4"
        chnset abs:k(randi:k(1.1, 100, 2)), "meterTest"
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
	SIdent sprintf "corners(%d) ", rnd(100)/80
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "outlinecolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "outlinethickness(%f) ", rnd(100)/50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "overlaycolour(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "metercolour:0(%d, %d, %d) ", rnd(255), rnd(255), rnd(255)
	SIdentifier strcat SIdentifier, SIdent  
	SIdent sprintf "alpha(%f) ", 50 + rnd(50)/50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "pos(%d, 140) ", 100 + rnd(100)
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "size(%d, %d) ", abs(rnd(200))+40, abs(rnd(100))+50
	SIdentifier strcat SIdentifier, SIdent
	SIdent sprintf "visible(%d) ", (rnd(100) > 80 ? 0 : 1)
	SIdentifier strcat SIdentifier, SIdent
    ;send identifier string to Cabbage
    chnset SIdentifier, "widgetIdent"           
endin
                

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
i1 0 z
i2 0 z
</CsScore>
</CsoundSynthesizer>
