
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; GateReverb.csd
; Written by Iain McCurdy, 2015.

; This effect implements a gate reverb effect using a multitap delay.
; The number of delay taps can be very large so their times are not defined explicitly but instead are derived from a number of global input parameters.

; Duration    -    the duration (in seconds) of a gate period
; N.Taps    -    the number of taps within the gate period (NB. increasing this parameter increases CPU and RAM demand)

; Scat.Amp.    -    random scattering of tap amplitudes
; Shape        -    envelope shaping of the gate reverb: 0=flat, <0=decay, >0=build
; Scat.Tim.    -    Scattering of delay tap times.

; Input/Ouput    -    input/output modes:
;             Mono        -    input taken from left channel input, a single processed channel is sent to both outputs
;             Stereo Match    -    stereo in / stereo out. 
;                        Random values for left and right channels used for amplitude scattering and delay tap time scattering match resulting in a narrower stereo effect.
;             Stereo Diff    -    stereo in / stereo out. 
;                        Random values for left and right channels used for amplitude scattering and delay tap time scattering differ resulting in a wider stereo effect.
;             (NB both stereo modes will employ double the number of taps specified by 'N.Taps' thereby doubling CPU and RAM usage for this function with respect to the mono mode.)
; Delay Dry    -    When activated the dry signal is delayed by the amount specified by 'Duration'. This can be particularly useful when using a 'build' reverb shape with a 50:50 dry/wet mix.

; Filter option filters each tap at a randomly selected cutoff frequency
; **Note that N.Taps may need to be reduced if CPU overloads**
; Choose between 'Bypass' (no filter), 'Lowpass' (moogvcf2 resonant filter) or 'bandpass' (butbp)
; Min.        -    minimum limit for random cutoff frequency selection
; Max.        -    maximum limit for random cutoff frequency selection
; Q.        -    Filter quality: Lowpass=resonance Bandpass=bandwidth


; Mix        -    dry/wet mix. 
;             Note that on account of the accumulation of overlapping delay taps, the wet signal is liable to be much louder than the dry signal.
;             This can be compensated for using the dry/wet control.
; Level        -    output level

<Cabbage>
form caption("Gate Reverb"), size(850, 85), pluginId("GtRv"), colour(13, 50, 67,50)
image              bounds(  0,  0,850, 85), colour(DarkSlateGrey),  shape("sharp")

#define RSliderStyle # colour(150,205,150), textColour(200,250,200), outlineColour(140,140,140), trackerColour(yellow), fontColour(250,250,150)#

image   bounds(  0,  0,135, 85), colour(0,0,0,0), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("delays"){
rslider bounds(  0,  5, 75, 75), channel("TotDel"), text("Duration"), range(0.01,4,0.4,0.5,0.01), valueTextBox(1), textBox(1), $RSliderStyle
rslider bounds( 60,  5, 75, 75), channel("TotNum"), text("N.Taps"),   range(1,300,100,1,1), valueTextBox(1), textBox(1), $RSliderStyle
}

image   bounds(133,  0,194, 85), colour(0,0,0,0), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("scatter"){
rslider bounds(  0,  5, 75, 75), channel("ScatAmp"),text("Scat.Amp."), range(0,1,0.5),  valueTextBox(1), textBox(1), $RSliderStyle
rslider bounds( 60,  5, 75, 75), channel("Shape"),text("Shape"), range(-20,20,0,1,0.1), valueTextBox(1), textBox(1), $RSliderStyle
rslider bounds(120,  5, 75, 75), channel("ScatTim"),text("Scat.Tim."), range(0,1,0.5),  valueTextBox(1), textBox(1), $RSliderStyle
}

image    bounds(325,  0,112, 85), colour(0,0,0,0), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("InputOutputMode"){
label    bounds(  8,  8, 95, 12), text("Input/Output"), fontColour(200,250,200)
combobox bounds(  8, 21, 95, 20), channel("InOutMode"), text("Mono","Stereo Match","Stereo Diff."), value(3), fontColour(250,250,150)
checkbox bounds(  8, 50, 95, 12), channel("DelayDry"), text("Delay Dry"), value(0), fontColour:0(250,250,150), fontColour:1(250,250,150), colour("yellow")
}

image    bounds(435,  0,282, 85), colour(0,0,0,0), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("Filter"){
label    bounds(  8,  8, 85, 12), text("Filter"), fontColour(200,250,200)
combobox bounds(  8, 21, 85, 20), channel("FiltSelect"), text("Bypass","Lowpass","Bandpass"), value(1), fontColour(250,250,150)
rslider bounds( 90,  5, 75, 75), channel("FiltMin"),    text("Min."),      range(4,13.5,4,1,0.1),    valueTextBox(1), textBox(1), $RSliderStyle
rslider bounds(150,  5, 75, 75), channel("FiltMax"),    text("Max."),      range(4,13.5,13.5,1,0.1), valueTextBox(1), textBox(1), $RSliderStyle
rslider bounds(210,  5, 75, 75), channel("Q"),          text("Q."),        range(0,1,0.1),           valueTextBox(1), textBox(1), $RSliderStyle
}

image   bounds(715,  0,135, 85), colour(0,0,0,0), outlineColour("white"), outlineThickness(2), shape("sharp"), plant("mixer"){
rslider bounds(  0,  5, 75, 75), channel("Mix"),    text("Mix"),      range(0,1.00,0.5), valueTextBox(1), textBox(1), $RSliderStyle
rslider bounds( 60,  5, 75, 75), channel("Level"),  text("Level"),    range(0,1,0.7),    valueTextBox(1), textBox(1), $RSliderStyle
}

</Cabbage>
                    
<CsoundSynthesizer>

<CsOptions>   
-dm0 -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32
nchnls         =     2
0dbfs        =    1
        seed    0
        
; RANDOM DISTRIBUTION - UNIFORM VALUES 0 TO 1
giRnd    ftgen    0, 0, 4096, 21, 1, 1

; UDO
opcode    Taps,a,iiiiiikkkko
 iInOutMode,iTotDel,iScatAmp,iShape,iScatTim,iTotNum,kFiltSelect,kFiltMin,kFiltMax,kQ,iCount    xin
 aMix,aOut    init    0                                ; INITIALISE THIS TAP AND MIX OF ALL OTHER TAPS
 if iInOutMode==2 then                                    ; IF STEREO MATCHED INPUT/OUTPUT MODE
  iRndDel    table    iCount,giRnd                            ; READ A RANDOM DELAY OFFSET VALUE FROM RANDOM DISTRIBUTION TABLE (FIXED LAYER BY LAYER) 
  iRndDel    =    ((iRndDel*2)-1) * ((iTotDel*iScatTim)/iTotNum)            ; RESCALE DELAY OFFSET VALUE
  iRndAmp    table    iCount+1000,giRnd                        ; READ RANDOM AMPLITUDE VALUE FROM TABLE
  iRndAmp    =    1 - (iRndAmp * iScatAmp)                    ; RESCALE RANDOM AMPLITUDE VALUE
 else                                            ; OTHERWISE (RANDOM DIFF. OR MONO MODE SELECTED) 
  iRndAmp    random    (1-iScatAmp),1                             ; CREATE A RANDOM AMPLITUDE VALUE
  iRndDel    random    -(iTotDel*iScatTim)/iTotNum, (iTotDel*iScatTim)/iTotNum        ; CREATE A RANDOM DELAY TAP OFFSET VALUE
 endif 
 iDel        limit    ((iTotDel/iTotNum)*iCount)+iRndDel,ksmps/sr,iTotDel        ; LIMIT DELAY TIMES SO THAT IT WON'T EXCEED BUFFER SIZE OR BE ZERO OR LESS
 aOut        deltapi    iDel                                ; CREATE DELAY TAP

 /* FILTER */
 iRndCFOct    random    i(kFiltMin),i(kFiltMax)
 iCF        =    cpsoct(iRndCFOct)
 if kFiltSelect==2 then
  aOut        moogvcf2    aOut,iCF,kQ*0.9
 elseif kFiltSelect==3 then
  aOut        butbp    aOut,iCF,(((1-kQ)*2) + 0.05)*iCF, 1
 endif

 if iShape!=0 then                                    ; IF 'SHAPE' VALUE IS NOT ZERO
  if iShape>0 then                                    ; IF A POSITIVE VALUE... (BUILD SHAPE)
   aOut    *=    (iCount/iTotNum)^iShape                            ; AMPLITUDE SCALED ACCORDING TO TAP NUMBER
  else                                            ; OTHERWISE... (DECAY SHAPE)
   aOut    *=    ((iTotNum-iCount)/iTotNum)^abs(iShape)                  ; AMPLITUDE SCALED ACCORDING TO TAP NUMBER (REVERSED)
  endif
 endif
 if iCount<iTotNum then                                    ; IF NUMBER OF REQUIRED TAPS IS NOT YET COMPLETED...
  aMix        Taps    iInOutMode,iTotDel,iScatAmp,iShape,iScatTim,iTotNum,kFiltSelect,kFiltMin,kFiltMax,kQ,iCount+1    ; CALL FURTHER LAYER(S) FOR ADDITIONAL TAPS AS REQUIRED (INCREMENT COUNTER)
 endif
 iPol    =       (round(random:i(0, 1)) * 2) -1
         xout    (aOut*(iRndAmp^2)*iPol)+aMix                        ; ADD THIS TAP (AND APPLY RANDOM AMPLITUDE FOR THIS TAP) WITH ALL SUBSEQUENT TAPS
endop

instr    1
 kInOutMode    chnget    "InOutMode"                            ; READ IN WIDGETS
 kInOutMode    init    1                                ; INIT (TO PREVENT) A ZERO AT I-TIME
 kTotNum    chnget    "TotNum"
 kTotNum    init    100
 kTotDel    chnget    "TotDel"
 kTotDel    init    0.4
 kMix        chnget    "Mix"
 kLevel        chnget    "Level"                                                                       
 kScatAmp    chnget    "ScatAmp"
 kShape        chnget    "Shape"
 kScatTim    chnget    "ScatTim"
 kDelayDry    chnget    "DelayDry"
 kFiltSelect    chnget    "FiltSelect"
 kFiltMin    chnget    "FiltMin"        
 kFiltMax    chnget    "FiltMax"
 kQ        chnget    "Q"
         
 aL,aR        ins                                    ; READ IN STEREO INPUT 

 if changed(kInOutMode,kTotNum,kTotDel,kScatAmp,kShape,kScatTim,kDelayDry,kFiltMin,kFiltMax)==1 then        ; FORCE A REINITIALISATION IF ANY OF THE LISTED K-VARIABLES CHANGE
          reinit    RESTART
 endif
 RESTART:

 if i(kInOutMode)==1 then                                    ; IF MONO MODE IS SELECTED CREATE JUST A SINGLE CHANNEL AND SEND IT TO BOTH OUTPUTS
  aBuf        delayr    i(kTotDel)                                 ; SET UP BUFFER
  aDelL        Taps    i(kInOutMode),i(kTotDel),i(kScatAmp),i(kShape),i(kScatTim),i(kTotNum),kFiltSelect,kFiltMin,kFiltMax,kQ    ; CALL UDO
         delayw    aL                                    ; WRITE INTO BUFFER
  if i(kDelayDry)==1 then
   aL    delay    aL,i(kTotDel)
  endif
  aMix        ntrpol    aL,aDelL,kMix                                ; MIX DRY AND WET SIGNALS
     outs    aMix*kLevel, aMix*kLevel                            ; SEND SAME SIGNAL TO BOTH CHANNELS
 else                                                ; OTHERWISE (EITHER OF THE STEREO MODES HAS BEEN SELECTED)
  aBuf        delayr    i(kTotDel)                                 ; LEFT CHANNEL...
  aDelL        Taps    i(kInOutMode),i(kTotDel),i(kScatAmp),i(kShape),i(kScatTim),i(kTotNum),kFiltSelect,kFiltMin,kFiltMax,kQ    ;
         delayw    aL                                    ;
  aBuf        delayr    i(kTotDel)                                 ; RIGHT CHANNEL...
  aDelR        Taps    i(kInOutMode),i(kTotDel),i(kScatAmp),i(kShape),i(kScatTim),i(kTotNum),kFiltSelect,kFiltMin,kFiltMax,kQ    ;
         delayw    aR                                    ;
  if i(kDelayDry)==1 then
   aL    delay    aL,i(kTotDel)
   aR    delay    aR,i(kTotDel)
  endif
  aMixL        ntrpol    aL,aDelL,kMix                                ; MIX DRY AND WET SIGNALS FOR BOTH CHANNELS SEPARATELY
  aMixR        ntrpol    aR,aDelR,kMix                                ;
     outs    aMixL*kLevel, aMixR*kLevel                            ; STEREO OUTPUT
 endif
 
endin

</CsInstruments>

<CsScore>
i 1 0 3600
</CsScore>

</CsoundSynthesizer>
