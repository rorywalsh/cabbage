<Cabbage>
form caption("Radio Buttons") size(300, 200), pluginId("RdBt"), guiRefresh(10)

button bounds(8, 122, 60, 25), channel("but10"), radioGroup(101), colour:1("blue"), value(1), text("Zero", "One")
button bounds(82, 122, 60, 25), channel("but11"), radioGroup(101), colour:1("blue"), text("Zero", "One")
button bounds(154, 122, 60, 25), channel("but12"), radioGroup(101), colour:1("blue"), text("Zero", "One")
checkbox bounds(8, 12, 102, 25), text("Check1"), channel("but1"), radioGroup(100), colour:1("red"), colour:0("blue"), value(1)
checkbox bounds(8, 38, 102, 25), text("Check2"), channel("but2"), radioGroup(100), colour:1("red"), colour:0("blue")
checkbox bounds(8, 64, 102, 25), text("Check3"), channel("but3"), radioGroup(100), colour:1("red"), colour:0("blue")
checkbox bounds(8, 90, 102, 25), text("Check4"), channel("but4"), radioGroup(100), colour:1("red"), colour:0("blue")
checkbox bounds(110, 12, 102, 25), text("Check5"), channel("but5"), radioGroup(100), colour:1("red"), colour:0("blue")
checkbox bounds(110, 38, 102, 25), text("Check6"), channel("but6"), radioGroup(100), colour:1("red"), colour:0("blue")
checkbox bounds(110, 64, 102, 25), text("Check7"), channel("but7"), radioGroup(100), colour:1("red"), colour:0("blue")
checkbox bounds(110, 90, 102, 25), text("Check8"), channel("but8"), radioGroup(100), colour:1("red"), colour:0("blue")
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
;check check boxes for change
kTrig changed chnget:k("but1"), chnget:k("but2"),chnget:k("but3"),chnget:k("but4"),chnget:k("but5"),chnget:k("but6"),chnget:k("but7"),chnget:k("but8") 
if kTrig==1 then
	printks "--\nCheck1:%d\n", 0, chnget:k("but1") 
	printks "Check2:%d\n", 0, chnget:k("but2") 
	printks "Check3:%d\n", 0, chnget:k("but3") 
	printks "Check4:%d\n", 0, chnget:k("but4") 
	printks "Check5:%d\n", 0, chnget:k("but5") 
	printks "Check6:%d\n", 0, chnget:k("but6") 
	printks "Check7:%d\n", 0, chnget:k("but7") 
	printks "Check8:%d\n", 0, chnget:k("but8") 
endif


;check regular buttons
kTrig changed chnget:k("but10"),chnget:k("but11"),chnget:k("but12") 
if kTrig==1 then
	printks "--\nButton1:%d\n", 0, chnget:k("but10") 
	printks "Button2:%d\n", 0, chnget:k("but11") 
	printks "Button3:%d\n", 0, chnget:k("but12") 
endif
endin
		
</CsInstruments>
<CsScore>
i 1 0 [3600*24*7]
</CsScore>
</CsoundSynthesizer>