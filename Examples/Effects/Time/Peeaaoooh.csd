
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Peeaaoooh.csd
; Written by Iain McCurdy, 2015

; Generates reverberation effects reminiscent of spring reverbs.

; The main element is a stack of delays with geometrically related delay times

; NB. although delays are used, their delay times are typically quite short and are expressed here in hertz, i.e. frequency of echoes (freq = 1/delay_time).
; Furthermore, ratios between delay times of parallel delays are expressed in semitones.

; Base        -    Base Frequency of the stack of delays (in hertz)    
; Layers    -    Number of layers (delays). Bear in mind that increasing this increaeses CPU and memory demand.
; Shape        -    Shapes the amplitude relationship between layers. If Shape=0, all layers are equal in amplitude, as Shape is increased, longer delays have their amplitudes attenuated.
; Interval    -    Interval between consecutive delays (in semitones).
; Scatter    -    Adds a fixed random value to the delay times of the delays. Increasing this value will gradually obliterate the distinctive 'peeaaoo' sound and also add a stereo broadening effect.
; Spread    -    Amount by which left and right fo the same delat layer are alternately panned left or right. This can be used as an alternative to 'Scatter' to create stereo broadening but without the obliteration of the 'peeaaoo' sound.
; Feedback    -    Ratio of the output signal from each delay that is fed back into the input. The knob turns red to warn of greater than 100% feedback.
; HPF        -    Cutoff frequency of a highpass filter within the feedback loop of each delay
; LPF        -    Cutoff frequency of a lowpass filter within the feedback loop of each delay
; Test Noise-    When activated, the live audio input is replaced with a sparse 'dust' source sound. Good for testing the effect.
; Sep.Mode    -    There are two separation modes: Linear: sequential delays in the stack are separated by equal pitch intervals
;                            Exponential: intervals between sequential delays increase exponentially
;            Linear offers a more distinctive 'peeaaoooh' sound and arpeggios when 'Base' is low. Exponential offers a more 'scattered' sounding reverb.
;            When 'Interval' is low, both modes sound quite similar.
; Post-Filter
; HPF        -    Cutoff frequency of a highpass filter applied to the output signal (dry and wet signals)
; LPF        -    Cutoff frequency of a lowpass filter applied to the output signal (dry and wet signals)

; Mix        -    Dry/wet mix
; Level        -    Output level (dry and wet signals)

; NB. Feedback greater than 1 (i.e. more than 100% of the signal) are possible. 
; Normally this would result in a runaway increase in amplitude but control is still possible if the main highpass and lowpass filters also remove part of the audio in the feedback loop.
; Nonetheless surity is not guaranteed so therefore experimentation and caution is advised. 
; There is however a safety mechanism which will reduce the feedback control if the amplitude starts to runaway.

;                   (feedback)
;             +----[LPF]--[HPF]---+
;             |                   |  
;             v   +-----------+   | (mix)  +----------+  +-----------+
;  --IN--+----+---|DELAY_STACK|---+---+----|POST DELAY|--|POST FILTER|--OUT-->
;        |        +-----------+       ^    +----------+  +-----------+
;        |                            |
;        +----------------------------+

<Cabbage>
form caption("Peeaaoooh") size(650,207), pluginId("Peao")
image pos(0, 0),          size(650,207), colour( 10, 20, 30), shape("sharp"), outlineColour("white"), outlineThickness(0) 

rslider   bounds( 10, 10, 70, 70),  channel("Base"),text("Base"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(0.5, 1000, 50, 0.5, 0.001)
rslider   bounds( 80, 10, 70, 70),  channel("Layers"),text("Layers"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(1, 60, 8,1,1)
rslider   bounds(150, 10, 70, 70),  channel("Interval"),text("Interval"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(0.001, 1, 0.07,0.5,0.001)
rslider   bounds(220, 10, 70, 70),  channel("Shape"),text("Shape"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(0, 3, 0,1,0.001)
rslider   bounds(290, 10, 70, 70),  channel("Scatter"),text("Scatter"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(0, 1, 0,0.5,0.0001)
rslider   bounds(360, 10, 70, 70),  channel("Spread"),text("Spread"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(0, 1, 0)
rslider   bounds(430, 10, 70, 70),  channel("Feedback"),text("Feedback"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(0, 1.5, 0.95), identChannel("FeedbackID")
checkbox  bounds(430, 82, 80, 14),  channel("Overload"),text("Overload"), colour(255, 60, 60), fontColour:0("white"), fontColour:1("white"), shape("Ellipse"), active(0)
rslider   bounds(500, 10, 70, 70),  channel("HPF"),   text("HPF"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(20,20000,20,0.5,1)
rslider   bounds(570, 10, 70, 70),  channel("LPF"),   text("LPF"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(20,20000,20000,0.5,1)

image     bounds( 20,100,235, 95), outlineColour("white"), outlineThickness(1), colour(50,50,50,100), plant("PostFilter") {
label     bounds(  0,  6,235, 14), text("G l o b a l    F i l t e r"), fontColour("white") 
combobox  bounds( 10, 40, 75, 20),  channel("PrePostFilter"),text("Pre-mix","Post-mix"), textColour("white"), value(2)
rslider   bounds( 85, 22, 70, 70),  channel("PostHPF"),   text("HPF"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(20,20000,20,0.5,1)
rslider   bounds(155, 22, 70, 70),  channel("PostLPF"),   text("LPF"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(20,20000,20000,0.5,1)
}


;image     bounds(270,100,235, 95), outlineColour("white"), outlineThickness(1), colour(50,50,50,100), plant("PostDelay") {
;label     bounds(  0,  6,235, 14), text("G l o b a l    D e l a y"), fontColour("white") 
;rslider   bounds(  5, 22, 70, 70),  channel("PostDlyTim"),   text("Time"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(20,20000,20,0.5,1)
;rslider   bounds( 75, 22, 70, 70),  channel("PostDlyMix"),   text("Mix"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(20,20000,20000,0.5,1)
;rslider   bounds(155, 22, 70, 70),  channel("PostDlyFB"),    text("Feedback"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(20,20000,20000,0.5,1)
;}

checkbox  bounds(360,112,110, 15), text("Test Clicks"), channel("TestNoise"), fontColour:0("white"), fontColour:1("white")
label     bounds(360,137,110, 12), text("Separation Mode"), fontColour("white") 
combobox  bounds(360,150,110, 20),  channel("SepMode"),text("Linear","Exponential"), textColour("white"), value(1)

rslider   bounds(480,110, 70, 70),  channel("Mix"),text("Mix"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(0, 1, 1)
rslider   bounds(560,110, 70, 70),  channel("Level"),text("Level"), colour( 20, 60, 70), trackerColour(200,240,250), textColour("white"), range(0, 2, 1,0.5,0.001)

label     bounds(530,195,100, 10), text("Iain McCurdy |2015|"), fontColour(200,200,200) 

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     16    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1
        seed    0
        
opcode    DelayLayer,a,akkkkkkkkkip
 aInSig,kBase,kShape,kInterval,kScatter,kSpread,kSepMode,kHPF,kLPF,kFeedback,iLayers,iCount    xin
 iRnd    random    -0.5,0.5            ; i-time random value. A unique fixed value for each delay layer (and channel).
 kRnd    =    octave(iRnd * kScatter)        ; scale random value by GUI widget control
 /* SELECT THE SEPARATION MODE */
 if kSepMode==1 then                                ; linear
  aDel    interp    limit:k(kRnd/(kBase * semitone(kInterval*(iCount-1))),1/kr,2)
 else                                        ; exponential
  aDel    interp    limit:k(kRnd/(kBase * semitone((kInterval+1)^iCount)),1/kr,2)
 endif
 kAmp    =    (iCount/iLayers) ^ kShape
 abuf    delayr    2
 aWG    deltapi    aDel
 aWG    atone    aWG,kHPF
 aWG    tone    aWG,kLPF
 aWG    dcblock2    aWG
     delayw    aInSig + (aWG*kFeedback)
 if iCount<iLayers then
  aMix    DelayLayer    aInSig,kBase,kShape,kInterval,kScatter,1-kSpread,kSepMode,kHPF,kLPF,kFeedback,iLayers,iCount+1
 endif
    xout    aWG*limit:k(kSpread*2,0,1)*kAmp + aMix
 aMix    =    0
endop

instr    1

 /* READ IN WIDGETS */
 kporttime    linseg    0,0.001,0.1
 kBase        chnget    "Base"
 iBase        chnget    "Base"
 kBase        init    iBase
 kBase        portk    kBase,kporttime
 kInterval    chnget    "Interval"
 iInterval    chnget    "Interval"
 kinterval    init    iInterval
 kInterval    portk    kInterval,kporttime
 kScatter    chnget    "Scatter"
 kScatter    portk    kScatter,kporttime
 kSpread    chnget    "Spread"
 kFeedback    chnget    "Feedback"
 kLevel        chnget    "Level"
 kLayers    chnget    "Layers"
 kShape        chnget    "Shape"
 kHPF        chnget    "HPF"
 kLPF        chnget    "LPF"
 kSepMode    chnget    "SepMode"
 kPostHPF    chnget    "PostHPF"
 kPostLPF    chnget    "PostLPF"
 kMix        chnget    "Mix"
 kPrePostFilter        chnget    "PrePostFilter"
 kTestNoise    chnget    "TestNoise"
 
 if kTestNoise==1 then
  aL    dust2    1,0.5*randomi:k(0.5,2,1,1)
  aR    dust2    1,0.5*randomi:k(0.5,2,1,1)
  aL    butlp    aL,cpsoct(randomh:k(5,11,10))
  aR    butlp    aR,cpsoct(randomh:k(5,11,10))
 else
  aL,aR    ins
 endif
  
 /* CALL DELAY STACK (VIA A UDO) */
 if changed(kLayers)==1 then
  reinit    UPDATE
 endif
 UPDATE:
 iAmpScl    =    sqrt(i(kLayers))                            ; amplitude will be scaled according to the number of layers selected
 aWG_L    DelayLayer    aL,kBase,kShape,kInterval,kScatter,0.5+kSpread/2,kSepMode,kHPF,kLPF,kFeedback,i(kLayers)
 aWG_R    DelayLayer    aR,kBase,kShape,kInterval,kScatter,0.5-kSpread/2,kSepMode,kHPF,kLPF,kFeedback,i(kLayers)
 aWG_L    /=    iAmpScl                                        ; scale amplitude according to the number of layers chosen
 aWG_R    /=    iAmpScl
 rireturn
        
 /* TURN FEEDBACK KNOB RED IF GREATER THAN 1 */       
 if trigger:k(kFeedback,1,0)==1 then        ; trigger to turn red
  chnset "colour(170, 60, 70)","FeedbackID"    ; send message
 elseif trigger:k(kFeedback,1,1)==1 then    ; trigger to turn back to normal
  chnset "colour( 20, 60, 70)","FeedbackID"    ; send message
 endif
         
 /* PREVENT OVERLOADS */
 kRMS    rms    aWG_L                ; scan rms
 kOn    =    1                ; on value (needs to be a k-rate var.)
 kOff    =    0                ; off value (needs to be a k-rate var.)
 kOverload    chnget    "Overload"        ; read in value of 'Overload' LED
 if kRMS>0.9 then                ; if rms exceeds a given value...
  chnset    kOn,"Overload"            ; turn on 'Overload' LED
  chnset    (kFeedback - (0.0003)) - ((kFeedback-1)*0.001),"Feedback"    ; decrement 'Feedback' slider. This will be repeated every k-cycle until rms is back below the threshold value prescribed above.
 elseif kOverload==1 then            ; otherwise..., i.e. if 'Overload' LED is on but rms is beneath the threshold...  
  chnset    kOff,"Overload"            ; turn 'overload' LED off
 endif    

 /* GLOBAL FILTERING CAN BE APPLIED TO JUST THE WET SIGNAL OR A MIXTURE OF THE WET AND DRY SIGNALS, ACCORDING TO A SELECTION MADE USING A COMBOBOX */ 
 /* PRE-MIXER FILTERING OPTION */
 if kPrePostFilter==1 then
  if kPostHPF>20 then            ; if HPF is at its minimum value (20) bypass filtering to conserve CPU
   aWG_L    buthp    aWG_L,kPostHPF    ; highpass filter left channel
   aWG_R    buthp    aWG_R,kPostHPF    ; highpass fiter right channel
  endif
  if kPostLPF<20000 then         ; if HPF is at its minimum value (20) bypass filtering to conserve CPU
   aWG_L    butlp    aWG_L,kPostLPF        
   aWG_R    butlp    aWG_R,kPostLPF        
  endif
  aMixL    ntrpol    aL, aWG_L, kMix
  aMixR    ntrpol    aR, aWG_R, kMix
 /* PRE-MIXER FILTERING OPTION */
 elseif kPrePostFilter==2 then
  aMixL    ntrpol    aL, aWG_L, kMix
  aMixR    ntrpol    aR, aWG_R, kMix
  if kPostHPF>20 then
   aMixL    buthp    aMixL,kPostHPF        
   aMixR    buthp    aMixR,kPostHPF        
  endif
  if kPostLPF<20000 then 
   aMixL    butlp    aMixL,kPostLPF        
   aMixR    butlp    aMixR,kPostLPF        
  endif
 endif

    outs    aMixL*kLevel, aMixR*kLevel
endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
