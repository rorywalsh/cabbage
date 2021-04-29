y <Cabbage>
form caption("Filmstrips") size(1050, 400), pluginId("def1")
groupbox bounds(6, 4, 207, 375) text("Rotary Sliders")

rslider bounds(18, 30, 80, 80), channel("gain1"), range(-60, 100, 15, .5, 0.01), text("Gain"), filmstrip("rSliderFilmStrip.png", 111, "vertical"), valueTextBox(1) 
rslider bounds(118, 30, 80, 80), channel("gain2"), range(0, 10, 0, 1, 0.01), text("Gain"), valueTextBox(1)
rslider bounds(18, 120, 80, 80), channel("gain3"), range(0, 1, 0.48, 1, 0.01), text("Gain"), filmstrip("rSliderFilmStrip.png", 111, "vertical")
rslider bounds(118, 120, 80, 80), channel("gain4"), range(0, 1, 0.48, 1, 0.01), text("Gain") 
rslider bounds(18, 210, 80, 80), channel("gain5"), range(0, 1, 0.48, 1, 0.01), filmstrip("rSliderFilmStrip.png", 111, "vertical")
rslider bounds(118, 210, 80, 80), channel("gain6"), range(0, 1, 0.48, 1, 0.01) 
rslider bounds(18, 290, 80, 80), channel("gain7"), range(0, 1, 0.48, 1, 0.01) filmstrip("rSliderFilmStrip.png", 111, "vertical"), valueTextBox(1) 
rslider bounds(118, 290, 80, 80), channel("gain8"), range(0, 1, 0.48, 1, 0.01) valueTextBox(1)

groupbox bounds(216, 4, 500, 295) colour(88, 92, 95), text("Vertical Sliders")
vslider bounds(228, 46, 60, 200) range(0, 10, 0, 1, 0.001), channel("vSlider1") text("Gain"), trackerColour(100, 100, 100, 255) valueTextBox(1)
vslider bounds(288, 46, 60, 200) range(0, 10, 0, 1, 0.001), channel("vSlider2"), text("Gain"), valueTextBox(1), filmstrip("roland SH 101 knob.png", 31, .15, .3)
vslider bounds(348, 46, 60, 200) range(0, 1, 0.364, 1, 0.001), channel("vSlider3"), text("Gain"), trackerColour(100,100,100)
vslider bounds(408, 46, 60, 200) range(0, 10, 0, 1, 0.001), channel("vSlider4"), text("Gain"), filmstrip("roland SH 101 knob.png", 31, .05, .20)
vslider bounds(468, 46, 60, 200) range(0, 1, 0.364, 1, 0.001), channel("vSlider5"), trackerColour(100,100,100)
vslider bounds(528, 46, 60, 200) range(0, 1, 0, 1, 0.001) , channel("vSlider6") filmstrip("roland SH 101 knob.png", 31,  .05, .15)
vslider bounds(588, 46, 60, 200) range(0, 1, 0.364, 1, 0.001), channel("vSlider7"), valueTextBox(1), trackerColour(100,100,100)
vslider bounds(648, 46, 60, 200) range(0, 1, 0, 1, 0.001) , channel("vSlider8"), valueTextBox(1), filmstrip("roland SH 101 knob.png", 31, .05, .25)

groupbox bounds(720, 4, 320, 380) colour(100, 10, 10), text("Horizontal Sliders")
hslider bounds(732, 20, 300, 60), channel("gain10"), range(0, 1, 0.364, 1, 0.001), text("Gain"), valueTextBox(1) 
hslider bounds(732, 60, 300, 60), channel("gain11"), range(0, 1, 0, 1, 0.001), text("Gain"), valueTextBox(1), filmstrip("h_slider_mixer.png", 31, 0.1, .35)
hslider bounds(732, 100, 300, 60), channel("gain12"), range(0, 1, 0.364, 1, 0.001), text("Gain")
hslider bounds(732, 140, 300, 60), channel("gain13"), range(0, 1, 0, 1, 0.001), text("Gain"), filmstrip("h_slider_mixer.png", 31, .1, .2)
hslider bounds(732, 200, 300, 60), channel("gain14"), range(0, 1, 0.364, 1, 0.001)
hslider bounds(732, 240, 300, 60), channel("gain15"), range(0, 1, 0, 1, 0.001), filmstrip("h_slider_mixer.png", 31, .1, .22)
hslider bounds(732, 280, 300, 60), channel("gain16"), range(0, 1, 0.364, 1, 0.001), valueTextBox(1) 
hslider bounds(732, 320, 300, 60), channel("gain17"), range(0, 1, 0, 1, 0.001), valueTextBox(1), filmstrip("h_slider_mixer.png", 31, .1, .35)
;
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
