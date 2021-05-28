
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; MultiReverb.csd
; Written by Iain McCurdy, 2012

; Reverb effect that employs screverb and freeverb opcodes.
; Pitch Mod. is only used by reverbsc.

<Cabbage>
form caption("Multi Reverb") size(415, 90), pluginId("Rvrb")
image  bounds(  7, 22, 73, 45), colour( 39, 31, 20), shape("rounded"), outlineColour("white"), outlineThickness(1) 
image  bounds(  0,  0,415, 90), colour( 39, 31, 20), shape("rounded"), outlineColour("white"), outlineThickness(4) 
button bounds( 11, 21, 66, 20), text("screverb","screverb"), channel("scType"),   value(1), fontColour:0(50,50,50), fontColour:1("lime"), radioGroup(1)
button bounds( 11, 44, 66, 20), text("freeverb","freeverb"), channel("freeType"), value(0), fontColour:0(50,50,50), fontColour:1("lime"), radioGroup(1)
rslider bounds( 80, 11, 70, 70), text("Size"),         textColour("white"),         channel("fblvl"),     range(0, 1.00, 0.8), colour( 79, 81, 70), trackerColour(240,210,170)
rslider bounds(145, 11, 70, 70), text("Damping"),     textColour("white"),         channel("fco"),     range(0, 1.00, 0.6), colour( 79, 81, 70), trackerColour(240,210,170)
rslider bounds(210, 11, 70, 70), text("Pitch Mod."),     textColour("white"),         channel("pitchm"),     range(0, 20.0, 1),   colour( 79, 81, 70), trackerColour(240,210,170), identChannel("pitchmID")
rslider bounds(275, 11, 70, 70), text("Mix"),         textColour("white"),         channel("mix"),     range(0, 1.00, 1),   colour( 79, 81, 70), trackerColour(240,210,170)
rslider bounds(340, 11, 70, 70), text("Level"),         textColour("white"),         channel("amp"),     range(0, 1.00, 1),   colour( 79, 81, 70), trackerColour(240,210,170)
</Cabbage>

<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>

; sr is set by host
ksmps     =     32    ;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls     =     2    ;NUMBER OF CHANNELS (2=STEREO)
0dbfs    =    1

; Author: Iain McCurdy (2012)

instr    1
    kscType        chnget    "scType"
    kfreeType    chnget    "freeType"
    ktype        =    (kscType==1?0:1)
    kfblvl        chnget    "fblvl"
    kfco        chnget    "fco"
    kpitchm        chnget    "pitchm"
    kmix        chnget    "mix"
    kamp        chnget    "amp"

    
    if changed(ktype)==1||timeinstk()==1 then
     Smsg    sprintfk    "visible(%d)",1-ktype
     chnset    Smsg,"pitchmID"
    endif

    ainL,ainR    ins
    denorm        ainL, ainR    ;DENORMALIZE BOTH CHANNELS OF AUDIO SIGNAL
    if ktype==0 then
     kfco    expcurve    kfco, 4    ;CREATE A MAPPING CURVE TO GIVE A NON LINEAR RESPONSE
     kfco    scale    kfco,20000,20    ;RESCALE 0 - 1 TO 20 TO 20000
     kSwitch        changed        kpitchm    ;GENERATE A MOMENTARY '1' PULSE IN OUTPUT 'kSwitch' IF ANY OF THE SCANNED INPUT VARIABLES CHANGE. (OUTPUT 'kSwitch' IS NORMALLY ZERO)
     if    kSwitch=1    then    ;IF kSwitch=1 THEN
        reinit    UPDATE        ;BEGIN A REINITIALIZATION PASS FROM LABEL 'UPDATE'
     endif                ;END OF CONDITIONAL BRANCHING
     UPDATE:                ;A LABEL
     arvbL, arvbR     reverbsc     ainL, ainR, kfblvl, kfco, sr, i(kpitchm)
     rireturn            ;RETURN TO PERFORMANCE TIME PASSES
    else
     arvbL, arvbR     freeverb     ainL, ainR, kfblvl, 1-kfco
    endif
    amixL        ntrpol        ainL, arvbL, kmix    ;CREATE A DRY/WET MIX BETWEEN THE DRY AND THE REVERBERATED SIGNAL
    amixR        ntrpol        ainR, arvbR, kmix    ;CREATE A DRY/WET MIX BETWEEN THE DRY AND THE REVERBERATED SIGNAL
            outs        amixL * kamp, amixR * kamp
endin
        
</CsInstruments>

<CsScore>
i 1 0 [3600*24*7]
</CsScore>

</CsoundSynthesizer>
