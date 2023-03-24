
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; wguide1.csd
; Written by Iain McCurdy, 2013,2015,2021.

; Encapsulation of the 'wguide1' opcode, used for filtering and synthesis.
; A waveguide filter produces resonance peaks that follow the frequencies of the harmonic series with diminishing strength above its defined base frequency.

; frequency of the wave guide can be determined either in hertz of as a note number

; Activating the 'Keyboard Input' switch will allow frequency control from the keyboard (internal or external) and polyphony. 
;  In this mode, 'Frequency' and 'Note Num.' controls will be disabled.
; A range of tuning systems  are available once MIDI control has been chosen.

; Source input sound can be either 'live audio input', or synthetic gravel, dust, air, a click or a pluck.
;  Click and pluck are intended to used with 'Keyboard Input' activated.

; Frequency    -    frequency of the waveguide
; Note Num.    -    frequency of the waveguide expressed as a note number
; Width        -    offsets the frequencies of the left and right channels to imbue added stereo width
; Cutoff       -    cutoff frequency of a lowpass filter within the feedback loop of the waveguide
;             cutoff can be defined as a fixed value (in hertz) or as a ratio with the frequency of the waveguide (Ratio x Frequency = cutoff)
; Feedback     -    feedback ratio of the waveguide unit.
;             negative feedback will shift the fundamental down one octave and only odd harmonics will be preset

; Bandpass Filters
; ----------------
; Single Bandpass is a double butterworth bandpass filter can be inserted, the cutoff frequency of which relates to the frequency defined for the waveguide.
; Balance      -    inserts a dynamic balancing to compensate for power loss on account of the bandpass filtering
; Width        -    bandwidth of the filters
; Harm.        -    ratio of bandpass filter cutoff to waveguide frequency: Harm x Frequency = bandpass cutoff frequency

; Multiple Bandpass applies a stack of bandpass filters, the centre frequencies of which correspond the the harmonic resonances created by the waveguide
; Bandpass On/Off - turns the bandpass filter on and off
; Balance      -    choose between two modes of amplitude balancing
; Width        -    bandwidth of the filters
; Harm.        -    number of filters in the stack

; Mix          -    dry/wet mix
; Level        -    amplitude scaling

<Cabbage>
form caption("wguide1") size(770,200), pluginId("WGu1"), colour("black")
image           bounds(0, 0, 770,120), colour(125, 95, 55), shape("sharp"), outlineColour(225,195,155), outlineThickness(2)

image    bounds( 10,  0,195,111), colour(0,0,0,0), plant("GUIcontrol"), visible(1) {
checkbox bounds( 10,  7,120, 12), text("Keyboard Input"), channel("input"), fontColour:0("white"), fontColour:1("white"), colour(yellow)
checkbox bounds( 10, 22,120, 12), text("Low Cut"), channel("LowCut"), fontColour:0("white"), fontColour:1("white"), colour(yellow) value(0)
label    bounds(130,  5, 62, 11), text("Source"), fontColour("white")
combobox bounds(130, 16, 62, 16), channel("source"), value(1), text("Live","Gravel","Dust","Air","Click","Pluck")
rslider  bounds(  0, 41, 70, 70),  text("Frequency"),  channel("freq"),      trackerColour(225,195,155), range(8.2, 12542, 160, 0.25),  colour( 85, 55,15), textColour(white), fontColour("silver"), identChannel("freqID")
rslider  bounds( 65, 41, 70, 70),  text("Note Num."),  channel("notnum"),    trackerColour(225,195,155), range(0, 127, 51, 1,1),        colour( 85, 55,15), textColour(white), fontColour("silver"), identChannel("notnumID")

image    bounds( 30, 50,230, 70) colour(0,0,0,0), identChannel("MIDITuningID"), visible(0) {
label    bounds(  0,  0, 83, 12), text("MIDI TUNING"), fontColour("white")
combobox bounds(  0, 13, 83, 20), channel("Tuning"), items("12-TET", "24-TET", "12-TET rev.", "24-TET rev.", "10-TET", "36-TET", "Just C", "Just C#", "Just D", "Just D#", "Just E", "Just F", "Just F#", "Just G", "Just G#", "Just A", "Just A#", "Just B"), value(1),fontColour("white")
}

rslider  bounds(130, 41, 70, 70),  text("Width"),      channel("StWidth"),   trackerColour(225,195,155), range(-0.2, 0.2, 0, 1,0.001),        colour( 85, 55,15), textColour(white), fontColour("silver")
}

image    bounds(207,  4, 66,112), colour(125, 95, 55), outlineColour("silver"), outlineThickness(1), line(1) {
label    bounds(  5,  4, 56, 11),  text("Cutoff"), fontColour("white")
combobox bounds(  5, 16, 56, 15),  text("Fixed","Ratio"),channel("CutoffMode"), value(1)
rslider  bounds(  0, 37, 65, 70),  text("Hertz"),     channel("cutoff"),     trackerColour(225,195,155), range(20,20000,8000,0.25), visible(1), colour( 85, 55,15), textColour(white), fontColour("silver"), identChannel("cutoff_ident")
rslider  bounds( -4, 37, 65, 70),  text("Ratio"),     channel("CutoffRatio"),trackerColour(225,195,155), range(1,40,8),             visible(0), colour( 85, 55,15), textColour(white), fontColour("silver"), identChannel("CutoffRatio_ident")
}

image    bounds(275, 41, 66,112), colour(0,0,0,0) {
rslider  bounds(  0,  0, 70, 70),  text("Feedback"),   channel("feedback"),   trackerColour(225,195,155), range(-0.9999999, 0.9999999, 0.8), colour( 85, 55,15), textColour(white), fontColour("silver")
}

line     bounds(350,  5,  2,110)
combobox bounds(365,  5,100, 16), text("Bandpass Off","Single Bandpass","Multiple Bandpass"), channel("BandpassMode"), value(1)
checkbox bounds(365, 24,120, 12), text("Balance"),         channel("BalanceOnOff"), fontColour:0("white"), fontColour:1("white"), colour(yellow) value(0)
rslider  bounds(355, 41, 70, 70),  text("Bandwidth"),      channel("Bandwidth"),   trackerColour(225,195,155), range(0.001,100.00,0.2,0.5,0.001),             colour( 85, 55,15), textColour(white), fontColour("silver")
rslider  bounds(420, 41, 70, 70),  text("Harm."),          channel("HarmRatio"),trackerColour(225,195,155), range(1,48,1,1,1),             colour( 85, 55,15), textColour(white), fontColour("silver")
line     bounds(495,  5,  2,110)

image     bounds(505,  0,140,112), colour(0,0,0,0) {
checkbox  bounds( 10, 15,100, 12), text("Detune"), channel("Detune"), fontColour:0("white"), fontColour:1("white"), colour(yellow)
rslider   bounds(  5, 41, 70, 70), text("Cents"), channel("DetuneAmt"), range(-50, 50, -10, 1,0.1), trackerColour(225,195,155), colour( 85, 55,15), textColour(white), fontColour("silver")
line      bounds( 90,  5,  2,110)
}

image    bounds(615, 0,150,112), colour(0,0,0,0) {
label    bounds(  5,  4,136, 17), align("centre"), text("W A V E G U I D E"), fontColour(silver)
image    bounds(  5,  7,125,  1), colour(200,170,130), shape("sharp"), outlineColour(silver), outlineThickness(1)
image    bounds(  7, 18,124,  1), colour(200,170,130), shape("sharp"), outlineColour(silver), outlineThickness(1)
rslider  bounds(  0, 41, 70, 70),  text("Mix"),        channel("mix"),       trackerColour(225,195,155), range(0, 1.00, 1),             colour( 85, 55,15), textColour(white), fontColour("silver")
rslider  bounds( 65, 41, 70, 70),  text("Level"),      channel("level"),     trackerColour(225,195,155), range(0, 5.00, 0.7,0.5,0.001),           colour( 85, 55,15), textColour(white), fontColour("silver")
}

keyboard bounds(  0,210, 770, 80), visible(1), identChannel("keyboardID")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
;-d -n
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

;sr is set by the host
ksmps         =     32    ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ; NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1
massign    0,2

; Author: Iain McCurdy (2012)

;			         FN_NUM | INIT_TIME | SIZE | GEN_ROUTINE | NUM_GRADES | REPEAT |  BASE_FREQ  | BASE_KEY_MIDI | TUNING_RATIOS:-0-|----1----|---2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|----10-----|---11----|---12---|---13----|----14---|----15---|---16----|----17---|---18----|---19---|----20----|---21----|---22----|---23---|----24----|----25----|----26----|----27----|----28----|----29----|----30----|----31----|----32----|----33----|----34----|----35----|----36----|
giTTable1	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(60),      60,                       1, 1.059463,1.1224619,1.1892069,1.2599207,1.33483924,1.414213,1.4983063,1.5874001,1.6817917,1.7817962, 1.8877471,     2	;STANDARD
giTTable2	ftgen      0,         0,       64,       -2,          24,          2,   cpsmidinn(60),      60,                       1, 1.0293022,1.059463,1.0905076,1.1224619,1.1553525,1.1892069,1.2240532,1.2599207,1.2968391,1.33483924,1.3739531,1.414213,1.4556525,1.4983063, 1.54221, 1.5874001, 1.6339145,1.6817917,1.73107,  1.7817962,1.8340067,1.8877471,1.9430623,    2	;QUARTER TONES
giTTable3	ftgen      0,         0,       64,       -2,          12,        0.5,   cpsmidinn(60),      60,                       2, 1.8877471,1.7817962,1.6817917,1.5874001,1.4983063,1.414213,1.33483924,1.2599207,1.1892069,1.1224619,1.059463,      1	;STANDARD REVERSED
giTTable4	ftgen      0,         0,       64,       -2,          24,        0.5,   cpsmidinn(60),      60,                       2, 1.9430623,1.8877471,1.8340067,1.7817962,1.73107, 1.6817917,1.6339145,1.5874001,1.54221,  1.4983063, 1.4556525,1.414213,1.3739531,1.33483924,1.2968391,1.2599207,1.2240532,1.1892069,1.1553525,1.1224619,1.0905076,1.059463, 1.0293022,    1	;QUARTER TONES REVERSED
giTTable5	ftgen      0,         0,       64,       -2,          10,          2,   cpsmidinn(60),      60,                       1, 1.0717734,1.148698,1.2311444,1.3195079, 1.4142135,1.5157165,1.6245047,1.7411011,1.8660659,     2	;DECATONIC
giTTable6	ftgen      0,         0,       64,       -2,          36,          2,   cpsmidinn(60),      60,                       1, 1.0194406,1.0392591,1.059463,1.0800596, 1.1010566,1.1224618,1.1442831,1.1665286,1.1892067,1.2123255,1.2358939,1.2599204,1.284414,1.3093838, 1.334839, 1.3607891,1.3872436,1.4142125,1.4417056,1.4697332,1.4983057,1.5274337,1.5571279,1.5873994, 1.6182594,1.6497193, 1.6817909, 1.7144859, 1.7478165, 1.7817951, 1.8164343, 1.8517469, 1.8877459, 1.9244448, 1.9618572,      2	;THIRD TONES
giTTable7	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(60),      60,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable8	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(61),      61,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable9	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(62),      62,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable10	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(63),      63,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable11	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(64),      64,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable12	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(65),      65,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable13	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(66),      66,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable14	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(67),      67,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable15	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(68),      68,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable16	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(69),      69,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable17	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(70),      70,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   
giTTable18	ftgen      0,         0,       64,       -2,          12,          2,   cpsmidinn(71),      71,                       1,   16/15,     9/8,     6/5,      5/4,       4/3,     45/32,     3/2,     8/5,      5/3,      9/5,       15/8,    2	;JUST INTONATION                                                                                                                                                                                                                                   


instr    1
    kporttime      linseg   0,0.01,0.03                ; CREATE A VARIABLE THAT WILL BE USED FOR PORTAMENTO TIME
    gkinput        chnget   "input"                    ; READ WIDGETS...
    gkLowCut       chnget   "LowCut"
    gkfreq         chnget   "freq"
    gknotnum       chnget   "notnum"
    gkBandpassMode chnget   "BandpassMode"
    gkBalanceOnOff chnget   "BalanceOnOff"
    gkBandwidth    chnget   "Bandwidth"
    gkBandwidth    portk    gkBandwidth, kporttime
    gkHarmRatio    chnget   "HarmRatio"
    gkHarmRatio    port     gkHarmRatio,0.5
    gkCutoffMode   chnget   "CutoffMode"
    gkCutoffMode   init     1
    gkcutoff       chnget   "cutoff"
    gkCutoffRatio  chnget   "CutoffRatio"
    gkfeedback     chnget   "feedback"
    gkmix          chnget   "mix"
    gkmix          portk    gkmix,kporttime
    gklevel        chnget   "level"
    gklevel        portk    gklevel,kporttime
    gkfreq         portk    gkfreq,kporttime
    gafreq         interp   gkfreq
    gkStWidth      chnget   "StWidth"
    gkDetune       chnget   "Detune"
    gkDetuneAmt    chnget   "DetuneAmt"

    if changed(gkinput)==1||timeinstk()==1 then
     chnset    sprintfk("visible(%d)",1-gkinput),"freqID"
     chnset    sprintfk("visible(%d)",1-gkinput),"notnumID"
     chnset    sprintfk("visible(%d)",gkinput),"MIDITuningID"
;     chnset    sprintfk("visible(%d)",gkinput),"keyboardID"     
    endif

    schedkwhen    trigger(gkinput,0.5,0),0,0,99,0,1,210,115
    schedkwhen    trigger(gkinput,0.5,1),0,0,99,0,1,120,210

    ktrig1        changed    gkfreq
    ktrig2        changed    gknotnum
    if ktrig1==1 then                        ; DUAL FREQUENCY AND NOTE NUMBER CONTROLS
     koct    =    octcps(gkfreq)
     chnset    (koct-3)*12,"notnum"
    elseif ktrig2==1 then
     chnset    cpsmidinn(gknotnum),"freq"
    endif
    
    event_i    "i",2,0,-1
    ktrig    trigger    gkinput,0.5,2
    if ktrig=1 then
     if gkinput=0 then
      event    "i",2,0,-1
     else
      turnoff2    2,0,0
     endif
    endif

    /* SOURCE SOUND */
    gksource    chnget    "source"
    if gksource==1 then
      gasigL, gasigR    ins
     elseif gksource==2 then
      gasigL    gausstrig    0.5, 100, 8, 0
      gasigR    gausstrig    0.5, 100, 8, 0
     elseif gksource==3 then
      kdens    random    475,525
     gasigL    dust2    0.5, kdens
      kdens    random    525,475
     gasigR    dust2    0.5, kdens
     elseif gksource==4 then
     gasigL    pinkish    0.2
     gasigR    pinkish    0.2
    endif    
endin


instr    2
    /* MIDI AND GUI INTEROPERABILITY */
    iMIDIflag    =    0                 ; IF MIDI ACTIVATED = 1, NON-MIDI = 0
    mididefault    1, iMIDIflag         ; IF NOTE IS MIDI ACTIVATED REPLACE iMIDIflag WITH '1'

    if iMIDIflag==1 then                ; IF THIS IS A MIDI ACTIVATED NOTE...
     iTuning chnget "Tuning"
     icps    cpstmid  giTTable1 + iTuning - 1
     inum    notnum
     ivel    veloc    0,1
     p1    =    p1 + (rnd(1000)*0.0001)
     if gkinput=0 then
      turnoff
     endif
     kfreq    init        icps
     afreq    init        icps
    else
     kfreq    =        gkfreq
     afreq    =        gafreq 
    endif                        ; END OF THIS CONDITIONAL BRANCH

    if gksource==5 then
     asigL    mpulse    ivel*5,0,0.001
     asigR    =    asigL
    elseif gksource==6 then
      kenv    expseg    1,0.08,0.001,1,0.001
      asigL    dust2    20*(kenv-0.001),100
      icf    =    icps*(1+(4*ivel))
      icf    limit    icf,20,4000
      asigL    butlp    asigL,icf
      asigR    =    asigL
    else
     aenv        linsegr    0,0.05,1,0.05,0
     asigL    =    gasigL*aenv
     asigR    =    gasigR*aenv
    endif
    
    if changed(gkCutoffMode)==1 then
     if gkCutoffMode==1 then
      chnset    "visible(1)","cutoff_ident"
      chnset    "visible(0)","CutoffRatio_ident"
     else
      chnset    "visible(0)","cutoff_ident"
      chnset    "visible(1)","CutoffRatio_ident"
     endif
    endif


    if gkCutoffMode==2 then
     kcutoff    limit    gkCutoffRatio*kfreq,20,sr/2
    else
     kcutoff    =    gkcutoff
    endif    

    kLDiff        =    semitone(-gkStWidth)
    kRDiff        =    semitone(gkStWidth)    
    
    aresL         wguide1 asigL, afreq*kLDiff, kcutoff, gkfeedback
    aresR         wguide1 asigR, afreq*kRDiff, kcutoff, gkfeedback
    if gkDetune==1 then
     aresL +=     wguide1:a(asigL, afreq*kLDiff*cent(gkDetuneAmt), kcutoff, gkfeedback)
     aresR +=     wguide1:a(asigR, afreq*kRDiff*cent(gkDetuneAmt), kcutoff, gkfeedback)
    endif

    aresL         dcblock    aresL
    aresR         dcblock    aresR
    
    /* LOW CUT */
    if gkLowCut==1 then
     aresL        buthp    aresL, kfreq
     aresR        buthp    aresR, kfreq
     aresL        buthp    aresL, kfreq
     aresR        buthp    aresR, kfreq
    endif
    
    /* BANDPASS FILTER */
    if gkBandpassMode==2 then
     aFiltL        butbp    aresL,kfreq*gkHarmRatio,kfreq*gkBandwidth
     aFiltR        butbp    aresR,kfreq*gkHarmRatio,kfreq*gkBandwidth
     aFiltL        butbp    aFiltL,kfreq*gkHarmRatio,kfreq*gkBandwidth
     aFiltR        butbp    aFiltR,kfreq*gkHarmRatio,kfreq*gkBandwidth
     if gkBalanceOnOff==1 then
      aresL    balance2    aFiltL, aresL, 2
      aresR    balance2    aFiltR, aresR, 2
     else
      aresL    =    aFiltL
      aresR    =    aFiltR
     endif
    elseif gkBandpassMode==3 then
     kcf           =      k(afreq) * kLDiff
     knum          limit  int(gkHarmRatio), 1, int((sr/5) / kcf)
     kbw           =      kcf * gkBandwidth * 0.01
     ksep          =      knum + 1
     isepmode      =      1
     kscl          =      gkBalanceOnOff + 1
     if changed:k(knum, kscl)==1 then
      reinit RESTART_RESONY
     endif
     RESTART_RESONY:
     aresL         resony aresL, kcf, kbw, i(knum)+1, ksep, isepmode, i(kscl)
     aresR         resony aresR, kcf, kbw, i(knum)+1, ksep, isepmode, i(kscl)
     rireturn
    endif
    
    amixL        ntrpol    gasigL*aenv, aresL, gkmix
    amixR        ntrpol    gasigR*aenv, aresR, gkmix

    kenv        linsegr    1,15,0
    amixL        =    amixL * gklevel * kenv
    amixR        =    amixR * gklevel * kenv
    ktime        timeinsts
    krms        rms    amixL,3
    if krms<0.00001&&ktime>0.2&&iMIDIflag==1 then
     turnoff2    p1,4,0
    endif

            outs    amixL, amixR        ; WGUIDE1 OUTPUTS ARE SENT OUT
endin


instr    99
    ky    line    p4,p3,p5
    if metro(16)==1 then
     chnset    sprintfk("pos(0,%d)",ky),"keyboardID"
    endif
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>



























