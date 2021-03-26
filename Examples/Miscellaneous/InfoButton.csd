<Cabbage>
form caption("Csound output") size(480,400), pluginId("InBt"), guiRefresh(10)
infobutton bounds(10, 10, 100, 50), file("example1.html"), text("Help"), colour("red"), fontColour("blue")
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

endin
		
</CsInstruments>
<CsScore>
i 1 0 [3600*24*7]
</CsScore>
</CsoundSynthesizer>