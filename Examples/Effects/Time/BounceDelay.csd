
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; BounceDelay.csd
; Written by Iain McCurdy, 2015,2021

; Creates layers of delays, their delay times mathematically related so as to create the impression of sounds bouncing and coming to rest (and other sound effects).
; The gesture can be further shaped by applying progessive amounts of amplitude reduction, lowpass filtering and highpass filtering.

; Total time  -    total time of the 'bounce' gesture / the length of the longest delay (0 to 6 seconds).
; Number      -    number of delays (1 to 128)
; Time Curve  -    amount of progressive time warping of the delays:
;                  If 'Warp' = 0 then delays are uniformly spaced
;                  If 'Warp' < 0 then delays are progressively more narrowly spaced (the typical 'bounce' structure)
;                  If 'Warp' > 0 then delays are progressively more widely spaced (a reverse 'bounce' structure)
; Amp.Curve   -    Delay by delay amplitude shaping    
;                  If 'Amp.Curve' = 0, no shaping is applied.
;                  If 'Amp.Curve' < 0, amplitudes are progressively attenuated. The more negative this value is, the more rapidly amplitudes are reduced / the steeper the slope of the curve.
;                  If 'Amp.Curve' > 0, amplitudes are progressively increased (starting at silence). The larger this value is, the more rapidly amplitudes are increased / the steeper the slope of the curve.
; Mix         -    Dry/wet mix.
; Level       -    Output level

; Lowpass Filter
; --------------
; LPF Curve    -    Delay by delay lowpass filtering. Curve shaping operates in a similar fashion to 'Amp.Curve'
; LFO Mode     -    Bypass, 6dB/oct, 12dB/oct

; Highpass Filter
; --------------
; HPF Curve    -    Delay by delay highpass filtering. Curve shaping operates in a similar fashion to 'Amp.Curve'
; HFO Mode     -    Bypass, 6dB/oct, 12dB/oct

; Note that the dry signal is included in amplitude and filtering shaping so depending on the three curve settings it may be filtered or even attenuated completely.
    
<Cabbage>
#define RSLIDER_STYLE1 trackerInsideRadius(.85), trackerOutsideRadius(1), colour(  0, 40, 50), trackerColour(100,140,150), textColour(200,240,250) markerColour(200,240,250)
#define RSLIDER_STYLE2 trackerInsideRadius(.85), trackerOutsideRadius(1), colour( 50, 90,100), trackerColour(100,140,150), textColour(200,240,250) markerColour(200,240,250)
#define LABEL_STYLE fontColour(200,240,250)
#define COMBOBOX_STYLE colour(  10, 40, 50), textColour(200,240,250), textColour(200,240,250) 
 
form caption("Bounce Delay"), size(490,360), pluginId("BnDl"), colour("Black")
rslider   bounds( 10, 10, 70, 70), channel("TotalTime"), text("Total Time"), range(0.01,6, 1.7, 0.5), $RSLIDER_STYLE1
rslider   bounds( 90, 10, 70, 70), channel("Number"), text("Number"),        range(1, 128,12, 1,1), $RSLIDER_STYLE1
rslider   bounds(170, 10, 70, 70), channel("Warp"), text("Time Curve"),      range(-1, 1, -0.1), $RSLIDER_STYLE2
rslider   bounds(250, 10, 70, 70), channel("AmpCurve"), text("Amp.Curve"),   range(-1, 1, -0.7), $RSLIDER_STYLE2
rslider   bounds(330, 10, 70, 70), channel("Mix"), text("Mix"),              range(0, 1, 0.5), $RSLIDER_STYLE1
rslider   bounds(410, 10, 70, 70), channel("Level"), text("Level"),          range(0, 1, 1), $RSLIDER_STYLE1

image     bounds( 20,100,240,100), colour(0,0,0,0), outlineColour("white"), outlineThickness(1), plant("LPF")
{
label     bounds(  0,  3,240, 14), text("F i l t e r"), fontColour(100, 100,250,170)
rslider   bounds( 10, 25, 70, 70), channel("FiltCurve"), text("Filt.Curve"),   range(-1, 1, -0.8), $RSLIDER_STYLE2
label     bounds( 90, 30,120, 14), fontColour("white"), text("Filter Mode"), $LABEL_STYLE
combobox  bounds( 90, 45,120, 20),  channel("FiltType"),text("Bypass","Lowpass 6dB/oct","Lowpass 12dB/oct","Highpass 6dB/oct","Highpass 12dB/oct","Bandpass"), value(1), $COMBOBOX_STYLE
}

; display bars 
image bounds( 18,218,454,124), colour(250,250,250,0), shape("sharp"), outlineColour("Silver"), outlineThickness(1)
gentable bounds( 20,220,450,120), tableNumber(1), identChannel("Table1"), ampRange(0,1,1), fill(0), tableColour(200,200,250,250), tableBackgroundColour(10,10,50,250), tableGridColour(0,0,0,0), outlineThickness(1)
image bounds( 20,220,450,120), colour( 20, 20, 20, 0), shape("sharp"), plant("display") 
{
image bounds(-100,  0,  4,100), colour(250,250,250,200), shape(sharp), visible(1), widgetArray("Bars",128)
}
label bounds(  5,342, 50,12), fontColour("white"), text("0 sec"), $LABEL_STYLE
label bounds(225,342, 50,12), fontColour("white"), text("3 sec"), $LABEL_STYLE
label bounds(447,342, 50,12), fontColour("white"), text("6 sec"), $LABEL_STYLE

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n
</CsOptions>

<CsInstruments>

; sr is set by host
ksmps   =  32   ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls  =  2    ; NUMBER OF CHANNELS (2=STEREO)
0dbfs   =  1

iFilt ftgen 1,0,1024,2,0

giMaxDels =         128
giMaxDelTim = 6
gkDels[]  init      giMaxDels
gkAmps[]  init      giMaxDels
gkFilts[] init      giMaxDels

opcode DelayTap, a, k[]k[]k[]kkip
kDels[],kAmps[],kFilts[],kFiltType,kPortTime,iNumber,iCount xin
kTime  =       kDels[iCount-1]
kAmp   =       kAmps[iCount-1]
kFilt  =       kFilts[iCount-1]
kTime  portk   kTime,kPortTime 
aTap   deltapi a(kTime)
if kFiltType==2 then
 aTap   tone    aTap, cpsoct(kFilt)
elseif kFiltType==3 then
 aTap   butlp   aTap, cpsoct(kFilt)
elseif kFiltType==4 then
 aTap   atone    aTap, cpsoct(kFilt)
elseif kFiltType==5 then
 aTap   buthp   aTap, cpsoct(kFilt)
elseif kFiltType==6 then
 aTap   butbp   aTap, cpsoct(kFilt), cpsoct(kFilt)*0.2
endif
aMix   =       0
if iCount<iNumber then
 aMix   DelayTap kDels,kAmps,kFilts,kFiltType,kPortTime,iNumber,iCount+1
endif
       xout    aTap*kAmp + aMix
endop


instr    1
 kTotTim    chnget   "TotalTime"
 kNumber    chnget   "Number"
 kWarp      chnget   "Warp"   
 kAmpCurve  chnget   "AmpCurve"
 kFiltCurve chnget   "FiltCurve"
 kFiltType  chnget   "FiltType"
 kMix       chnget   "Mix"
 kLevel     chnget   "Level"
  
 aInL,aInR  ins

 ; REBUILD DELAYS ARRAY
 if changed:k(kTotTim,kWarp,kNumber)==1 then
  kDel     = 1       ; first delay
  kTot     = kDel    ; running total
  ; create unscaled delays
  kCount   = 1    ; counter
  while kCount<=kNumber do
   gkDels[kCount-1]  =  kTot
   kDel     *=     2 ^ kWarp
   kTot     +=     kDel
   kCount   +=     1
  od
  kScale   = kTotTim/kTot
  ; scale all delays
  kCount    = 1
  while kCount<=kNumber do
  kDel   limit       gkDels[kCount-1]*kScale, 1/kr, giMaxDelTim
  ;printk2 kDel
  gkDels[kCount-1]  = kDel
  kCount += 1
  od
 endif
 
 ; REBUILD AMPS ARRAY
 if changed:k(kTotTim,kAmpCurve,kNumber)==1 then
  kAmp     = 1       ; first amplitude
  kMax     = 0    ; running total
  ; create unscaled amplitudes
  kCount   = 1    ; counter
  while kCount<=kNumber do
   kAmp     *=     2 ^ kAmpCurve
   gkAmps[kCount-1]  =  kAmp
   kMax     =      kAmp>kMax?kAmp:kMax
   kCount   +=     1
  od
  kScale   = 1/kMax
  ; scale all amps
  kCount    = 1
  while kCount<=kNumber do
  kAmp   =       gkAmps[kCount-1]*kScale
  gkAmps[kCount-1]  = kAmp
  kCount += 1
  od
 endif

 ; REBUILD FILTS ARRAY
 if changed:k(kTotTim,kFiltCurve,kNumber)==1 then
  kFilt    = 1    ; first filt freq
  kMax     = 0    ; running total
  ; create unscaled freqs
  kCount   = 1    ; counter
  while kCount<=kNumber do
   kFilt    *=     2 ^ kFiltCurve
   gkFilts[kCount-1]  =  kFilt
   kMax     =      kFilt>kMax?kFilt:kMax
   kCount   +=     1
  od
  kScale   = 10/kMax
  ; scale all Filts
  kCount    = 1
  while kCount<=kNumber do
  kFilt   =       (gkFilts[kCount-1]*kScale) + 4
  gkFilts[kCount-1]  = kFilt
  kCount += 1
  od
 endif
 if changed:k(kTotTim,kFiltCurve,kFiltType)==1 then
  reinit REBUILD_FILT_CURVE
 endif
 REBUILD_FILT_CURVE:
 iDir  =      i(kFiltCurve)>0?1:0
 iFilt  ftgen  1,0,1024,16,1-iDir,1024*i(kTotTim)/giMaxDelTim,(i(kFiltCurve)*8),iDir,0,0,0
 chnset "tableNumber(1)","Table1"
 rireturn

 kPortTime  linseg  0, 0.001, 0.05

 if changed:k(kNumber)==1 then
  reinit RESTART_DELAYS
 endif
 RESTART_DELAYS:

 
 aBuf  delayr  giMaxDelTim + 1
 aTapL DelayTap gkDels, gkAmps, gkFilts, kFiltType, kPortTime, i(kNumber)
       delayw  aInL

 aBuf  delayr  giMaxDelTim + 1
 aTapR DelayTap gkDels, gkAmps, gkFilts, kFiltType, kPortTime, i(kNumber)
       delayw  aInR
 rireturn
 
 aMixL      ntrpol   aInL, aTapL, kMix    ; dry/wet mixing
 aMixR      ntrpol   aInR, aTapR, kMix

            outs     aMixL * kLevel, aMixR * kLevel


 if changed:k(kNumber,kTotTim,kWarp,kNumber,kAmpCurve)==1 then
  kCount = 1
  while kCount<=kNumber do
  Smsg   sprintfk  "visible(1), bounds(%d,%d,1,%d)", gkDels[kCount-1]/giMaxDelTim * 450, (1-gkAmps[kCount-1]) * 120, gkAmps[kCount-1] * 120
  SID    sprintfk  "Bars_ident%d",kCount
  chnset Smsg, SID
  kCount += 1
  od
  
  kCount = kNumber+1
  while kCount<=giMaxDels do
  SID    sprintfk  "Bars_ident%d",kCount
        chnset  "visible(0)", SID
  kCount +=  1
  od
 endif


endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>