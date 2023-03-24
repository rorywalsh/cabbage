
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; HarpFilter.csd

; Creates a stack of waveguides simulating a resonating bank of strings
; ** WARNING **   THIS EFFECT CAN APPLY VAST AMOUNTS OF RESONATING FEEDBACK THEREFORE LARGE AMOUNTS OF GAIN ARE POSSIBLE.
;                 TAKE PARTICULAR CARE WHEN USING A LIVE AUDIO INPUT.
;                 IF IN DOUBT, REDUCE THE FEEDBACK VALUE.

; Tunings of strings are not controlled individually but are instead defined using several global controls.

; Frequency       -    Base frequency of the stack of waveguide filters
; Cutoff          -    Cutoff frequency of a 1st order lowpass filter within the feedback loop of each waveguide unit
; Spacing         -    The spacing method used between adjacent waveguide filters: Geometric or Arithmetic
; Interval        -    Interval factor between adjacent filters.
;                 If 'Spacing' is geometric then Interval is applied geometrically, each time multiplying it to the previous frequency to derive the next.
;                 In this mode the value Interval actually defines an interval in semitones so an interval of 12 will produce a ratio of 2
;                 e.g. if base frequency is 200 and interval is 12, the sequence is 200,400,800,1600 
;                 If 'Spacing' is 'Arithmetic' then this is applied arithmetically each time adding base_frequency to the frequency of the previous filter to derive the frequency of the next.
;                 e.g. if base frequency is 200, interval is 1, the sequence is 200,400,600,800 etc... i.e. harmonic
; Number          -    The number of waveguides to be created
; Lowest          -    The Lowest filter in the sequence. i.e. shift the stacks up in steps as this is increased.
; Reflect         -    If activated, additional waveguide filters are created at frequencies reflected beneath the base frequency according to the geometric or arithmetric rules. Activating 'Reflect' will double the number of filters used.
; Strength        -    number of series iterations of the filters (single/double/triple). Increasing numbers of iterations sharpens the filtering effect and increases the resonance.
; Filter Type     -    choose between wguide1 and streson. streson will provide better tuning but wguide1 will provide smoother results when modulating its cutoff frequency.
; Width           -    offsets the frequencies of the left and right channels to imbue added stereo width
; Random          -    range of random offsets added to waveguide frequencies
; Lowcut          -    Inserts a 24dB/oct highpass filter after each waveguide, the cutoff of which corresponds to the cutoff of that filter.
; Port.           -    Portamento time applied to changes made to frequency for each waveguide (and by extension also changes made to 'Interval'). Large portamento times are possible thereby permitting slow morphs. 
; Feedback        -    feedback ratio of each waveguide unit.
;                      negative feedback will shift the fundemental down one octave and only odd harmonics will be preset
; Attack          -    Attack time of each new note played 
; Decay           -    Decay time of the input sound once a note is released
; Release         -    Release time of the outputs of the waveguides after a note is released
; Level           -    Output amplitude control

<Cabbage>
form caption("Harp Filter") size(1080,205), pluginId("HaFi")
image             bounds(  0,  0,1080,125), colour("silver"), shape("sharp")

;checkbox  bounds(  7,  5, 70, 12), text("Keyboard"), channel("input"), fontColour("black")

label     bounds(  7, 20, 80, 13), text("Freq.Input"), fontColour("black")
combobox  bounds(  7, 34, 80, 16), text("Freq.","Note Number","Keyboard"), channel("input"), value(1)  

label     bounds(  7, 60, 80, 13), text("Audio Input"), fontColour("black")
combobox  bounds(  7, 74, 80, 16), text("Live","Dust","P.Noise","W.Noise"), channel("InSigMode"), value(1)  

label     bounds( 92, 42, 66, 13), text("Frequency"), fontColour("black"), identChannel("freqID0")
rslider   bounds(102, 58, 42, 42),  channel("freq"), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(1,5000,150,0.5,0.01), identChannel("freqID1")
nslider bounds( 93,100, 60, 22),  channel("freqD"), colour("white"), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(1,5000,150,0.5,0.01), identChannel("freqID2")

label     bounds( 92, 42, 66, 13), text("Note Num."), fontColour("black"), identChannel("NNID0"), visible(0)
rslider   bounds(102, 58, 42, 42),  channel("NoteNumber"), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(10,110,50,1,0.01), identChannel("NNID1"), visible(0)
nslider bounds( 93,100, 60, 22),  channel("NoteNumberD"), colour("white"), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(10,110,50,1,0.01), identChannel("NNID2"), visible(0)

image    bounds(160,  5, 75,115), outlineThickness(1), line(1), outlineColour("darkslategrey"), shape("sharp"), plant("cutoff") {
label    bounds(  5,  3, 65, 11), text("LPF Cutoff"), fontColour("black")
combobox bounds(  5, 15, 65, 16), text("Fixed","Ratio"), channel("CutoffMode"), value(1)  
rslider  bounds(  0, 35, 75, 75), text("Hertz"),      channel("cutoff"),      valueTextBox(1), textBox(1), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(20,22000,8000,0.5,1), identChannel("cutoff_ident")
rslider  bounds(  0, 35, 75, 75), text("Ratio"),      channel("CutoffRatio"), valueTextBox(1), textBox(1), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(1,60,21,1,1), identChannel("CutoffRatio_ident")
}

image    bounds(245,  5,395,115), outlineThickness(1), line(1), outlineColour("darkslategrey"), shape("sharp")
label    bounds(260,  8, 80, 11), text("Spacing"), fontColour("black")
combobox bounds(260, 20, 80, 16), text("Geometric","Arithmetic"), channel("type"), value(2)  
checkbox bounds(360, 20, 55, 12), text("Reflect"),      channel("dual"), fontColour:0("black"), fontColour:1("black")
label    bounds(440,  8,  80, 11), text("Strength"), fontColour("black")
combobox bounds(440, 20, 80, 16), text("Single","Double","Triple","Quadruple"), channel("Iterations"), value(1)
label    bounds(545,  8,  80, 11), text("Filter Type"), fontColour("black")
combobox bounds(545, 20, 80, 16), text("wguide1","streson"), channel("FilterType"), value(1)
rslider   bounds(250, 40, 75, 75), text("Interval"),  channel("interval"),   valueTextBox(1), textBox(1), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(-12,12,0.25)
nslider bounds(260, 97, 55, 22),                    channel("intervalD"),   range(-24,24,0.25,1,0.0001), colour("white"), fontColour("black")
rslider  bounds(320, 40, 75, 75), text("Number"),     channel("max"),         valueTextBox(1), textBox(1), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(1,100,11,1,1)
rslider  bounds(380, 40, 75, 75), text("Lowest"),     channel("min"),      valueTextBox(1), textBox(1), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(1,100,1,1,1)
rslider  bounds(440, 41, 75, 75), text("Width"),      channel("StWidth"),   range(-0.5, 0.5, 0, 1,0.001), valueTextBox(1), textBox(1), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey")
rslider  bounds(500, 41, 75, 75), text("Random"),     channel("RndFactor"),   range(0, 5, 0, 0.5,0.001), valueTextBox(1), textBox(1), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey")
rslider  bounds(560, 41, 75, 75), text("Tune"),       channel("Tune"),       range(-100,100, 0, 1,1), valueTextBox(1), textBox(1), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey")

checkbox bounds(660, 20, 70, 12), text("Low Cut"),     channel("LowCut"), fontColour:0("black"), fontColour:1("black")
rslider  bounds(640, 40, 75, 75), text("Port."),   channel("Portamento"), valueTextBox(1), textBox(1), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(0.1,99,0.1,0.5,0.01)
nslider bounds(710, 55, 55, 25), text("Feedback"),  channel("feedback"), range(-0.99999,0.99999,0.99,1,0.00001), colour("white"), fontColour("black"), textColour("black")
rslider  bounds(760, 40, 75, 75), text("Attack"), channel("Att"),         valueTextBox(1), textBox(1), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(0.05,10,0.05,0.5)
rslider  bounds(820, 40, 75, 75), text("Decay"),  channel("Dec"),         valueTextBox(1), textBox(1), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(0.05,2,0.05,0.5)
rslider  bounds(880, 40, 75, 75), text("Release"),channel("Rel"),         valueTextBox(1), textBox(1), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(0.05,20,15,0.5,0.01)
rslider  bounds(940, 40, 75, 75), text("Mix"),    channel("Mix"),         valueTextBox(1), textBox(1), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(0,1,1,0.5,0.001)
rslider  bounds(1000, 40, 75, 75), text("Level"),  channel("amp"),        valueTextBox(1), textBox(1), fontColour("black"), textColour("black"), trackerColour("DarkSlateGrey"), range(0,2,0.1,0.5,0.001)

keyboard bounds(  0,125,1080, 80)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr is set by host
ksmps         =        32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls        =        2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs         =        1
              seed     0
              massign  0,2

giTriDist     ftgen    0,0,1024,21,3,1

gkFilterType  init     1

;A RECURSIVE UDO IS USED TO CREATE THE STACK OF WGUIDE1S
opcode    filterstack, a, akkkkkkkkii                    ;OPCODE DEFINITION
ain,kfreq,kRndFactor,kcutoff,kLowCut,kfeedback,kint,kPortTime,ktype,icount,imax    xin        ;INPUT ARG NAMES
    amix      =    0
    iRnd      trirand    1
    kRnd      =    iRnd * kRndFactor
    if ktype==0 then                        ;IF GEOMETRIC MODE HAS BEEN CHOSEN...
     kfreq2   =    kfreq*semitone(kint*(icount-1) + kRnd)            ;DEFINE FREQUENCY FOR THIS WGUIDE1 ACCORDING TO THE BASE FREQUENCY, INTERVAL AND THE COUNTER (LOCATION IN SERIES)
    else                                ;OTHERWISE MUST BE ARITHMETIC MODE
     kfreq2   =    (kfreq+(kfreq*(icount-1)*kint)) * semitone(kRnd)            ;DEFINE FREQUENCY FOR THIS WGUIDE1 ACCORDING TO THE BASE FREQUENCY, INTERVAL AND THE COUNTER (LOCATION IN SERIES)
    endif                                ;END OF CONDITIONAL
    if abs(kfreq2)>sr/3||abs(kfreq2)<20 then            ;IF FREQUENCY IS OUTSIDE OF A SENSIBLE RANGE JUMP THE CREATION OF THE WGUIDE1 ALTOGETHER
     asig     =    0
    else
     kramp    linseg    0,0.001,1
     kfreq2   portk    kfreq2,kPortTime*kramp
     if gkFilterType==1 then
      asig    wguide1 ain, kfreq2, kcutoff, kfeedback            ;CREATE THE WGUIDE1 SIGNAL
     else
      asig    streson    ain, kfreq2, kfeedback
;      asig    butlp    asig,a(kcutoff)
      asig    clfilt    asig,kcutoff,0,2
     endif
     if kLowCut==1 then
      asig    buthp    asig,kfreq2
      asig    buthp    asig,kfreq2      
     endif
    endif
    if icount<imax then                        ;IF THERE ARE STILL MORE WGUIDES TO CREATE IN THE STACK...
      amix        filterstack    ain,kfreq,kRndFactor,kcutoff,kLowCut,kfeedback,kint,kPortTime,ktype,icount+1,imax    ;CALL THE UDO AGAIN
    endif                                ;END OF CONDITIONAL
    skip:                                ;LABEL - SKIP TO HERE IF THE FREQUENCY WAS OUT OF RANGE
            xout        asig + amix            ;SEND MIX OF ALL AUDIO BACK TO CALLER INSTRUMENT
endop                                    ;END OF UDO

opcode ParallelWidgets, k, SS
SStr1,SStr2 xin
k1         chnget       SStr1
k2         chnget       SStr2
if changed:k(k1)==1 then
           chnset       k1,SStr2
elseif changed:k(k2)==1 then
           chnset       k2,SStr1    
endif
           xout         k1
endop


instr    1
    gkInSigMode        chnget    "InSigMode"                ; input audio signal
    gkinput        chnget        "input"                    ; frequency input method (slider/keyboard)
    gkinput        init        1
    if gkInSigMode==1 then
     gasigL,gasigR    ins
    elseif gkInSigMode==2 then                        ; &&gkinput!=1(temporaraily shelved) ; don't generate dust if 'keyboard' input is selected. It will be generated in instr 2.
     gasigL    dust2    0.5,10*randomh:k(0.5,2,4)
     gasigR    dust2    0.5,10*randomh:k(0.5,2,4)
     gasigL    tone    gasigL,cpsoct(randomh:k(4,14,50))
     gasigR    tone    gasigR,cpsoct(randomh:k(4,14,50))
    elseif gkInSigMode==3 then
     gasigL        =    pinker()*0.2
     gasigR        =    pinker()*0.2
    else
     gasigL    noise    0.2,0
     gasigR    noise    0.2,0
    endif
    
    kporttime    linseg        0,0.001,0.03
    
    gkfreq        ParallelWidgets "freq","freqD"
    
    gkNoteNumber  ParallelWidgets "NoteNumber","NoteNumberD"

    gkinterval    ParallelWidgets "interval","intervalD"
    
    ;gkfreq        portk        gkfreq,kporttime
    gkCutoffMode  chnget        "CutoffMode"
    gkcutoff      chnget        "cutoff"
    gkcutoff      portk        gkcutoff,kporttime
    gkCutoffRatio chnget        "CutoffRatio"
    gkfeedback    chnget        "feedback"
    gkfeedback    portk        gkfeedback,kporttime
    ;gkinterval    portk        gkinterval,kporttime
    gkmax         chnget        "max"
    gkmin         chnget        "min"
    ktype         chnget        "type"
    ktype         init        2
    gktype        =        ktype - 1    ; COMBOBOX TO 0-1
    gkAtt         chnget        "Att"
    gkDec         chnget        "Dec"
    gkRel         chnget        "Rel"
    gkMix         chnget        "Mix"
    gkamp         chnget        "amp"
    gkPortamento  chnget        "Portamento"
    gkdual        chnget        "dual"
    gkLowCut      chnget        "LowCut"
    gkStWidth     chnget        "StWidth"
    gkRndFactor   chnget        "RndFactor"
    gkTune        chnget        "Tune"
    gkTune        *=            0.01                ; CONVERT FROM CENTS TO SEMITONES
    gkLDiff       =             semitone(-gkStWidth+gkTune)
    gkRDiff       =             semitone(gkStWidth+gkTune)    
    gkIterations  chnget        "Iterations"
    gkFilterType  chnget        "FilterType"
    if changed(gkCutoffMode)==1 then
     if gkCutoffMode==1 then
                  chnset        "visible(1)","cutoff_ident"
                  chnset        "visible(0)","CutoffRatio_ident"
     else
                  chnset        "visible(0)","cutoff_ident"
                  chnset        "visible(1)","CutoffRatio_ident"
     endif   
    endif

    if changed(gkinput)==1 then
     if gkinput==1 then                    ; frequency
      chnset    "visible(1)","freqID0"
      chnset    "visible(1)","freqID1"
      chnset    "visible(1)","freqID2"
      chnset    "visible(0)","NNID0"
      chnset    "visible(0)","NNID1"
      chnset    "visible(0)","NNID2"
     elseif gkinput==2 then                ; note number
      chnset    "visible(0)","freqID0"
      chnset    "visible(0)","freqID1"
      chnset    "visible(0)","freqID2"
      chnset    "visible(1)","NNID0"
      chnset    "visible(1)","NNID1"
      chnset    "visible(1)","NNID2"
     else                                ; keyboard
      chnset    "visible(0)","freqID0"
      chnset    "visible(0)","freqID1"
      chnset    "visible(0)","freqID2"
      chnset    "visible(0)","NNID0"
      chnset    "visible(0)","NNID1"
      chnset    "visible(0)","NNID2"     
     endif   
    endif
    
    event_i    "i",2,0.001,-1                ; start instr 2 at startup

    if changed(gkinput)==1 then            ; 
     if gkinput<3 then
      event    "i",2,0,-1
     endif
    endif
endin
        
instr    2
    /* MIDI AND GUI INTEROPERABILITY */
    iMIDIflag    =    0            ; IF MIDI ACTIVATED = 1, NON-MIDI = 0
    mididefault    1, iMIDIflag            ; IF NOTE IS MIDI ACTIVATED REPLACE iMIDIflag WITH '1'

    if iMIDIflag==1 then                ; IF THIS IS A MIDI ACTIVATED NOTE...
     inum    notnum
     ivel    veloc    0,1
     p1    =    p1 + (rnd(1000)*0.0001)
     if gkinput<3 then
      turnoff
     endif
     icps    cpsmidi                    ; READ MIDI PITCH VALUES - THIS VALUE CAN BE MAPPED TO GRAIN DENSITY AND/OR PITCH DEPENDING ON THE SETTING OF THE MIDI MAPPING SWITCHES
     kfreq    init        icps
    else
     if gkinput==1 then                        ; frequency input
      kfreq    =        gkfreq
     elseif gkinput==2 then                    ; note number input
      kfreq    =        cpsmidinn(gkNoteNumber)
     endif
    endif                        ; END OF THIS CONDITIONAL BRANCH

    if trigger:k(gkinput,2.5,0)==1&&iMIDIflag==0 then        ; turnoff non-midi notes if keyboard mode is selected
     turnoff
    endif
    
    kRelease    release
    
    /* INPUT SIGNAL ENVELOPE */
    aenv        linsegr    0,i(gkAtt),1,i(gkDec),0
    
    
    ; generating asig within MIDI instrument seems unreliable so is shelved in the meantime...
    /*
    ;print    i(gkInSigMode)
    if i(gkInSigMode)==2&&iMIDIflag==1 then                ; ensure a unique dust distribution if keyboard mode is selected
     ;asigL    dust2    0.5,10*randomh:k(0.5,2,4)            ; dust2 seems unreliable at the moment, use gausstrig instead...
     ;asigR    dust2    0.5,10*randomh:k(0.5,2,4)
     asigL    gausstrig 0.5, 15, 5
     asigR    gausstrig 0.5, 15, 5
     asigL    tone    asigL,cpsoct(randomh:k(4,14,50))
     asigR    tone    asigR,cpsoct(randomh:k(4,14,50))
     asigL    =        asigL*aenv
     asigR    =        asigR*aenv
    else
     asigL    =        gasigL * aenv
     asigR    =        gasigR * aenv
    endif
    */
    
    asigL    =        gasigL * aenv
    asigR    =        gasigR * aenv
    
    
    /* DERIVE LOWPASS FILETR CUTOFF DEPENDING UPON MODE SELECTION */
    if gkCutoffMode==2 then
     kcutoff    limit    gkCutoffRatio*kfreq,20,sr/2
    else
     kcutoff    =    gkcutoff
    endif    

    /*PORTAMENTO TIME FUNCTION*/
    kPortTime    linseg    0,0.001,1
    kPortTime    *=    gkPortamento

    kchange        changed        gkmax,gkmin,gkIterations,gkdual,gkFilterType,gkLowCut        ;reiniting can also smooths interruptions and prevent very loud clicks
    if kchange==1 then                    ;IF NUMBER OF WGUIDE1S NEEDED OR THE START POINT IN THE SERIES HAS CHANGED...
      reinit    update                    ;REINITIALISE THE STACK CREATION
    endif            
    update:                            ;REINIT FROM HERE
    ;CALL THE UDO. (ONCE FOR EACH CHANNEL.)
    aresL        filterstack    asigL, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
    aresR        filterstack    asigR, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
    if i(gkIterations)>1 then
     aresL        filterstack    aresL*0.03, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
     aresR        filterstack    aresR*0.03, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
    endif
    if i(gkIterations)>2 then
     aresL        filterstack    aresL*0.03, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
     aresR        filterstack    aresR*0.03, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
    endif
    if i(gkIterations)>3 then
     aresL        filterstack    aresL*0.03, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
     aresR        filterstack    aresR*0.03, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, gkinterval, kPortTime, gktype, i(gkmin), i(gkmax)+i(gkmin)-1
    endif

    if i(gkdual)==1 then    ;DUAL DIRECTION WGUIDE1S SELECTED (NOTE NEGATIVE 'kinterval'
     if i(gkmin)==1 then    ;DON'T DOUBLE UP FUNDEMENTAL IF 'Lowest' IS '1'
      imin    =    i(gkmin)+1
      imax    =    i(gkmax)+i(gkmin)-2
     else
      imin    =    i(gkmin)
      imax    =    i(gkmax)+i(gkmin)-1
     endif
     if gkmin==1&&gkmax==1 kgoto skip    ;IF 'Num.wguides' AND 'Lowest' ARE BOTH '1', DON'T CREATE ANY REFLECTED WGUIDE1S AT ALL     
     aresL2    filterstack    asigL, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
     aresR2    filterstack    asigR, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
     if i(gkIterations)>1 then
      aresL2    filterstack    aresL2*0.03, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
      aresR2    filterstack    aresR2*0.03, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
     endif
     if i(gkIterations)>2 then
      aresL2    filterstack    aresL2*0.03, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
      aresR2    filterstack    aresR2*0.03, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
     endif
     if i(gkIterations)>3 then
      aresL2    filterstack    aresL2*0.03, kfreq*gkLDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
      aresR2    filterstack    aresR2*0.03, kfreq*gkRDiff, gkRndFactor, kcutoff, gkLowCut, gkfeedback, -gkinterval, kPortTime, gktype, imin, imax
     endif
     aresL    +=                aresL2
     aresR    +=                aresR2
     skip:
    endif
    aresL    dcblock2    aresL
    aresR    dcblock2    aresR
    rireturn                        ;RETURN FROM REINIT
    

    /* EXTEND RELEASE */
    kenv        linsegr    1,i(gkRel),0
    aresL        =    aresL * kenv
    aresR        =    aresR * kenv
    ktime        timeinsts
    krms        rms    aresL,3
    if krms<0.00001&&ktime>0.2&&iMIDIflag==1 then
     turnoff2    p1,4,0
    endif

    /* WET_DRY MIX */
    aOutL    ntrpol    asigL, aresL, gkMix
    aOutR    ntrpol    asigR, aresR, gkMix
    
            outs    aOutL*gkamp, aOutR*gkamp    ;SEND wguide OUTPUT TO THE AUDIO OUTPUTS AND SCALE USING THE FLTK SLIDER VARIABLE gkamp
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>