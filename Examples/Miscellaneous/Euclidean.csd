<Cabbage>
form caption("Euclidean Rhythms") size(740, 420), guiMode("queue"), pluginId("def1")

image bounds(188, 16, 347, 354) channel("imageBack"), colour(34, 34, 34), corners(10) outlineColour("white") outlineThickness(5)
;controls for clock 1
rslider bounds(82, 32, 60, 60) range(1, 16, 1, 1, 1), text("Hits"), channel("hits1") colour(125, 0, 255, 255), textColour("white")
rslider bounds(16, 32, 60, 60) range(1, 16, 16, 1, 1), text("Beats"), channel("beats1") colour(125, 0, 255, 255), textColour("white")
checkbox bounds(150, 34, 20, 19), channel("enable1"), value(0), fontColour("white")
nslider bounds(568, 56, 62, 22) range(36, 96, 48, 1, 1), channel("note1") colour(125, 0, 255, 255) velocity(50), fontColour("white")
rslider bounds(662, 32, 60, 60) range(1, 16, 1, 1, 1), text("Rotation"), channel("patternRotate1") colour(125, 0, 255, 255), fontColour("white")
label bounds(558, 36, 79, 14), text("Midi Note"), fontColour("white")
image bounds(14, 110, 130, 2)
image bounds(555, 110, 170, 2)

;controls for clock 2
rslider bounds(82, 132, 60, 60) range(1, 16, 2, 1, 1), text("Hits"), fontColour("white"), channel("hits2") colour(225, 0, 255, 255) textColour("white")
rslider bounds(16, 132, 60, 60) range(1, 16, 16, 1, 1), text("Beats"), , fontColour("white")channel("beats2") colour(125, 0, 255, 255) textColour("white")
checkbox bounds(150, 134, 20, 19) , fontColour("white"), channel("enable2")
nslider bounds(568, 156, 62, 22) range(36, 96, 60, 1, 1), channel("note2") , fontColour("white")colour(125, 0, 255, 255) velocity(50)
rslider bounds(662, 132, 60, 60) range(1, 16, 1, 1, 1), text("Rotation"), , fontColour("white")channel("patternRotate2") colour(125, 0, 255, 255)
label bounds(558, 136, 79, 14), text("Midi Note"), fontColour("white")
image bounds(14, 210, 130, 2)
image bounds(555, 210, 170, 2)

;controls for clock 3
rslider bounds(82, 232, 60, 60) range(1, 16, 3, 1, 1), text("Hits"), channel("hits3") colour(225, 0, 255, 255), textColour("white")
rslider bounds(16, 232, 60, 60) range(1, 16, 16, 1, 1), text("Beats"), channel("beats3") colour(125, 0, 255, 255), textColour("white")
checkbox bounds(150, 234, 20, 19), channel("enable3") fontColour("white")
nslider bounds(568, 256, 62, 22) range(36, 96, 67, 1, 1), channel("note3") colour(125, 0, 255, 255) velocity(50)
rslider bounds(662, 232, 60, 60) range(1, 16, 1, 1, 1), text("Rotation"), channel("patternRotate3") colour(125, 0, 255, 255)
label bounds(558, 236, 79, 14), text("Midi Note")
image bounds(14, 310, 130, 2)
image bounds(555, 310, 170, 2)

;controls for clock 4
rslider bounds(82, 332, 60, 60) range(1, 16, 4, 1, 1), text("Hits"), channel("hits4") colour(225, 0, 255, 255), textColour("white")
rslider bounds(16, 332, 60, 60) range(1, 16, 16, 1, 1), text("Beats"), channel("beats4") colour(125, 0, 255, 255), textColour("white")
checkbox bounds(150, 334, 20, 19), channel("enable4"), fontColour("white")
nslider bounds(568, 356, 62, 22) range(36, 96, 72, 1, 1), channel("note4") colour(125, 0, 255, 255) velocity(50)
rslider bounds(662, 332, 60, 60) range(1, 16, 1, 1, 1), text("Rotation"), channel("patternRotate4") colour(125, 0, 255, 255)
label bounds(558, 336, 79, 14), text("Midi Note"), fontColour("white")


hslider bounds(228, 374, 186, 30) range(1, 20, 8, 1, 1), channel("tempo"), text("Tempo")
button bounds(422, 374, 83, 30), channel("updateChord"), text("Update Notes"), corners(5)

</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

; Rory Walsh, 2021


;=====================================================
;; Iterative Euclidean Beat Generator
;; Returns string of 1 and 0's
;; Steven Yi - https://github.com/kunstmusik/csound-live-code/blob/master/livecode.orc#L419-L457 
;=====================================================
opcode euclid_str, S, ii
  ihits, isteps xin

  Sleft = "1"
  Sright = "0"

  ileft = ihits
  iright = isteps - ileft

  while iright > 1 do
    if (iright > ileft) then
      iright = iright - ileft 
      Sleft = strcat(Sleft, Sright)
    else
      itemp = iright
      iright = ileft - iright
      ileft = itemp 
      Stemp = Sleft
      Sleft = strcat(Sleft, Sright)
      Sright = Stemp
    endif
  od

  Sout = ""
  indx = 0 
  while (indx < ileft) do
    Sout = strcat(Sout, Sleft)
    indx += 1
  od
  indx = 0 
  while (indx < iright) do
    Sout = strcat(Sout, Sright)
    indx += 1
  od

  xout Sout
endop

opcode RotatePattern, S,Si
    SPattern, iStart xin
    SNewPattern = ""
    iCnt init 0

    while iCnt < 16 do
        SNewPattern strcat SNewPattern, strsub(SPattern, (iCnt+iStart)%16, ((iCnt+iStart)%16)+1)
        iCnt += 1
    od
    xout SNewPattern
endop

;=====================================================
; Simple UDO to caluclate hit positions
;=====================================================
opcode GetPositionForPoint, ii,iiiii
    iX, iY, iRadius, iBeat, iPoints xin
    iSlice = ($M_PI*2)/(iPoints)
    iAngle = iSlice*iBeat
    iXPoint = iX + iRadius * cos(iAngle)
    iYPoint = iY + iRadius * sin(iAngle)
    xout iXPoint, iYPoint
endop

;=====================================================
; Simple UDO to enable disable UIs for each pattern
;=====================================================
opcode ShowHideCircles, 0,iiii
    iVoice, iShow, iStart, iEnd xin
    
    while iStart < iEnd do
        SCircleChannel sprintf "voice%dCircle%d", iVoice, iStart
        cabbageSet SCircleChannel, sprintf("visible(%d)", iShow)
        iStart += 1
    od
endop

;=====================================================
; Simple UDO to enable disable UIs for each pattern
;=====================================================
opcode HideClock, 0,iiii
    iVoice, iShow, iStart, iEnd xin
    
    while iStart < iEnd do
        SCircleChannel sprintf "voice%dCircle%d", iVoice, iStart
        cabbageSet SCircleChannel, sprintf("visible(%d)", iShow)
        iStart += 1
    od
endop
;=====================================================
; Geneates Euchlidean clocks
;=====================================================
instr GenerateCirclePoints

    iVoice = p4
    iCircleCentreX = p5
    iCircleCentreY = p6
    iCircleRadius = p7
    iR = p8
    iG = p9
    iB = p10

    iPoints init 16
    iCnt init 0
    iSlice = ($M_PI*2)/(iPoints)
    
    ;create arm
    SArmString = sprintf("bounds(%d, %d, %d, 2), channel(\"%s\") colour(%d, %d, %d)", iCircleCentreX, iCircleCentreY+1, iCircleRadius-25, sprintf("voice%dArm", iVoice), iR, iG, iB) 
    cabbageCreate "image", SArmString

    while iCnt < 16 do
        iXPoint, iYPoint GetPositionForPoint iCircleCentreX, iCircleCentreY, iCircleRadius, iCnt, iPoints
        SChannel sprintf "voice%dCircle%d", iVoice, iCnt
        SCircleString = sprintf("bounds(%d, %d, 5, 5), shape(\"circle\"), channel(\"%s\")", iXPoint, iYPoint, SChannel)
        cabbageCreate "image", SCircleString
        iCnt += 1
    od
    
    ;theme existing sliders
    cabbageSet sprintf("hits%d", iVoice), sprintf("colour(%d, %d, %d)", iR, iG, iB)
    cabbageSet sprintf("beats%d", iVoice), sprintf("colour(%d, %d, %d)", iR, iG, iB)
    cabbageSet sprintf("note%d", iVoice), sprintf("colour(%d, %d, %d)", iR, iG, iB)
    cabbageSet sprintf("rotate%d", iVoice), sprintf("colour(%d, %d, %d)", iR, iG, iB)
endin

;=====================================================
; Sequencer for the main voices
;=====================================================
instr Voice
    iNotes[] fillarray 48, 60, 72, 64, 65, 36
    iVoice = p4
    iRadius = p5
    iR = p6
    iG = p7
    iB = p8
    event_i "i", "GenerateCirclePoints", 0, 1, iVoice, 360, 200, iRadius, iR, iG, iB
    
    kBeatIndex init 0
    kRotate init 0 
    
    kTempo chnget "tempo"
    
    SHitsChannel sprintf "hits%d", iVoice
    kHits = chnget:k(SHitsChannel)

    SBeatsChannel sprintf "beats%d", iVoice
    kBeats = chnget:k(SBeatsChannel)
    
    SEnableChannel sprintf "enable%d", iVoice
    kEnable = chnget:k(SEnableChannel)

       
    SPatternRotateChannel sprintf "patternRotate%d", iVoice
    kRotatePattern = chnget:k(SPatternRotateChannel)
       
    SNoteChannel sprintf "note%d", iVoice
    kNote = chnget:k(SNoteChannel)   
    
    kInterval = int(kBeats / kHits)
    
    
    ;logic for making sure sliders don't overlap
    if kHits > kBeats then        
        chnset kHits, SBeatsChannel
    endif
    if kBeats < kHits then
        chnset kBeats, SHitsChannel
    endif
       
    if changed(kHits, kBeats, kRotatePattern) == 1 then
        event "i", "GenerateRyhthm", 0, 0, kHits, kBeats, iVoice, 360, 200, iRadius, iR, iG, iB, kRotatePattern
    endif
    
    kUpdateNote chnget "updateChord"
    if changed(kUpdateNote) == 1 then
        chnset iNotes[int(random:k(0, 6))], SNoteChannel
        ;printk 0, kNotes[int(random:k(0, 6))]
    endif
    
    
    if metro(kTempo) == 1 then
        if tab:k(kBeatIndex, iVoice) == 1 && kEnable == 1 then
            event "i", "Synth", 0, 1, kNote
        endif
        
        kBeatIndex = (kBeatIndex < kBeats - 1 ? kBeatIndex+1 : 0) 
        
    endif
    
    if changed(kEnable) == 1 then
        if kEnable == 1 then
            event "i", "GenerateRyhthm", 0, 0, kHits, kBeats, iVoice, 360, 200, iRadius, iR, iG, iB, kRotatePattern
        else
            event "i", "HideClock", 0, 0, iVoice
        endif
    endif
    
    SArmChannel sprintf "voice%dArm", iVoice
    if kEnable == 1 then
        cabbageSet 1, SArmChannel, "rotate", ($M_PI*2)*kRotate, 0, 0
    endif
    kRotate = kRotate < 1 ? kRotate + ((ksmps/sr)*kTempo/kBeats) : 0
 
endin

;=====================================================
; Rhythm generator
; - fills tables for each voice
; - updates Euchlidean clocks
;=====================================================
instr GenerateRyhthm
    iVoice = p6
    iBeats = p5
    iHits = p4
    iX = p7
    iY = p8
    iRadius = p9
    iR = p10
    iG = p11
    iB = p12
    iPatternRotate = p13
    SRhythm euclid_str iHits, iBeats
    SRhythm RotatePattern SRhythm, iPatternRotate

    iBeat init 0
    while iBeat < iBeats do
        SValue = strsub(SRhythm, iBeat, iBeat+1)
        iActive = ( strcmp:i(SValue, "1") == 0 ? 1 : 0)
        iSize = (iActive == 1 ? 20 : 5)
        tablew iActive, iBeat, iVoice 
        SCircleChannel sprintf "voice%dCircle%d", iVoice, iBeat
        iXPoint, iYPoint GetPositionForPoint iX, iY, iRadius, iBeat, iBeats
        SMessage sprintf "bounds(%d, %d, %d, %d), visible(1), colour(123, 0, 255), %s, shape(\"circle\")", iXPoint-iSize/2, iYPoint-iSize/2, iSize, iSize, sprintf("colour(%d, %d, %d)", iR, iG, iB)
        cabbageSet SCircleChannel, SMessage
        iBeat += 1
    od 
    
    SArmChannel sprintf "voice%dArm", iVoice
    cabbageSet SArmChannel, "visible(1)"
    ShowHideCircles iVoice, 0, iBeat, 16

endin

instr HideClock
    iStart init 0
    iVoice = p4
    while iStart < 16 do
        SCircleChannel sprintf "voice%dCircle%d", iVoice, iStart
        cabbageSet SCircleChannel, "visible(0)"
        iStart += 1
    od
    
    SArmChannel sprintf "voice%dArm", iVoice
    cabbageSet SArmChannel, "visible(0)"
    
endin

;=====================================================
; Extremely simple sine wave synth - each clock 
; drives one of these
;=====================================================
instr Synth
    a1 expseg 0.001, 0.01, 1, p3-.01, 0.001
    a2 oscili a1, cpsmidinn(p4), 99
    outs a2, a2
endin

</CsInstruments>
<CsScore>
f99 0 4096 10 1 .45 .17
f1 0 16 2 1 0
f2 0 16 2 1 0
f3 0 16 2 1 0
f4 0 16 2 1 0
f5 0 16 2 1 0
f6 0 16 2 1 0
f7 0 16 2 1 0
f8 0 16 2 1 0



;start a new voice instrument for each note
i"Voice" 0 [60*60*24*7] 1 150 24 224 128
i"Voice" 0 [60*60*24*7] 2 120 0 125 255
i"Voice" 0 [60*60*24*7] 3 90 200 100 220
i"Voice" 0 [60*60*24*7] 4 60 40 75 100

i"HideClock" .1 0 1
i"HideClock" .1 0 2
i"HideClock" .1 0 3
i"HideClock" .1 0 4
</CsScore>
</CsoundSynthesizer>
