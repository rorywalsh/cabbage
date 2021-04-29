y <Cabbage>
form caption("Filmstrips") size(380, 140), pluginId("def1")
groupbox bounds(6, 4, 365, 120) text("Filmstrip Sliders")

rslider bounds(18, 30, 80, 80), channel("gain1"), range(-60, 100, 15, .5, 0.01), text("Gain"), filmstrip("rSliderFilmStrip.png", 111, "vertical"), valueTextBox(1) 

rslider bounds(104, 30, 80, 80), channel("gain3"), range(0, 1, 0.48, 1, 0.01), text("Gain"), filmstrip("rSliderFilmStrip.png", 111)

rslider bounds(190, 30, 80, 80), channel("gain5"), range(0, 1, 0.48, 1, 0.01), filmstrip("rSliderFilmStrip.png", 111)

rslider bounds(276, 30, 80, 80), channel("gain7"), range(0, 1, 0.48, 1, 0.01) filmstrip("rSliderFilmStrip.png", 111), valueTextBox(1) 


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

;the following film strip sliders are included with teh Cabbage examples. 
;https://www.g200kg.com/en/webknobman/gallery.php?m=p&p=1659
;https://www.g200kg.com/en/webknobman/gallery.php?m=p&p=1044
;https://www.g200kg.com/en/webknobman/index.html?f=slider_knobman2.knob&n=460
;https://www.g200kg.com/en/webknobman/gallery.php?m=p&p=534
;https://www.g200kg.com/en/webknobman/gallery.php?m=p&p=987
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
