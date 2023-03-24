
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; TabRecPlay.csd
; Written by Iain McCurdy, 2012

<Cabbage>
form caption("Tab.Rec/Play") size(300, 360), pluginId("tbrp"), colour("Black")

groupbox bounds(  0,  0,300, 95), text("Transport")

label    bounds( 10, 75, 70, 14), text("Record") 
label    bounds( 80, 75, 70, 14), text("Pause") 
label    bounds(150, 75, 70, 14), text("Play Loop") 
label    bounds(220, 75, 70, 14), text("Play Once") 
checkbox bounds( 15, 25, 60, 50), channel("Record"), value(0), shape("square"), colour("red")
checkbox bounds( 85, 25, 60, 50), channel("Pause"), value(0), shape("square"), colour("Blue")
checkbox bounds(155, 25, 60, 50), channel("PlayLoop"), value(0), shape("square")
checkbox bounds(225, 25, 60, 50), channel("PlayOnce"), value(0), shape("square"), colour("yellow")   

hslider bounds( 10,  95, 280,50), channel("Speed"), range(0, 4, 1)    ;, text("Speed")
label   bounds(100, 135, 100,13), text("Speed"), align(centre)
hslider bounds( 10, 145, 280,50), channel("LoopBeg"), range(0, 1, 0)    ;, text("Loop Begin")
label   bounds(100, 185, 100,13), text("Loop Begin"), align(centre)
hslider bounds( 10, 195, 280,50), channel("LoopEnd"), range(0, 1, 1)    ;, text("Loop End")
label   bounds(100, 235, 100,13), text("Loop End"), align(centre)
hslider bounds( 10, 245, 280,50), channel("InGain"), range(0, 1, 1)    ;, text("Input Gain")
label   bounds(100, 285, 100,13), text("Input Gain"), align(centre)
hslider bounds( 10, 295, 280,50), channel("OutGain"), range(0, 1, 1)    ;, text("Output Gain")
label   bounds(100, 335, 100,13), text("Output Gain"), align(centre)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps     =     32
nchnls     =     2
0dbfs    =    1

;Author: Iain McCurdy (2012)

gistorageL    ftgen    0,0,1048576,-7,0    ;AUDIO DATA STORAGE SPACE (ABOUT 23 SECONDS)
gistorageR    ftgen    0,0,1048576,-7,0    ;AUDIO DATA STORAGE SPACE (ABOUT 23 SECONDS)
gkRecDur    init    0

instr    1
    gitablelen    =    ftlen(gistorageL)    ;DERIVE TABLE LENGTH
    
    gkRecord    chnget    "Record"        ;READ IN CABBAGE WIDGET CHANNELS
    gkPause        chnget    "Pause"
    gkPlayLoop    chnget    "PlayLoop"
    gkPlayOnce    chnget    "PlayOnce"    
    
    gkPlayOnceTrig    changed    gkPlayOnce
    gkSpeed        chnget    "Speed"
    gkLoopBeg    chnget    "LoopBeg"
    gkLoopEnd    chnget    "LoopEnd"
    gkInGain    chnget    "InGain"
    gkOutGain    chnget    "OutGain"

#define    TURN_ON_OFF(NAME)
    #
    i$NAME    nstrnum    "$NAME"
    kOnTrig$NAME    trigger    gk$NAME,0.5,0
    kOffTrig$NAME    trigger    gk$NAME,0.5,1
    if kOnTrig$NAME==1 then        ;IF BUTTON IS TURNED ON...
     event    "i",i$NAME,0,3600
    elseif kOffTrig$NAME==1 then        ;IF BUTTON IS TURNED ON...
     turnoff2,i$NAME,0,0
    endif
    #
    $TURN_ON_OFF(Record)
    $TURN_ON_OFF(PlayOnce)
    $TURN_ON_OFF(PlayLoop)
endin

instr    Record
    if    gkPause=1    goto SKIP_RECORD        ;IF PAUSE BUTTON IS ACTIVATED TEMPORARILY SKIP RECORDING PROCESS

        ainL,ainR    ins                    ;READ AUDIO FROM LIVE INPUT CHANNEL 1
        aRecNdx    line        0,gitablelen/sr,1    ;CREATE A POINTER FOR WRITING TO TABLE - FREQUENCY OF POINTER IS DEPENDENT UPON TABLE LENGTH AND SAMPLE RATE
        aRecNdx    =        aRecNdx*gitablelen    ;RESCALE POINTER ACCORDING TO LENGTH OF FUNCTION TABLE 
        gkRecDur    downsamp    aRecNdx            ;CREATE A K-RATE GLOBAL VARIABLE THAT WILL BE USED BY THE 'PLAYBACK' INSTRUMENT TO DETERMINE THE LENGTH OF RECORDED DATA            
              tablew        ainL*gkInGain,  aRecNdx, gistorageL;WRITE AUDIO TO AUDIO STORAGE TABLE
              tablew        ainR*gkInGain,  aRecNdx, gistorageR;WRITE AUDIO TO AUDIO STORAGE TABLE
        if    gkRecDur>=gitablelen    then            ;IF MAXIMUM RECORD TIME IS REACHED...
        kRecord=0
        endif                        ;END OF CONDITIONAL BRANCH
    SKIP_RECORD:
endin

instr    PlayLoop
    if gkPlayLoop==0 then
     turnoff
    endif
    if    gkPause=1    goto SKIP_PLAY_LOOP            ;IF PAUSE BUTTON IS ACTIVATED SKIP ALL RECORDING AND PLAYBACK...


    kporttime    linseg    0,0.001,0.05            ;PORTAMENTO TIME RAMPS UP RAPIDLY TO A HELD VALUE
    kLoopBeg    portk    gkLoopBeg, kporttime        ;APPLY PORTAMENTO SMOOTHING TO CHANGES OF LOOP BEGIN SLIDER
    kLoopEnd    portk    gkLoopEnd, kporttime        ;APPLY PORTAMENTO SMOOTHING TO CHANGES OF LOOP END SLIDER
    kLoopBeg    =    kLoopBeg * gkRecDur        ;RESCALE gkLoopBeg (RANGE 0-1) TO BE WITHIN THE RANGE 0-FILE_LENGTH. NEW OUTPUT VARIABLE kLoopBeg.
    kLoopEnd    =    kLoopEnd * gkRecDur        ;RESCALE gkLoopEnd (RANGE 0-1) TO BE WITHIN THE RANGE 0-FILE_LENGTH. NEW OUTPUT VARIABLE kLoopEnd.
    kLoopLen    =    kLoopEnd - kLoopBeg        ;DERIVE LOOP LENGTH FROM LOOP START AND END POINTS
    kPlayPhasFrq    divz    gkSpeed,    (kLoopLen/sr),       0.00001    ;SAFELY DIVIDE, PROVIDING ALTERNATIVE VALUE INCASE DENOMINATOR IS ZERO 
    aPlayNdx        phasor    kPlayPhasFrq                ;DEFINE PHASOR POINTER FOR TABLE INDEX
    kLoopBeg    =    (kLoopBeg < kLoopEnd ? kLoopBeg : kLoopEnd)    ;CHECK IF LOOP-BEGINNING AND LOOP-END SLIDERS HAVE BEEN REVERSED
    aLoopLen    interp    abs(kLoopLen)
    aLoopBeg    interp    kLoopBeg
    aPlayNdx    =    (aPlayNdx*aLoopLen) + aLoopBeg    ;RESCALE INDEX POINTER ACCORDING TO LOOP LENGTH AND LOOP BEGINING
    aL        tablei    aPlayNdx,    gistorageL        ;READ AUDIO FROM AUDIO STORAGE FUNCTION TABLE
    aR        tablei    aPlayNdx,    gistorageR        ;READ AUDIO FROM AUDIO STORAGE FUNCTION TABLE
                outs    aL*gkOutGain,aR*gkOutGain        ;SEND AUDIO TO OUTPUTS

    SKIP_PLAY_LOOP:
endin

instr    PlayOnce
    koff    =    0

    if    gkPause=1    goto SKIP_PLAY_ONCE            ;IF PAUSE BUTTON IS ACTIVATED SKIP ALL RECORDING AND PLAYBACK...

    kPlayOnceNdx    init    0
    ;kPlayOnceTrig    changed    kPlayOnce
    if kPlayOnceNdx<=gkRecDur then                ;IF PLAYBACK IS NOT YET COMPLETED THEN...
     kLoopBeg    =    gkLoopBeg * gkRecDur        ;RESCALE gkLoopBeg (RANGE 0-1) TO BE WITHIN THE RANGE 0-FILE_LENGTH. NEW OUTPUT VARIABLE kLoopBeg.
     kLoopEnd    =    gkLoopEnd * gkRecDur        ;RESCALE gkLoopEnd (RANGE 0-1) TO BE WITHIN THE RANGE 0-FILE_LENGTH. NEW OUTPUT VARIABLE kLoopEnd.
     if kLoopEnd>kLoopBeg then                ;IF LOOP END SLIDER IS AT A LATER POSITION TO LOOP BEGIN SLIDER...
      aPlayOnceNdx    line    0,1,1                ;CREATE A MOVING POINTER
      aPlayOnceNdx    =    (aPlayOnceNdx*gkSpeed)+kLoopBeg;RESCALE MOVING POINTER VALUE ACCORDING TO LOOP BEGIN POSITION AND SPEED SLIDER SETTING
      kPlayOnceNdx    downsamp    aPlayOnceNdx        ;CREATE kndx, A K-RATE VERSION OF andx. THIS WILL BE USED TO CHECK IF PLAYBACK OF THE DESIRED CHUNK OF AUDIO HAS COMPLETED.
      if kPlayOnceNdx>=kLoopEnd then       
       turnoff
      endif
     else                        ;OTHERWISE (I.E. LOOP BEGIN SLIDER IS AT A LATER POSITION THAT LOOP END)
      aPlayOnceNdx    line    0,1,-1            ;CREATE A NEGATIVE MOVING POINTER
      aPlayOnceNdx    =    kLoopBeg-(aPlayOnceNdx*gkSpeed);RESCALE MOVING POINTER VALUE ACCORDING TO LOOP BEGIN POSITION AND SPEED SLIDER SETTING
      kPlayOnceNdx    downsamp    aPlayOnceNdx    ;CREATE kndx, A K-RATE VERSION OF andx
      if kPlayOnceNdx<=kLoopEnd then
       turnoff
      endif
     endif                        ;END OF CONDITIONAL BRANCH
     ;OUT     OPCODE         INDEX        | FUNCTION_TABLE
     aL    tablei        aPlayOnceNdx*sr,    gistorageL    ;READ AUDIO FROM AUDIO STORAGE FUNCTION TABLE
     aR    tablei        aPlayOnceNdx*sr,    gistorageR    ;READ AUDIO FROM AUDIO STORAGE FUNCTION TABLE
          outs        aL*gkOutGain,aR*gkOutGain        ;SEND AUDIO TO OUTPUT
    else
     turnoff
    endif                            ;END OF CONDITIONAL BRANCH

    SKIP_PLAY_ONCE:
    krelease    release
    if krelease==1 then
     chnset    1-krelease,"PlayOnce"
    endif
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>