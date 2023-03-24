
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; FilterLFO.csd
; Author: Iain McCurdy (2013)

; INTRODUCTION
; ------------
; multimode filter with a 2 multimode LFOs on the cutoff frequency
; additional controls for specific filter models are accessed using the pop-up buttons

; The outputs of both LFOs are added to the main cutoff frequency (Freq.)
; LFO amplitude are in 'octaves'
; LFO rates are in beats per minute
; 'Smoothing' adds a small amount of portamento to changes in cutoff frequency
;   this can be useful in square wave-type modulations 

; 'Type' is ignored when 'moogladder', 'resonz', 'phaser2' or 'resony' are chosen as 'model'

; some subtlety is required when using the more complex filter models (e.g. phaser2, resony)
;    often low LFO amplitudes and rate work better with these

; CONTROL
; -------
; Freq.                  --    global manual frequency control. Like an LFO offset value.
; Res.                   --    resonance control. Affects moogladder, resonz and phaser2 models
; Mix                    --    dry/wet (filtered/unfiltered) mix
; Level                  --    global output level control
; Model                  --    (drop down menu) filter model
; Type                   --    Filter type: highpass/lowpass - affects tone, butterworth, cl-butterworth, cl-Chebychev models only
; Input                  --    choose between live input and (for testing) sawtooth tone and pink noise
; Resync                 --    resync. (i.e. restart) the two LFOs
; clfilt/phaser2/resony  --    pop-up panels with further controls for these models
; LFO Type               --    sine, triangle, square (bipolar), square (unipolar), sawtooth up, sawtooth down, random sample & hold and random spline
; LFO Amp                --    amplitude of the LFOs
; LFO Rate               --    rates of the LFOs in beats per minute
; Link Rates             --    when this switch is activate Rate controls for the two LFOs will be linked
; LFO Rate Div.          --    integer division of Rate control value (unaffected by 'Link Rates' setting)
; Smooth                 --    a small amount of smoothing can be appied to the LFO to smooth abrupt changes in value (may be useful and 'square' and 'rand.S&H' type modulations

<Cabbage>
form caption("Filter LFO") size(790,170), pluginId("FLFO")
image pos(0, 0),           size(790,170), colour(0,0,0,170), shape("rounded"), outlineColour("white"), outlineThickness(4) 
rslider bounds( 10, 11, 70, 70), text("Freq."), colour( 30, 30, 30),    trackerColour("white"),    fontColour("white"),         channel("cf"),         range(1, 20000, 300, 0.333), markerColour("silver")
rslider bounds( 75, 11, 70, 70), text("Res."),  colour( 30, 30, 30),    trackerColour("white"),    fontColour("white"),         channel("res"),        range(0,1.00,0.75), markerColour("silver")
rslider bounds(140, 11, 70, 70), text("Mix"),   colour( 30, 30, 30),    trackerColour("white"),    fontColour("white"),         channel("mix"),     range(0,1.00,1), markerColour("silver")
rslider bounds(205, 11, 70, 70), text("Level"), colour( 30, 30, 30),    trackerColour("white"),    fontColour("white"),         channel("level"),     range(0, 1.00, 0.2), markerColour("silver")

label    bounds( 40, 90, 50, 12), text("MODEL"), fontColour("white")
combobox bounds( 20,102, 100, 18), channel("model"), value(6), text("Tone","Butterworth","Moogladder","cl-Butterworth","cl-Chebychev I","resonz","phaser2","resony")

label    bounds(160, 90, 50, 12), text("TYPE"),  fontColour("white")
combobox bounds(140,102, 100, 18), channel("type"), value(1), text("Low-pass","High-pass")

label    bounds( 40,127, 50, 12), text("INPUT"), fontColour("white")
combobox bounds( 20,139,100, 18), channel("input"), value(2), text("Live","Tone","Noise")

button   bounds(140,139, 80, 18), colour:0(50,150,50), colour:1(150,255,150), text("RESYNC.", "RESYNC."), channel("resync"), latched(0)

; controls pertaining to the setup of clfilt accessed in a pop-up panel.
button   bounds(280, 15,100, 30), channel("clfiltButton"), text("clfilt")
groupbox bounds(280, 15,150, 90),  colour("black"), plant("clfilt"), outlineThickness(0), popup(1), identChannel("clfiltPlant");, fontColour("white")
{
rslider bounds(  5, 16, 70, 70), text("N.Poles"), colour( 30 , 30, 30),    trackerColour("white"), fontColour("white"), channel("npol"),   range(2, 80, 2, 1, 2)
rslider bounds( 75, 16, 70, 70), text("Ripple"),  colour( 30 , 30, 30),    trackerColour("white"), fontColour("white"), channel("pbr"),    range(0.1, 50.00, 1, 0.5, 0.001)
}

; controls pertaining to the setup of phaser2 accessed in a pop-up panel.
button   bounds(280, 55,100, 30), channel("phaser2Button"), text("phaser2")
groupbox bounds(280, 55,315, 90), colour("black"), plant("phaser2"), outlineThickness(0), popup(1), identChannel("phaser2Plant");, fontColour(white)
{
rslider  bounds(  5, 16, 70, 70), text("Q"),       channel("q"),   range(0.0001,4,3),       colour( 30 , 30, 30),    trackerColour("white"), fontColour("white"), markerColour("silver")
rslider  bounds( 75, 16, 70, 70), text("N.Ords."), channel("ord"), range(1, 256, 8, 0.5,1), colour( 30 , 30, 30),    trackerColour("white"), fontColour("white"), markerColour("silver")
label    bounds(145, 20, 90, 14), text("Sep. Mode:"), fontColour("white")
combobox bounds(145, 35, 90, 20), channel("mode"), value(1), text("Equal", "Power"), colour( 30 , 30, 30),    trackerColour("white"), fontColour("white")
rslider  bounds(240, 16, 70, 70), text("Separation"), channel("sep"), range(-3, 3.00, 0.9), colour( 30 , 30, 30),    trackerColour("white"), fontColour("white"), markerColour("silver")
}

; controls pertaining to the setup of resony accessed in a pop-up panel.
button   bounds(280, 95,100, 30), channel("resonyButton"), text("resony")
groupbox bounds(280, 95,565, 90),  colour("black"), plant("resony"), outlineThickness(0), popup(1), identChannel("resonyPlant");, fontColour(white)
{
rslider  bounds(  5, 16, 70, 70), text("BW."),           fontColour("white"), channel("bw"),    range(0.01, 1000, 13, 0.5), colour( 30 , 30, 30),    trackerColour("white"), markerColour("silver")
rslider  bounds( 75, 16, 70, 70), text("Num."),          fontColour("white"), channel("num"),   range(1, 80, 10, 1,1),      colour( 30 , 30, 30),    trackerColour("white"), markerColour("silver")
rslider  bounds(145, 16, 70, 70), text("Sep.oct."),      fontColour("white"), channel("sepR"),  range(-11, 11, 2),          colour( 30 , 30, 30),    trackerColour("white"), markerColour("silver")
rslider  bounds(215, 16, 70, 70), text("Sep.semi."),     fontColour("white"), channel("sepR2"), range(-48, 48, 24,1,1),     colour( 30 , 30, 30),    trackerColour("white"), markerColour("silver")
label    bounds(285, 20,130, 14), text("Scaling Mode"), fontColour("white")
combobox bounds(285, 35,130, 20), channel("scl"), value(2), text("none", "peak response", "RMS")
label    bounds(425, 20,130, 14), text("Separation Mode"), fontColour("white")
combobox bounds(425, 35,130, 20), channel("sepmode"), value(1), text("octs.total", "hertz", "octs.adjacent")
}

;checkbox bounds(400, 50,100, 15), text("Balance") channel("balance"), fontColour("white"), colour("yellow")  value(0)


line bounds(390, 10,  2,150), colour("Grey")

;LFO
label    bounds(405, 11, 45, 17), text("LFO 1"), fontColour("white")
combobox bounds(405, 50, 100, 18), channel("LFOtype"), value(3), text("Sine","Triangle","Square[bi]","Square[uni]","Saw Up","Saw Down","Rand.S&H","Rand.Spline")
rslider  bounds(515, 11, 70, 70), text("Amp"),      colour( 30, 30 ,30),    trackerColour("white"), fontColour("white"), channel("LFOamp"), range(0, 9.00, 0.67), markerColour("silver")
rslider  bounds(580, 11, 70, 70), text("Rate"),     colour( 30, 30 ,30),    trackerColour("white"), fontColour("white"), channel("LFOBPM"), range(0, 480, 60, 1, 1), markerColour("silver")
rslider  bounds(645, 11, 70, 70), text("Rate Div."),colour( 30, 30 ,30),    trackerColour("white"), fontColour("white"), channel("LFOBPMDiv"), range(1, 64, 1, 1, 1), markerColour("silver")
rslider  bounds(710, 11, 70, 70), text("Smoothing"),colour( 30, 30 ,30),    trackerColour("white"), fontColour("white"), channel("LFOport"), range(0, 0.1, 0.005, 0.25, 0.000001), markerColour("silver")
checkbox bounds(405, 31, 80, 12), text("Link Rates"), channel("RateLink"),colour(yellow), fontColour:0("white"), fontColour:1("white"),  value(0)

;LFO2
label    bounds(405, 91, 45, 17), text("LFO 2"), fontColour("white")
combobox bounds(405,130, 100, 18), channel("LFOtype2"), value(8), text("Sine","Triangle","Square[bi]","Square[uni]","Saw Up","Saw Down","Rand.S&H","Rand.Spline")
rslider  bounds(515, 91, 70, 70), text("Amp"), colour( 30, 30 ,30),            trackerColour("white"), fontColour("white"), channel("LFOamp2"), range(0, 9.00, 2.5), markerColour("silver")
rslider  bounds(580, 91, 70, 70), text("Rate"),colour( 30, 30 ,30),            trackerColour("white"), fontColour("white"), channel("LFOBPM2"), range(0, 480,  1, 1, 1), markerColour("silver")
rslider  bounds(645, 91, 70, 70), text("Rate Div."),colour( 30, 30 ,30),    trackerColour("white"), fontColour("white"), channel("LFOBPMDiv2"), range(1, 64, 1, 1, 1), markerColour("silver")
rslider  bounds(710, 91, 70, 70), text("Smoothing"),colour( 30, 30 ,30),    trackerColour("white"), fontColour("white"), channel("LFOport2"), range(0, 0.1, 0.001, 0.25, 0.000001), markerColour("silver")

label   bounds(220,150, 170, 12), text("Author: Iain McCurdy |2013|"), fontColour("grey")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

;sr is set by the host
ksmps   = 32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls  = 2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs   = 1

; Author: Iain McCurdy (2013)

opcode    resony2,a,akkikii
 ain, kbf, kbw, inum, ksep , isepmode, iscl    xin
 
 ;IF 'Octaves (Total)' MODE SELECTED...
 if isepmode==0 then
  ireScale  divz   inum,inum-1,1    ; PREVENT ERROR IF NUMBER OF FILTERS = ZERO
  ksep      =      ksep * ireScale         ; RESCALE SEPARATION
 
 ;IF 'Hertz' MODE SELECTED...    
 elseif isepmode==1 then
  inum      =      inum + 1
  ksep      =      inum
 
 ;IF 'Octaves (Adjacent)' MODE SELECTED...
 elseif isepmode==2 then 
  ireScale  divz   inum,inum-1,1    ; PREVENT ERROR IF NUMBER OF FILTERS = ZERO
  ksep      =      ksep * ireScale               ; RESCALE SEPARATION
  ksep      =      ksep * (inum-1)               ; RESCALE SEPARATION INTERVAL ACCORDING TO THE NUMBER OF FILTERS CHOSEN
  isepmode  =      0
 endif
 
 aout         resony     ain, kbf, kbw, inum, ksep , isepmode, iscl, 0
         xout    aout
endop


instr    1
kporttime    linseg    0, 0.001, 1

kcf          chnget    "cf"                ;
kcf          portk     kcf,kporttime*0.05
kres         chnget    "res"               ;
kmodel       chnget    "model"             ;
ktype        chnget    "type"              ;
kresync      chnget    "resync"            ;

kmix         chnget    "mix"               ;
klevel       chnget    "level"             ;

kpbr         chnget    "pbr"               ;
kpbr         init      1
knpol        chnget    "npol"              ;
knpol        init      2

kq           chnget    "q"                 ;
kmode        chnget    "mode"              ;
kmode        init      1
kmode        init      i(kmode)-1
ksep         chnget    "sep"               ;
kfeedback    chnget    "feedback"          ;
kord         chnget    "ord"               ;

;resony
kbw          chnget    "bw"
knum         chnget    "num"
ksepR        chnget    "sepR"
ksepR2       chnget    "sepR2"
ksepmode     chnget    "sepmode"
ksepmode     =         ksepmode - 1
ksepmode     init      1
kscl         chnget    "scl"
kscl         =         kscl - 1
kscl         init      1

;kbalance    chnget    "balance"          ;

kLFOtype     chnget    "LFOtype"
kLFOamp      chnget    "LFOamp"
kLFOamp      portk     kLFOamp, kporttime*0.05
kLFOBPM      chnget    "LFOBPM"
kLFOBPMDiv   chnget    "LFOBPMDiv"
kLFOcps      =         (kLFOBPM*4)/(60*kLFOBPMDiv)
kLFOport     chnget    "LFOport"

kLFOtype2    chnget    "LFOtype2"
kLFOamp2     chnget    "LFOamp2"
kLFOamp2     portk     kLFOamp2, kporttime*0.05
kLFOBPM2     chnget    "LFOBPM2"
kLFOBPMDiv2  chnget    "LFOBPMDiv2"
kLFOcps2     =         (kLFOBPM2*4)/(60*kLFOBPMDiv2)
kLFOport2    chnget    "LFOport2"

kRateLink    chnget    "RateLink"
if kRateLink=1 then
 ktrig1      changed   kLFOBPM
 ktrig2      changed   kLFOBPM2
 if ktrig1=1 then
             chnset    kLFOBPM,"LFOBPM2"
 elseif ktrig2=1 then
             chnset    kLFOBPM2,"LFOBPM"
 endif
endif
/* INPUT */
kinput       chnget    "input"
if kinput=1 then
 aL,aR    ins
elseif kinput=2 then
 aL          vco2       0.2, 150
 aR          =          aL
else
 aL          pinkish    0.2
 aR          pinkish    0.2
endif

; RETRIGGERING FOR I-RATE VARIABLE
kLFOtype     init       1
kLFOtype2    init       1

ktrig    changed    knpol,kpbr,kLFOtype,kLFOtype2,kmodel,ktype,kord,kmode, kscl, knum, ksepmode, kresync,kLFOBPMDiv,kLFOBPMDiv2,kRateLink
if ktrig==1 then
 reinit UPDATE
endif
UPDATE:

; LFO
if i(kLFOtype)==7 then
 klfo    randomh    -kLFOamp, kLFOamp, kLFOcps
elseif    i(kLFOtype)==8 then
 klfo    jspline    kLFOamp, kLFOcps, kLFOcps*5
else
 klfo        lfo    kLFOamp, kLFOcps, i(kLFOtype)-1
endif
klfo        portk    klfo,kporttime*kLFOport
;kcf        limit    kcf * octave(klfo),20,sr/2

; LFO2
if i(kLFOtype2)==7 then
 klfo2    randomh    -kLFOamp2, kLFOamp2, kLFOcps2
elseif    i(kLFOtype2)==8 then
 klfo2    jspline    kLFOamp2, kLFOcps2, kLFOcps2*5
else
 klfo2        lfo    kLFOamp2, kLFOcps2, i(kLFOtype2)-1
endif
klfo2        portk    klfo2,kporttime*kLFOport2
kcf        limit    kcf * octave(klfo) * octave(klfo2),20,sr/2


; FILTER MODEL SELECT
if i(kmodel)==1 then
 if i(ktype)==1 then            ; tone
  aFiltL    tone    aL, kcf
  aFiltR    tone    aR, kcf
 else
  aFiltL    atone    aL, kcf
  aFiltR    atone    aR, kcf
 endif
elseif i(kmodel)==2 then        ; butterworth
 if i(ktype)==1 then
  aFiltL    butlp    aL, kcf
  aFiltR    butlp    aR, kcf
 else
  aFiltL    buthp    aL, kcf
  aFiltR    buthp    aR, kcf        
 endif
elseif i(kmodel)==3 then            ; moogladder
 kres    scale        kres,0.95,0
 aFiltL    moogladder    aL,kcf,kres
 aFiltR    moogladder    aR,kcf,kres        
elseif i(kmodel)==4 then            ; cl-butterworth
 if i(ktype)==1 then
  aFiltL    clfilt    aL, kcf, 0, i(knpol)
  aFiltR    clfilt    aR, kcf, 0, i(knpol)
 else
  aFiltL    clfilt    aL, kcf, 1, i(knpol)
  aFiltR    clfilt    aR, kcf, 1, i(knpol)
 endif
elseif i(kmodel)==5 then            ; cl-chebychev I
 if i(ktype)==1 then
  aFiltL    clfilt    aL, kcf, 0, i(knpol), 1, i(kpbr)
  aFiltR    clfilt    aR, kcf, 0, i(knpol), 1, i(kpbr)
 else
  aFiltL    clfilt    aL, kcf, 1, i(knpol), 1, i(kpbr)
  aFiltR    clfilt    aR, kcf, 1, i(knpol), 1, i(kpbr)
 endif
elseif i(kmodel)==6 then            ; resonz
 kres    logcurve    kres,4
 kbw    scale    1-kres,3,0.1
 aFiltL    resonz    aL, kcf, kcf*kbw, 1
 aFiltR    resonz    aR, kcf, kcf*kbw, 1
elseif i(kmodel)==7 then            ; phaser2
 kfeedback    scale    kres,0.99,0
 aFiltL    phaser2        aL, kcf, kq, kord, kmode, ksep, kfeedback    ; PHASER2 IS APPLIED TO THE LEFT CHANNEL
 aFiltR    phaser2        aR, kcf, kq, kord, kmode, ksep, kfeedback    ; PHASER1 IS APPLIED TO THE RIGHT CHANNEL
elseif i(kmodel)==8 then            ; resony    
 ;CALL resony2 UDO
 aFiltL         resony2 aL, kcf, kbw, i(knum), ksepR , i(ksepmode), i(kscl)
 aFiltR         resony2    aR, kcf, kbw, i(knum), ksepR , i(ksepmode), i(kscl)
endif

rireturn

/*
if kbalance==1 then
 aFiltL    balance    aFiltL,aL
 aFiltR    balance    aFiltR,aR
endif
*/


aL       ntrpol    aL,aFiltL,kmix
aR       ntrpol    aR,aFiltR,kmix
         outs      aL*klevel,aR*klevel
endin

instr    UpdateWidgets
ksepR    chnget    "sepR"
ksepR2   chnget    "sepR2"
ktrig1   changed   ksepR
ktrig2   changed   ksepR2
if ktrig1==1 then
 chnset  ksepR*12, "sepR2"
elseif  ktrig2==1 then
         chnset    ksepR2/12, "sepR"
endif
endin

instr 1000                ; launches plant popups

klaunch    init    0

#define LAUNCH_PLANT(name)
#
kpressed    chnget "$name.Button"
if changed(kpressed)==1 then
  Smsg sprintfk "visible(%d), pos(1, 19)", klaunch
  chnset Smsg, "$name.Plant"
endif
#

$LAUNCH_PLANT(clfilt)
$LAUNCH_PLANT(phaser2)
$LAUNCH_PLANT(resony)

klaunch    =    1

endin
    
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
i "UpdateWidgets" 0 [3600*24*7]
i 1000 0 [60*60*24*7]    ; plant pop-ups
</CsScore>

</CsoundSynthesizer>
