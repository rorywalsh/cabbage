<Cabbage>
form caption("Gentable Example") size(400, 650), guiMode("queue"), colour(2, 145, 209), pluginId("def1")
gentable bounds(13, 7, 370, 200), channel("gentable1"), outlineThickness(3), tableNumber(1.0), tableGridColour(155, 155, 155, 255) tableBackgroundColour(0,0,0,0) tableColour:0(147, 210, 0, 255)
hslider bounds(14, 212, 368, 40), channel("harm1"), range(0, 1, 1, 1, 0.01), textColour(255, 255, 255, 255), text("Harm1")
hslider bounds(14, 244, 368, 40), channel("harm2"), range(0, 1, 0, 1, 0.01), textColour(255, 255, 255, 255), text("Harm2")
hslider bounds(14, 276, 368, 40), channel("harm3"), range(0, 1, 0, 1, 0.01), textColour(255, 255, 255, 255), text("Harm3")
hslider bounds(14, 308, 368, 40), channel("harm4"), range(0, 1, 0, 1, 0.01), textColour(255, 255, 255, 255), text("Harm4")
hslider bounds(14, 340, 368, 40), channel("harm5"), range(0, 1, 0, 1, 0.01), textColour(255, 255, 255, 255), text("Harm5")
checkbox bounds(16, 380, 120, 20), channel("normal"), text("Normalise"), value(1), fontColour:1(255, 255, 255, 255)
checkbox bounds(140, 380, 120, 20), channel("fill"), text("Fill Table"), value(1), fontColour:1(255, 255, 255, 255)
texteditor bounds(16, 410, 367, 219) channel("infoText"), wrap(1), text("A 'GEN' table widget will display the contents of a sound function table. In this example, a basic sine wave is stored in function table 1, which is defined in the CsScore section. Whenever a slider is changed, instr 1 will trigger the 'UpdateTable' instrument, which in turns creates a new function shape accordingly. It writes the shape to the same table number as defined when declaring the gentable widget.", "Although a gentable can be passed new function tables at run-time, it might incur a slight performance hit especially if the new function table is a larger. It is a better idea is to just copy the contents of one table to another using the table copy opcodes in Csound.")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n -m0d
</CsOptions>
<CsInstruments>
;sr is set by the host
ksmps 		= 	32
nchnls 		= 	2
0dbfs		=	1


; Rory Walsh 2021 
;
; License: CC0 1.0 Universal
; You can copy, modify, and distribute this file, 
; even for commercial purposes, all without asking permission. 


instr	1

    SText  = "A 'GEN' table widget will display the contents of a sound function table. In this example, a basic sine wave is stored in function table 1, which is defined in the CsScore section. Whenever a slider is changed, instr 1 will trigger the 'UpdateTable' instrument, which in turns creates a new function shape accordingly. It writes the shape to the same table number as defined when declaring the gentable widget.\n\nAlthough a gentable can be passed new function tables at run-time, it might incur a slight performance hit especially if the new function table is a larger. It is a better idea is to just copy the contents of one table to another using the table copy opcodes in Csound."
    cabbageSet "infoText", "text", SText
    
    ;toggle fill
    kFill, kTrig cabbageGetValue "fill"
    cabbageSet kTrig, "gentable1", "fill", kFill 

    k1 chnget "harm1"
    k2 chnget "harm2"
    k3 chnget "harm3"
    k4 chnget "harm4"
    k5 chnget "harm5"

    aEnv linen 1, 1, p3, 1
    a1 oscili .2, 200, 1
    outs a1, a1

    kChanged changed k1, k2, k3, k4, k5
    if kChanged==1 then
        ;if a slider changes trigger instrument 2 to update table
        event "i", "UpdateTable", 0, .01, k1, k2, k3, k4, k5
    endif

endin

instr UpdateTable
    iNormal = (chnget:i("normal")==0 ? -1 : 1)
    iTable	ftgen	1, 0,   1024, 10*iNormal, p4, p5, p6, p7, p8
    cabbageSet	"gentable1", "tableNumber", 1	; update table display
endin

</CsInstruments>
<CsScore>
f1 0 1024 10 1
i1 0 [3600*24*7]
</CsScore>
</CsoundSynthesizer>
