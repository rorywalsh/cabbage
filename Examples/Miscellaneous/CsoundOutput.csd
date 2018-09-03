<Cabbage>
form caption("Csound output") size(480,400), pluginid("CSOp"), guirefresh(10)
csoundoutput bounds(8, 4, 467, 353), colour("black"), fontcolour("lime")
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
printks "Hello\n", 1
endin
		
</CsInstruments>
<CsScore>
i 1 0 [3600*24*7]
</CsScore>
</CsoundSynthesizer>