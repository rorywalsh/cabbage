
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Gate.csd
; Written by Iain McCurdy, 2015.

; Two 'Processing Configutions' are available:
;  'Stereo Mixed'       -   the two input channels are mixed before being sent to a single envelope follower. 
;                   Subsequent processing (gating filtering) is still carried out on the stereo input, just the gate open/closed control data will be the same on both channels
;  'Stereo Separate'        -   the two input channels are sent to independent envelope followers. 
;                   Therefore gate open/closed control data for the two channels can differ.  
; Right Channel Sidechain   -   In this mode, the gate is triggered by the signal received in the right channel but the gated signal is the left channel 

; Input signal is sent to an envelope follower and a gate state (open/closed) is assessed according to user defined thresholds.
; Independent thresholds and time durations can be defined for gate opening and closing.

; Pre-filter filters the input signal through a highpass filter and lowpass filter in series. 
; (Note this is only the signal sent into the envelope follower and the signal sent to the actual gate and the then the output is unfiltered.)
; This feature can be used to fine tune gate opening for particular frequency bands. 
; E.G. Opening for a voice singing but not for low frequency rumble picked up through the microphone stand. 

; The 'Filter Gate' can gate the signal using a lowpass filter (either 12 dB/oct or 24 dB/oct). 
; The user sets the 'Min.' (gate closed) and 'Max.' (gate open) cutoff values for the filter (in oct format)

; 'Atten.' sets the amount of amplitude attenuation to be applied.
; 'Delay' delays the audio (after envelope following but before gating is applied)
;   This can be used to recover the attack of a sound, particularly if the attack time of the gate is long.
;   If delay time is zero then the delay is completely bypassed.  
                                                                       
<Cabbage>                                                                                                                   
form caption("Gate"), colour( 20, 20, 30), size(430, 305), pluginId("Gate"), scrollBars(0)

image     bounds(  5, 10,420, 40), shape("sharp"), outlineColour("white"), colour(0,0,0,0), outlineThickness(1), plant("Input") {
label     bounds( 20, 10,180, 14), fontColour("white"), text("Processing Configuration:"), colour(0,0,0,0), align("right")
combobox  bounds(202, 10,180, 16), channel("InputMode"), text("Stereo Mixed","Stereo Separate","Right Channel Side Chain"), value(1), colour( 70, 70, 70), fontColour("white")
}

image     bounds(  5, 55,207, 90), shape("sharp"), outlineColour("white"), colour(0,0,0,0), outlineThickness(1), plant("OnThresh") {
label     bounds(  0,  5,207, 12), fontColour("white"), text("On Threshold"), colour(0,0,0,0)
rslider   bounds( 10, 20, 60, 60), range(0,0.1,0.01,0.5,0.0001),  channel("OnThresh"), text("Level"), textColour("white")
nslider   bounds( 65, 31, 45, 25), channel("OnThresh_dB"), range(-90,120,-90,1,0.1)
label     bounds(110, 37, 20, 12), fontColour("white"), text("dB"), colour(0,0,0,0)
rslider   bounds(135, 20, 60, 60), range(0,0.3,0.04,0.5,0.0001),  channel("AttTime"), text("Time"), textColour("white")
}

image     bounds(218, 55,207, 90), shape("sharp"), outlineColour("white"), colour(0,0,0,0), outlineThickness(1), plant("OffThresh") {
label     bounds(  0,  5,207, 12), fontColour("white"), text("Off Threshold"), colour(0,0,0,0)
rslider   bounds( 10, 20, 60, 60), range(0,0.1,0.007,0.5,0.0001),  channel("OffThresh"), text("Level"), textColour("white")
nslider   bounds( 65, 31, 45, 25), channel("OffThresh_dB"), range(-90,120,-90,1,0.1)
label     bounds(110, 37, 20, 12), fontColour("white"), text("dB"), colour(0,0,0,0)
rslider   bounds(135, 20, 60, 60), range(0,0.3,0.03,0.5,0.0001),  channel("RelTime"), text("Time"), textColour("white")
}

image    bounds(  5,150,420, 50), shape("sharp"), outlineColour("white"), colour(0,0,0,0), outlineThickness(1), plant("PreFilter") {
checkbox bounds( 10, 15, 70, 12), channel("PreFiltOnOff"), text("Pre-Filter"), fontColour("white")
;hslider2 bounds( 85,  0,330, 40), channel("HPF","LPF"), min(200), max(12000), range(20,20000, 0, 0.5, 1), identChannel("Filter")
hrange   bounds(  85,  5,330, 20), channel("HPF","LPF"), range(20, 20000, 200:12000, 0.5, 1), identChannel("Filter")
label    bounds( 85, 30,330, 12), text("Highpass / Lowpass"), fontColour("white"), identChannel("FilterL")
}

image    bounds(  5,205,210, 95), shape("sharp"), outlineColour("white"), colour(0,0,0,0), outlineThickness(1), plant("FilterGate") {
label    bounds(  5, 22, 80, 12), text("Filter Gate"), fontColour("white")
combobox bounds(  5, 35, 80, 20), channel("FilterGate"), text("Bypass","12 dB/Oct","24 dB/Oct"), value(1)
rslider  bounds( 75,  5, 80, 80), range(2,14,2,0.5,0.01),  channel("FiltGateMin"), text("Min"), textColour("white"), textBox(1), valueTextBox(1)
rslider  bounds(135,  5, 80, 80), range(2,14,14,0.5,0.01),  channel("FiltGateMax"), text("Max"), textColour("white"), textBox(1), valueTextBox(1)
}

image    bounds(220,205,205, 95), shape("sharp"), outlineColour("white"), colour(0,0,0,0), outlineThickness(1), plant("Master") {
checkbox bounds( 14, 30, 15, 13), channel("GateIndicOp"), shape("ellipse"), colour( 50,255, 50), active(0)
checkbox bounds( 14, 50, 15, 13), channel("GateIndicCl"), shape("ellipse"), colour(255, 50, 50), value(1), active(0)
rslider  bounds( 20,  5, 80, 80), range(0,90,90,0.5,0.1),  channel("Atten"), text("Atten."), textColour("white"), textBox(1), valueTextBox(1)
rslider  bounds( 75,  5, 80, 80), range(0,0.1,0,0.8,0.001),  channel("DelTim"), text("Delay"), textColour("white"), textBox(1), valueTextBox(1)
rslider  bounds(130,  5, 80, 80), range(0,2,1,0.5,0.01),  channel("Gain"), text("Gain"), textColour("white"), textBox(1), valueTextBox(1)
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n
</CsOptions>

<CsInstruments>

;sr is set by host
ksmps   =   32
nchnls  =   2
0dbfs   =   1

; Author: Iain McCurdy (2015)
                                     
opcode  SwitchPort, k, kkk
    kin, kupport, kdnport   xin
    kold        init        0
    kporttime   =           (kin<kold?kdnport:kupport)
    kout        portk       kin, kporttime
    kold        =           kout
                xout        kout
endop

instr   1
 /* READ IN WIDGETS */
 kOnThresh      chnget      "OnThresh"
 kOffThresh     chnget      "OffThresh"
 kAttTime       chnget      "AttTime"
 kRelTime       chnget      "RelTime"
 kAtten         chnget      "Atten"
 kGain          chnget      "Gain"
 kFilterGate    chnget      "FilterGate"
 kFiltGateMin   chnget      "FiltGateMin"
 kFiltGateMax   chnget      "FiltGateMax"
 kInputMode     chnget      "InputMode"
 kHPF           chnget      "HPF"
 kLPF           chnget      "LPF"
 kPreFiltOnOff  chnget      "PreFiltOnOff"
 kDelTim        chnget      "DelTim"
 
 kporttime      linseg      0, 0.001, 0.05        ;
 kGain          portk       kGain, kporttime

 /* INPUT */
 ;aL,aR          diskin2    "029_HPF.wav",1,0,1 ; use diskin for testing
 aL,aR          ins
 
 /* SHOW OR HIDE PRE-FILTER SLIDERS */
 if changed(kPreFiltOnOff)==1 then
  if kPreFiltOnOff==1 then
                chnset      "visible(1)","Filter"
                chnset      "visible(1)","FilterL"
  else
                chnset      "visible(0)","Filter"  
                chnset      "visible(0)","FilterL"  
  endif
 endif
 
 /* STEREO MIX MODE */
 if kInputMode==1 then
 
  aInSig        sum         aL, aR                   ; mix left and right inputs

  if kPreFiltOnOff==1 then                           ; if pre-filter switch is on...
   aInSig       buthp       aInSig, kHPF             ; highpass filter
   aInSig       buthp       aInSig, kHPF             ; and again to steepen slope
   aInSig       butlp       aInSig, kLPF             ; lowpass filter
   aInSig       butlp       aInSig, kLPF             ; and again to steepen slope
  endif

  kRMS          rms         aInSig                   ; scan rms of input signal

  /* OPEN AND CLOSE GATE*/
  kGate         init        1
  if kRMS < kOffThresh && kGate ==1 then
   kGate        =           0
  elseif kRMS >= kOnThresh && kGate == 0 then
   kGate        =           1
  endif
  
  /* TURN GATE STATUS INDICATORS ON AND OFF */
  if changed(kGate)==1 then
                chnset      1 - kGate, "GateIndicCl"
                chnset      kGate, "GateIndicOp"
  endif
   
  /* SMOOTH GATE OPENING AND CLOSING (CALL UDO) */
  kGateD        SwitchPort  kGate, kAttTime, kRelTime   ; smooth opening and closing
  
  /* AMPLITUDE GATE */
  kGateDA   scale   kGateD,1,ampdb(-kAtten) ; modify gating function according to user defined attenuation setting
  aGate interp  kGateDA                     ; create an arate version (smoother)
    
  /* DELAY */
  if kDelTim>0 then
   aL           vdelay      aL, kDelTim*1000, 100
   aR           vdelay      aR, kDelTim*1000, 100
  endif
   
  /* APPLY GATE */
  aL            *=          aGate
  aR            *=          aGate
  
  /* FILTER GATE */
  if kFilterGate>1 then             
   kcfoct       scale       kGateD,kFiltGateMax,kFiltGateMin
   acf          interp      cpsoct(kcfoct)
   if kFilterGate==2 then
    aL          tone        aL, acf
    aR          tone        aR, acf
   else
    aL          butlp       aL, acf
    aR          butlp       aR, acf
   endif
  endif                
  
 /* STEREO SEPARATE MODE */
 elseif kInputMode==2 then
 
 aInSigL        =           aL
 aInSigR        =           aR

  if kPreFiltOnOff==1 then
   aInSigL      buthp       aInSigL,kHPF
   aInSigL      buthp       aInSigL,kHPF
   aInSigL      butlp       aInSigL,kLPF
   aInSigL      butlp       aInSigL,kLPF
   aInSigR      buthp       aInSigR,kHPF
   aInSigR      buthp       aInSigR,kHPF
   aInSigR      butlp       aInSigR,kLPF
   aInSigR      butlp       aInSigR,kLPF
  endif


  kRMSL rms aInSigL*2
  kRMSR rms aInSigR*2

  kGateL,kGateR init    1

  if kRMSL < kOffThresh && kGateL ==1 then
   kGateL       =           0
  elseif kRMSL >= kOnThresh && kGateL == 0 then
   kGateL       =           1
  endif

  if kRMSR < kOffThresh && kGateR ==1 then
   kGateR       =           0
  elseif kRMSR >= kOnThresh && kGateR == 0 then
   kGateR       =           1
  endif
  
  if changed(kGateL)==1 then
                chnset       1 - kGateL, "GateIndicCl"
                chnset       kGateL, "GateIndicOp"
  endif
   
  kGateDL       SwitchPort   kGateL, kAttTime, kRelTime  ; smooth opening and closing
  kGateDR       SwitchPort   kGateR, kAttTime, kRelTime  ; smooth opening and closing
  
  kGateDAL      scale        kGateDL, 1, ampdb(-kAtten)
  kGateDAR      scale        kGateDR, 1, ampdb(-kAtten)
  
  aGateL        interp       kGateDAL
  aGateR        interp       kGateDAR
   
  /* DELAY */
  if kDelTim>0 then
   aL           vdelay       aL, kDelTim * 1000, 100
   aR           vdelay       aR, kDelTim * 1000, 100
  endif
   
  /* APPLY GATE */
  aL            *=           aGateL
  aR            *=           aGateR
  
  if kFilterGate>1 then
   kcfoctL      scale        kGateDL, kFiltGateMax, kFiltGateMin
   kcfoctR      scale        kGateDR, kFiltGateMax, kFiltGateMin
   acfL         interp       cpsoct(kcfoctL)
   acfR         interp       cpsoct(kcfoctR)
   if kFilterGate==2 then
    aL          tone         aL, acfL
    aR          tone         aR, acfR
   else
    aL          butlp        aL, acfL
    aR          butlp        aR, acfR
   endif
  endif

 ; Right Channel Side Chain
 else
 
  if kPreFiltOnOff==1 then                               ; if pre-filter switch is on...
   aR           buthp        aR, kHPF                    ; highpass filter
   aR           buthp        aR, kHPF                    ; and again to steepen slope
   aR           butlp        aR, kLPF                    ; lowpass filter
   aR           butlp        aR, kLPF                    ; and again to steepen slope
  endif


  kRMS          rms          aR                          ; scan rms of input signal

  /* OPEN AND CLOSE GATE*/
  kGate         init         1
  if kRMS < kOffThresh && kGate ==1 then
   kGate        =            0
  elseif kRMS >= kOnThresh && kGate == 0 then
   kGate        =            1
  endif
  
  /* TURN GATE STATUS INDICATORS ON AND OFF */
  if changed(kGate)==1 then
                chnset       1 - kGate, "GateIndicCl"
                chnset       kGate, "GateIndicOp"
  endif
   
  /* SMOOTH GATE OPENING AND CLOSING (CALL UDO) */
  kGateD        SwitchPort   kGate, kAttTime, kRelTime   ; smooth opening and closing
  
  /* AMPLITUDE GATE */
  kGateDA       scale        kGateD,1,ampdb(-kAtten)     ; modify gating function according to user defined attenuation setting
  aGate         interp       kGateDA                     ; create an arate version (smoother)
    
  /* DELAY */
  if kDelTim>0 then
   aL           vdelay       aL, kDelTim*1000, 100
  endif
   
  /* APPLY GATE */
  aL            *=           aGate
  
  /* FILTER GATE */
  if kFilterGate>1 then             
   kcfoct       scale        kGateD, kFiltGateMax, kFiltGateMin
   acf          interp       cpsoct(kcfoct)
   if kFilterGate==2 then
    aL          tone         aL, acf
   else
    aL          butlp        aL, acf
   endif
  endif
  aR            =            aL

 endif
                outs         aL * kGain, aR * kGain

endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>