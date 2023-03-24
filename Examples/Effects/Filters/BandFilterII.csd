
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; BandFilterII.csd
; Written by Iain McCurdy, 2015.

; Modify filter limits using the xypad

; The limit double slider is more just for display.
                                                                                   
<Cabbage>                                                                                                                   
form caption("Bandfilter II"), colour(20,20,20), size(510, 655), pluginId("BFII"), guiMode("queue")

checkbox bounds(9,3,90,15), channel("TestSound"), text("Test Sound"), value(0), fontColour(150,150,150)

xypad bounds(5,20, 500, 500), channel("shift", "width"), rangeX(0, 1, 0.5), rangeY(0, 1, 0.5), text("x:cutoff y:bandwidth"), colour(20,20,30), fontColour("white")

hrange   bounds(  5,513, 500, 40), channel("Lim1","Lim2"), range(4, 14, 4:14, 1, 0.001)

label    bounds(  5,543,500, 13), text("L I M I T S"), fontColour(150,150,150)

checkbox bounds( 15,570, 80, 11), channel("HPF_active"), text("HPF Active"), fontColour(150,150,150), value(1)
label    bounds( 15,595, 80, 13), text("HPF Type"), fontColour(150,150,150)
combobox bounds( 15,610, 80, 20), value(2), channel("HPF_type"),   text("6dB/oct","12dB/oct","24dB/oct","36dB/oct","48dB/oct","Resonant"), colour("black")
rslider  bounds( 95,560, 60, 85), range(0,1,0),  channel("HPF_res"),  identChannel("HPF_resID"),   text("Res"),   textBox(1), colour("black"), fontColour(150,150,150), textColour("black"), fontColour(150,150,150)

checkbox bounds(175,570, 80, 11), channel("LPF_active"), text("LPF Active"), fontColour(150,150,150), value(1)
label    bounds(175,595, 80, 13), text("LPF Type"), fontColour(150,150,150)
combobox bounds(175,610, 80, 20), value(2), channel("LPF_type"),  text("6dB/oct","12dB/oct","24dB/oct","36dB/oct","48dB/oct","Resonant"), colour("black")
rslider  bounds(255,560, 60, 85), range(0,1,0),  channel("LPF_res"),  identChannel("LPF_resID"),   text("Res"),   textBox(1), colour("black"), fontColour(150,150,150), textColour("black")

combobox bounds(370,565, 95, 20), channel("PassRej"), text("Bandpass","Bandreject","Bypass"), value(1)

hslider bounds(330,605,170,15), range(0,0.1,0.03,0.75,0.0001),  channel("Smoothing"), textColour("black")

hslider bounds(330,635,170,15), range(0,1,1),  channel("Gain"), textColour("black")
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-dm0 -n -+rtmidi=NULL -M0
</CsOptions>

<CsInstruments>

;sr is set by host
ksmps   =   32
nchnls  =   2
0dbfs   =   1

; Author: Iain McCurdy (2015)

opcode HighpassNetwork, aa, aakkk
 aL,aR,kHPF,kHPF_res,kHPF_type  xin
 if kHPF_type==1 then 
  aL      atone                aL, cpsoct(kHPF)
  aR      atone                aR, cpsoct(kHPF)
 elseif kHPF_type==2 then 
  aL      buthp                aL, cpsoct(kHPF)
  aR      buthp                aR, cpsoct(kHPF)
 elseif kHPF_type==3 then 
  aL      buthp                aL, cpsoct(kHPF)
  aR      buthp                aR, cpsoct(kHPF)
  aL      buthp                aL, cpsoct(kHPF)
  aR      buthp                aR, cpsoct(kHPF)
 elseif kHPF_type==4 then 
  aL      buthp                aL, cpsoct(kHPF)
  aR      buthp                aR, cpsoct(kHPF)
  aL      buthp                aL, cpsoct(kHPF)
  aR      buthp                aR, cpsoct(kHPF)
  aL      buthp                aL, cpsoct(kHPF)
  aR      buthp                aR, cpsoct(kHPF)
 elseif kHPF_type==5 then 
  aL      buthp                aL, cpsoct(kHPF)
  aR      buthp                aR, cpsoct(kHPF)
  aL      buthp                aL, cpsoct(kHPF)
  aR      buthp                aR, cpsoct(kHPF)
  aL      buthp                aL, cpsoct(kHPF)
  aR      buthp                aR, cpsoct(kHPF)
  aL      buthp                aL, cpsoct(kHPF)
  aR      buthp                aR, cpsoct(kHPF)
 elseif kHPF_type==6 then  
  aL      bqrez                aL, cpsoct(kHPF), 1 + (kHPF_res * 40), 1
  aR      bqrez                aR, cpsoct(kHPF), 1 + (kHPF_res * 40), 1
 endif
          xout                 aL, aR
endop

opcode LowpassNetwork,aa,aakkk
 aL,aR,kLPF,kLPF_res,kLPF_type  xin
 if kLPF_type==1 then                                           
  aL          tone             aL, cpsoct(kLPF)                                
  aR          tone             aR, cpsoct(kLPF)
 elseif kLPF_type==2 then 
  aL          butlp            aL, cpsoct(kLPF)
  aR          butlp            aR, cpsoct(kLPF)
 elseif kLPF_type==3 then 
  aL          butlp            aL, cpsoct(kLPF)
  aR          butlp            aR, cpsoct(kLPF)
  aL          butlp            aL, cpsoct(kLPF)
  aR          butlp            aR, cpsoct(kLPF)
 elseif kLPF_type==4 then 
  aL          butlp            aL, cpsoct(kLPF)
  aR          butlp            aR, cpsoct(kLPF)
  aL          butlp            aL, cpsoct(kLPF)
  aR          butlp            aR, cpsoct(kLPF)
  aL          butlp            aL, cpsoct(kLPF)
  aR          butlp            aR, cpsoct(kLPF)
 elseif kLPF_type==5 then 
  aL          butlp            aL, cpsoct(kLPF)
  aR          butlp            aR, cpsoct(kLPF)
  aL          butlp            aL, cpsoct(kLPF)
  aR          butlp            aR, cpsoct(kLPF)
  aL          butlp            aL, cpsoct(kLPF)
  aR          butlp            aR, cpsoct(kLPF)
  aL          butlp            aL, cpsoct(kLPF)
  aR          butlp            aR, cpsoct(kLPF)
 elseif kLPF_type==6 then
  aL          moogladder       aL,cpsoct(kLPF),kLPF_res
  aR          moogladder       aR,cpsoct(kLPF),kLPF_res
 endif
              xout             aL, aR
endop

instr   1
 kshift       cabbageGetValue           "shift"
 kwidth       cabbageGetValue           "width"
 kgain        cabbageGetValue           "Gain"
 kPassRej     cabbageGetValue           "PassRej"
 kPassRej     init             1
 kTestSound   cabbageGetValue           "TestSound"
 kSmoothing   cabbageGetValue           "Smoothing"
 
 kLPF_type    cabbageGetValue           "LPF_type"
 kHPF_type    cabbageGetValue           "HPF_type"
 kLPF_type    init             2
 kHPF_type    init             2
 kLPF_res     cabbageGetValue           "LPF_res"
 kHPF_res     cabbageGetValue           "HPF_res"

 kLPF_active  cabbageGetValue           "LPF_active"
 kHPF_active  cabbageGetValue           "HPF_active"

 if changed(kHPF_type)==1 then
  if kHPF_type==6 then
              cabbageSet        k(1), "HPF_res", "visible", 1
  else
              cabbageSet        k(1), "HPF_res", "visible", 0
  endif
 endif  

 if changed(kLPF_type)==1 then
  if kLPF_type==6 then
              cabbageSet        k(1), "LPF_res", "visible", 1
  else
              cabbageSet        k(1), "LPF_res", "visible", 0
  endif
 endif  

 kLim1        limit            scale:k(limit:k(kshift - (kwidth * kshift),0,1), 14, 4), 4, 14
 kLim2        limit            scale:k(limit:k(kshift + (kwidth * kshift),0,1), 14, 4), 4, 14
 
       cabbageSet changed:k(kLim1), "Lim1", "minValue", kLim1  
       cabbageSet changed:k(kLim2), "Lim1", "maxValue", kLim2


 if kTestSound==1 then
  aL          pinkish          0.5                      ; use for testing
  aR          pinkish          0.5                      ; use for testing
 else
  aL,aR ins
 endif
 
 kporttime    linseg           0, 0.01, 1
 kporttime    *=               kSmoothing
 kLim1        portk            kLim1, kporttime
 kLim2        portk            kLim2, kporttime
 
 if kPassRej==1 then
  if kHPF_active==1 then
   aL,aR      HighpassNetwork  aL, aR, kLim1, kHPF_res, kHPF_type
  endif
  if kLPF_active==1 then
   aL,aR    LowpassNetwork     aL, aR, kLim2, kLPF_res, kLPF_type
  endif
 elseif kPassRej==2 then
  if kHPF_active==1 then
   aL1,aR1  HighpassNetwork    aL, aR, kLim2, kHPF_res, kHPF_type
  endif
  if kLPF_active==1 then  
   aL2,aR2  LowpassNetwork     aL, aR, kLim1, kLPF_res, kLPF_type
  endif
  aL1, aR1, aL2, aR2  init    0
  aL        sum                aL1, aL2
  aR        sum                aR1, aR2
            clear              aL1, aR1, aL2, aR2
 endif

            outs               aL * kgain, aR * kgain

endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>