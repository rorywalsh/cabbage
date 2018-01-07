<Cabbage>
form caption("ADSR Enveloper") size(400, 280), colour(40, 40, 40), pluginID("def1")
keyboard bounds(8, 170, 381, 95)

gentable bounds(5, 10, 380, 140), tablenumber(1), amprange(0, 1, 1, 0.0100), active(1), tablecolour:0(0, 200, 0, 100), identchannel("genTable1")
label bounds(50, 10, 15, 15), text("A"), colour(255, 255, 255, 255), identchannel("attIdent")
label bounds(100, 140, 15, 15), text("D"), colour(255, 255, 255, 255), identchannel("decIdent")
label bounds(180, 72, 15, 15), text("S"), colour(255, 255, 255, 255), identchannel("susIdent")
label bounds(280, 140, 15, 15), text("R"), colour(255, 255, 255, 255), identchannel("relIdent")

</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

gkAtt init .1
gkDec init .1
gkSus init .5
gkRel init .5

instr 1
iDurationScale = 2
print(i(gkRel))
aEnv madsr i(gkAtt)*iDurationScale, (1-i(gkDec))*iDurationScale, i(gkSus), (1-i(gkRel))*iDurationScale
a1 oscil aEnv, p4
outs a1, a1
endin

instr 10
;init size and position variables
iLabelSize init 15
iGtXPos init 5
iGtYPos init 10
iGtWidth init 380
iGtHeight init 140
kAPos init iGtWidth/8
kDPos init (iGtWidth/8)*2
kSPos init iGtWidth/2
kSLevel init iGtHeight/2
kRPos init (iGtWidth/8)*6

kX chnget "MOUSE_X"
kY chnget "MOUSE_Y"

kAttDragging init 0
kDecDragging init 0
kSusDragging init 0
kRelDragging init 0

;reposition labels with mouse click and drags
if chnget:k("MOUSE_DOWN_LEFT")==1 && kY< iGtHeight+iGtYPos then
    if kX > -2 && kX < kDPos && kDecDragging!=1 && kSusDragging!=1 && kRelDragging!=1 then
        kAPos = kX - iGtXPos 
        SMessage sprintfk "pos(%f, 5)", kAPos
        chnset SMessage, "attIdent"
        kAttDragging = 1
    elseif kX > kAPos+(iLabelSize/2) && kX < kSPos && kAttDragging!=1 && kSusDragging!=1 && kRelDragging!=1 then
        kDPos = kX - iGtXPos
        SMessage sprintfk "pos(%f, 140)", kDPos
        chnset SMessage, "decIdent"
        if kDPos > kSPos-iLabelSize then
            kSPos = kX + iGtXPos + iLabelSize/2
            SMessage sprintfk "pos(%f, %f)", kSPos, kSLevel  
            chnset SMessage, "susIdent"      
        endif
        kDecDragging = 1
    elseif kX > kDPos+(iLabelSize/2) && kX < kRPos && kAttDragging!=1 && kDecDragging!=1 && kRelDragging!=1 \
                                                                    && kY>iGtYPos-iLabelSize && kY <iGtHeight-iLabelSize/2 then
        kSPos = kX - iGtYPos
        kSLevel = kY + iGtYPos 
        SMessage sprintfk "pos(%f, %f)", kSPos, kSLevel
        chnset SMessage, "susIdent"
        kSusDragging = 1 
    elseif kX > kDPos+(iLabelSize/2) && kX < iGtXPos+iGtWidth && kAttDragging!=1 && kSusDragging!=1 && kDecDragging!=1 then
        kRPos = kX - iGtXPos
        SMessage sprintfk "pos(%f, 140)", kRPos
        chnset SMessage, "relIdent"
        if kRPos < kSPos then
            kSPos = kX - iGtYPos
            SMessage sprintfk "pos(%f, %f)", kSPos, kSLevel  
            chnset SMessage, "susIdent"      
        endif
        kRelDragging = 1
    endif       
else
kAttDragging = 0
kDecDragging = 0
kSusDragging = 0
kRelDragging = 0
endif

;update underlying table
if changed(kAPos, kDPos, kSPos, kSLevel, kRPos) == 1 then
    gkAtt limit (kAPos+iGtXPos)/iGtWidth, 0, 1
    gkDec limit (kDPos+iGtXPos)/iGtWidth, 0, 1
    gkSus limit 1-(kSLevel/iGtHeight), 0, 1
    gkRel limit (kRPos+iGtXPos)/iGtWidth, 0, 1 
;    printks "Att:%f\n", 0, kAtt
;    printks "Dec:%f\n", 0, kDec
;    printks "Sus:%f\n", 0, 1-kSus
;    printks "Rel:%f\n", 0, 1-kRel 
    event "i", "UpdateTable", 0, 0, gkAtt*4096, gkDec*4096-gkAtt*4096, gkSus, gkRel*4096
endif
endin

instr UpdateTable
    iAtt = p4
    iDec = p5
    iSus = p6
    iRel = p7
    gi1 ftgen 1, 0, 4096, 7, 0, iAtt, 1, iDec, iSus, iRel-iDec-iAtt, p6, 4096-iRel, 0
    chnset "tablenumber(1)", "genTable1"
endin


</CsInstruments>
<CsScore>
f1 0 4096 7 0 512 1 512 .5 2048 .5 1024 0
;causes Csound to run for about 7000 years...
f0 z
i10 0 z
</CsScore>
</CsoundSynthesizer>
