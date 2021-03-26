<Cabbage>
form caption("Popup Plants") size(285, 100), pluginId("PupP"), guiRefresh(10)
groupbox bounds(10, 76, 220, 130), text("Popup Plant"), plant("pop1"), popup(1), colour(20, 20, 20), identChannel("pops") {
rslider bounds(6, 24, 50, 50), channel("rslider1"), range(0, 100, 10)
rslider bounds(58, 24, 50, 50), channel("rslider2"), range(0, 100, 20)
rslider bounds(110, 24, 50, 50), channel("rslider3"), range(0, 100, 30)
rslider bounds(162, 24, 50, 50), channel("rslider4"), range(0, 100, 40)
rslider bounds(6, 76, 50, 50), channel("rslider5"), range(0, 100, 50)
rslider bounds(58, 76, 50, 50), channel("rslider6"), range(0, 100, 60)
rslider bounds(110, 76, 50, 50), channel("rslider7"), range(0, 100, 70)
rslider bounds(162, 76, 50, 50), channel("rslider8"), range(0, 100, 80)
}
button bounds(40, 22, 221, 25), channel("but1"), alpha(1), text("Show popup plant")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>
;sr is set by the host
ksmps 		= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1

instr	1
kTrig chnget "but1"
if changed:k(kTrig)==1 then
	chnset "visible(1)", "pops"
endif
endin
		
</CsInstruments>
<CsScore>
i 1 0 [3600*24*7]
</CsScore>
</CsoundSynthesizer>