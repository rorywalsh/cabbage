
<Cabbage>
form caption("Meters") size(400, 300), colour(20, 20, 20)
vmeter bounds(10, 8, 35, 198) channel("vMeter1") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 
vmeter bounds(50, 8, 35, 198) channel("vMeter2") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 
vmeter bounds(90, 8, 35, 198) channel("vMeter3") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 
vmeter bounds(130, 8, 35, 198) channel("vMeter4") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 

hmeter bounds(170, 10, 180, 35) channel("hMeter1") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 
hmeter bounds(170, 50, 180, 35) channel("hMeter2") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 
hmeter bounds(170, 90, 180, 35) channel("hMeter3") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 
hmeter bounds(170, 130, 180, 35) channel("hMeter4") value(0) overlaycolour(70, 53, 53, 255) metercolour:0(0, 255, 0, 255) metercolour:1(0, 103, 171, 255) metercolour:2(23, 0, 123, 255) outlinethickness(4) 


</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL   
</CsOptions>   
<CsInstruments>
; Initialize the global variables.   
sr = 44100  
ksmps = 32
nchnls = 2
0dbfs = 1  

instr 1

if metro(20) == 1 then
	chnset abs(randi:k(1.1, 100, 2)), "vMeter1"
	chnset abs(randi:k(1.1, 100, 2)), "vMeter2"
	chnset abs(randi:k(1.1, 100, 2)), "vMeter3"
	chnset abs(randi:k(1.1, 100, 2)), "vMeter4"

	chnset abs(randi:k(1.1, 100, 2)), "hMeter1"
	chnset abs(randi:k(1.1, 100, 2)), "hMeter2"
	chnset abs(randi:k(1.1, 100, 2)), "hMeter3"
	chnset abs(randi:k(1.1, 100, 2)), "hMeter4"
endif

endin

</CsInstruments>
<CsScore>
i1 0 10000
</CsScore>
</CsoundSynthesizer>
