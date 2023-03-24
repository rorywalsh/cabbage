
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; pdclip.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("pdclip") size(490,100), pluginId("pdcl")
label        bounds(  5,-12,380,100), colour(20,100,100, 0), fontColour(70,150,150,200), text("pdclip"), shape("rounded"), outlineColour("white"), outlineThickness(4) 
image        bounds(  0,  0,490,100), colour(10,100,200,200), shape("rounded"), outlineColour("white"), outlineThickness(4) 
rslider      bounds( 10, 11, 70, 70), text("Input Gain"),  channel("InGain"),  range(0, 5.00, 1,0.375),     textColour("white"), colour(5, 30,80), trackerColour(255,255,25), outlineColour(30,30,50)
rslider      bounds( 75, 11, 70, 70), text("Width"),  channel("width"),  range(0, 1.00, 0),     textColour("white"), colour(5, 30,80), trackerColour(255,255,25), outlineColour(30,30,50)
rslider      bounds(140, 11, 70, 70), text("Centre"), channel("center"), range(-1.00, 1.00, 0), textColour("white"), colour(5, 30,80), trackerColour(255,255,25), outlineColour(30,30,50)
rslider      bounds(300, 11, 70, 70), text("Level"),  channel("level"),  range(0, 1.00, 0.7),   textColour("white"), colour(5, 30,80), trackerColour(255,255,25), outlineColour(30,30,50)
combobox     bounds(215, 20, 80, 20), channel("bipolar"), value(2), text("Unipolar", "Bipolar")
checkbox     bounds(215, 48,130, 12), channel("TestTone"), fontColour:0("white"), fontColour:1("white"),  value(0), text("Sine Tone"), colour(yellow)
gentable bounds(375, 10,100, 70), tableNumber(1000), identChannel("TF"), ampRange(-1.1,1.1,1000), tableColour(160,160,220)
label    bounds(375, 80,100, 10), text("Transfer Function"), fontColour(255,255,255)
}
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

;Author: Iain McCurdy (2012)

gisine     ftgen   0, 0, 4096, 10, 1
i_         ftgen   1000, 0, 256, 10, 1

instr   1
    kporttime      linseg    0, 0.001, 0.05         ; portamento time ramps up from zero
    gkInGain       chnget    "InGain"               ; READ WIDGETS...
    gkInGain       portk     gkInGain,kporttime
    gkwidth        chnget    "width"
    gkwidth        portk     gkwidth,kporttime
    gkcenter       chnget    "center"
    gkcenter       portk     gkcenter,kporttime
    gkbipolar      chnget    "bipolar"
    gkbipolar      init      1
    gkbipolar      =   gkbipolar-1
    gklevel        chnget    "level"
    gklevel        portk     gklevel,kporttime
    gkTestTone     chnget    "TestTone"
    if gkTestTone==1 then                            ; if test tone selected...
     koct          rspline   4, 8, 0.2, 0.5                     
     asigL         poscil    1, cpsoct(koct), gisine ; ...generate a tone
     asigR         =   asigL                         ; right channel equal to left channel
    else                                             ; otherwise...
     asigL, asigR  ins                               ; read live inputs
    endif

    ifullscale      =         0dbfs                  ; DEFINE FULLSCALE AMPLITUDE VALUE
    kSwitch         changed   gkbipolar              ; GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
    if  kSwitch=1   then                             ; IF A VARIABLE CHANGE INDICATOR IS RECEIVED...
        reinit  START                                ; ...BEGIN A REINITIALISATION PASS FROM LABEL 'START' 
    endif                                            ; END OF CONDITIONAL BRANCHING
    START:                                           ; LABEL
    ;CLIP THE AUDIO SIGNAL USING pdclip
    aL              pdclip    asigL * gkInGain, gkwidth, gkcenter, i(gkbipolar) ; [, ifullscale]]
    aR              pdclip    asigR * gkInGain, gkwidth, gkcenter, i(gkbipolar) ; [, ifullscale]]
    rireturn                                             ; RETURN TO PERFORMANCE PASSES FROM INITIALISATION PASS
    alevel          interp    gklevel
                    outs      aL * alevel, aR * alevel   ; pdclip OUTPUT ARE SENT TO THE SPEAKERS


    ; display transfer function
    if changed:k(gkwidth,gkcenter,gkbipolar)==1 then
     reinit REBUILD_TF
    endif
    REBUILD_TF:
    kPtr   init      0
    while kPtr<ftlen(1000) do
    kVal   init      -1
    aVal   pdclip    a(kVal), gkwidth, gkcenter, i(gkbipolar) ; [, ifullscale]]
           tablew    k(aVal), kPtr, 1000
    kPtr   +=        1
    kVal   +=        2/ftlen(1000)
    od
           chnset    "tableNumber(1000)", "TF"
    rireturn

endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>