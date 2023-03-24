
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; DopplerSpin.csd
; Written by Iain McCurdy, 2013

; CONTROLS

; Speed              --    Frequency of the spinning
; Loc.               --    Listening position in relation to the circular motion (range from ('centre' to 'edge'). Can be thought of as a control for the amount of amplitude modulation.
; Orient.            --    direction of listening (in radians)
; Width              --    width of panning directionality in the modulation 
; Depth              --    depth of doppler effect (circle size).
; (Reverb) Scaling   --    a dry/wet mixer. Mixing the dry and wet signals can be used to create chorus effects.
; (Reverb) Time      --    amount of amplitude drop off as the source moves away from the source. 
; (Reverb) Damping   --    Amount of left-right movement in the output as the source swings past the microphone
; Mix                --    scales the output signal
; Level              --    Output Level
; Auto-Frequency     --    If this button is activated the frequency of the spinning moves randomly between -Frequency of Rotation and +Frequency of Rotation

; This example uses three LFOs to create the effect of a sound moving in a circle around the listener. 
; The three parameters controlled by these LFOs are amplitude, panning and delay time.
; The modulation of the delay time also results in a modulation of pitch which is sometimes referred to as the Doppler effect.                                             
; Note that in this example the 'vdelayxw' opcode is used to implement the delay and doppler shift. 
; This opcode is unique in that the delay time of the write pointer rather than the read pointer is modulated. 
; This is appropriate here as it is the sound source that is moving, not the listener.          
; Crucial to this effect is that that all three LFOs share the same frequency value. 
; Negative frequency values are also allowed - this would represent a change in direction of the source sounds motion around us.                             
; Also of crucial importance is the phase relationship between the three LFOs as this defines exactly where the sound source is in relation to the listener.                             
; The panning LFO should be at its points of minimum rate of change when the sound source is moving parallel to the direction in which the listener is facing, i.e. directly to the left or to the right of the listener.                   
; The delay time LFO (pitch modulation/doppler) should be at its points of minimum rate of change when the sound source is  moving perpendicular to the direction in which the listener is facing, i.e. directly in front of or behind the listener.
; The phase difference between these two LFOs is either 90 or 270 degrees, depending on whether the source sound is moving in a clockwise or anticlockwise direction around us.        
; Amplitude modulation comes into play whenever we are not listening from the centre of the circle of motion. 
; The close to the edge of the circle we are the greater the amount of amplitude modulation we will experience. 
; If the amplitude modulation is extreme then the circle of the source sound's motion must be extremely large. The phase of the amplitude modulation LFO is also adjustable ('Orientation' slider -   
; this define which edge of the circle we are closest to, e.g. upper, lower, left, right etc. It is probably best to always include at least a small amount of amplitude modulation as we perceive sounds directly to our left or to our right to be
; louder, even if they remain equidistant from us. In this case the amplitude LFO phase ('Orientation') should be 0.5 (radians).                                                  
; The waveform for all three LFOs is a sine wave. 
; This defines the object's motion as being circular. 
; If we were to use a different waveform this would model non-circular motion. 
; There is interesting potential in experimentation in this direction with this example.                                
; Finally as the moving signal becomes more distant, i.e. when the amplitude scaling function is at its minimum, a reverberated version of the signal can become more evident. 
; The degree to which this is present can be scaled using the 'Reverb Scaling' slider.                                    

<Cabbage>
form caption("Doppler Spin"), size(700,265), pluginId("SDop"), colour("Black")

label    bounds( 8 , 4, 70, 15), text("i  n  p  u  t"), fontColour("white")
combobox bounds(10, 30, 60,20), channel("input"), value(1), text("left","right","mixed","test")

line     bounds( 85,  5,  2, 70), colour("Grey")
label    bounds(185,  4, 90, 15), text("r  e  v  e  r  b"), fontColour("white")
checkbox bounds(100, 33,110, 20), text("Reverb On/Off") channel("RvbOnOff"), FontColour("White"), colour("lime")  value(1)
rslider  bounds(210, 23, 55, 55), channel("RvbScaling"), range(0,1.000,0.3,1,0.001),     text("Scaling"),               TextBox(0), colour( 95, 45,115), trackerColour(white)
rslider  bounds(270, 23, 55, 55), channel("RvbTime"),    range(0.3,0.990,0.7,1,0.001),   text("Time"),                  TextBox(0), colour( 85, 45,125), trackerColour(white)
rslider  bounds(330, 23, 55, 55), channel("RvbFilt"), range(20,20000,4000,0.5,0.001),    text("Damping"),               TextBox(0), colour( 75, 45,135), trackerColour(white)

line     bounds(390,  5,  2, 70), colour("Grey")
label    bounds(410,  4, 90, 15), text("o  u  t  p  u  t"), fontColour("white")
rslider  bounds(400, 23, 55, 55), channel("mix"),        range(0,1.000,1,1,0.001),       text("Mix"),                   TextBox(0), colour( 65, 45,145), trackerColour(white)
rslider  bounds(460, 23, 55, 55), channel("OutGain"),    range(0,1.000,0.7,1,0.001),     text("Level"),                 TextBox(0), colour( 55, 45,155), trackerColour(white)
line     bounds(520,  5,  2, 70), colour("Grey")

checkbox bounds(530, 33,120, 20), text("Random Speed") channel("RandSpeed"), FontColour("White"), colour("lime")  value(0)

hslider bounds(  5, 80,690, 35), channel("freq"),        range(-10,10.0,0.2,1,0.01),     text("Speed"),         TextBox(1), colour(145, 45, 65), trackerColour(white)    ;Frequency of Rotation
hslider bounds(  5,110,690, 35), channel("AmpDepth"),    range(0,1.000,0.7,1,0.001),        text("Loc."),          TextBox(1), colour(135, 45, 75), trackerColour(white)    ;Central/Edge
hslider bounds(  5,140,690, 35), channel("AmpPhase"),    range(0,1.000,0.5,1,0.001),        text("Orient."),       TextBox(1), colour(125, 45, 85), trackerColour(white)    ;Orientation [radians]
hslider bounds(  5,170,690, 35), channel("PanDepth"),    range(0,1.000,1,1,0.001),          text("Width"),         TextBox(1), colour(115, 45, 95), trackerColour(white)    ;Panning Width
hslider bounds(  5,200,690, 35), channel("DopDep"),      range(0,0.030,0.003,1,0.0001), text("Depth"),         TextBox(1), colour(105, 45,105), trackerColour(white)    ;Doppler Depth

label   bounds( 5,247, 170, 12), text("Author: Iain McCurdy |2013|"), FontColour("grey")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1    ;MAXIMUM AMPLITUDE VALUE

gisine    ftgen    0,0,131072,10,1

instr    1
gkinput      chnget    "input"  
kfreq        chnget    "freq"
kAmpDepth    chnget    "AmpDepth"      
kAmpPhase    chnget    "AmpPhase"      
kPanDepth    chnget    "PanDepth"      
kDopDep      chnget    "DopDep"        
kRvbScaling  chnget    "RvbScaling"    
kRvbTime     chnget    "RvbTime"       
kRvbFilt     chnget    "RvbFilt"       
kmix         chnget    "mix"       
kOutGain     chnget    "OutGain"       
kRvbOnOff    chnget    "RvbOnOff"
kRandSpeed   chnget    "RandSpeed"       

/* INPUT */
aL,aR        ins
if gkinput=1 then
 asig    =    aL
elseif gkinput=2 then
 asig    =    aR
elseif gkinput=3 then
 asig    =    (aL+aR)*0.677
else
  ;INPUT TONE=============================================(for testing)
 asig        vco2    .5, 300        ;GENERATE TONE
 asig        tone    asig, 1200    ;LOW PASS FILTER TO SOFTEN THE TONE
 ;=======================================================
endif     

if kRandSpeed=1 then
 kfreq       jspline   kfreq,1,4
endif



kporttime    linseg    0,0.001,0.1            ;CREATE 'PORTAMENTO TIME'. A FUNCTION THAT RISES QUICKLY FROM ZERO TO A HELD VALUE.
kAmpPhase    portk     kAmpPhase, kporttime        ;APPLY PORTAMENTO TO gkAmpPhase. CREATE NEW OUTPUT VARIABLE kAmpPhase (GLOBAL VARIABLES CAN'T BE BOTH INPUT AND OUTPUT)
kDopDep      portk     kDopDep, kporttime        ;APPLY PORTAMENTO TO gkDopDep. CREATE NEW OUTPUT VARIABLE kDopDep (GLOBAL VARIABLES CAN'T BE BOTH INPUT AND OUTPUT)

aAmp         osciliktp kfreq, gisine, kAmpPhase    ;AN LFO DEFINES A VARIABLE USED TO MODULATE AMPLITUDE (NOTE: VARIABLE PHASE). THIS MODELS THE LOCATION WITHIN THE CIRCLE FROM WHICH WE ARE LISTENING. NO AMPLITUDE MODULATION REPRESENT REPRESENTS OUR LISTENING POSITION BEING EXACTLY CENTRAL, MAXIMUM MODULATION REPRESENTS US BEING NEAR TO THE EDGE OF A LARGE CIRCLE. THE CONTROL OVER PHASE OF THIS LFO REPRESENTS WHICH EDGE WE ARE CLOSER TO.
aAmp         =         (aAmp * 0.5 * kAmpDepth) + 0.5    ;RESCALE AND OFFSET AMPLITUDE MODULATION LFO

aPan         oscili    (kPanDepth * 0.5), kfreq, gisine, 0.75    ;AN LFO DEFINES A VARIABLE FOR PANNING CONTROL - I.E. WHETHER SOUND IS CURRENTLY TO OUT LEFT OR TO OUT RIGHT. NOTE THAT PHASE IS 0.75 AND THEREFORE 0.75 RADIANS (OR 270 DEGREES OUT OF PHASE) WITH THE DELAY MODULATION 
aPan         =    aPan + 0.5                    ;OFFSET PANNING LFO

iMaxDelay    =    1        ;DEFINE A VARIABLE THAT WILL BE USE FOR 'MAXIMUM DELAY TIME' (BUFFER LENGTH)
aDelTim      oscili    kDopDep, kfreq, gisine, 0    ;AN LFO DEFINES A VARIABLE FOR DELAY TIME (NOTE PHASE AT ZERO)
aDelTim      =    aDelTim + kDopDep        ;DELAY TIME  VARIABLE 'aDelay' IS OFFSET TO STAY WITHIN THE POSITIVE DOMAIN    
    
;vdelayxw IS USED FOR THE DELAY READ/WRITE AS IT MODULATES THE WRITE POINTER RATHER THAN THE READ POINTER.
;THIS IS MORE APPROPRIATE IN THIS EXAMPLE AS THE SOURCE IS MOVING BUT THE POINT OF LISTENING IS STATIONARY
aDelTap    vdelayxw    asig, aDelTim, iMaxDelay, 16

aL, aR    pan2     aDelTap, aPan, 1    ;APPLY PANNING TO SIGNAL OUTPUT FROM DELAY USING pan2 OPCODE. CREATE A NEW 

aL        =        aL * (aAmp^0.5)        ;APPLY AMPLITUDE MODULATION (CREATE A NEW AUDIO SIGNAL - DRY (UN-REVERBERATED) SIGNAL)
aR        =        aR * (aAmp^0.5)        ;APPLY AMPLITUDE MODULATION (CREATE A NEW AUDIO SIGNAL - DRY (UN-REVERBERATED) SIGNAL)

if kRvbOnOff=1 then
 aRvbL, aRvbR    reverbsc    aL, aR, kRvbTime, kRvbFilt    ;REVERB (UNAFFECTED BY AMPLITUDE MODULATION)
 aL       =        (aL+(aRvbL*kRvbScaling))
 aR       =        (aR+(aRvbL*kRvbScaling))
endif

aL       ntrpol    asig,aL,kmix
aR       ntrpol    asig,aR,kmix

         outs      aL * kOutGain , aR * kOutGain     ;SEND AUDIO TO OUTPUTS. MIX DRY AND REVERBERATED SIGNALS.
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7*52]
</CsScore>

</CsoundSynthesizer>