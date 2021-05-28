
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; BreakBeatCutter.csd
; Iain McCurdy, 2013.

; Break Beat Cut Up using the bbcut opcode with additional processing

; ==BBCUT=================================================================================================================
; 'Sub-division' determines the note duration used as the base unit in  cut-ups. 
; For example a value of 8 represents quavers (eighth notes), 16 represents semiquavers (sixteenth notes) and so on.                                                   
; 
; 'Bar Length' represents the number of beats per bar. For example, a value of 4 represents a 4/4 bar and so on. 
; 
; 'Phrase' defines the number of bars that will elapse before the cutting up pattern restarts from the beginning.          
; 
; 'Stutter' is a separate cut-up process which occasionally will take a very short fragment of the input audio and repeat
; it many times. 
; 
; 'Stutter Speed' defines the duration of each stutter in relation to 'Sub-division'. 
; If subdivision is 8 (quavers / eighth notes) and 'Stutter Speed' is 2 then each stutter will be a semiquaver / sixteenth note.
; 
; 'Stutter Chance' defines the frequency of stutter moments. 
; The range for this parameter is 0 to 1. Zero means stuttering will be very unlikely, 1 means it will be very likely.       
; 'Repeats' defines the number of repeats that will be employed in normal cut-up events.                                     
; When processing non-rhythmical, unmetered material it may be be more interesting to employ non-whole numbers for parameters such as 'Sub-division', 'Phrase' and 'Stutter Speed'.                                                      
; ========================================================================================================================




; ==FILTER================================================================================================================
; Additionally in this example a randomly moving band-pass filter has been implemented. 
; 
; 'Filter Mix' crossfades between the unfiltered bbcut signal and the filtered bbcut signal.   
; 
; 'Cutoff Freq.' consists of two small sliders which determine the range from which random cutoff values are derived.       
; 
; 'Interpolate<=>S&H' fades continuously between an interpolated random function and a sample and hold type random function. 
; 
; 'Filter Div.' controls the frequency subdivision with which new random cutoff frequency values are generated - a value of '1' means that new values are generated once every bar.                                    
; ========================================================================================================================



; ==WGUIDE================================================================================================================
; A waveguide effect can randomly and rhythmically cut into the audio stream
; 'Chance' defines the probability of this happening. 0=never 1=always
; The range of frequencies the effect will choose from is defined by the user as note values.
; Frequencies are quatised to adhere to equal temperament.
; ========================================================================================================================



; ==SQUARE MOD. (Square wave ring modulation)=============================================================================
; This effect can similarly randomly and rhythmically cut into the audio stream using the 'chance' control
; The range of frequencies the modulator waveform can move between is defined as 'oct' values.
; ========================================================================================================================


; ==F.SHIFT (Frequency Shifter)===========================================================================================
; Similar to the above except using a frequency shifter effect.
; ========================================================================================================================

<Cabbage>
form          size(440,470), caption("Break Beat Cutter"), pluginId("bbct")


groupbox bounds( 0,  0,440,170), text("CUTTER"), plant("cutter"),colour(20,20,20), FontColour(silver){
rslider bounds( 10, 25, 70, 70), colour("Tan"), trackerColour("Tan"), fontColour("silver"), text("Sub-div."),       channel("subdiv"),  range(1,  512,  8, 1, 1)
rslider bounds( 80, 25, 70, 70), colour("Tan"), trackerColour("Tan"), fontColour("silver"), text("Bar Length"),     channel("barlen"),  range(1,   16,  2, 1, 1)
rslider bounds(150, 25, 70, 70), colour("Tan"), trackerColour("Tan"), fontColour("silver"), text("Phrase"),         channel("phrase"),  range(1, 512, 8, 1, 1)
rslider bounds(220, 25, 70, 70), colour("Tan"), trackerColour("Tan"), fontColour("silver"), text("Repeats"),        channel("repeats"), range(1, 32, 2, 1, 1)
rslider bounds(290, 25, 70, 70), colour("Tan"), trackerColour("Tan"), fontColour("silver"), text("Stut. Speed"),  channel("stutspd"), range(1, 32, 4, 1, 1)
rslider bounds(360, 25, 70, 70), colour("Tan"), trackerColour("Tan"), fontColour("silver"), text("Stut. Chance"), channel("stutchnc"), range(0, 1.00, 0.5)
hslider bounds(  5, 95,425, 40), colour("Tan"), trackerColour("Tan"), fontColour("silver"), textBox(1)    channel("BPM"), range(10,  500, 110,1,1)
label   bounds(198,127, 25, 11), text("BPM"), FontColour("silver")
label  bounds( 10,142, 80,12), text("Clock Source:")
button bounds( 90,140, 60,18), text("Internal","External"), channel("ClockSource"), value(0)
label   bounds(338,158, 100, 10), text("Iain McCurdy [2013]"), FontColour("grey"), align("right")
}



groupbox bounds( 0,170,440,100), text("FILTER"), plant("filter"), FontColour(silver),colour(20,20,20){
rslider bounds( 10, 25, 70, 70), colour(200,100,50,255), trackerColour(200,100,50,255), fontColour("silver"), text("Mix"),    channel("FltMix"), range(0, 1.00, 0.6)
rslider bounds( 80, 25, 70, 70), colour(200,100,50,255), trackerColour(200,100,50,255), fontColour("silver"), text("Division"),    channel("fltdiv"), range(1, 16, 1,1,1)
rslider bounds(150, 25, 70, 70), colour(200,100,50,255), trackerColour(200,100,50,255), fontColour("silver"), text("Bandwidth"),    channel("bw"), range(0.1, 10, 1, 0.5, 0.001)
hslider bounds(220, 25,140, 35), colour(200,100,50,255), trackerColour(200,100,50,255), fontColour("silver"),    channel("cfmin"), range(50, 10000, 50  ,0.5,0.1)
hslider bounds(220, 50,140, 35), colour(200,100,50,255), trackerColour(200,100,50,255), fontColour("silver"),    channel("cfmax"), range(50, 10000, 10000,0.5,0.1)
label   bounds(254, 77, 80, 12), text("Cutoff Freq."), FontColour("white")
rslider bounds(360, 25, 70, 70), colour(200,100,50,255), trackerColour(200,100,50,255), fontColour("silver"), text("Int./S&H"),    channel("i_h"), range(0, 1, 0)
}

groupbox bounds( 0,270,220,100), text("WAVE GUIDE"), plant("waveguide"), FontColour(silver),colour(20,20,20){
rslider bounds( 10, 25, 70, 70), colour(150,150,50,255), trackerColour(150,150,50,255), fontColour("silver"), text("Chance"),    channel("WguideChnc"), range(0, 1.00, 0.2)
hslider bounds( 80, 25,140, 35), colour(150,150,50,255), trackerColour(150,150,50,255), fontColour("silver"),    channel("wguidemin"), range(22, 100, 50,1,1)
hslider bounds( 80, 50,140, 35), colour(150,150,50,255), trackerColour(150,150,50,255), fontColour("silver"),    channel("wguidemax"), range(22, 100, 70,1,1)
label   bounds(108, 77, 85, 12), text("Pitch Range"), FontColour("white")
}

groupbox bounds(220,270,220,100), text("SQUARE MOD."), plant("sqmod"), FontColour(silver),colour(20,20,20){
rslider bounds( 10, 25, 70, 70), colour(200,150,200,255), trackerColour(200,150,200,255), fontColour("silver"), text("Chance"),    channel("SqModChnc"), range(0, 1.00, 0.2)
hslider bounds( 80, 25,140, 35), colour(200,150,200,255), trackerColour(200,150,200,255), fontColour("silver"),    channel("sqmodmin"), range(1, 14.0,  6)
hslider bounds( 80, 50,140, 35), colour(200,150,200,255), trackerColour(200,150,200,255), fontColour("silver"),    channel("sqmodmax"), range(1, 14.0, 12)
label   bounds(108, 77, 85, 12), text("Freq.Range"), FontColour("white")
}

groupbox bounds(0,370,220,100), text("FREQUENCY SHIFT"), plant("fshift"), FontColour(silver),colour(20,20,20){
rslider bounds( 10, 25, 70, 70), colour(250,110,250,255), trackerColour(250,110,250,255), fontColour("silver"), text("Chance"),    channel("FshiftChnc"), range(0, 1.00, 0.2)
hslider bounds( 80, 25,140, 35), colour(250,110,250,255), trackerColour(250,110,250,255), fontColour("silver"),    channel("fshiftmin"), range(-4000, 4000,-1000)
hslider bounds( 80, 50,140, 35), colour(250,110,250,255), trackerColour(250,110,250,255), fontColour("silver"),    channel("fshiftmax"), range(-4000, 4000, 1000)
label   bounds(108, 77, 85, 12), text("Freq.Range"), FontColour("white")
}

groupbox bounds(220,370,220,100), text("OUTPUT"), plant("output"), FontColour(silver),colour(20,20,20){
rslider bounds( 10, 25, 70, 70), colour("Tan"), trackerColour("Tan"), fontColour("silver"), text("Layers"),       channel("layers"), range(1, 20, 1,1,1)
rslider bounds( 80, 25, 70, 70), colour("Tan"), trackerColour("Tan"), fontColour("silver"), text("Dry/Wet"),      channel("DryWet"), range(0, 1.00, 0.6)
rslider bounds(150, 25, 70, 70), colour("Tan"), trackerColour("Tan"), fontColour("silver"), text("Level"),        channel("gain"),   range(0, 1.00, 0.75)
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-n -dm0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps = 64
nchnls = 2
0dbfs=1
seed    0

gisine    ftgen    0,0,131072,10,1

opcode    BBCutIteration,aa,aaiiiiiiiii
 aL,aR,iBPS, isubdiv,  ibarlen,  iphrase, irepeats, istutspd, istutchnc, icount, ilayers    xin
 abbcutL        bbcutm    aL,   iBPS, isubdiv,  ibarlen,  iphrase, irepeats, istutspd, istutchnc
 abbcutR        bbcutm    aR,   iBPS, isubdiv,  ibarlen,  iphrase, irepeats, istutspd, istutchnc
 amixL    =    0
 amixR    =    0
 if icount<ilayers then
   amixL,amixR    BBCutIteration    aL,aR, iBPS, isubdiv,  ibarlen,  iphrase, irepeats, istutspd, istutchnc, icount+1, ilayers
 endif
 xout    abbcutL+amixL,abbcutR+amixL
endop

opcode    FreqShifter,a,aki
    ain,kfshift,ifn    xin                    ;READ IN INPUT ARGUMENTS
    areal, aimag hilbert ain                ;HILBERT OPCODE OUTPUTS TWO PHASE SHIFTED SIGNALS, EACH 90 OUT OF PHASE WITH EACH OTHER
    asin     oscili       1,    kfshift,     ifn,          0
    acos     oscili       1,    kfshift,     ifn,          0.25    
    ;RING MODULATE EACH SIGNAL USING THE QUADRATURE OSCILLATORS AS MODULATORS
    amod1    =        areal * acos
    amod2    =        aimag * asin    
    ;UPSHIFTING OUTPUT
    aFS    = (amod1 - amod2)
        xout    aFS                ;SEND AUDIO BACK TO CALLER INSTRUMENT
endop

instr 1    ; read widgets
 gksubdiv     chnget    "subdiv"    ; read in widgets
 gkbarlen     chnget    "barlen"
 gkphrase     chnget    "phrase"
 gkrepeats    chnget    "repeats"
 gkstutspd    chnget    "stutspd"
 gkstutchnc    chnget    "stutchnc"

 gkClockSource    chnget    "ClockSource"
 if gkClockSource==0 then
  gkBPM        chnget    "BPM"
 else
  gkBPM        chnget    "HOST_BPM"
  gkBPM        limit    gkBPM, 10,500    
 endif

 gkfltdiv    chnget    "fltdiv"
 gkDryWet        chnget    "DryWet"
 gkFltMix     chnget    "FltMix"
 gkbw         chnget    "bw"
 gkcfmin      chnget    "cfmin"
 gkcfmax      chnget    "cfmax"
 gki_h        chnget    "i_h"
 gklayers        chnget    "layers"
 gkgain        chnget    "gain"
 konoff        chnget    "onoff"
endin

instr    2
 kmetro        metro        4
 kSwitch    init    0
 if kmetro==1 then
  kSwitch    changed        gkBPM, gkrepeats, gkphrase, gkstutspd, gkstutchnc, gkbarlen, gksubdiv, gkfltdiv, gklayers    ;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
 endif
 if    kSwitch==1    then        ;IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
     reinit    UPDATE            ;BEGIN A REINITIALISATION PASS FROM LABEL 'UPDATE'
 endif
 UPDATE:

 /* INPUT */
 aL,aR ins                    ; live input

;;;;  use a loop sound file for testing
;#define SOUNDFILE    #"loop.wav"#        ; macro
; ilen    filelen    $SOUNDFILE            ; length of sound file
; ibeats =    8                ; number of beats in the drum loop
; ispd    =    (ilen/ibeats) * (i(gkBPM)/60)    ; speed ratio for this sample to sync with master clock
; aL    diskin    $SOUNDFILE,ispd,0,1        ; read sound file
; aR    =    aL                ; right channel same as left

 iBPS    =    i(gkBPM)/60
 kmetro    metro    iBPS        ; metronome used for triggering random parameter changes
 
 ; call UDO
 ;OUTPUT        OPCODE            INPUT |   BPM      | SUBDIVISION | BAR_LENGTH | PHRASE_LENGTH | NUM.OF_REPEATS | STUTTER_SPEED | STUTTER_CHANCE    
 abbcutL,abbcutR    BBCutIteration    aL,aR, i(gkBPM)/60, i(gksubdiv),  i(gkbarlen),   i(gkphrase),    i(gkrepeats),   i(gkstutspd),   i(gkstutchnc),  1, i(gklayers)
 
 ;FILTER=================================================================================================================================================================
 ifreq    =    iBPS * i(gkfltdiv)            ; FREQUENCY WITH WHICH NEW FILTER CUTOFF VALUES ARE GENERATED
 
 kcf1h        randomh    gkcfmin, gkcfmax, ifreq        ; sample and hold random frequency values
 kcf1i        lineto    kcf1h, 1/ifreq            ; interpolate values
 kcf1        ntrpol    kcf1i, kcf1h, gki_h           ; crossfade between interpolating and sample and hold type random values
 abbFltL    resonz    abbcutL, kcf1, kcf1*gkbw, 2    ; band-pass filter
 abbMixL    ntrpol    abbcutL, abbFltL, gkFltMix    ; crossfade between unfiltered and filter audio signal
 kcf2h        randomh    gkcfmin, gkcfmax, ifreq        ;   RIGHT CHANNEL
 kcf2i        lineto    kcf2h, 1/ifreq            ; 
 kcf2        ntrpol    kcf2i, kcf2h, gki_h           ; 
 abbFltR    resonz    abbcutR, kcf2, kcf2*gkbw, 2    ; 
 abbMixR    ntrpol    abbcutR, abbFltR, gkFltMix    ; 
 ;=======================================================================================================================================================================

 ;WGUIDE1================================================================================================================================================================
 kchance    chnget    "WguideChnc"
 kdice        trandom    kmetro,0,1
 if kdice<kchance then
  kwguidemin    chnget    "wguidemin"
  kwguidemax    chnget    "wguidemax"
  knum        randomh    kwguidemin,kwguidemax,iBPS
  afrq        interp    cpsmidinn(int(knum))
  kfb        randomi    0.8,0.99,iBPS/4
  kcf        randomi    800,4000,iBPS
  abbMixL    wguide1    abbMixL*0.7,afrq,kcf,kfb
  abbMixR    wguide1    abbMixR*0.7,afrq,kcf,kfb
 endif  
 ;=======================================================================================================================================================================

 ;SQUARE MOD==============================================================================================================================================================
 kchance    chnget    "SqModChnc"            ; read in widgets
 ksqmodmin    chnget    "sqmodmin"            ;
 ksqmodmax    chnget    "sqmodmax"            ; 
 kDiceRoll    trandom    kmetro,0,1            ; new 'roll of the dice' upon each new time period
 if kDiceRoll<kchance then                ; if 'roll of the dice' is within chance boundary... 
  kratei    randomi    ksqmodmin,ksqmodmax,iBPS    ; interpolating random function for modulating waveform frequency
  krateh    randomh    ksqmodmin,ksqmodmax,iBPS    ; sample and hold random function for modulating waveform frequency
  kcross    randomi    0,1,iBPS            ; crossfader for morphing between interpolating and S&H functions
  krate        ntrpol    kratei,krateh,kcross        ; create crossfaded rate function
  amod        lfo    1,cpsoct(krate),2        ; modulating waveform (square waveform)
  kcf        limit    cpsoct(krate)*4,20,sr/3        ; cutoff freq for filtering some of the high freq. content of the square wave
  amod        clfilt    amod,kcf,0,2            ; low-pass filter square wave
  abbMixL    =    abbMixL*amod            ; ring modulate audio
  abbMixR    =    abbMixR*amod            ;
 endif
 ;=======================================================================================================================================================================

 ;FSHIFT=================================================================================================================================================================
 kchance    chnget    "FshiftChnc"            ; read in widgets                         
 kdice        trandom    kmetro,0,1            ; new 'roll of the dice' upon each new time period                                                                         
 if kdice<kchance then                    ; if 'roll of the dice' is within chance boundary...                                                                           
  kfshiftmin    chnget    "fshiftmin"                     ; read in widgets                           
  kfshiftmax    chnget    "fshiftmax"            ; 
  kfsfrqi    randomi    kfshiftmin,kfshiftmax,iBPS*2    ; interpolating random function for modulating waveform frequency          
  kfsfrqh    randomh    kfshiftmin,kfshiftmax,iBPS*2    ; sample and hold random function for modulating waveform frequency            
  kcross    randomi    0,1,iBPS*2            ; crossfader for morphing between interpolating and S&H functions          
  kfsfrq    ntrpol    kfsfrqi,kfsfrqh,kcross          ; create crossfaded rate function  modulating waveform (square waveform)
  abbMixL    FreqShifter    abbMixL,kfsfrq,gisine   ;                                                 
  abbMixR    FreqShifter    abbMixR,kfsfrq,gisine   ;                                                         
 endif                                                  ;                                                                            
 ;=======================================================================================================================================================================

 rireturn                        ; RETURN FROM REINITIALISATION PASS TO PERFORMANCE TIME PASSES
 
 amixL    sum    aL*(1-gkDryWet), abbMixL*gkDryWet    ;SUM AND MIX DRY SIGNAL AND BBCUT SIGNAL (LEFT CHANNEL)
 amixR    sum    aR*(1-gkDryWet), abbMixR*gkDryWet    ;SUM AND MIX DRY SIGNAL AND BBCUT SIGNAL (RIGHT CHANNEL)




 outs    amixL*gkgain, amixR*gkgain            ;SEND AUDIO TO OUTPUTS
endin

</CsInstruments>  
<CsScore>
i1 0 36000
i2 0 36000
</CsScore>
</CsoundSynthesizer>