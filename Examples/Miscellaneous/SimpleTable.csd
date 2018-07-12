<Cabbage>
form caption("Tables") size(400, 410), pluginid("STbl")
gentable bounds(5, 5, 370, 200), tablenumber(1), tablecolour("red"), identchannel("table")
hslider bounds(6, 210, 368, 30), channel("harm1"), range(0, 1, 1, 1, 0.01), colour(255, 255, 255, 255), trackercolour("red"), text("Harm1")
hslider bounds(6, 242, 368, 30), channel("harm2"), range(0, 1, 0, 1, 0.01), colour(255, 255, 255, 255), trackercolour("red"), text("Harm2")
hslider bounds(6, 274, 368, 30), channel("harm3"), range(0, 1, 0, 1, 0.01), colour(255, 255, 255, 255), trackercolour("red"), text("Harm3")
hslider bounds(6, 306, 368, 30), channel("harm4"), range(0, 1, 0, 1, 0.01), colour(255, 255, 255, 255), trackercolour("red"), text("Harm4")
hslider bounds(6, 338, 368, 30), channel("harm5"), range(0, 1, 0, 1, 0.01), colour(255, 255, 255, 255), trackercolour("red"), text("Harm5")
checkbox bounds(8, 378, 120, 20), channel("normal"), text("Normalise"), colour(255, 0, 0)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-d -n -m0d
</CsOptions>
<CsInstruments>
sr 		= 	44100
ksmps 		= 	32
nchnls 		= 	2
0dbfs		=	1
;Rory. 2014


instr	1
k1 chnget "harm1"
k2 chnget "harm2"
k3 chnget "harm3"
k4 chnget "harm4"
k5 chnget "harm5"

a1 oscili .5, 200, 1
outs a1, a1

kChanged changed k1, k2, k3, k4, k5
if kChanged==1 then
	;if a slider changes trigger instrument 2 to update table
	event "i", 2, 0, .01, k1, k2, k3, k4, k5
endif

endin

instr	2
iNormal = (chnget:i("normal")==0 ? -1 : 1)
iTable	ftgen	1, 0,   1024, 10*iNormal, p4, p5, p6, p7, p8
chnset	"tablenumber(1)", "table"	; update table display
endin

</CsInstruments>
<CsScore>
f1 0 1024 10 1
i1 0 [3600*24*7]
</CsScore>
</CsoundSynthesizer>
