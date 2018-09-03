<Cabbage>
form size(400, 200), caption("Toggling Plants"), pluginid("TgPl"), guirefresh(10)
button bounds(12, 12, 60, 25), channel("but1"), text("Toggle", "Toggle")

groupbox bounds(80, 10, 200, 160), text("White Sliders"), visible(0), identchannel("sliders1"), plant("GUIabst_1"){
hslider bounds(0, 30, 200, 30), channel("hslider1"), range(0, 100, 55), colour("white")
hslider bounds(0, 60, 200, 30), channel("hslider2"), range(0, 100, 23), colour("white")
hslider bounds(0, 90, 200, 30), channel("hslider3"), range(0, 100, 45), colour("white")
hslider bounds(0, 120, 200, 30), channel("hslider4"), range(0, 100, 56), colour("white")
}

groupbox bounds(80, 10, 200, 160), text("Red Sliders"), visible(0), identchannel("sliders2"), plant("GUIabst_2"){
hslider bounds(0, 30, 200, 30), channel("hslider5"), range(0, 100, 25), colour("red")
hslider bounds(0, 60, 200, 30), channel("hslider6"), range(0, 100, 63), colour("red")
hslider bounds(0, 90, 200, 30), channel("hslider7"), range(0, 100, 15), colour("red")
hslider bounds(0, 120, 200, 30), channel("hslider8"), range(0, 100, 16), colour("red")
}

groupbox bounds(80, 10, 200, 160), text("Yellow Sliders"), visible(1), identchannel("sliders3"), plant("GUIabst_3"){
hslider bounds(0, 30, 200, 30), channel("hslider9"), range(0, 100, 5), colour("yellow")
hslider bounds(0, 60, 200, 30), channel("hslider10"), range(0, 100, 63), colour("yellow")
hslider bounds(0, 90, 200, 30), channel("hslider11"), range(0, 100, 25), colour("yellow")
hslider bounds(0, 120, 200, 30), channel("hslider12"), range(0, 100, 96), colour("yellow")
}

</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 64
nchnls = 2
0dbfs=1

instr 1
	kCnt init 1
	kTrig changed chnget:k("but1") 

	if kTrig==1 then
		Smessage sprintfk "visible(%d)", kCnt==1 ? 1 : 0
		chnset Smessage, "sliders1"
		Smessage sprintfk "visible(%d)", kCnt==2 ? 1 : 0
		chnset Smessage, "sliders2"
		Smessage sprintfk "visible(%d)", kCnt==3 ? 1 : 0
		chnset Smessage, "sliders3"
		kCnt = (kCnt>2 ? 1 : kCnt+1)
		printk2 kCnt
	endif
	
endin


</CsInstruments>  
<CsScore>
f1 0 2048 10 1 
i1 0 1000
</CsScore>
</CsoundSynthesizer>