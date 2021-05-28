
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; phaser.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("phaser2") size(685, 90), pluginId("phs2")
image pos(0, 0),        size(685, 90), colour(80,80,105), shape("rounded"), outlineColour("white"), outlineThickness(4) 
label     bounds( 15, 15, 55, 12), text("INPUT"), fontColour("white")
checkbox  bounds( 15, 30, 55, 12), text("Live"),  fontColour:0("white"),  fontColour:1("white"), channel("input"),  value(1), radioGroup(1)
checkbox  bounds( 15, 45, 55, 12), text("Noise"), fontColour:0("white"),  fontColour:1("white"), channel("input2"), value(0), radioGroup(1)
rslider  bounds( 70, 11, 70, 70),  text("Frequency"),  channel("freq"),     range(20.0, 5000, 100, 0.25), colour(100,100,200), trackerColour(silver), textColour("white")
rslider  bounds(140, 16, 60, 60),  text("Port."),  channel("port"),     range(0, 30, 0.1, 0.5,0.01), colour(100,100,200), trackerColour(silver), textColour("white")
rslider  bounds(195, 11, 70, 70),  text("Q"),          channel("q"),        range(0.01,10,1,0.5),            colour(100,100,200), trackerColour(silver), textColour("white")
rslider  bounds(260, 11, 70, 70), text("N.Ords."),    channel("ord"),      range(1, 256, 8, 0.5,1),      colour(100,100,200), trackerColour(silver), textColour("white")
label    bounds(335, 20, 61,12),  text("Sep. Mode:"), fontColour("white")
rslider  bounds(410, 11, 70, 70), text("Separation"), channel("sep"),      range(-3, 3.00, 1),         colour(100,100,200), trackerColour(silver), textColour("white")
rslider  bounds(475, 11, 70, 70), text("Feedback"),   channel("feedback"), range(-0.99, 0.99, 0.9),      colour(100,100,200), trackerColour(silver), textColour("white")
rslider  bounds(540, 11, 70, 70), text("Mix"),        channel("mix"),      range(0, 1.00, 1),            colour(100,100,200), trackerColour(silver), textColour("white")
rslider  bounds(605, 11, 70, 70), text("Level"),      channel("level"),    range(0, 1.00, 0.7),          colour(100,100,200), trackerColour(silver), textColour("white")
combobox bounds(330, 34, 80,25), channel("mode"), value(1), text("Equal", "Power"), fontColour("white")
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

; sr set by host
ksmps         =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls         =     2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs        =    1

;Author: Iain McCurdy (2012)

instr    1
    gkport        chnget    "port"
    kRampUp        linseg    0,0.001,1
    kfreq        chnget    "freq"                    ;READ WIDGETS...
    gkfreq        portk    kfreq,kRampUp*gkport
    kq        chnget    "q"                    ;
    gkq        portk    kq,kRampUp*gkport
    gkmode        chnget    "mode"                    ;
    gkmode        init    1
    gkmode        init    i(gkmode)-1
    ksep        chnget    "sep"                    ;
    gksep        portk    ksep,kRampUp*gkport
    gkfeedback    chnget    "feedback"                ;
    gkord        chnget    "ord"                    ;
    gkmix        chnget    "mix"                    ;
    gklevel        chnget    "level"                    ;
    gkinput        chnget    "input"                    ;
    if gkinput==1 then
     asigL,asigR    ins
    else
     asigL    pinker
     asigR    pinker
    endif
    kporttime    linseg    0,0.01,0.03                ;CREATE A VARIABLE THAT WILL BE USED FOR PORTAMENTO TIME
    kfreq        portk    gkfreq, kporttime            ;PORTAMENTO IS APPLIED TO 'SMOOTH' SLIDER MOVEMENT    
    kq        portk    gkq, kporttime                ;PORTAMENTO IS APPLIED TO 'SMOOTH' SLIDER MOVEMENT
    ksep        portk    gksep, kporttime                ;PORTAMENTO IS APPLIED TO 'SMOOTH' SLIDER MOVEMENT
    kSwitch        changed    gkord,gkmode                ;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
    if    kSwitch=1    then                    ;IF I-RATE VARIABLE CHANGE TRIGGER IS '1'...
        reinit    UPDATE                        ;BEGIN A REINITIALISATION PASS FROM LABEL 'UPDATE'
    endif                                ;END OF CONDITIONAL BRANCH
    UPDATE:                                ;BEGIN A REINITIALISATION PASS FROM HERE
    aphaserl    phaser2        asigL, kfreq, kq, gkord, gkmode, ksep, gkfeedback    ; PHASER2 IS APPLIED TO THE LEFT CHANNEL
    aphaserr    phaser2        asigR, kfreq, kq, gkord, gkmode, ksep, gkfeedback    ; PHASER1 IS APPLIED TO THE RIGHT CHANNEL
    rireturn                            ;RETURN FROM REINITIALISATION PASS TO PERFORMANCE TIME PASSES

    aphaserl    dcblock2    aphaserl            ;PHASER2 CAN TEND TO PRODUCE A DC OFFSET
    aphaserr    dcblock2    aphaserr
    
    amixL        ntrpol    asigL,aphaserl,gkmix
    amixR        ntrpol    asigR,aphaserr,gkmix
            outs    amixL*gklevel, amixR*gklevel        ;PHASER OUTPUTS ARE SENT OUT
endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>


</CsoundSynthesizer>



























