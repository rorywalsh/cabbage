<Cabbage>
form caption("OSCRec"), size(270, 160), pluginId("OSCR"), guiRefresh(10)
rslider bounds(10, 10, 110, 110), channel("test1"), range(0, 1, 0, 1, 0.01)
rslider bounds(140, 10, 110, 110), channel("test2"), range(0, 1, 0, 1, 0.01)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-dm0 -n 
</CsOptions>
<CsInstruments>
;sr is set by the host
ksmps 	= 	32
nchnls 	= 	2
0dbfs	=	1

gihandle OSCinit 7770
  
instr 1
kf1 init 0
kf2 init 0
kk  OSClisten gihandle, "/foo/bar", "ff", kf1, kf2
printk2 kf1
chnset kf1, "test1"
chnset kf2,  "test2"
endin

</CsInstruments>
<CsScore>
i 1 0 [3600*24*7]
</CsScore>
</CsoundSynthesizer>
