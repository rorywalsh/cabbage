<Cabbage>
form caption("Knobman Filmstrip Example") size(450, 180), colour(93, 93, 93), pluginId("KMSl")

image bounds(10, 23, 32, 107), file("roland SH 101 knob.png"), crop(0, 0, 32, 107), identChannel("sliderIdent1")
vslider bounds(10, 22, 27, 106) channel("vslider1") range(0, 1, 0, 1, 0.001) alpha(0)
image bounds(50, 23, 32, 107), file("roland SH 101 knob.png"), crop(0, 0, 32, 107), identChannel("sliderIdent2")
vslider bounds(50, 22, 32, 106) channel("vslider2") range(0, 1, 0, 1, 0.001) alpha(0)
image bounds(90, 23, 32, 107), file("roland SH 101 knob.png"), crop(0, 0, 32, 107), identChannel("sliderIdent3")
vslider bounds(90, 22, 32, 106) channel("vslider3") range(0, 1, 0, 1, 0.001) alpha(0)
image bounds(130, 23, 32, 107), file("roland SH 101 knob.png"), crop(0, 0, 32, 107), identChannel("sliderIdent4")
vslider bounds(130, 22, 32, 106) channel("vslider4") range(0, 1, 0, 1, 0.001) alpha(0)

image bounds(324, 24, 50, 50) file("BigKnobSplit.png") crop(0, 0, 110, 110), identChannel("rsliderIdent1")
rslider bounds(324, 24, 50, 50) channel("rslider1") range(0, 1, 0, 1, 0.001) alpha(0)
image bounds(390, 24, 50, 50) file("BigKnobSplit.png") crop(0, 0, 110, 110), identChannel("rsliderIdent2")
rslider bounds(390, 24, 50, 50) channel("rslider2") range(0, 1, 0, 1, 0.001) alpha(0)
image bounds(324, 74, 50, 50) file("BigKnobSplit.png") crop(0, 0, 110, 110), identChannel("rsliderIdent3")
rslider bounds(324, 74, 50, 50) channel("rslider3") range(0, 1, 0, 1, 0.001) alpha(0)
image bounds(390, 74, 50, 50) file("BigKnobSplit.png") crop(0, 0, 110, 110), identChannel("rsliderIdent4")
rslider bounds(390, 74, 50, 50) channel("rslider4") range(0, 1, 0, 1, 0.001) alpha(0)

image bounds(190, 18, 120, 120) crop(0, 0, 128, 128), file("meter_black.png") identChannel("vuMeterIdent")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 -m0d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
;sr is set by the host
ksmps = 32
nchnls = 2
0dbfs = 1

;this file uses slider imges found 
;http://www.g200kg.com/en/webknobman/gallery.php?m=p&p=987
;http://www.g200kg.com/en/webknobman/gallery.php?m=p&p=970
;http://www.g200kg.com/en/webknobman/gallery.php?m=p&p=54

opcode filmStrip, 0,Skii
	Sident, kPos, iWidth, iHeight xin 
	if changed(kPos) == 1 then
		SMessage sprintfk "crop(0, %d, %d, %d)", kPos, iWidth, iHeight
		chnset SMessage, Sident
	endif
endop 

instr 1
kvSlider1 chnget "vslider1"
kvSlider2 chnget "vslider2"
kvSlider3 chnget "vslider3"
kvSlider4 chnget "vslider4"

krSlider1 chnget "rslider1"
krSlider2 chnget "rslider2"
krSlider3 chnget "rslider3"
krSlider4 chnget "rslider4"


filmStrip "sliderIdent1", int(kvSlider1*30)*107, 32, 107
filmStrip "sliderIdent2", int(kvSlider2*30)*107, 32, 107
filmStrip "sliderIdent3", int(kvSlider3*30)*107, 32, 107
filmStrip "sliderIdent4", int(kvSlider4*30)*107, 32, 107

filmStrip "rsliderIdent1", int(krSlider1*30)*110, 110, 110
filmStrip "rsliderIdent2", int(krSlider2*30)*110, 110, 110
filmStrip "rsliderIdent3", int(krSlider3*30)*110, 110, 110
filmStrip "rsliderIdent4", int(krSlider4*30)*110, 110, 110

kVuMeter randi 1, 5
filmStrip "vuMeterIdent", int(abs(kVuMeter)*30)*128, 128, 128


endin

</CsInstruments>
<CsScore>
;causes Csound to run for about 7000 years...
f0 z
;starts instrument 1 and runs it for a week
i1 0 [60*60*24*7] 
</CsScore>
</CsoundSynthesizer>
