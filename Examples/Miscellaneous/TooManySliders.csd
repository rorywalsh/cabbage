<Cabbage>
form size(520, 300), colour(20, 20, 20), caption("Too many sliders!"), pluginid("TMSd")

image bounds(0, 8, 500, 206), colour(0,0,0,0), plant("sliders"), identchannel("slidersPlant"){
vslider bounds(10,  10, 30, 200), text("1"), range(0, 100, 10), channel("s1")
vslider bounds(40,  10, 30, 200), text("2"), range(0, 100, 20), channel("s2")
vslider bounds(70,  10, 30, 200), text("3"), range(0, 100, 30), channel("s3")
vslider bounds(100, 10, 30, 200), text("4"), range(0, 100, 40), channel("s4")
vslider bounds(130, 10, 30, 200), text("5"), range(0, 100, 50), channel("s5")
vslider bounds(160, 10, 30, 200), text("6"), range(0, 100, 60), channel("s6")
vslider bounds(190, 10, 30, 200), text("7"), range(0, 100, 70), channel("s7")
vslider bounds(220, 10, 30, 200), text("8"), range(0, 100, 80), channel("s8")
vslider bounds(250, 10, 30, 200), text("9"), range(0, 100, 70), channel("s9")
vslider bounds(280, 10, 30, 200), text("10"), range(0, 100, 60), channel("s10")
vslider bounds(310, 10, 30, 200), text("11"), range(0, 100, 50), channel("s11")
vslider bounds(340, 10, 30, 200), text("12"), range(0, 100, 40), channel("s12")
vslider bounds(370, 10, 30, 200), text("13"), range(0, 100, 30), channel("s13")
vslider bounds(400, 10, 30, 200), text("14"), range(0, 100, 20), channel("s14")
vslider bounds(430, 10, 30, 200), text("15"), range(0, 100, 10), channel("s15")
vslider bounds(460, 10, 30, 200), text("16"), range(0, 100, 0), channel("s16")
vslider bounds(490, 10, 30, 200), text("17"), range(0, 100, 10), channel("s17")
vslider bounds(520, 10, 30, 200), text("18"), range(0, 100, 20), channel("s18")
vslider bounds(550, 10, 30, 200), text("19"), range(0, 100, 30), channel("s19")
vslider bounds(580, 10, 30, 200), text("20"), range(0, 100, 40), channel("s20")
vslider bounds(610, 10, 30, 200), text("21"), range(0, 100, 50), channel("s21")
vslider bounds(640, 10, 30, 200), text("22"), range(0, 100, 60), channel("s31")
vslider bounds(670, 10, 30, 200), text("23"), range(0, 100, 70), channel("s41")
vslider bounds(700, 10, 30, 200), text("24"), range(0, 100, 60), channel("s51")
vslider bounds(730, 10, 30, 200), text("25"), range(0, 100, 50), channel("s61")
vslider bounds(760, 10, 30, 200), text("26"), range(0, 100, 40), channel("s71")
vslider bounds(790, 10, 30, 200), text("27"), range(0, 100, 30), channel("s81")
vslider bounds(820, 10, 30, 200), text("28"), range(0, 100, 20), channel("s2341")
vslider bounds(850, 10, 30, 200), text("29"), range(0, 100, 10), channel("12s1")
vslider bounds(880, 10, 30, 200), text("30"), range(0, 100, 20), channel("1s2")
vslider bounds(910, 10, 30, 200), text("31"), range(0, 100, 30), channel("23s1")
vslider bounds(940, 10, 30, 200), text("32"), range(0, 100, 40), channel("1s1")
}

;slider to move plant left...
hslider bounds(24, 216, 473, 30), channel("scrollSlider"), popuptext(0), range(0, 1, 0.001)
;block off outer edges of plant when it moves
image bounds(500, 4, 20, 218), colour(20, 20, 20)
image bounds(0, 0, 20, 218), colour(20, 20, 20)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n --displays
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 16
nchnls = 2
0dbfs=1

instr 1
iLeftOffset = 490
kScroll chnget "scrollSlider"
if changed(kScroll)==1 then
	Smessage sprintfk "bounds(%d, 8, %d, 206)", 10-(kScroll*iLeftOffset), 500+(kScroll*iLeftOffset)
	chnset Smessage, "slidersPlant"
endif
endin


</CsInstruments>
<CsScore>
i1 0 3600
</CsScore>
</CsoundSynthesizer>
