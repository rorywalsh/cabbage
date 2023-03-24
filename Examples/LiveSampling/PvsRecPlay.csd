
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; PvsRecPlay.csd
; Written by Iain McCurdy, 2012

<Cabbage>
form caption("PVS Rec/Play") size(300, 410), pluginId("pvrp") colour("Black")

groupbox bounds(  0,  0,300, 95), text("Transport")

label    bounds( 15, 75, 60, 14), text("Record") 
label    bounds( 85, 75, 60, 14), text("Pause") 
label    bounds(155, 75, 60, 14), text("Play Loop") 
label    bounds(225, 75, 60, 14), text("Play Once") 
checkbox bounds( 15, 25, 60, 50), channel("Record"), value(0), shape("square"), colour("red")
checkbox bounds( 85, 25, 60, 50), channel("Pause"), value(0), shape("square"), colour("Blue")
checkbox bounds(155, 25, 60, 50), channel("PlayLoop"), value(0), shape("square")
checkbox bounds(225, 25, 60, 50), channel("PlayOnce"), value(0), shape("square"), colour("yellow")



hslider bounds( 10,  95, 280,50), channel("Speed"), range(-4.00, 4.00, 1)
label   bounds(100, 135, 100,13), text("Speed") , align(centre)
hslider bounds( 10, 145, 280,50), channel("Pitch"), range(0.25, 4.00, 1)
label   bounds(100, 185, 100,13), text("Pitch") , align(centre) 
hslider bounds( 10, 195, 280,50), channel("LoopBeg"), range(0, 1, 0)
label   bounds(100, 235, 100,13), text("Loop Begin")  , align(centre)
hslider bounds( 10, 245, 280,50), channel("LoopEnd"), range(0, 1, 1)
label   bounds(100, 285, 100,13), text("Loop End")  , align(centre)
hslider bounds( 10, 295, 280,50), channel("InGain"), range(0, 1, 1)
label   bounds(100, 335, 100,13), text("Input Gain")  , align(centre)
hslider bounds( 10, 345, 280,50), channel("OutGain"), range(0, 1, 1)
label   bounds(100, 385, 100,13), text("Output Gain")  , align(centre)

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
gkRecDur    init    0            ;DURATION OF THE MOST RECENTLY RECORDED BUFFER
gibuflen    init    60            ;PVS BUFFER LENGTH

instr    1    ;READ IN WIDGETS AND START AND STOP THE VARIOUS RECORDING AND PLAYBACK INSTRUMENTS
    gitablelen    =    ftlen(gistorageL)    ;DERIVE TABLE LENGTH
    
    gkRecord    chnget    "Record"        ;READ IN CABBAGE WIDGET CHANNELS
    gkPause        chnget    "Pause"
    gkPlayLoop    chnget    "PlayLoop"
    gkPlayOnce    chnget    "PlayOnce"
    gkPlayOnceTrig    changed    gkPlayOnce
    gkSpeed        chnget    "Speed"
    gkPitch        chnget    "Pitch"
    gkLoopBeg    chnget    "LoopBeg"
    gkLoopEnd    chnget    "LoopEnd"
    gkInGain    chnget    "InGain"
    gkOutGain    chnget    "OutGain"

#define    TURN_ON(NAME)
    #
    i$NAME    nstrnum    "$NAME"
    kOnTrig$NAME    trigger    gk$NAME,0.5,0
    if kOnTrig$NAME==1 then        ;IF BUTTON IS TURNED ON...
     event    "i",i$NAME,0,3600
    endif
    #
    $TURN_ON(Record)
    $TURN_ON(PlayOnce)
    $TURN_ON(PlayLoop)
endin

instr    Record
    if gkRecord==0 then        ;IF BUTTON IS TURNED ON...
     turnoff
    endif
    
    if    gkPause=1    goto SKIP_RECORD        ;IF PAUSE BUTTON IS ACTIVATED TEMPORARILY SKIP RECORDING PROCESS
    ainL,ainR    ins                    ;READ AUDIO FROM LIVE INPUT CHANNEL 1

;MACRO THAT DEFINES THE CODED NEEDED TO RECORD A SINGLE CHANNEL PVS BUFFER
#define    REC_BUF(CHAN)
    #
    iFFTsize    =    1024
    ioverlap    =    256
    iwinsize    =    1024
    iwintype    =    1
    ;kPhOffset    =    0
    f_anal$CHAN    pvsanal        ain$CHAN, iFFTsize, ioverlap, iwinsize, iwintype    ;ANALYSE THE LEFT CHANNEL AUDIO. OUTPUT AN F-SIGNAL.
    ibuf$CHAN,ktime    pvsbuffer       f_anal$CHAN, gibuflen                    ;BUFFER FSIG
    gkhandle$CHAN    init         ibuf$CHAN                        ;INITIALISE HANDLE TO BUFFER
    #
    ;EXPAND BUFFER TWICE, ONCE FOR EACH STEREO CHANNEL
    $REC_BUF(L)
    $REC_BUF(R)
    gkRecDur    timeinsts    ;DURATION OF CURRENT RECORDING
    if gkRecDur>=gibuflen then    ;IF BUFFER IS FULL (I.E. DO NOT OVERWRITE THE BEGINNING OF THE BUFFER
     turnoff            ;TURN OFF THIS INSTRUMENT
    endif                ;ENDO OF THIS CONDITIONAL BRANCH
    SKIP_RECORD:            ;JUMP TO HERE WHEN 'PAUSE' BUTTON IS ACTIVE
endin

instr    PlayLoop
    if gkPlayLoop==0 then        ;IF BUTTON IS TURNED ON...
     turnoff
    endif

    if gkPlayLoop==0 then                    ;IF 'PLAY LOOPED' BUTTON IS INACTIVE...
     turnoff                        ;TURN THIS INSTRUMENT OFF
    endif                            ;END OF THIS CONDITIONAL BRANCH
    if    gkPause=1    goto SKIP_PLAY_LOOP        ;IF PAUSE BUTTON IS ACTIVATED SKIP PLAYBACK CODE
    kporttime    linseg    0,0.001,0.05            ;PORTAMENTO TIME RAMPS UP RAPIDLY TO A HELD VALUE
    kLoopBeg    portk    gkLoopBeg, kporttime        ;APPLY PORTAMENTO SMOOTHING TO CHANGES OF LOOP BEGIN SLIDER
    kLoopEnd    portk    gkLoopEnd, kporttime        ;APPLY PORTAMENTO SMOOTHING TO CHANGES OF LOOP END SLIDER
    kLoopBeg    =    kLoopBeg * gkRecDur        ;RESCALE gkLoopBeg (RANGE 0-1) TO BE WITHIN THE RANGE 0-FILE_LENGTH.
    kLoopEnd    =    kLoopEnd * gkRecDur        ;RESCALE gkLoopEnd (RANGE 0-1) TO BE WITHIN THE RANGE 0-FILE_LENGTH.
    kLoopLen    =    abs(kLoopEnd - kLoopBeg)    ;DERIVE LOOP LENGTH FROM LOOP START AND END POINTS
    kPlayPhasFrq    divz    gkSpeed, kLoopLen, 0.00001    ;SAFELY DIVIDE, PROVIDING ALTERNATIVE VALUE IN CASE DENOMINATOR IS ZERO 
    kPlayNdx    phasor    kPlayPhasFrq            ;DEFINE PHASOR POINTER FOR BUFFER READ INDEX
    ;kLoopBeg    =    (kLoopBeg < kLoopEnd ? kLoopBeg : kLoopEnd)    ;CHECK IF LOOP-BEGINNING AND LOOP-END SLIDERS HAVE BEEN REVERSED
    kPlayNdx    =    (kPlayNdx*kLoopLen) + kLoopBeg    ;RESCALE INDEX POINTER ACCORDING TO LOOP LENGTH AND LOOP BEGINING

    f_bufL         pvsbufread      kPlayNdx , gkhandleL    ;READ BUFFER
    f_scaleL    pvscale     f_bufL, gkPitch        ;RESCALE FREQUENCIES
    aL         pvsynth      f_scaleL                   ;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL    

    f_bufR         pvsbufread      kPlayNdx , gkhandleR    ;READ BUFFER
    f_scaleR    pvscale     f_bufR, gkPitch        ;RESCALE FREQUENCIES
    aR         pvsynth      f_scaleR                   ;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL    

                outs    aL*gkOutGain,aR*gkOutGain    ;SEND AUDIO TO OUTPUTS
    SKIP_PLAY_LOOP:                        ;JUMP TO HERE WHEN 'PAUSE' BUTTON IS ACTIVE
endin

instr    PlayOnce
    if gkPlayOnce==0 then        ;IF BUTTON IS TURNED ON...
     turnoff
    endif

    if    gkPause=1    goto SKIP_PLAY_ONCE        ;IF PAUSE BUTTON IS ACTIVATED SKIP PLAYBACK
    
    kPlayOnceNdx    init    0                ;INITIALISE PLAYBACK POINTER
    if kPlayOnceNdx<=gkRecDur then                ;IF PLAYBACK IS NOT YET COMPLETED THEN CONTINUE PLAYBACK
     kLoopBeg    =    gkLoopBeg * gkRecDur        ;RESCALE gkLoopBeg (RANGE 0-1) TO BE WITHIN THE RANGE 0-FILE_LENGTH.
     kLoopEnd    =    gkLoopEnd * gkRecDur        ;RESCALE gkLoopEnd (RANGE 0-1) TO BE WITHIN THE RANGE 0-FILE_LENGTH.
     kPlayOnceNdx    line    0,1,1                ;CREATE A MOVING POINTER
     if kLoopEnd>kLoopBeg then                ;IF LOOP END SLIDER IS AT A LATER POSITION TO LOOP BEGIN SLIDER...
      kPlayOnceNdx    =    (kPlayOnceNdx*gkSpeed)+kLoopBeg    ;RESCALE MOVING POINTER VALUE ACCORDING TO LOOP BEGIN POSITION AND SPEED SLIDER SETTING
      if kPlayOnceNdx>=kLoopEnd then            ;IF PLAY INDEX IS EQUAL TO OR GREATER THAN THE DURATION OF THE RECORDED BUFFER (STOP PLAYBACK)...
       turnoff                        ;TURN THIS INSTRUMENT OFF
      endif                            ;END OF CONDITIONAL BRANCH
     else                            ;OTHERWISE (I.E. LOOP BEGIN SLIDER IS AT A LATER POSITION THAT LOOP END)
      kPlayOnceNdx    =    kLoopBeg-(kPlayOnceNdx*gkSpeed)    ;RESCALE MOVING POINTER VALUE ACCORDING TO LOOP BEGIN POSITION AND SPEED SLIDER SETTING
      if kPlayOnceNdx<=kLoopEnd then            ;IF PLAY POINTER HAS REACHED THE BEGINNING OF THE PRESCRIBED CHUNK BETWEEN LOOP BEGIN AND LOOP END (STOP PLAYBACK)...
       turnoff                        ;TURN THIS INSTRUMENT OFF
      endif                            ;END OF CONDITIONAL BRANCH
     endif                            ;END OF CONDITIONAL BRANCH
    endif
    f_bufL         pvsbufread      kPlayOnceNdx , gkhandleL    ;READ BUFFER
    f_scaleL    pvscale     f_bufL, gkPitch            ;RESCALE FREQUENCIES
    aL         pvsynth      f_scaleL                       ;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL    

    f_bufR         pvsbufread      kPlayOnceNdx , gkhandleR    ;READ BUFFER
    f_scaleR    pvscale     f_bufR, gkPitch            ;RESCALE FREQUENCIES
    aR         pvsynth      f_scaleR                    ;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL    

          outs        aL*gkOutGain,aR*gkOutGain        ;SEND AUDIO TO OUTPUT
    ;else
    ; turnoff
    ;endif                            ;END OF CONDITIONAL BRANCH

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