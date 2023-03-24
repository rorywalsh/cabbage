
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Spiccato.csd
; Written by Iain McCurdy, 2016

; This instrument emulates the 'spiccato' playing technique on bowed instruments through careful use of its 'bow pressure' envelope,
;  however a range of other effects are possible.
; This instrument is based on the wgbow opcode which is itself based on Perry Cook's waveguide-based algorithm for a bowed string 

; CONTROLS
; --------
; B O W   P R E S S U R E
; Attack    -    time taken for bow pressure to reach the level defined by 'Att.Lev'
; Att.Lev.    -    the level of bow pressure attained after 'Attack' seconds
; Decay        -    time taken to reach the bow pressure level defined by 'Sustain'
; Sustain    -    the bow pressure level that is sustained while a note is being held after the attack and decay portions are completed
; Release    -    time taken for bow pressure to return to minimum after a note is released

; B O W   P O S I T I O N
; Bow.Pos.    -    position of the bow along the length of the string
; Rand.Offset    -    random offset of bow position. (Issued just at the beginning of each note.)
; Mod.Amp.    -    amplitude of random spline variation of bow position
; Mod.Freq.    -    frequency of random spline variation of bow position

; K E Y B O A R D
; (NB For normal keyboard behaviour set 'Scale' to '1' and 'Shift' to 0)
; Scale        -    Scale (multiply) all note numbers
; Shift        -    Shift (add this value to) all note numbers
;    These controls can be used to create microtonal pitch clusters

; V I B R A T O
; Depth        -    Depth of the vibrato
; Rate        -    Rate of the vibrato
; Rise        -    Time taken to rise to maximum vibrato depth as defined by 'Depth'
; Dereg.    -    Dergulation factor of vibrato depth and vibrato amplitude. This control can be used to humanise the vibrato or, with higher values, as a special effect.

; P O L Y P H O N Y
; Legato    -    Switches between normal unrestricted polyphony playing and mono-legato
; Mode(drop-down menu)    -    Fixed Rate: constant rate of change in interval during note to note portamento
;            -    Fixed Time: all portamento glides last for the same amount of time, regardless of the interval covered 
; Time            -    Portamento time. Behaviour differs slightly depending on the 'Mode' chosen.

; R E V E R B
; (screverb)
; Dry/Wet         -    Dry/Wet Mix: 100% dry to 100% wet
; Size            -    Reverb size

; P R E S E T S
; Choose from 10 'factory' presets to gte a quick idea of the sounds possible

; O U T P U T
; Pan Spread        -    range of random pan positions between notes. Issued at note beginning only. If this is set to '1', all notes will be positioned centrally.
; Level            -    output gain control

<Cabbage>
form caption("Spiccato"), size(990,285), colour(255,230,220), pluginId("Spic")

#define SLIDERDESIGN , textColour(60,60,60), fontColour(60,60,60), colour(215,190,180), markerColour( 55, 30, 20), trackerColour(255,230,220)

image    bounds(  5,  5,360, 95), colour(0,0,0,0), outlineThickness(1), outlineColour("black"), plant("PresEnv") {
label    bounds(  0,  5,360, 10), text("B   O   W      P   R   E   S   S   U   R   E"), fontColour("black")
rslider  bounds(  5, 20, 70, 70), channel("PAtt"),   range(0.001,3,0.1,0.5), text("Attack") $SLIDERDESIGN
rslider  bounds( 75, 20, 70, 70), channel("PAttLev"),range(-90, -0.4,-0.4,2), text("Att.Lev.") $SLIDERDESIGN
rslider  bounds(145, 20, 70, 70), channel("PDec"),   range(0.001, 3,0.1,0.5), text("Decay") $SLIDERDESIGN
rslider  bounds(215, 20, 70, 70), channel("PSus"),   range(-90, -0.4,-90,2), text("Sustain") $SLIDERDESIGN
rslider  bounds(285, 20, 70, 70), channel("PRel"),   range(0.001, 3,0.1,0.5), text("Release") $SLIDERDESIGN
}

image    bounds(370,  5,295, 95), colour(0,0,0,0), outlineThickness(1), outlineColour("black"), plant("BowPos") {
label    bounds(  0,  5,295, 10), text("B   O   W      P   O   S   I   T   I   O   N"), fontColour("black")
rslider  bounds(  5, 20, 70, 70), channel("BowPos"), range(0.01,0.5,0.127236,1,0.000001), text("Bow Pos.") $SLIDERDESIGN
rslider  bounds( 75, 20, 70, 70), channel("PosIRnd"), range(0,0.2,0.05,0.5,0.001), text("Rand.Offset") $SLIDERDESIGN
rslider  bounds(145, 20, 70, 70), channel("PosA"), range(0,1,0.05,0.5,0.001), text("Mod.Amp.") $SLIDERDESIGN
rslider  bounds(215, 20, 70, 70), channel("PosF"), range(0.1,50,0.5,0.5,0.01), text("Mod.Freq.") $SLIDERDESIGN
}

image    bounds(670,  5,150, 95), colour(0,0,0,0), outlineThickness(1), outlineColour("black"), plant("Keyboard") {
label    bounds(  0,  5,150, 10), text("K   E   Y   B   O   A   R   D"), fontColour("black")
rslider  bounds(  5, 20, 70, 70), channel("KybdScale"), range(0,  1,1,1,0.001), text("Scale") $SLIDERDESIGN
rslider  bounds( 75, 20, 70, 70), channel("KybdShift"), range(0,127,0,1,1), text("Shift") $SLIDERDESIGN
}


image    bounds(  5,105,290, 95), colour(0,0,0,0), outlineThickness(1), outlineColour("black"), plant("Vibrato") {
label    bounds(  0,  5,290, 10), text("V   I   B   R   A   T   O"), fontColour("black")
rslider  bounds(  5, 20, 70, 70), channel("vibamp"), range(0,0.3,0.006,0.5), text("Depth") $SLIDERDESIGN
rslider  bounds( 75, 20, 70, 70), channel("vibf"),   range(0, 30, 3.5,0.5), text("Rate") $SLIDERDESIGN
rslider  bounds(145, 20, 70, 70), channel("VibRise"), range(0, 7,  2,0.5), text("Rise") $SLIDERDESIGN
rslider  bounds(215, 20, 70, 70), channel("Dereg"), range(0, 5,  1,0.5), text("Dereg.") $SLIDERDESIGN
}

image    bounds(825,  5,160, 95), colour(0,0,0,0), outlineThickness(1), outlineColour("black"), plant("MonoPoly") {
label    bounds(  0,  5,160, 10), text("P   O   L   Y   P   H   O   N   Y"), fontColour("black")
checkbox bounds( 10, 25, 70, 12), text("Legato"), channel("legato"), fontColour:0("black"), fontColour:1("black")
combobox bounds( 10, 65, 80, 18), text("Fixed Rate","Fixed Time"), channel("LegMode"), fontColour("white"), value(1), identChannel("LegModeID"), visible(0)
rslider  bounds( 85, 20, 70, 70), channel("LegTime"), text("Rate"), range(0.001,2,0.2,0.5), identChannel("LegTimeID"), visible(0) $SLIDERDESIGN
}

image    bounds(525,105,150, 95), colour(0,0,0,0), outlineThickness(1), outlineColour("black"), plant("Reverb") {
label    bounds(  0,  5,150, 10), text("R   E   V   E   R   B"), fontColour("black")
rslider  bounds(  5, 20, 70, 70), channel("RvbDryWet"), range(0,1,0.4), text("Dry/Wet") $SLIDERDESIGN
rslider  bounds( 75, 20, 70, 70), channel("RvbSize"), range(0,0.99,0.7,2), text("Size") $SLIDERDESIGN
}

image    bounds(680,105,150, 95), colour(0,0,0,0), outlineThickness(1), outlineColour("black"), plant("Output") {
label    bounds(  0,  5,150, 10), text("O   U   T   P   U   T"), fontColour("black")
rslider  bounds(  5, 20, 70, 70), channel("PanWidth"), range(0,1,0.2,0.5), text("Pan Spread") $SLIDERDESIGN
rslider  bounds( 75, 20, 70, 70), channel("level"), range(0,3,1,0.5), text("Level") $SLIDERDESIGN
}

image    bounds(835,105,150, 95), colour(0,0,0,0), outlineThickness(1), outlineColour("black"), plant("Presets") {
label    bounds(  0,  5,150, 10), text("P   R   E   S   E   T   S"), fontColour("black")
combobox bounds( 50, 40, 40, 15), channel("preset"), text("1","2","3","4","5","6","7","8","9","10"),  textColour("white"), fontColour("white"), colour("black"), value(1)
}

image    bounds(300,105,220, 95), colour(0,0,0,0), outlineThickness(1), outlineColour("black"), plant("Layers") {
label    bounds(  0,  5,220, 10), text("L   A   Y   E   R   S"), fontColour("black")
rslider  bounds(  5, 20, 70, 70), channel("Layers"), range(1,10,1,1,1), text("Layers") $SLIDERDESIGN
rslider  bounds( 75, 20, 70, 70), channel("IntSemis"), range(-24,24,2,1,1), text("Semitones") $SLIDERDESIGN
rslider  bounds(145, 20, 70, 70), channel("Detune"), range(0,100,0,1,1), text("Detune") $SLIDERDESIGN
}

keyboard  bounds( 0,205,985,80)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

;sr is set by the host
ksmps         =     4        ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2        ;NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1

massign    0,2

givibwav    ftgen    0, 0, 4096, 10, 1
giDry        ftgen    0, 0, 1024, -7, 1, 512, 1, 512, 0
giWet        ftgen    0, 0, 1024, -7, 0, 512, 1, 512, 1

indx    =    0
loop:
ival    table    indx,givibwav
isig    =    ival<0?-1:1
    tableiw    (abs(ival)^0.75)*isig,indx,givibwav
    loop_lt    indx,1,ftlen(givibwav),loop
    
gasendL,gasendR    init    0

gkactive    init    0    ; Will contain number of active instances of instr 3 when legato mode is chosen. NB. notes in release stage will not be regarded as active. 

opcode    sspline,k,Kiii
    kdur,istart,iend,icurve    xin                                        ;READ IN INPUT ARGUMENTS
    imid    =    istart+((iend-istart)/2)                                ;SPLINE MID POINT VALUE
    isspline    ftgentmp    0,0,4096,-16,istart,4096*0.5,icurve,imid,(4096/2)-1,-icurve,iend    ;GENERATE 'S' SPLINE
    kspd    =    i(kdur)/kdur                                        ;POINTER SPEED AS A RATIO (WITH REFERENCE TO THE ORIGINAL DURATION)
    kptr    init    0                                            ;POINTER INITIAL VALUE    
    kout    tablei    kptr,isspline                                        ;READ VALUE FROM TABLE
    kptr    limit    kptr+((ftlen(isspline)/(i(kdur)*kr))*kspd), 0, ftlen(isspline)-1            ;INCREMENT THE POINTER BY THE REQUIRED NUMBER OF TABLE POINTS IN ONE CONTROL CYCLE AND LIMIT IT BETWEEN FIRST AND LAST TABLE POINT - FINAL VALUE WILL BE HELD IF POINTER ATTEMPTS TO EXCEED TABLE DURATION
        xout    kout                                            ;SEND VALUE BACK TO CALLER INSTRUMENT
endop

opcode    wgbowSR,a,ikkkkkiikki                                            ;wgbow ENCAPSULATED WITHIN A UDO SO THAT ksmps CAN BE REDUCED TO IMPROVE SOUND QUALITY OF VIBRATO AND BOWING POSITION CHANGES 
    ivel, kcps, kpres, kBowPos, kvibf, kvibamp, ivibwav, iMinFrq, kPosA, kPosF, iPosIRnd    xin
        setksmps    1                                        ;REDUCED ksmps. (ksmps = 1 DOESN'T SEEM TO WORK!)
    kporttime    linseg    0,0.001,0.05

    if kPosA>0 then                        ; random spline randomise bowing position
     kRatMod    rspline    -kPosA,kPosA,kPosF*0.5,kPosF*2
     kBowPos        mirror    kBowPos + kRatMod, 0, 1
    else
     kBowPos        portk    kBowPos, kporttime
    endif
    iPosRnd    random    -iPosIRnd, iPosIRnd            ; init time bow random position
        kBowPos    mirror    kBowPos + iPosRnd, 0, 1

    abow    wgbow    ivel*0.7, kcps, kpres, kBowPos, kvibf, kvibamp, ivibwav, iMinFrq

        xout    abow
endop

opcode    RecursiveInstrument,aa,iiiip
    inum,ivel,iLayers,iIntSemis,iCount    xin
    aMixL    =            0
    aMixR    =            0    
    aL,aR    subinstr    p1+1,inum+((iCount-1)*iIntSemis),ivel    ;call voice instrument as a subinstrument. Audio will be fed back to this instrument before being sent to the outputs.
    if iCount<iLayers then
     aMixL,aMixR    RecursiveInstrument    inum,ivel,iLayers,iIntSemis,iCount+1
    endif
            xout    aL+aMixR, aR+aMixR
endop

instr    1    ; READ IN WIDGETS. ALWAYS ON.
    gkBowPos        chnget    "BowPos"
    gkPEnv        chnget    "PEnv"
    gkvibamp    chnget    "vibamp"
    gkvibf        chnget    "vibf"
    gkVibRise    chnget    "VibRise"
    gkDereg        chnget    "Dereg"
    gkLegTime    chnget    "LegTime"
    gkRvbSize    chnget    "RvbSize"
    gkRvbDryWet    chnget    "RvbDryWet"
    gkDry        table    gkRvbDryWet,giDry,1
    gkWet        table    gkRvbDryWet,giWet,1
    gkPosIRnd    chnget    "PosIRnd"
    gkPosA        chnget    "PosA"
    gkPosF        chnget    "PosF"
    gklevel        chnget    "level"
    gkPanWidth    chnget    "PanWidth"
    gkKybdShift    chnget    "KybdShift"
    gkKybdScale    chnget    "KybdScale"
    gkLayers    chnget    "Layers"
    gkIntSemis    chnget    "IntSemis"
    gkDetune    chnget    "Detune"
    
    gklegato    chnget    "legato"
    if changed(gklegato)==1 then
     if gklegato==1 then
      chnset    "visible(1)","LegTimeID"
      chnset    "visible(1)","LegModeID"      
     else
      chnset    "visible(0)","LegTimeID"
      chnset    "visible(0)","LegModeID"       
     endif
    endif

    ; PRESETS
    kpreset    chnget    "preset"
    #define    SEND_PRESET(N'PAtt'PAttLev'PDec'PSus'PRel'BowPos'PosIRnd'PosA'PosF'KybdScale'KybdShift'vibamp'vibf'VibRise'Dereg'legato'LegMode'LegTime'RvbDryWet'RvbSize'PanWidth'level'Layers'IntSemis'Detune)
    #
    if kpreset==$N then
        chnset    k($PAtt     ),"PAtt"
        chnset    k($PAttLev  ),"PAttLev"
        chnset    k($PDec     ),"PDec"
        chnset    k($PSus     ),"PSus"
        chnset    k($PRel     ),"PRel"
        chnset    k($BowPos   ),"BowPos"
        chnset    k($PosIRnd  ),"PosIRnd"
        chnset    k($PosA     ),"PosA"
        chnset    k($PosF     ),"PosF"
        chnset    k($KybdScale),"KybdScale"
        chnset    k($KybdShift),"KybdShift"
        chnset    k($vibamp   ),"vibamp"
        chnset    k($vibf     ),"vibf"
        chnset    k($VibRise  ),"VibRise"
        chnset    k($Dereg    ),"Dereg"
        chnset    k($legato   ),"legato"
        chnset    k($LegMode  ),"LegMode"
        chnset    k($LegTime  ),"LegTime"
        chnset    k($RvbDryWet),"RvbDryWet"
        chnset    k($RvbSize  ),"RvbSize"
        chnset    k($PanWidth ),"PanWidth"
        chnset    k($level    ),"level"
        chnset    k($Layers   ),"Layers"
        chnset    k($IntSemis ),"IntSemis"
        chnset    k($Detune   ),"Detune"
    endif
    #
    if changed(kpreset)==1 then
     ;            N 'PAtt'PAttLev'PDec'PSus'PRel'BowPos  'PosIRnd'PosA'PosF'KybdScale'KybdShift'vibamp'vibf'VibRise'Dereg'legato'LegMode'LegTime'RvbDryWet'RvbSize'PanWidth'level'Layers'IntSemis'Detune
     $SEND_PRESET(1 '0.1 '-0.4   '0.1 '-90 '0.1 '0.127236'0.05   '0.05'0.5 '1        '0        '0.006 '3.5 '2      '1    '0     '1      '0.2    '0.4      '0.7    '0.2     '1    '1     '2       '0)
     $SEND_PRESET(2 '0.004'-0.4  '0.05'-90 '0.1 '0.32    '0.05   '0.05'0.5 '1        '0        '0.006 '3.5 '2      '1    '0     '1      '0.2    '0.4      '0.7    '0.2     '2    '1     '2       '0)
     $SEND_PRESET(3 '0.001'-90   '1   '-13 '2   '0.05    '0.007  '0.176'0.1'1        '0        '0.009 '0.29'7      '0.177'0     '1      '0.2    '0.3      '0.8    '0.53    '0.8  '1     '2       '0)
     $SEND_PRESET(4 '0.001'-11.8 '0.43'-90 '0.1 '0.21    '0.007  '0.00'0.0 '1        '0        '0.028 '9.5 '2      '0.177'1     '1      '0.2    '0.4      '0.7    '0.5     '2.5  '1     '2       '0)
     $SEND_PRESET(5 '0.17 '-11.9 '3   '-29.3'0.1'0.25    '0.007  '0.028'25.6'1        '0        '0.007'1.5 '2      '1    '1     '1      '0.044  '0.6      '0.54   '0.002   '3    '1     '2       '0)
     $SEND_PRESET(6 '0.5 '-0.4   '0.277'-90 '0.1 '0.127236'0.05   '0.05'0.5 '1        '0        '0.006 '3.5 '2      '1    '0     '1      '0.2    '0.4      '0.7    '0.2    '0.2  '1     '2       '0)
     $SEND_PRESET(7 '0.5 '-15    '1 '-15 '0.1 '0.1       '0.05   '0.05'0.5 '0.95     '6        '0.013 '0.3 '0      '3.6  '0     '1      '0.2    '0.5      '0.56   '1       '0.75 '1     '2       '0)
     $SEND_PRESET(8 '0.001'-4.96 '0.261'-16'0.1 '0.127236'0.004  '0.009'0.5'0.235    '60       '0.006'3.5  '2      '1    '0     '1      '0.2    '0.76     '0.85   '0.2     '1    '1     '2       '0)
     $SEND_PRESET(9'0.001'-4.96 '0.261'-16'0.1 '0.127236'0.004  '0.114'1.6 '1        '0        '0.069'0.018'0      '0.872'0     '1      '0.2    '0.46     '0.85   '1       '1.5  '1     '2       '0)
     $SEND_PRESET(10'0.001'-4.96 '0.261'-16'0.1 '0.127236'0.004  '0.009'0.5'1        '0        '0.006'3.5  '2      '1    '1     '1      '0.2    '0.76     '0.85   '0.2     '1    '1     '2       '0)
    endif
endin

instr    2
    gkNoteTrig    init    1    ;at the beginning of a new note set note trigger flag to '1'
    inum        notnum
    gknum        =    inum
    ivel        ampmidi    1    ;read in midi note velocity

    if i(gklegato)==0 then        ;if we are *not* in legato mode...

     aL,aR    RecursiveInstrument        inum - (i(gkDetune)*0.01*0.5), ivel,i(gkLayers), i(gkIntSemis)+(i(gkDetune)*0.01)
         outs    aL,aR        ;send audio to outputs
          
    else                ;otherwise... (i.e. legato mode)
     iactive    =    i(gkactive)            ;number of active notes of instr 3 (note in release are disregarded)
     if iactive==0 then                    ;...if no notes are active
      iCount    =    1
      loop2:
      event_i    "i",p1+1,0,3600,inum - (i(gkDetune)*0.01*0.5),ivel,(iCount-1)*(i(gkIntSemis)+(i(gkDetune)*0.01))        ;...start a new held note
      loop_le    iCount,1,i(gkLayers),loop2
     endif
    endif
endin

instr    3
    if i(gklegato)==1 then                ;if we are in legato mode...
     krel    release                    ;sense when  note has been released
     ;gkactive    =    1-krel            ;if note is in release, gkactive=0, otherwise=1
     gkactive    =    1-krel            ;if note is in release, gkactive=0, otherwise=1
     if krel==0 then
      ktrig    changed    gknum                        ;...GENERATE A TRIGGER IS A NEW NOTE NUMBER IS GENERATED (FROM INSTR. 1)
      gkNoteTrig    =    0
     endif
     kOldNum    init    p4                                                ; OLD NOTE NUMBER = FIRST NOTE NUMBER UPON INITIAL NOTE BEING PLAYED 
     if ktrig=1 then                                                    ; IF A NEW (LEGATO) NOTE HAS BEEN PRESSED
      reinit    S_CURVE_2                                                ; BEGIN A REINITIALISATION PASS FROM LABEL
     endif                                                            ; END OF CONDITIONAL BRANCH
     S_CURVE_2:                                                        ; A LABEL. REINITIALISATION BEGINS FROM HERE.
     iLegMode    chnget    "LegMode"        ;0=fixed rate 1=fixed rate
     idiff    =    iLegMode == 0 ? 1+abs(i(gknum)-i(kOldNum)) : 1                                ; ABSOLUTE DIFFERENCE BETWEEN OLD NOTE AND NEW NOTE IN STEPS (+ 1)
     knum    sspline    (gkLegTime*idiff)+ksmps/sr,i(kOldNum),i(gknum),1     ; CALL sspline UDO (PORTAMENTO TIME MULTIPLIED BY NOTE GAP (idiff))
     rireturn                                                            ; RETURN FROM INITIALISATION PASS
     kOldNum    =    knum                                                ; SET OLD NUMBER CURRENT NUMBER
     kactive    active    p1-1                                                ; ...check number of active midi notes (previous instrument)
     if kactive==0 then                                                    ; if no midi notes are active...
      turnoff                                                        ; ... turn this instrument off
     endif
    else                                                            ; otherwise... (polyphonic / non-legato mode)
     knum    =    p4
    endif

    knum    +=    p6

    iPAtt        chnget    "PAtt"
    iPAttLev    chnget    "PAttLev"
    iPDec        chnget    "PDec"
    iPSus    chnget    "PSus"
    iPRel    chnget    "PRel"

    ; KEYBOARD NOTE NUMBER SCALING
    knum    =    (knum*gkKybdScale)+gkKybdShift

    kcps    =    cpsmidinn(knum)
    ivel    =    p5
             
    ; bow pressure envelope
    kpres        expsegr    90, iPAtt, -iPAttLev, iPDec, -iPSus, iPRel, 90
    ;PITCH BEND INFORMATION IS READ
    aenv    linsegr        1,3600,1,0.01+iPRel,0        ;ANTI-CLICK ENVELOPE

    ; vibrato
    kRndVibF    rspline    -gkDereg,gkDereg,2,5        ; deregulation of vibrato amplitude and frequency for more humanistic results
    kvibf        =    gkvibf * (2 ^ kRndVibF)
    kRndVibA    rspline    -gkDereg,gkDereg,2,5
    kvibamp        =    gkvibamp * (2 ^ kRndVibA)
    if i(gkVibRise)>0 then                    ; vibrato amplitude envelope
     kVibEnv    transeg    0,i(gkVibRise),4,1
     kvibamp    =    kvibamp * kVibEnv
    endif

    abow    wgbowSR        ivel*0.7, kcps, kpres, gkBowPos, kvibf, kvibamp, givibwav, 20, gkPosA, gkPosF, i(gkPosIRnd)

;    kporttime    linseg    0,0.001,0.05
;    if gkPosA>0 then                        ; random spline randomise bowing position
;     kRatMod    rspline    -gkPosA,gkPosA,gkPosF*0.5,gkPosF*2
;     kBowPos        mirror    gkBowPos + kRatMod, 0, 1
;    else
;     kBowPos        portk    gkBowPos, kporttime
;    endif
;    iPosRnd    random    -i(gkPosIRnd), i(gkPosIRnd)            ; init time bow random position
;        kBowPos    mirror    kBowPos + iPosRnd, 0, 1
;
;    abow    wgbow    ivel*0.7, kcps, kpres, kBowPos, kvibf, kvibamp, givibwav, 20

    abow    =    abow * aenv * gklevel
    
    ipan    random    0,1
    aL,aR    pan2    abow, 0.5 + ((ipan-0.5)*gkPanWidth)
    
    aL    delay    aL, (ipan     * i(gkPanWidth) * 0.1) + ksmps/sr
    aR    delay    aR, ((1-ipan) * i(gkPanWidth) * 0.1) + ksmps/sr
    
    chnmix   aL * gkWet, "SendL"
    chnmix   aR * gkWet, "SendR"
    
             outs        aL * gkDry, aR * gkDry    ;SEND AUDIO TO OUTPUTS
endin

instr    999
    aInL    chnget      "SendL"
    aInR    chnget      "SendR"
            chnclear    "SendL"
            chnclear    "SendR"            
    aL,aR    reverbsc    aInL, aInR, gkRvbSize, 8000
        outs        aL,aR
endin

</CsInstruments>

<CsScore>
i 1   0 [3600*24*7]
i 999 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>