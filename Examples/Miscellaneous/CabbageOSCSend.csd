<Cabbage>
form caption("OSCSend"), size(470, 260), pluginid("OSCS"), guirefresh(10)
rslider bounds(10, 10, 80, 80), channel("rslider1"), colour("red"), range(0, 1, 0, 1, .01)
rslider bounds(100, 10, 80, 80), channel("rslider2"), colour("red"), range(0, 1, 0, 1, .01)

</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-dm0 -n 
</CsOptions>
<CsInstruments>
sr 	= 	44100
ksmps 	= 	32
nchnls 	= 	2
0dbfs	=	1


instr 1
kData1 chnget "rslider1"
kData2 chnget "rslider2"


OSCsend   kData1+kData2, "localhost", 7770, "/foo/bar", "ff", kData1, kData2

endin

</CsInstruments>
<CsScore>
i 1 0 [3600*24*7]
</CsScore>
</CsoundSynthesizer>

