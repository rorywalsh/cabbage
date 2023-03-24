
/* Attribution-NonCommercial-ShareAlike 4.0 International
Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode */

; EnvelopeFollower.csd
; Written by Iain McCurdy, 2012.

<Cabbage>
form caption("Envelope Follower") size(530, 100), pluginId("envf"), guiRefresh(16)
image                  pos(0, 0), size(530, 100), colour("brown"), shape("rounded"), outlineColour("white"), outlineThickness(4)
vmeter   bounds(20, 10, 15, 80) channel("Meter") value(0) outlineColour("black"), overlayColour(20, 3, 3,255) meterColour:0(255,100,100,255) meterColour:1(255,150,155, 255) meterColour:2(255,255,123, 255) outlineThickness(3) 

rslider bounds( 40, 11, 75, 75), text("Sensitivity"), channel("sens"),  range(0, 1, 0.65),                   colour(255,100,100), textColour(255,255,200), trackerColour(255,255,150)
rslider bounds(110,  6, 45, 45), text("Att."),        channel("att"),   range(0.001, 0.5, 0.01, 0.5, 0.001), colour(255,200,100), textColour(255,255,200), trackerColour(255,255,150)
rslider bounds(110, 51, 45, 45), text("Dec."),        channel("rel"),   range(0.001, 0.5, 0.2, 0.5, 0.001),  colour(255,200,100), textColour(255,255,200), trackerColour(255,255,150)
rslider bounds(150, 11, 75, 75), text("Frequency"),   channel("freq"),  range(10, 10000, 1000, 0.5),         colour(255,100,100), textColour(255,255,200), trackerColour(255,255,150)
label    bounds(225, 15, 85, 14), text("Type"), fontColour(255,255,200)
combobox bounds(225, 30, 85, 20), text("lpf18","moogladder","butlp","tone"), value("1"), channel("type")
rslider bounds(310, 11, 75, 75), text("Resonance"),   channel("res"),   range(0,  1, 0.75),                  colour(255,100,100), textColour(255,255,200), trackerColour(255,255,150), identChannel("resID")
rslider bounds(380, 11, 75, 75), text("Distortion"),  channel("dist"),  range(0,  1.00, 0),                  colour(255,100,100), textColour(255,255,200), trackerColour(255,255,150), identChannel("distID")
rslider bounds(450, 11, 75, 75), text("Level"),       channel("level"), range(0, 1.00, 1),                   colour(255,200,100), textColour(255,255,200), trackerColour(255,255,150)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>

; sr is set by host
ksmps  =  64
nchnls =  2
0dbfs  =  1

; Author: Iain McCurdy (2012)

opcode  EnvelopeFollower,a,akkkkkkk
    ain, ksens, katt, krel, kfreq, ktype, kres, kdist  xin            ; READ IN INPUT ARGUMENTS
    setksmps    4
    ;                    ATTCK.REL.  -  ADJUST THE RESPONSE OF THE ENVELOPE FOLLOWER HERE
    aFollow     follow2           ain, katt, krel                     ; AMPLITUDE FOLLOWING AUDIO SIGNAL
    kFollow     downsamp          aFollow                             ; DOWNSAMPLE TO K-RATE
    kFollow     expcurve          kFollow/0dbfs, 0.5                  ; ADJUSTMENT OF THE RESPONSE OF DYNAMICS TO FILTER FREQUENCY MODULATION
    kFrq        =                 kfreq + (kFollow * ksens * 10000)   ; CREATE A LEFT CHANNEL MODULATING FREQUENCY BASE ON THE STATIC VALUE CREATED BY kfreq AND THE AMOUNT OF DYNAMIC ENVELOPE FOLLOWING GOVERNED BY ksens
    kFrq        limit             kFrq, 20,sr/2                       ; LIMIT FREQUENCY RANGE TO PREVENT OUT OF RANGE FREQUENCIES  
    if ktype==1 then
     aout       lpf18             ain, kFrq, kres, kdist              ; REDEFINE AUDIO SIGNAL AS FILTERED VERSION OF ITSELF
    elseif ktype==2 then
     aout       moogladder        ain, kFrq, kres                     ; REDEFINE AUDIO SIGNAL AS FILTERED VERSION OF ITSELF
    elseif ktype==3 then
     aFrq   interp  kFrq
     aout       butlp             ain, aFrq                           ; REDEFINE AUDIO SIGNAL AS FILTERED VERSION OF ITSELF
    elseif ktype==4 then
     aout       tone              ain, kFrq                           ; REDEFINE AUDIO SIGNAL AS FILTERED VERSION OF ITSELF
    endif
                xout              aout                                ; SEND AUDIO BACK TO CALLER INSTRUMENT
endop

opcode  SwitchPort, k, kii
    kin,iupport,idnport xin
    kold        init              0
    kporttime   =                 (kin < kold ? idnport : iupport)
    kout        portk             kin, kporttime
    kold        =                 kout
                xout              kout
endop

instr 1
ksens           chnget            "sens"
katt            chnget            "att"
krel            chnget            "rel"
kfreq           chnget            "freq"
ktype           chnget            "type"
ktype           init              1
kres            chnget            "res"
kdist           chnget            "dist"
klevel          chnget            "level"
a1,a2           ins


if changed:k(ktype)==1 then
 if ktype==1 then
                chnset            "visible(1)", "distID"
                chnset            "visible(1)", "resID"
 elseif ktype==2 then
                chnset            "visible(0)", "distID"
                chnset            "visible(1)", "resID"
 else
                chnset            "visible(0)", "distID"
                chnset            "visible(0)", "resID"
 endif
endif

/*level meter*/
amix            sum               a1, a2
krms            rms               amix * 0.5
krms            pow               krms, 0.75
krms            SwitchPort        krms, 0.01, 0.05
                chnset            krms, "Meter"

a1              EnvelopeFollower  a1, ksens, katt, krel, kfreq, ktype, kres * 0.95, kdist * 100
a2              EnvelopeFollower  a2, ksens, katt, krel, kfreq, ktype, kres * 0.95, kdist * 100
a1              =                 a1 * klevel * (1 - ((kdist * 0.3) ^ 0.02))                     ; scale amplitude according to distortion level (to compensate for gain increases it applies)
a2              =                 a2 * klevel * (1 - ((kdist * 0.3) ^ 0.02))
                outs              a1, a2
endin

</CsInstruments>

<CsScore>
i 1 0 [60 * 60 * 24 * 7]
</CsScore>

</CsoundSynthesizer>