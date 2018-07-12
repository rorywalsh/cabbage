<Cabbage>
form caption("Alpha") size(385, 220), pluginid("AlpF")
rslider bounds(14, 10, 160, 160), channel("hslider1"), colour("yellow") identchannel("hslider_ident1")
rslider bounds(200, 10, 160, 160), channel("hslider2"), colour("yellow") identchannel("hslider_ident2")
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n
</CsOptions>
<CsInstruments>
sr 		= 	44100
ksmps 		= 	32
nchnls 		= 	2
0dbfs		=	1
;Rory. 2014

;the alpha identifier allows one to control the alpha
;blend for an entire widget. 0 will make the widget insible
;while 1 will make it visible

instr	1
kTrig metro 10
kLeft oscili 1, .5, -1, .5
kRight oscili 1, .5, -1
if kTrig==1 then
	Smessage sprintfk "alpha(%f)", (kLeft+1)/2
	chnset Smessage, "hslider_ident1"

	Smessage sprintfk "alpha(%f)", (kRight+1)/2
	chnset Smessage, "hslider_ident2"
endif
endin

</CsInstruments>
<CsScore>
i1 0 [3600*24*7]
</CsScore>
</CsoundSynthesizer>
