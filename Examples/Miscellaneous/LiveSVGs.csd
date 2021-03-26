<Cabbage>
form caption("Live SVGs") size(330, 180), colour("black"), pluginId("LSVG"), svgpath(".")

rslider bounds(12, 14, 130, 122), channel("rslider"), range(0, 1, 0), identChannel("svgIdent")
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

; the following example

instr	1
if metro(1)==1 then
	kRand random 0, 10000
	chnset sprintfk("svgdebug(%d)", kRand), "svgIdent"
endif
endin
		
</CsInstruments>
<CsScore>
i 1 0 [3600*24*7]
</CsScore>
</CsoundSynthesizer>