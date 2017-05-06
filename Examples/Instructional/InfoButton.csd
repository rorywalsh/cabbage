<Cabbage>
form caption("Csound output") size(480,400), pluginID("SMo1"), guirefresh(10)
infobutton bounds(10, 10, 100, 50), file("example1.html"), text("Help"), colour("red"), fontcolour("blue")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>
sr 		= 	44100	;SAMPLE RATE
ksmps 		= 	32	;NUMBER OF AUDIO SAMPLES IN EACH CONTROL CYCLE
nchnls 		= 	2	;NUMBER OF CHANNELS (2=STEREO)
0dbfs		=	1

instr	1

endin
		
</CsInstruments>
<CsScore>
i 1 0 [3600*24*7]
</CsScore>
</CsoundSynthesizer>