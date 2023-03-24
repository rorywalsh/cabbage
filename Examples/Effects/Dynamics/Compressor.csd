
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Compressor.csd
; Written by Iain McCurdy, 2016, 2021

; Encapsulation of the compressor opcode

; A compressor is a dynamics processor that essentially applies waveshaping to an audio signal.
; In the case of a typical compressor, increasingly high dynamics will be increasingly attenuated thereby reducing the dynamic range of a signal.
; This can be useful in forcing a dynamically expressive signal to sit better within a mix of signals or for processing a final mix so that it functions better when listened to in an environment with background noise. 

; When reference is made to a soft knee, this is the part of the dynamic mapping function that separates the lower response region and the upper response region. 
; Below the 'knee' the dynamic tranformation is 1:1, i.e. no change. 

; Threshold -   Lowest decibel level that will be allowed through. Can be used to remove low level noise from a signal. A setting of zero will defeat this feature.  
; Low Knee  -   Decibel point at which the 'soft knee' 
; High Knee -   Decibel point at which the 'soft knee' terminates
; Attack    -   Attack time of the compressor
; Release   -   Release time of the compressor
; Ratio     -   Compression ratio of the upper compressor response region
; Lookahead -   Essentially this is a delay that will be applied to the signal that will be compressed (the track signal will always be un-delayed).
;           This can be useful for making sure that the compressor responds appropriately to fast attacks (at the expense of some added latency by the delay).
;           This is an initialisation time parameter so making changes to it will interrupt the realtime audio stream
; Gain      -   A make-up gain control. Use this to compensate for loss of power caused by certain compressor settings.
; Autogain  -   automatic compensation of gain loss caused by increasing ratio and reducing the knee breakpoints

<Cabbage>
#define SLIDER_APPEARANCE trackerInsideRadius(.79), trackerOutsideRadius(1) trackerColour(150,150,30), textColour("black"), valueTextBox(1), fontColour("black"), popupText(0)
form caption("Compressor") size(810,155), pluginId("comp"), colour("silver")
image            bounds(  0,  0,810,155), outlineThickness(6), outlineColour("white"), colour("silver")
rslider bounds( 10, 10, 70, 90), channel("InGain"), text("In Gain"), range(-36,36,0,1,1), $SLIDER_APPEARANCE
rslider bounds( 80, 10, 70, 90), channel("thresh"), text("Threshold"), range(-90,0,-90,1,1), $SLIDER_APPEARANCE
rslider bounds(150, 10, 70, 90), channel("att"), text("Attack"),  range(0,1,0.01,0.5), $SLIDER_APPEARANCE
rslider bounds(220, 10, 70, 90), channel("rel"), text("Release"), range(0,1,0.05,0.5), $SLIDER_APPEARANCE
rslider bounds(290, 10, 70, 90), channel("ratio"), text("Ratio"), range(1,300,5,0.5), $SLIDER_APPEARANCE
rslider bounds(360, 10, 70, 90), channel("look"), text("Lookahead"), range(0,1,0.01,0.5), $SLIDER_APPEARANCE
rslider bounds(430, 10, 70, 90), channel("gain"), text("Out Gain"), range(-36,36,0,1,1), $SLIDER_APPEARANCE
hrange  bounds( 10,103,560, 30), channel("LowKnee","HighKnee"), range(-90, 0, -36:-24), $SLIDER_APPEARANCE
label   bounds( 10,131,560, 13), text("Knee break points"), fontColour("black")
nslider bounds( 15,130, 40, 22), channel("LowKneeD"), range(-90,0,-36,1,1)
nslider bounds( 55,130, 40, 22), channel("HighKneeD"), range(-90,0,-24,1,1)
checkbox bounds(510, 35,80,15), text("Autogain"), channel("Autogain"), fontColour:0("black"), fontColour:1("black")

gentable bounds(600, 10,130,130), tableNumber(2), tableColour:0(255,255,0), identChannel("table1"), tablebackgroundColour(0,0,0), outlineThickness(2), fill(0), tableGridColour(100,100,100,100), ampRange(0,1,2), zoom(-1), active(1)

label    bounds(573,  7, 25, 10), text("0"), fontColour("black"), align("right")
label    bounds(573,135, 25, 10), text("-120"), fontColour("black"), align("right")
label    bounds(713,135, 25, 10), text("0"), fontColour("black"), align("right")

vmeter   bounds(745, 10, 20, 125) channel("vMeter1") value(0) overlayColour(0, 0, 0, 255) meterColour:0(255, 255, 0) meterColour:1(255, 103, 171) meterColour:2(250,250, 0) outlineThickness(1)
vmeter   bounds(775, 10, 20, 125) channel("vMeter2") value(0) overlayColour(0, 0, 0, 255) meterColour:0(255, 255, 0) meterColour:1(255, 103, 171) meterColour:2(250,250, 0) outlineThickness(1)
label    bounds(745,135, 20,  12), text("In"), fontColour("black"), align("centre")
label    bounds(775,135, 20,  12), text("Out"), fontColour("black"), align("centre")

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=null -M0
</CsOptions>

<CsInstruments>

;sr is set by host
ksmps    =     32
nchnls   =     2
0dbfs    =     1

; Author: Iain McCurdy (2016,21)

giTabSize = 4096
gidBRange = 90
giTF  ftgen  1, 0, giTabSize, 2, 0 ; used for creating the table
giTF2 ftgen  2, 0, giTabSize, 2, 0 ; used for display

instr 1
 kInGain       chnget      "InGain"                        ; read in widgets
 kthresh       chnget      "thresh"
 kLowKnee      chnget      "LowKnee"
 kHighKnee     chnget      "HighKnee"
               chnset      kLowKnee, "LowKneeD"
               chnset      kHighKnee, "HighKneeD"
 katt          chnget      "att"
 krel          chnget      "rel"
 kratio        chnget      "ratio"
 kgain         chnget      "gain"
 klook         chnget      "look"                          ; look-ahead time (this will have to be cast as an i-time variable)
 klook         init        0.01                            
 aL,aR         ins                                         ; read in live audio
 ;aL diskin2 "/Users/imccurdy/Documents/iainmccurdy.org/CsoundRealtimeExamples/SourceMaterials/Benjamin.wav",1,0,1
 ;aR=aL
 aL *= ampdbfs(kInGain)
 aR *= ampdbfs(kInGain)
 
 if changed:k(kthresh,kLowKnee,kHighKnee,kratio)==1 then
  reinit REBUILD_TABLE
 endif
 REBUILD_TABLE:
 iThrBP = gidBRange + i(kthresh)  
 iBP    = gidBRange + i(kHighKnee) + ((i(kLowKnee) - i(kHighKnee)) * 0.5)
 iv1    = 0
 id1    = (iThrBP/gidBRange)
 iv2    = iThrBP/gidBRange
 id2    = iBP/gidBRange - iv2
 iv3    = iBP/gidBRange
 id3    = (1 - id2 - id1) * i(kratio)
 iv4    = 1
 giTF  ftgen      1, 0, giTabSize, -7, iv1, id1*giTabSize, 0,0, iv2, id2*giTabSize, iv3, id3*giTabSize, iv4
       tableicopy 2, 1
       chnset "tableNumber(2)", "table1"
 rireturn
          
 if changed(klook)==1 then                                 ; if slider is moved...
               reinit      REINIT                          ; ... reinit
 endif
 REINIT:
 aC_L          compress2   aL, aL, kthresh, kLowKnee, kHighKnee, kratio, katt, krel, i(klook) ; compress both channels
 aC_R          compress2   aR, aR, kthresh, kLowKnee, kHighKnee, kratio, katt, krel, i(klook)
 aC_L          *=          ampdb(kgain)                    ; apply make up gain
 aC_R          *=          ampdb(kgain)
 rireturn
 
 kAutogain     chnget      "Autogain"
  kBP          =     kHighKnee - ((kHighKnee - kLowKnee) * 0.5)
  kFinal       =     kBP + abs(kBP)/kratio
  kMakeUpGain  =     ampdbfs(-kFinal)
 if kAutogain==1 then
 else
  kMakeUpGain  =           1
 endif
 aC_L          *=          kMakeUpGain
 aC_R          *=          kMakeUpGain
               outs        aC_L, aC_R
       
 ; meter
 if   metro:k(10)==1 then
 reinit REFRESH_METER
 endif
 REFRESH_METER:
 kres1,kres2 init    0
 kres1 limit   kres1-0.001,0,1 
 kres1 peak    aL
 kres2 limit   kres2-0.001,0,1 
 kres2 peak    aC_L
 rireturn
 kres1  lagud  kres1, 0.001, 0.01
 kres2  lagud  kres2, 0.001, 0.01
 chnset kres1, "vMeter1"              
 chnset kres2, "vMeter2"              

endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]   ; play instr 1 for 1 week
</CsScore>

</CsoundSynthesizer>