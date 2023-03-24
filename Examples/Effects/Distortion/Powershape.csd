
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; Powershape.csd
; Iain McCurdy 2012
;
; Encapsulation of the powershape opcode used for wave distortion
;
; CONTROLS
; --------
; Test Tone --  a glissandoing test tone
; Amount    --  amount of distortion
; Level     --  output level

<Cabbage>
form caption("Powershape") size(490,100), pluginId("pshp")
image    bounds(  0, 0,490,100),  colour("Brown"), shape("rounded"), outlineColour("white"), outlineThickness(4) 

checkbox bounds( 10, 15,130, 12), channel("TestTone"), fontColour:0("white"), fontColour:1("white"), value(0), text("Sine Test"), colour(yellow)

hslider  bounds( 95, 10,190, 20), channel("amount"), range(0.1, 100, 1, 0.25,0.001), colour(220,160,160), trackerColour(255,210,210)
label    bounds( 95, 30,190, 11), text("Power"), fontColour("white"), align("centre")

nslider  bounds(145, 50, 90, 30), text("Power (type value)"),  channel("amount"),  range(0.1, 1000, 1,1,0.001), textColour(white)

rslider  bounds(290, 15, 70, 70), channel("level"),  text("Level"), range(0, 50, 0.5, 0.25,0.000001), colour(220,160,160), trackerColour(255,210,210), textColour(255,255,255)

gentable bounds(375, 10,100, 70), tableNumber(1000), identChannel("TF"), ampRange(-1.1,1.1,1000), tableColour(220,160,160)
label    bounds(375, 80,100, 10), text("Transfer Function"), fontColour(255,255,255)
}
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

;sr is set by the host
ksmps   =       32      ; NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls  =       2       ; NUMBER OF CHANNELS (2=STEREO)
0dbfs   =       1

gisine  ftgen   0, 0, 4096, 10, 1  ;,0,1/2,0,1/4,0,1/8,0,1/16,0,1/32,0,1/64
i_      ftgen   1000, 0, 256, 10, 1

instr   1
    kporttime      linseg      0, 0.001, 0.05               ; portamento time ramps up from zero
    gkshape        chnget      "amount"                     ; READ WIDGETS...
    gkshape        portk       gkshape, kporttime
    gklevel        chnget      "level"                      ;
    gklevel        portk       gklevel, kporttime
    gklevel        portk       gklevel, kporttime
    gkTestTone     chnget      "TestTone"
    if gkTestTone==1 then                                   ; if test tone selected...
     koct          rspline     4, 8, 0.2, 0.5                     
     asigL         poscil      1, cpsoct(koct), gisine      ; ...generate a tone
     asigR         =           asigL                        ; right channel equal to left channel
    else                                                    ; otherwise...
     asigL, asigR  ins                                      ; read live inputs
    endif
    ifullscale     =           0dbfs   ;DEFINE FULLSCALE AMPLITUDE VALUE
    aL             powershape  asigL, gkshape, ifullscale   ; CREATE POWERSHAPED SIGNAL
    aR             powershape  asigR, gkshape, ifullscale   ; CREATE POWERSHAPED SIGNAL
    alevel         interp      gklevel
                   outs        aL * alevel, aR * alevel
    
    ; display transfer function
    if changed:k(gkshape)==1 then
     reinit REBUILD_TF
    endif
    REBUILD_TF:
    kPtr   init      0
    while kPtr<ftlen(1000) do
    kVal   init      -1
    aVal   powershape      a(kVal), gkshape, ifullscale   ; CREATE POWERSHAPED SIGNAL
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