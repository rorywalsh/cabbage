
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; HighpassFilter.csd
; Written by Iain McCurdy, 2012, 2021

<Cabbage>
form caption("Highpass Filter"), size(435,100), pluginId("HPFl")
image pos(0, 0),                 size(435,100), colour(  70,120, 90), shape("rounded"), outlineColour("white"), outlineThickness(4) 
label    bounds( 17, 27, 40, 11), text("INPUT"), fontColour("white")
combobox bounds( 10, 38, 60, 20), channel("input"), value(1), text("Live","Tone","Noise")
rslider  bounds( 75, 16, 70, 70), channel("cf"),        text("Freq."), colour(150,210,180), trackerColour(230,255,230),     textColour("white"),     range(20, 20000, 20, 0.333)
rslider  bounds(140, 16, 70, 70), channel("res"),       text("Res."),  colour(150,210,180), trackerColour(230,255,230),     textColour("white"),    range(0,1.00,0), identChannel("resID"), visible(0)
rslider  bounds(205, 16, 70, 70), channel("mix"),       text("Mix"),   colour(150,210,180), trackerColour(230,255,230),        textColour("white"),     range(0,1.00,1)
combobox bounds(280, 28, 80, 20), text("12dB/oct","24dB/oct","36dB/oct","48dB/oct","60dB/oct","Resonant"), channel("type"), value(1)
label    bounds(280, 15, 80, 12), text("Type"), fontColour("white")
rslider  bounds(360, 16, 70, 70), text("Level"),    colour(150,210,180), trackerColour(230,255,230),        textColour("white"),     channel("level"),     range(0, 1.00, 1)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps     =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls     =     2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs    =    1
 
;Author: Iain McCurdy (2012)

opcode ButhpIt, a, akip
aIn,kCF,iNum,iCnt xin
aOut  =      0 
aOut  buthp  aIn, kCF
if iCnt<iNum then
 aOut  ButhpIt  aOut, kCF, iNum, iCnt+1
endif
xout aOut
endop

instr    1
    kporttime    linseg    0,0.001,0.05
    /* READ IN WIDGETS */
    kcf            chnget    "cf"
    kres        chnget    "res"
    kmix        chnget    "mix"
    ktype    chnget    "type"
    kResType    chnget    "ResType"
    klevel        chnget    "level"
    klevel        portk    klevel,kporttime
    alevel        interp    klevel
    kcf    portk    kcf,kporttime
    acf    interp    kcf
    /* INPUT */
    kinput        chnget    "input"
    if kinput=1 then
     aL,aR    ins
    elseif kinput=2 then
     aL    vco2    0.2, 100
     aR    =    aL
    else
     aL    pinkish    0.2
     aR    pinkish    0.2
    endif
    
    if changed:k(ktype)==1 then
     if ktype==6 then
         chnset  "visible(1)", "resID"
     else
          chnset  "visible(0)", "resID"
     endif
    endif
    
    /* FILTER */
    if ktype==6 then
     aFiltL    bqrez    aL,acf,1+(kres*40),1
     aFiltR    bqrez    aR,acf,1+(kres*40),1    
    else
     if changed:k(ktype)==1 then
      reinit RESTART_FILTER
     endif
     RESTART_FILTER:
     aFiltL ButhpIt aL, kcf, i(ktype)
     aFiltR ButhpIt aR, kcf, i(ktype)
     rireturn
    endif    
    aL    ntrpol    aL,aFiltL,kmix
    aR    ntrpol    aR,aFiltR,kmix
        outs    aL*alevel,aR*alevel
endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
e
</CsScore>

</CsoundSynthesizer>
