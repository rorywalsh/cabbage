<Cabbage>
form caption("Widgets") size(590, 560), pluginId("def1") colour(79, 79, 79)
keyboard bounds(4, 450, 286, 95)


groupbox bounds(4, 4, 288, 228), text("Vertical Sliders"), colour(0,0,0,0) { 
vslider bounds(15, 30, 50, 180) range(0, 1, 0, 1, 0.001), text("Sld.1")
vslider bounds(45, 30, 50, 180) range(0, 1, 0, 1, 0.001), text("Sld.2")
vslider bounds(75, 30, 50, 180) range(0, 1, 0, 1, 0.001), text("Sld.3")
vslider bounds(105, 30, 50, 180) range(0, 1, 0, 1, 0.001), text("Sld.4")
vslider bounds(135, 30, 50, 180) range(0, 1, 0, 1, 0.001), text("Sld.5")
vslider bounds(165, 30, 50, 180) range(0, 1, 0, 1, 0.001), text("Sld.6")
vslider bounds(195, 30, 50, 180) range(0, 1, 0, 1, 0.001), text("Sld.7")
vslider bounds(225, 30, 50, 180) range(0, 1, 0, 1, 0.001), text("Sld.8")
}

groupbox bounds(298, 4, 288, 108), text("Rotary Sliders") {
rslider bounds(16, 32, 60, 60) range(0, 1, 0, 1, 0.001), colour(0,0,0,0)
rslider bounds(82, 32, 60, 60) range(0, 1, 0, 1, 0.001), colour(0,0,0,0)
rslider bounds(150, 32, 60, 60) range(0, 1, 0, 1, 0.001), colour(0,0,0,0)
rslider bounds(216, 32, 60, 60) range(0, 1, 0, 1, 0.001), colour(0,0,0,0)
}

groupbox bounds(298, 122, 288, 108), text("Horizontal Sliders"), colour(0,0,0,0) {
hslider bounds(10, 22, 266, 39) range(0, 1, 0, 1, 0.001), text("Slider 1")
hslider bounds(10, 46, 266, 39) range(0, 1, 0, 1, 0.001), text("Slider 2")
hslider bounds(10, 70, 266, 39) range(0, 1, 0, 1, 0.001), text("Slider 3")
}




groupbox bounds(4, 322, 288, 122), text("Controls"){
checkbox bounds(182, 60, 100, 30), text("CheckBox")
checkbox bounds(78, 60, 100, 30), text("CheckBox")
checkbox bounds(78, 28, 100, 30), text("CheckBox")
checkbox bounds(182, 28, 100, 30), text("CheckBox")
combobox bounds(78, 90, 80, 20)
combobox bounds(182, 90, 80, 20)
encoder bounds(10, 44, 60, 60) max(0) min(0), text("Encoder")
}



xypad bounds(298, 238, 284, 307) rangeX(0, 1, 0) rangeY(0, 1, 0), fontColour("green")

groupbox bounds(4, 238, 288, 78), text("Buttons"){
button bounds(16, 23, 84, 26) range(1) colour:1(60, 58, 58, 255) increment(0.001) colour:0(79, 79, 79, 255)
button bounds(104, 23, 84, 26) range(1) colour:1(60, 58, 58, 255) increment(0.001) colour:0(79, 79, 79, 255)
button bounds(190, 23, 84, 26) range(1) colour:1(60, 58, 58, 255) increment(0.001) colour:0(79, 79, 79, 255)
label bounds(16, 53, 84, 16), text("Hello")
label bounds(104, 53, 84, 16), text("Hello")
label bounds(190, 53, 84, 16), text("Hello")
}
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d --midi-key-cps=4 --midi-velocity-amp=5
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
ksmps = 32
nchnls = 2
0dbfs = 1

;instrument will be triggered by keyboard widget
instr 1
kEnv madsr .1, .2, .6, .4
aOut vco2 p5, p4
outs aOut*kEnv, aOut*kEnv
endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
</CsScore>
</CsoundSynthesizer>
