<Cabbage>
form caption("Filmstrips") size(400, 300), colour(0, 0, 0), pluginid("def1")
rslider bounds(10, 12, 101, 97), channel("gain1"), range(0, 1, 0, .5, 0.01), text("Gain"), filmstrip("knobMan.png", 111)
rslider bounds(238, 22, 51, 52), channel("gain2"), range(0, 1, 0, 1, 0.01), text("Gain"), filmstrip("knobMan3.png", 31, "vertical")
rslider bounds(142, 22, 51, 52), channel("gain3"), range(0, 1, 0, 1, 0.01), text("Gain"), filmstrip("knobMan2.png", 64, "horizontal")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

;https://www.g200kg.com/en/webknobman/gallery.php?m=p&p=1659
;https://www.g200kg.com/en/webknobman/index.html?f=while.knob&n=1625
;https://www.g200kg.com/en/webknobman/index.html?f=Big_Knob_02.knob&n=1633

instr 1
kGain chnget "gain"

a1 inch 1
a2 inch 2

outs a1*kGain, a2*kGain
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week

</CsScore>
</CsoundSynthesizer>
