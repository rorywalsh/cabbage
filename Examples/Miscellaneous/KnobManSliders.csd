<Cabbage>
form caption("Filmstrips") size(700, 700), pluginid("def1")
groupbox bounds(6, 4, 207, 295) filmstrip("", 0, "vertical"), text("Rotary Sliders")
rslider bounds(18, 30, 80, 80), channel("gain1"), range(0, 1, 0.48, 1, 0.01), text("Gain"), filmstrip("rSliderFilmStrip.png", 111, "vertical"), valuetextbox(1) 
rslider bounds(118, 30, 80, 80), channel("gain1"), range(0, 1, 0.48, 1, 0.01), text("Gain"), valuetextbox(1)
rslider bounds(18, 120, 80, 80), channel("gain1"), range(0, 1, 0.48, 1, 0.01), text("Gain"), filmstrip("rSliderFilmStrip.png", 111, "vertical")
rslider bounds(118, 120, 80, 80), channel("gain1"), range(0, 1, 0.48, 1, 0.01), text("Gain") 
rslider bounds(18, 210, 80, 80), channel("gain1"), range(0, 1, 0.48, 1, 0.01), filmstrip("rSliderFilmStrip.png", 111, "vertical")
rslider bounds(118, 210, 80, 80), channel("gain1"), range(0, 1, 0.48, 1, 0.01) 


groupbox bounds(218, 4, 390, 295) colour(88, 92, 95), text("Vertical Sliders")
vslider bounds(230, 46, 60, 200) range(0, 1, 0.364, 1, 0.001), channel("vSlider1") text("Gain"), trackercolour(100, 100, 100, 255) valuetextbox(1)
vslider bounds(290, 46, 60, 200) range(0, 1, 0.364, 1, 0.001), channel("vSlider2"), text("Gain"), valuetextbox(1), filmstrip("roland SH 101 knob.png", 31, "vertical")
vslider bounds(350, 46, 60, 200) range(0, 1, 0.364, 1, 0.001), channel("vSlider3"), text("Gain"), trackercolour(100,100,100)
vslider bounds(410, 46, 60, 200) range(0, 1, 0.364, 1, 0.001), channel("vSlider4"), text("Gain"), filmstrip("roland SH 101 knob.png", 31, "vertical")
vslider bounds(470, 46, 60, 200) range(0, 1, 0.364, 1, 0.001), channel("vSlider5"), trackercolour(100,100,100)
vslider bounds(530, 46, 60, 200) range(0, 1, 0, 1, 0.001) , channel("vSlider6") filmstrip("roland SH 101 knob.png", 31, "vertical")

groupbox bounds(6, 310, 400, 350) colour(10, 10, 10), text("Horizontal Sliders")
hslider bounds(18, 320, 300, 60), channel("gain10"), range(0, 1, 0.364, 1, 0.001), text("Gain"), valuetextbox(1) 
hslider bounds(18, 360, 300, 60), channel("gain11"), range(0, 1, 0.364, 1, 0.001), text("Gain"), valuetextbox(1), filmstrip("h_slider_mixer.png", 31, "vertical")
hslider bounds(18, 400, 300, 60), channel("gain12"), range(0, 1, 0.364, 1, 0.001), text("Gain")
hslider bounds(18, 440, 300, 60), channel("gain13"), range(0, 1, 0.364, 1, 0.001), text("Gain"), filmstrip("h_slider_mixer.png", 31, "vertical")
hslider bounds(18, 500, 300, 60), channel("gain14"), range(0, 1, 0.364, 1, 0.001)
hslider bounds(18, 540, 300, 60), channel("gain15"), range(0, 1, 0, 1, 0.001), filmstrip("h_slider_mixer.png", 31, "vertical")
;</Cabbage>
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
