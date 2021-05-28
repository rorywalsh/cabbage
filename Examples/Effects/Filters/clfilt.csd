
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; clfilt.csd
; Written by Iain McCurdy, 2012.

; Port.     -   portamento applied to changes that are made to 'Freq.'

<Cabbage>
form caption("clfilt - Multi-Mode Filter") size(655, 90), pluginId("clfl"), scrollBars(0)
image                              bounds(0, 0, 655, 90), colour( 50,50,65), shape("rounded"), outlineColour("white"), outlineThickness(4) 
rslider bounds( 10, 11, 70, 70), text("Freq."),       colour( 20,20, 80),   fontColour("white"),    channel("cf"),      range(20, 20000, 2000, 0.333), trackerColour(200,200,200), outlineColour(140,140,170), textColour("silver"), markerColour("silver")
rslider bounds( 75, 16, 60, 60), text("Port."),       colour( 20,20, 80),   fontColour("white"),    channel("port"),    range(0,  50, 0.1, 0.5,0.01), trackerColour(200,200,200), outlineColour(140,140,170), textColour("silver"), markerColour("silver")

rslider bounds(130, 11, 70, 70), text("N.Poles"),     colour( 20,20, 80),   fontColour("white"),    channel("npol"),    range(2,80,24,1,2),            trackerColour(200,200,200), outlineColour(140,140,170), textColour("silver"), markerColour("silver")
rslider bounds(195, 11, 70, 70), text("Ripple"),      colour( 20,20, 80),   fontColour("white"),    channel("pbr"),     range(0.1,50,14),              trackerColour(200,200,200), outlineColour(140,140,170), textColour("silver"), markerColour("silver")
rslider bounds(260, 11, 70, 70), text("Attenuation"), colour( 20,20, 80),   fontColour("white"),    channel("sba"),     range(-120,-1,-60),            trackerColour(200,200,200), outlineColour(140,140,170), textColour("silver"), markerColour("silver")
label                      bounds(365, 10, 40, 12), text("Type:"), fontColour("silver")
combobox channel("type"),  bounds(328, 23, 128, 18), value(1), text("Lowpass","Highpass"), fontColour(255,255,200)
label                      bounds(363, 46, 50, 12), text("Method:"), fontColour("silver")
combobox channel("kind"),  bounds(328, 59, 128, 18), value(2), text("Butterworth","Chebychev type I","Chebychev type II"), fontColour(255,255,200)
rslider bounds(455, 11, 70, 70), text("Mix"),         colour( 20,20, 80),   fontColour("white"),    channel("mix"),     range(0,1.00,1),   trackerColour(200,200,200), outlineColour(140,140,170), textColour("silver"), markerColour("silver")
rslider bounds(520, 11, 70, 70), text("Level"),       colour( 20,20, 80),   fontColour("white"),    channel("level"),   range(0,1.00,0.3), trackerColour(200,200,200), outlineColour(140,140,170), textColour("silver"), markerColour("silver")
checkbox channel("test"), pos(595, 35), text("TEST"), colour("yellow"), fontColour:0("silver"), fontColour:1("silver"), size(60, 15), value(0)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

;SR IS SET BY HOST
ksmps   =   32      ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls  =   2       ; NUMBER OF CHANNELS (2=STEREO)
0dbfs   =   1

;Author: Iain McCurdy (2013)

instr   1   ; widgets input
    gkcf        chnget   "cf"
    gkport      chnget   "port"
    gknpol      chnget   "npol"
    gknpol      init     2
    gkpbr       chnget   "pbr"
    gksba       chnget   "sba"
    gktype      chnget   "type"
    gktype      =        gktype - 1
    gktype      init     0
    gkkind      chnget   "kind"
    gkkind      =        gkkind - 1
    gkkind      init     0
    gkmix       chnget   "mix"
    gklevel     chnget   "level"
    gktest      chnget   "test"
endin

instr   2   ; clfilt - multimode filter
    kporttime   linseg   0, 0.001, 1
    kcf         portk    gkcf, kporttime * gkport
    kmix        portk    gkmix, kporttime * 0.1
    klevel      portk    gklevel, kporttime
    if gktest=0 then
     aL,aR      ins
    else
     aL         pinkish  0.5
     aR         =        aL
    endif
    
    ktrig       changed  gktype, gknpol, gkkind, gkpbr, gksba
    if ktrig==1 then
     reinit UPDATE
    endif
    UPDATE:
    aFiltL      clfilt   aL, kcf, i(gktype), i(gknpol), i(gkkind), i(gkpbr), i(gksba)
    aFiltR      clfilt   aR, kcf, i(gktype), i(gknpol), i(gkkind), i(gkpbr), i(gksba)
    rireturn

    aL          ntrpol   aL, aFiltL, kmix
    aR          ntrpol   aR, aFiltR, kmix
                outs     aL * klevel, aR * klevel
endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
i 2 0.01 [3600*24*7]
</CsScore>

</CsoundSynthesizer>