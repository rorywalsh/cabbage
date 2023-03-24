
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

<Cabbage>
form             size(525,100), caption("clip"), pluginId("Clip"), guiMode("queue")
image    bounds(0, 0, 525,100), colour( 20, 30, 40), shape("rounded"), outlineColour("white"), outlineThickness(4) 
rslider  bounds( 10, 15, 70, 70), text("Input Gain"), channel("InGain"), range(1, 10, 1, 0.5,0.01), colour(0,10,20), fontColour("silver"), trackerColour("lightblue")
rslider  bounds( 75, 15, 70, 70), text("Limit"), channel("limit"), range(0.001, 1, 1, 0.5,0.001), colour(0,10,20), fontColour("silver"), trackerColour("lightblue")
rslider  bounds(140, 15, 70, 70), text("Argument"), channel("arg"), range(0, 1.00, 0.5), colour(0,10,20), fontColour("silver"), trackerColour("lightblue")
checkbox bounds(128,  7,  8,  8), channel("clip_light"), text("clip"), colour("red"), fontColour("white"), shape("ellipse"), value(0), active(0)
label    bounds(137,  6, 25, 10), text("CLIP")
combobox bounds(210, 15, 80, 20), channel("method"), value(1), text("B.D.J.","Sine","Tanh")
checkbox bounds(210, 50, 15, 15), channel("GainComp"), text("Gain Compensation"), colour("lime"), fontColour("white")
label    bounds(225, 52,100, 10), text("GAIN COMPENSATION")
rslider  bounds(325, 15, 70, 70), text("Level"), channel("level"), range(0.00, 10.00, 0.7), colour(0,10,20), fontColour("silver"), trackerColour("lightblue")
gentable bounds(405, 10,100, 70), tableNumber(1000), channel("TF"), ampRange(-1.1,1.1,1000), tableColour(150,160,220)
label    bounds(405, 80,100, 10), text("Transfer Function")
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

;sr is set by the host
ksmps   =   32      ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls  =   2       ; NUMBER OF CHANNELS (2=STEREO)
0dbfs   =   1

i_  ftgen  1000,0,256,10,1

; Author: Iain McCurdy (2012)

instr   1
    kInGain     cabbageGetValue    "InGain"
    kGainComp   cabbageGetValue    "GainComp"
    klimit      cabbageGetValue    "limit"
    kmethod     cabbageGetValue    "method"
    kmethod     init      1
    karg        cabbageGetValue    "arg"
    klevel      cabbageGetValue    "level"
    asigL       =         inch:a(1) * kInGain
    asigR       =         inch:a(2) * kInGain
          
    ; CLIP METER
    krmsL       =         rms:k(asigL,10) * 3
    krmsR       =         rms:k(asigR,10) * 3
    if ( trigger:k(krmsL,klimit,0)==1 || trigger:k(krmsR,klimit,0)==1 ) then
           cabbageSetValue "clip_light", 1, 1
    elseif ( trigger:k(krmsL,klimit,1)==1 ||trigger:k(krmsR,klimit,1)==1 ) then
           cabbageSetValue "clip_light", 0, 1
    endif

    if  changed(klimit,kmethod,karg)==1 then                          ; IF A VARIABLE CHANGE INDICATOR IS RECEIVED...
        reinit  UPDATE                                                ; ...BEGIN A REINITIALISATION PASS FROM LABEL 'START' 
    endif                                                             ; END OF CONDITIONAL BRANCHING
    UPDATE:                                                           ; LABEL
    asigL       clip        asigL, i(kmethod)-1, i(klimit), i(karg)
    asigR       clip        asigR, i(kmethod)-1, i(klimit), i(karg)

    ; display transfer function
    kPtr   init      0
    while kPtr<ftlen(1000) do
    kVal   init      -1
    aVal   clip      a(kVal), i(kmethod)-1, i(klimit), i(karg)
           tablew    k(aVal), kPtr, 1000
    kPtr   +=        1
    kVal   +=        2/ftlen(1000)
    od
           cabbageSet "TF", "tableNumber", 1000           
    rireturn                                                          ; RETURN TO PERFORMANCE PASSES FROM INITIALISATION PASS
    if (kGainComp==1) then
     asigL      =           asigL * (1/sqrt(klimit))
     asigR      =           asigR * (1/sqrt(klimit))
    endif
                outs        asigL * klevel, asigR * klevel
endin

</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>