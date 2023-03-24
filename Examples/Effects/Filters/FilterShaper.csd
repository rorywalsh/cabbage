
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; FilterShaper.csd
; Iain McCurdy, 2015

; Gestural shaping using rspline random function generators applied to lowpass filter, highpass filter, panning and amplitude

<Cabbage>
form caption("Filter Shaper") size(595, 200), pluginId("FlSh")
image               bounds(  0, 0, 595, 200), colour(10,30,10), outlineColour("white"), outlineThickness(2), shape("sharp")

line     bounds( 15,  4,125, 15)
label    bounds( 15,  5,125, 12), fontColour("white"), text("R A T E")
rslider  bounds(  5, 20, 75, 75), text("Min"),  channel("RateMin"), range(0.01,20,0.5,0.5,0.01), textColour("white"), colour(37,59,59), trackerColour("silver"), markerColour("silver")
rslider  bounds( 75, 20, 75, 75), text("Max"),  channel("RateMax"), range(0.01,20,3,0.5,0.01), textColour("white"), colour(37,59,59), trackerColour("silver"), markerColour("silver")
checkbox bounds(160, 20, 75, 15), text("Bypass"),  channel("Bypass"), fontColour:0("white"), , fontColour:1("white")

image    bounds(220,  0,145, 95), colour(0,0,0,0), plant("panning") {
line     bounds( 15,  4,125, 15)
label    bounds( 15,  5,125, 12), fontColour("white"), text("P A N N I N G")
rslider  bounds(  5, 20, 75, 75), text("Width"),channel("PanWidth"), range(0,1,1), textColour("white"), colour(37,59,59), trackerColour("silver"), markerColour("silver")
rslider  bounds( 75, 20, 75, 75), text("Depth"),channel("PanDepth"), range(0,1,1), textColour("white"), colour(37,59,59), trackerColour("silver"), markerColour("silver")
}

image    bounds(445,  0,145, 95), colour(0,0,0,0), plant("amplitude") {
line     bounds( 10,  4,125, 15)
label    bounds( 10,  5,125, 12), fontColour("white"), text("A M P L I T U D E")
rslider  bounds(  0, 20, 75, 75), text("Min."),channel("AmpMin"), range(0,2,0.4,0.5,0.01), textColour("white"), colour(37,59,59), trackerColour("silver"), markerColour("silver")
rslider  bounds( 70, 20, 75, 75), text("Max."),channel("AmpMax"), range(0,2,0.8,0.5,0.01), textColour("white"), colour(37,59,59), trackerColour("silver"), markerColour("silver")
}

image    bounds( 15,100,285, 95), colour(0,0,0,0), plant("lowpass") {
line     bounds(  0,  4,275, 15)
label    bounds(  0,  5,275, 12), fontColour("white"), text("L O W P A S S   F I L T E R")
label    bounds(  0, 28, 75, 12), fontColour("white"), text("LPF Type")
combobox bounds(  0, 40, 75, 20), channel("LPFtype"), text("Bypass","6 db/oct","12 db/oct","24 db/oct","Resonant"), textColour("white"), colour(37,59,59), value(3)
rslider  bounds( 70, 20, 75, 75), text("Res."),channel("LPFres"), range(0,1,0.3,0.5,0.01), textColour("white"), colour(37,59,59), trackerColour("silver"), markerColour("silver"), identChannel("LPFresID")
rslider  bounds(140, 20, 75, 75), text("Min."),channel("LPFmin"), range(4,14, 6), textColour("white"), colour(37,59,59), trackerColour("silver"), markerColour("silver"), identChannel("LPFminID")
rslider  bounds(210, 20, 75, 75), text("Max."),channel("LPFmax"), range(4,14,14), textColour("white"), colour(37,59,59), trackerColour("silver"), markerColour("silver"), identChannel("LPFmaxID")
}

image    bounds(305,100,285, 95), colour(0,0,0,0), plant("highpass") {
line     bounds(  0,  4,275, 15)
label    bounds(  0,  5,275, 12), fontColour("white"), text("H I G H P A S S   F I L T E R")
label    bounds(  0, 28, 75, 12), fontColour("white"), text("HPF Type")
combobox bounds(  0, 40, 75, 20), channel("HPFtype"), text("Bypass","6 db/oct","12 db/oct","24 db/oct","Resonant"), textColour("white"), colour(37,59,59), value(3)
rslider  bounds( 70, 20, 75, 75), text("Res."),channel("HPFres"), range(0,1,0.3,0.5,0.01), textColour("white"), colour(37,59,59), trackerColour("silver"), markerColour("silver"), identChannel("HPFresID")
rslider  bounds(140, 20, 75, 75), text("Min."),channel("HPFmin"), range(4,14, 4), textColour("white"), colour(37,59,59), trackerColour("silver"), markerColour("silver"), identChannel("HPFminID")
rslider  bounds(210, 20, 75, 75), text("Max."),channel("HPFmax"), range(4,14, 6), textColour("white"), colour(37,59,59), trackerColour("silver"), markerColour("silver"), identChannel("HPFmaxID")
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>
;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1                                                         
 kRateMin    chnget "RateMin"
 kRateMax    chnget "RateMax"
 kLPFtype    chnget "LPFtype"
 kLPFres    chnget "LPFres"
 kLPFmin    chnget "LPFmin"
 kLPFmax    chnget "LPFmax"

 kHPFtype    chnget "HPFtype"
 kHPFres    chnget "HPFres"
 kHPFmin    chnget "HPFmin"
 kHPFmax    chnget "HPFmax"

 kPanWidth    chnget "PanWidth"
 kPanDepth    chnget "PanDepth"

 kAmpMin    chnget "AmpMin"
 kAmpMax    chnget "AmpMax"

 kBypass    chnget    "Bypass"
 
 aL,aR    ins
 
 if kBypass==1 then
      outs    aL,aR
      kgoto BYPASS
 endif
 
 ;aL    vco2    0.1,100
 ;aR    vco2    0.1,100
 
 kLPFoct    rspline    kLPFmin,kLPFmax,kRateMin,kRateMax
 kHPFoct    rspline    5,14,kRateMin,kRateMax

 kLPFoct    limit    kLPFoct,4,14
 kHPFoct    limit    kHPFoct,4,14
 
 if kLPFtype==2 then
  aL    tone    aL,cpsoct(kLPFoct)
  aR    tone    aR,cpsoct(kLPFoct)
 elseif kLPFtype==3 then
  aLPFoct    interp    kLPFoct
  aL    butlp    aL,cpsoct(aLPFoct)
  aR    butlp    aR,cpsoct(aLPFoct)
 elseif kLPFtype==4 then
  aLPFoct    interp    kLPFoct
  aL    butlp    aL,cpsoct(aLPFoct)
  aR    butlp    aR,cpsoct(aLPFoct)
  aL    butlp    aL,cpsoct(aLPFoct)
  aR    butlp    aR,cpsoct(aLPFoct)
 elseif kLPFtype==5 then
  aL    moogladder    aL,cpsoct(kLPFoct),kLPFres
  aR    moogladder    aR,cpsoct(kLPFoct),kLPFres
 endif

 if kHPFtype==2 then
  aL    atone    aL,cpsoct(kHPFoct)
  aR    atone    aR,cpsoct(kHPFoct)
 elseif kHPFtype==3 then
  aHPFoct    interp    kHPFoct
  aL    buthp    aL,cpsoct(aHPFoct)
  aR    buthp    aR,cpsoct(aHPFoct)
 elseif kHPFtype==4 then
  aHPFoct    interp    kHPFoct
  aL    buthp    aL,cpsoct(aHPFoct)
  aR    buthp    aR,cpsoct(aHPFoct)
  aL    buthp    aL,cpsoct(aHPFoct)
  aR    buthp    aR,cpsoct(aHPFoct)
 elseif kHPFtype==5 then
  aL    bqrez    aL,cpsoct(kHPFoct),1+(kHPFres*40),1
  aR    bqrez    aR,cpsoct(kHPFoct),1+(kHPFres*40),1
 endif

 aPan    rspline    -kPanWidth, kPanWidth, kRateMin, kRateMax
 aPan    =    (aPan*0.5) + 0.5 
 aOutL  =     aL * sin((aPan + 0.5) * $M_PI_2)
 aOutR  =     aR * cos((aPan + 0.5) * $M_PI_2)

 if kPanDepth>0 then
  aOutL    vdelay    aOutL, 0.1+(aPan*kPanDepth*3), 20
  aOutR    vdelay    aOutR, 0.1+((1-aPan)*kPanDepth*3), 20
 endif

 aAmp    rspline    kAmpMin, kAmpMax, kRateMin, kRateMax

    outs    aOutL*aAmp,aOutR*aAmp
 BYPASS:
endin

</CsInstruments>

<CsScore>                                              
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>                                                  