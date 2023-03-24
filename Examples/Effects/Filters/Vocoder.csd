
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Vocoder.csd
; Written by Iain McCurdy, 2012, 2021

; This is an implementation of a traditional analogue style vocoder. Two audio signals, referred to as a modulator and a carrier, are passed into the vocoder effect. The modulator is typically a voice and the carrier is typically a synthesizer. The modulator is analysed by being passed into a multiband filter (a bank of bandpass filters), the amplitude envelope of each band is tracked and a control function for each band generated. The carrier signal is also passed through a matching multiband filter, the set of amplitude envelopes derived from the modulator analysis is applied to the outputs of this second multiband filter.
; The uppermost band of the filter bank is always a highpass filter. This is to allow high frequency sibilants in the modulator signal to be accurately represented.
; The modulator signal is always the signal received at the left input channel. For best results it is recommended to use a high quality external microphone.

; Carrier Source   - Source used as carrier signal. Either an internal synth or an external signal. If external is chosen, audio is taken from the right input channel.
; Filter Steepness - Steepness of the filters used for both analysis and carrier processing.
; Synth Type       - Type of oscillator used by the internal synthesizer. Choose between sawtooth, square, pulse or noise.
; Base             - Frequency of the lowest filter (both analysis and processing) expressed as a MIDI note number.
; Number           - Number of filters that constitute the multibank filters used during both the analysis of the modulator and the carrier processing.
; Bandwidth        - Bandwidth of the bandpass filters expressed in octaves.
; Spacing          - Spacing between the bandpass filters expressed in semitones.
; BPF              - Gain of the bank of bandpass filters.
; HPF              - Gain of the single highpass filter.
; Gate Input       - Activating this switch will apply a noise gate to the modulator signal. This option might be useful if the microphone used in rather noisy, such as would be the case if using the built-in microphone on a laptop, or if working in a noisy environment.

; Additionally the user can select sound files for either or both the modulator and carrier in the vocoder.
; Pressing play on either of these, once a file has been loaded, will bypass other sources as options for modulator/carrier.
; Files will play looped.

<Cabbage>
form caption("Vocoder"), size(540, 500), pluginId("voco") guiRefresh(10)

image  bounds(0,0,540,500), colour(255,255,150), shape("rounded"), outlineColour("white"), line(1)

soundfiler bounds( 80,  1,460,148), channel("beg","len"), identChannel("filer1"),  colour(0, 255, 255, 255), fontColour(160, 160, 160, 255)
line       bounds(  0,150,540,   1), colour("DarkSlateGrey")
soundfiler bounds( 80,151,460,148), channel("beg","len"), identChannel("filer2"),  colour(0, 255, 255, 255), fontColour(160, 160, 160, 255)

label      bounds(  3,  8, 72, 12), text("MODULATOR"), fontColour("black")
label      bounds(  3, 22, 72, 12), text("(e.g. voice)"), fontColour("black")
label      bounds(  3,158, 72, 12), text("CARRIER"), fontColour("black")
label      bounds(  3,172, 72, 12), text("(e.g. synth)"), fontColour("black")

filebutton bounds(  3, 41, 72, 18), text("Open File","Open File"), fontColour("white") channel("filename1"), shape("ellipse"), colour:0(50,50,100)
button     bounds(  3, 66, 72, 18), text("PLAY","PLAY"), channel("Play1"), value(0), latched(1), fontColour:0(70,120,70), fontColour:1(205,255,205), colour:0(20,40,20), colour:1(0,150,0)

filebutton bounds(  3,191, 72, 18), text("Open File","Open File"), fontColour("white") channel("filename2"), shape("ellipse"), colour:0(50,50,100)
button     bounds(  3,216, 72, 18), text("PLAY","PLAY"), channel("Play2"), value(0), latched(1), fontColour:0(70,120,70), fontColour:1(205,255,205), colour:0(20,40,20), colour:1(0,150,0)

label      bounds( 82,  4,448,  9), text(""), align(left), colour(0,0,0,0), fontColour(200,200,200), identChannel("stringbox1")
label      bounds( 82,154,448,  9), text(""), align(left), colour(0,0,0,0), fontColour(200,200,200), identChannel("stringbox2")

image   bounds(145,304,180, 31), colour(0,0,0,0) { 
image   bounds(  0,  2,180, 31), colour( 10,225, 90,100), shape("ellipse"), outlineColour(205,205, 70), line(1)
label   bounds( 18,  1, 33, 33), text("v"), fontColour(100,100,100)
label   bounds( 38,  1, 33, 33), text("o"), fontColour(100,100,100)
label   bounds( 58,  1, 33, 33), text("c"), fontColour(100,100,100)
label   bounds( 78,  1, 33, 33), text("o"), fontColour(100,100,100)
label   bounds( 98,  1, 33, 33), text("d"), fontColour(100,100,100)
label   bounds(118,  1, 33, 33), text("e"), fontColour(100,100,100)
label   bounds(138,  1, 33, 33), text("r"), fontColour(100,100,100)
label   bounds( 21,  0, 30, 30), text("v"), fontColour(255,  0,  0)
label   bounds( 41,  0, 30, 30), text("o"), fontColour(235,  0, 20)
label   bounds( 61,  0, 30, 30), text("c"), fontColour(215,  0, 40)
label   bounds( 81,  0, 30, 30), text("o"), fontColour(195,  0, 60)
label   bounds(101,  0, 30, 30), text("d"), fontColour(175,  0, 80)
label   bounds(121,  0, 30, 30), text("e"), fontColour(155,  0,100)
label   bounds(141,  0, 30, 30), text("r"), fontColour(135,  0,120)
}

image    bounds(  9,309, 87, 89), colour(0,0,0,0) { 
label    bounds(  3,  0, 75, 11), text("Carrier Source"), fontColour("black")
image    bounds(  0, 11, 87, 18), colour("black"), shape("sharp")
combobox bounds(  1, 12, 85, 16), channel("CarSource"), value(1), text("Synth","Right chan"), fontColour(black), colour(255,255,200)
label    bounds(  1, 30, 85, 11), text("Filter Steepness"), fontColour("black")
image    bounds(  0, 41, 87, 18), colour("black"), shape("sharp")
combobox bounds(  1, 42, 85, 16), channel("steepness"), value(2), text("12dB/oct","24dB/oct"), fontColour(black), colour(255,255,200)
label    bounds( 11, 60, 60, 11), text("Synth Type"), fontColour("black")
image    bounds(  0, 71, 87, 18), colour("black"), shape("sharp")
combobox bounds(  1, 72, 85, 16), channel("SynType"), value(1), text("saw","square","pulse","noise"), fontColour(black), colour(255,255,200)
}


image   bounds(105,338,410, 70), colour(0,0,0,0) { 
rslider bounds(  0,  0, 70, 70), text("Base"),    colour( 55,105,50), textColour("Black"), channel("base"),  range(24, 80, 40,1,0.1),  trackerColour(150,255,150)
rslider bounds( 65,  0, 70, 70), text("Number"),    colour( 55,105,50), textColour("Black"), channel("num"),   range(1, 100, 16,1,1),  trackerColour(150,255,150)
rslider bounds(130,  0, 70, 70), text("Bandwidth"), colour( 55,105,50), textColour("Black"), channel("bw"),    range(0.01, 1, 0.1,0.5),  trackerColour(150,255,150)
rslider bounds(195,  0, 70, 70), text("Spacing"), colour( 55,105,50), textColour("Black"), channel("incr"),  range(1, 12, 5,0.5),  trackerColour(150,255,150)
}


image    bounds(350,310,185,100), colour(0,0,0,0) { 
vslider  bounds( 20,  0, 50, 90), colour( 55,105,50), textColour("Black"), channel("BPGain"), range(0, 1, 0.6, 0.5),  trackerColour(150,255,150)
vslider  bounds( 45,  0, 50, 90), colour( 55,105,50), textColour("Black"), channel("HPGain"), range(0, 1, 0.25, 0.5), trackerColour(150,255,150)
label    bounds( 35, 85, 25, 12), text("BPF"), fontColour("black")
label    bounds( 60, 85, 25, 12), text("HPF"), fontColour("black")
checkbox bounds( 90,  0, 80, 13), channel("gate"), text("Gate Input"), value(0), shape("square"), fontColour:0("black"), fontColour:1("black")
rslider  bounds( 90, 30, 70, 70), text("Level"), colour( 55,105,50), textColour("Black"), channel("level"), range(0,100,1, 0.5, 0.01), trackerColour(150,255,150)
vmeter   bounds(174,  0, 12, 95) channel("vMeter1") value(0) overlayColour(0, 0, 0, 255) meterColour:0(255, 255, 0) meterColour:1(255, 103, 171) meterColour:2(250,250, 0) outlineThickness(2), outlineColour("silver")
}


keyboard bounds( 5,415, 530,80)


</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

;sr is set by the host
ksmps       =        64
nchnls      =        2
0dbfs       =        1     ; MAXIMUM AMPLITUDE
            massign  0,2

gisine   ftgen   0, 0, 4096, 10, 1
gaSyn    init    0

opcode VocoderChannel, a, aakkkkii                        ; MODE UDO 
 aMod,aCar,ksteepness,kbase,kbw,kincr,icount,inum  xin    ; NAME INPUT VARIABLES
 kcf       =    cpsmidinn(kbase+(icount*kincr))           ; DERIVE FREQUENCY FOR *THIS* BANDPASS FILTER BASED ON BASE FREQUENCY AND FILTER NUMBER (icount)
 icount    =    icount + 1                                ; INCREMENT COUNTER IN PREPARTION FOR NEXT FILTER
 
 aModF     =    0
 if kcf>15000 goto SKIP                                   ; IF FILTER FREQUENCY EXCEEDS A SENSIBLE LIMIT SKIP THE CREATION OF THIS FILTER AND END RECURSION
 
 aModF    butbp      aMod,kcf,kbw*kcf                     ; BANDPASS FILTER MODULATOR
 
 if ksteepness=2 then                                     ; IF 24DB PER OCT MODE IS CHOSEN...
   aModF  butbp      aModF,kcf,kbw*kcf                    ; ...BANDPASS FILTER AGAIN TO SHARPEN CUTOFF SLOPES
 endif                                                    ; END OF THIS CONDITIONAL BRANCH
 aEnv     follow2    aModF, 0.05, 0.05                    ; FOLLOW THE ENVELOPE OF THE FILTERED AUDIO
 
 aCarF    butbp      aCar,kcf,kbw*kcf                     ; BANDPASS FILTER CARRIER
 if ksteepness=2 then                                     ; IF 24 DB PER OCT IS CHOSEN...
   aCarF  butbp      aCarF,kcf,kbw*kcf                    ; ...BANDPASS FILTER AGAIN TO SHARPEN CUTOFF SLOPES
 endif                                                    ; END OF THIS CONDITIONAL BRANCH
 
 amix    init    0                                        ; INITIALISE MIX VARIABLE CONTAINING ALL SUBSEQUENT BANDS
 
 if    icount < inum    then                              ; IF MORE FILTERS STILL NEED TO BE CREATED...
     amix    VocoderChannel    aMod,aCar,ksteepness,kbase,kbw,kincr,icount,inum    ;...CALL UDO AGAIN WITH INCREMENTED COUNTER
 endif                                                    ; END OF THIS CONDITIONAL BRANCH
 SKIP:                                                    ; LABEL
     xout    amix + (aCarF*aEnv)                          ; MIX LOCAL BAND WITH SUBSEQUENT BANDS GENERATED VIA RECURSION
endop                                                     ; END OF UDO





gSfilepath1    init    ""
gSfilepath2    init    ""
gkFileChans1,gkFileChans2    init    0


opcode FileNameFromPath,S,S        ; Extract a file name (as a string) from a full path (also as a string)
 Ssrc    xin                       ; Read in the file path string
 icnt    strlen    Ssrc            ; Get the length of the file path string
 LOOP:                             ; Loop back to here when checking for a backslash
 iasc    strchar Ssrc, icnt        ; Read ascii value of current letter for checking
 if iasc==92 igoto ESCAPE          ; If it is a backslash, escape from loop
 loop_gt    icnt,1,0,LOOP          ; Loop back and decrement counter which is also used as an index into the string
 ESCAPE:                           ; Escape point once the backslash has been found
 Sname    strsub Ssrc, icnt+1, -1  ; Create a new string of just the file name
    xout    Sname                  ; Send it back to the caller instrument
endop



instr    1    ;READ IN WIDGETS
 gSfilepath1    chnget    "filename1"        ; read in file path string from filebutton widget
 gSfilepath2    chnget    "filename2"        ; read in file path string from filebutton widget 
 if changed:k(gSfilepath1)==1 then           ; call instrument to update waveform viewer  
  event "i",98,0,0
 elseif changed:k(gSfilepath2)==1 then       ; call instrument to update waveform viewer  
  event "i",99,0,0
 endif

 gkPlay1        chnget    "Play1"
 gkPlay2        chnget    "Play2"
 
 if changed:k(gkPlay2)==1 then
  if gkPlay2==1 && gkFileChans2>0 then
   event "i",3,0,-1
  else
   turnoff2 3,0,0
  endif
 endif

 if changed:k(gkPlay1)==1 then
  if gkPlay1==1 && gkFileChans1>0 then
   event "i", 4, 0, -1
  else
   turnoff2 4, 0, 0
  endif
 endif
 
 kPortTime      linseg    0, 0.001, 0.1
 gkCarSource    chnget    "CarSource"
 gkbase         chnget    "base"
 gkbase         portk     gkbase, kPortTime
 gknum          chnget    "num"
 gkbw           chnget    "bw"     
 gkbw           portk     gkbw, kPortTime
 gkincr         chnget    "incr"
 gkincr         portk     gkincr, kPortTime    
 gkBPGain       chnget    "BPGain"
 gkHPGain       chnget    "HPGain"
 gksteepness    chnget    "steepness"
 gkSynType      chnget    "SynType"
 gkgate         chnget    "gate"
 gklevel        chnget    "level"
endin

instr    2    ;SIMPLE SYNTH
    icps    cpsmidi                  ; READ MIDI NOTE IN CPS FORMAT
    icps    =    icps*0.5            ; TRANSPOSE DOWN AND OCTAVE
    aenv    linsegr    0,0.01,1,0.02,0 ; CREATE A SIMPLE GATE-TYPE ENVELOPE

    if gkSynType==1 then             ; IF SYNTH TYPE CHOSEN FROM BUTTON BANK GUI IS SAWTOOTH...
     a1    vco2    1, icps           ; ...CREATE A SAWTOOTH WAVE TONE
     a1    tone    a1, 12000         ; LOWPASS FILTER THE SOUND
    elseif gkSynType=2 then          ; IF SYNTH TYPE CHOSEN FROM BUTTON BANK GUI IS SQUARE...
     a1    vco2    1, icps, 2, 0.5   ; ...CREATE A SQUARE WAVE TONE
     a1    tone    a1, 12000         ; LOWPASS FILTER THE SOUND
    elseif gkSynType=3 then          ; IF SYNTH TYPE CHOSEN FROM BUTTON BANK GUI IS PULSE...
     a1    vco2    1, icps, 2, 0.1   ; ...CREATE A PULSE WAVE TONE
     a1    tone    a1, 12000         ; LOWPASS FILTER THE SOUND
    else                             ; OTHERWISE...
     a1    pinkish 10                ; ...CREATE SOME PINK NOISE
     a1    butbp   a1, icps, icps    ; BANDPASS FILTER THE SOUND. BANDWIDTH = 1 OCTAVE. NARROW BANDWIDTH IF YOU WANT MORE OF A SENSE OF PITCH IN THE NOISE SIGNAL.
    endif                            ; END OF THIS CONDITIONAL BRANCH
    gaSyn    =    gaSyn + (a1*aenv)  ; APPLY ENVELOPE
endin

instr 3 ; Carrier file
  if i(gkFileChans2)==1 then
   gaCar1 diskin2 gSfilepath2,1,0,1
   gaCar2 =       gaCar1
  else
   gaCar1, gaCar2 diskin2 gSfilepath2,1,0,1
  endif
endin

instr 4 ; Modulator file
  if i(gkFileChans1)==1 then
   gaMod1 diskin2 gSfilepath1, 1 ,0, 1
   gaMod2 =       gaMod1
  else
   gaMod1, gaMod2 diskin2 gSfilepath1, 1, 0, 1
  endif
endin


instr    10 ; vocoder
 
 if gkPlay1==1 then
  aMod1 = gaMod1
  aMod2 = gaMod2
 else
  aMod1   inch 1                                ; READ LIVE AUDIO FROM THE COMPUTER'S LEFT INPUT CHANNEL
  aMod2   =    aMod1
 endif
 
 ;GATE MODULATOR SIGNAL
 if gkgate==1 then                               ; IF 'Gate Modulator' SWITCH IS ON....
  krms    rms      aMod1 + aMod2                 ; SCAN RMS OF MODUALTOR SIGNAL
  kgate   =        (krms < 0.05 ? 0 : 1)         ; IF RMS OF MODULATOR SIGNAL IS BELOW A THRESHOLD, GATE WILL BE CLOSED (ZERO) OTHERWISE IT WILL BE OPEN ('1'). LOWER THE THRESHOLD IF THE GATE IS CUTTING OUT TOO MUCH DESIRED SIGNAL, RAISE IT IF TOO MUCH EXTRANEOUS NOISE IS ENTERING THE OUTPUT SIGNAL.
  kgate   port     kgate, 0.01                   ; DAMP THE OPENING AND CLOSING OF THE GATE SLIGHTLY
  agate   interp   kgate                         ; INTERPOLATE GATE VALUE AND CREATE AN A-RATE VERSION
  aMod1   *=       agate                         ; APPLY THE GATE TO THE MODULATOR SIGNAL
  aMod2   *=       agate                         ; APPLY THE GATE TO THE MODULATOR SIGNAL
 endif
 
 aCar1    =        0
 aCar2    =        0

 if gkPlay2==1 then
  aCar1   =    gaCar1
  aCar2   =    gaCar2
 elseif gkCarSource==1 then                            ;  IF 'SYNTH' IS CHOSEN AS CARRIER SOURCE...
  aCar1   =    gaSyn                             ;  ...ASSIGN SYNTH SIGNAL FROM INSTR 2 AS CARRIER SIGNAL
  aCar2   =    gaSyn                             ;  ...ASSIGN SYNTH SIGNAL FROM INSTR 2 AS CARRIER SIGNAL
 else
  aCar1 inch 2
  aCar2 =    1
 endif

 ktrig    changed    gknum                       ; IF ANY OF THE INPUT VARIABLE ARE CHANGED GENERATE A MOMENTARY '1' VALUE (A BANG IN MAX-MSP LANGUAGE)
 if ktrig==1 then                                ; IF A CHANGED VALUE TRIGGER IS RECEIVED...
   reinit UPDATE                                 ; REINITIALISE THIS INSTRUMENT FROM THE LABEL 'UPDATE'
 endif                                           ; END OF THIS CONDITIONAL BRANCH
 UPDATE:                                         ; LABEL
 inum     init    i(gknum)                       ; CREATE AN INITIALISATION TIME VARIABLE FROM FLTK GUI CONTROL
     
 icount    init    0                             ; INITIALISE THE FILTER COUNTER TO ZERO
     amix1   VocoderChannel    aMod1,aCar1,gksteepness,gkbase,gkbw,gkincr,icount,inum    ;CALL 'VocoderChannel' UDO - (WILL RECURSE WITHIN THE UDO ITSELF FOR THEW REQUIRED NUMBER OF FILTERS
     amix2   VocoderChannel    aMod2,aCar2,gksteepness,gkbase,gkbw,gkincr,icount,inum    ;CALL 'VocoderChannel' UDO - (WILL RECURSE WITHIN THE UDO ITSELF FOR THEW REQUIRED NUMBER OF FILTERS
 rireturn
 
 ;HIGH-PASS CHANNEL
 kHPcf    =        cpsmidinn(gkbase+(inum*gkincr)+1) ; HIGHPASS FILTER CUTOFF (ONE INCREMENT ABOVE THE HIGHEST BANDPASS FILTER)
 kHPcf    limit    kHPcf,2000,18000                ; LIMIT THE HIGHPASS FILTER TO BE WITHIN SENSIBLE LIMITS
 
 aModHP1    buthp    aMod1, kHPcf                  ; HIGHPASS FILTER THE MODULATOR
 aModHP2    buthp    aMod2, kHPcf                  ; HIGHPASS FILTER THE MODULATOR
 aEnv       follow2  aModHP1+aModHP2,0.01,0.01     ; FOLLOW THE HIGHPASS FILTERED MODULATOR'S AMPLITUDE ENVELOPE
 aCarHP1    buthp    aCar1, kHPcf                  ; HIGHPASS FILTER THE CARRIER
 aCarHP2    buthp    aCar2, kHPcf                  ; HIGHPASS FILTER THE CARRIER
 amix1    =    ((amix1*gkBPGain*5)+(aCarHP1*aEnv*gkHPGain*3))    ;MIX THE HIGHPASS FILTERED CARRIER WITH THE BANDPASS FILTERS. APPLY THE MODULATOR'S ENVELOPE.
 amix2    =    ((amix2*gkBPGain*5)+(aCarHP2*aEnv*gkHPGain*3))    ;MIX THE HIGHPASS FILTERED CARRIER WITH THE BANDPASS FILTERS. APPLY THE MODULATOR'S ENVELOPE.

 ;amix1    balance   amix1, aMod1
 ;amix2    balance   amix2, aMod2
 amix1    *=        gklevel
 amix2    *=        gklevel
 
     outs     amix1,amix2                         ; SEND AUDIO TO THE OUTPUTS
     clear    gaSyn                               ; CLEAR THE INTERNAL SYNTH ACCUMULATING GLOBAL VARIABLE, READ FOR THE NEXT PERF. PASS

endin





instr    98
 Smessage sprintfk "file(%s)", gSfilepath1            ; print sound file image to fileplayer
 chnset Smessage, "filer1"

 /* write file name to GUI */
 Sname FileNameFromPath    gSfilepath1                ; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname                ; create string to update text() identifier for label widget
 chnset Smessage, "stringbox1"                    ; send string to  widget
 gkFileChans1 init  filenchnls:i(gSfilepath1)
endin

instr    99
 Smessage sprintfk "file(%s)", gSfilepath2            ; print sound file image to fileplayer
 chnset Smessage, "filer2"

 /* write file name to GUI */
 Sname FileNameFromPath    gSfilepath2                ; Call UDO to extract file name from the full path
 Smessage sprintfk "text(%s)",Sname                ; create string to update text() identifier for label widget
 chnset Smessage, "stringbox2"                    ; send string to  widget
 gkFileChans2 init  filenchnls:i(gSfilepath2)
endin


instr 2000 ; meter and spectrogram

a1,a2 monitor

; meter
if   metro:k(10)==1 then
reinit REFRESH_METER
endif
REFRESH_METER:
kres init    0
kres limit   kres-0.001,0,1 
kres peak    a1                            
rireturn
chnset kres, "vMeter1"              
if release:k()==1 then
 chnset k(0), "vMeter1"              
endif

endin


</CsInstruments>

<CsScore>
i 1    0 [60*60*24*7]    ;READ IN WIDGETS
i 10   0 [60*60*24*7]    ;VOCODER
i 2000 0 [60*60*24*7]    ;METER
</CsScore>

</CsoundSynthesizer>