
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; DistanceEmulator.csd
; Written by Iain McCurdy, 2015
    
<Cabbage>
form caption("Distance Emulator") size(700,300), colour(200,210,180), pluginId("DsEm")
hslider   bounds(  5,  5,240, 30), channel("Amp1"), text("Amp.1"),    colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(0, 1, 1, 0.5, 0.001)
hslider   bounds(255,  5,240, 30), channel("Amp2"), text("Amp.2"),    colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(0, 1, 0.25, 0.5, 0.001)
hslider   bounds(  5, 55,240, 30), channel("Rvb1"), text("Rvb.1"),    colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(0, 1, 0, 0.5, 0.001)
hslider   bounds(255, 55,240, 30), channel("Rvb2"), text("Rvb.2"),    colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(0, 1, 1, 0.5, 0.001)
nslider bounds(505, 55, 70, 30), channel("RvbSize"), text("Size"),  colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(0,0.999, 0.8,0.5,0.001)
nslider bounds(585, 55, 70, 30), channel("RvbLPF"), text("RvbLPF"), colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(20,20000,5000,0.5)
hslider   bounds(  5,105,240, 30), channel("LPF1"), text("LPF.1"),    colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(0, 1, 0)
hslider   bounds(255,105,240, 30), channel("LPF2"), text("LPF.2"),    colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(0, 1, 1)
nslider bounds(505,105, 70, 30), channel("LPF_CF"), text("Cutoff"), colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(20,20000,5000,0.5)
hslider   bounds(  5,155,240, 30), channel("HPF1"), text("HPF.1"),    colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(0, 1, 0)
hslider   bounds(255,155,240, 30), channel("HPF2"), text("HPF.2"),    colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(0, 1, 1)
nslider bounds(505,155, 70, 30), channel("HPF_CF"), text("Cutoff"), colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(20,20000,150,0.5)
hslider   bounds(  5,205,240, 30), channel("Wid1"), text("Width"),    colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(0, 1, 1)
hslider   bounds(255,205,240, 30), channel("Wid2"), text("Width"),    colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(0, 1, 1)
hslider   bounds(  5,255,690, 30), channel("Mor"), text("Morph"),     colour(100,140,150), trackerColour(200,240,250), textColour("DarkSlateGrey"), range(0, 1, 0)

</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1


instr    1
 kPortTime    linseg    0,0.001,0.2
 kAmp1        chnget    "Amp1"
 kAmp2        chnget    "Amp2"
 kRvb1        chnget    "Rvb1"
 kRvb2        chnget    "Rvb2"
 kRvbSize    chnget    "RvbSize"
 kRvbLPF    chnget    "RvbLPF"
 kLPF1        chnget    "LPF1"
 kLPF2        chnget    "LPF2"
 kLPF_CF    chnget    "LPF_CF"
 kHPF1        chnget    "HPF1"
 kHPF2        chnget    "HPF2"
 kHPF_CF    chnget    "HPF_CF"
 kWid1        chnget    "Wid1"
 kWid2        chnget    "Wid2"
 kMor        chnget    "Mor"
 kMor        portk    kMor,kPortTime
 
 aL,aR    ins
 ;aL,aR    diskin2    "ClassicalGuitar.wav",1,0,1
 
 kAmp    ntrpol    kAmp1,kAmp2,kMor
 kRvb    ntrpol    kRvb1,kRvb2,kMor
 kLPF    ntrpol    kLPF1,kLPF2,kMor
 kHPF    ntrpol    kHPF1,kHPF2,kMor
 kWid    ntrpol    kWid1,kWid2,kMor
 kHPF    pow    kHPF,0.2
 kLPF    pow    kLPF,0.2
 
 aL    *=    kAmp^2
 aR    *=    kAmp^2

 aLPF_L    butlp    aL,kLPF_CF
 aLPF_R    butlp    aR,kLPF_CF
 aL        ntrpol        aL,aLPF_L,kLPF 
 aR        ntrpol        aR,aLPF_R,kLPF 
     
 aHPF_L    buthp    aL,kHPF_CF
 aHPF_R    buthp    aR,kHPF_CF
 aL        ntrpol        aL,aHPF_L,kHPF 
 aR        ntrpol        aR,aHPF_R,kHPF 

 aRvbL,aRvbR    reverbsc    aL,aR,kRvbSize,kRvbLPF
 aL        ntrpol        aL,aRvbL,kRvb 
 aR        ntrpol        aR,aRvbR,kRvb 
 
 aWidL        ntrpol        aL,(aL+aR)*0.75,kWid
 aWidR        ntrpol        aR,(aL+aR)*0.75,kWid
 aL        =        aWidL
 aR        =        aWidR
 
     outs    aL,aR
 
 

endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>