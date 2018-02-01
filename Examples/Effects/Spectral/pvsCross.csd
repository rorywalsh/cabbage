; pvsCross.csd
; Written by Iain McCurdy, 2012.

; This effect implements cross-synthesis between the left and right inputs.
; By default, the left input is the 'source' signal and the right signal is the 'destination' signal.
;  This arrangement can be swapped with the swap button.

; LEFT----+
;         |
;      (SOURCE)                 +---LEFT
;         |                     |
;         +-CROSS-SYNTHESIS-----+
;         |                     |
;    (DESTINATION)              +---RIGHT
;         |
; RIGHT---+


; Source	-	level of the source signal sent to the output.
; Dest.		-	level of the destination signal sent to the output.
; Cross		-	level of the cross-synthesis signal sent to the output.
; Level		-	global control of the level of all signal that may be present at the output.

<Cabbage>
form caption("pvsCross") size(370, 90), pluginid("crss")
image            bounds(0, 0, 370, 90), colour("Cream"), outlinecolour("silver"), outlinethickness(5)

; filter switches
checkbox bounds( 10, 20, 94, 15), text("Swap Inputs"), channel("swap"), fontcolour(138, 54, 15), colour("lime")

label    bounds(15, 41, 60,13), text("FFT Size"), fontcolour(138, 54, 15)
combobox bounds(15, 55, 60,20), text("64","128","256","512","1024","2048","2048","4096","8192","16384"), channel("att_table"), value(5)

line     bounds(115, 10,165, 3), colour("Grey")

label   bounds(160,  5, 75, 12), text("M  I  X  E  R"), fontcolour(78, 24,  7), colour("Cream")
rslider bounds(100,18, 75, 65), text("Source"),     channel("src_lev"),   range(0, 1.00, 0),   textcolour(138, 54, 15), colour("chocolate"), trackercolour(138, 54, 15)
rslider bounds(160,18, 75, 65), text("Dest."),      channel("dst_lev"),   range(0, 1.00, 0),   textcolour(138, 54, 15), colour("chocolate"), trackercolour(138, 54, 15)
rslider bounds(220,18, 75, 65), text("Cross"),      channel("cross_lev"), range(0, 1.00, 1),   textcolour(138, 54, 15), colour("chocolate"), trackercolour(138, 54, 15)
rslider bounds(285, 8, 75, 75), text("Level"),      channel("lev"),       range(0, 1.00, 0.5), textcolour(138, 54, 15), colour("chocolate"), trackercolour(138, 54, 15)
</Cabbage>
<CsoundSynthesizer>

<CsOptions>
-d -n
</CsOptions>

<CsInstruments>
sr 		= 	44100
ksmps 		= 	64
nchnls 		= 	2
0dbfs		=	1	;MAXIMUM AMPLITUDE

;Iain McCurdy
;http://iainmccurdy.org/csound.html

/* FFT attribute tables */
giFFTattributes1	ftgen	0, 0, 4, -2,   64,  32,   64, 1
giFFTattributes2	ftgen	0, 0, 4, -2,  128,  64,  128, 1
giFFTattributes3	ftgen	0, 0, 4, -2,  256, 128,  256, 1
giFFTattributes4	ftgen	0, 0, 4, -2,  512, 128,  512, 1
giFFTattributes5	ftgen	0, 0, 4, -2, 1024, 128, 1024, 1
giFFTattributes6	ftgen	0, 0, 4, -2, 2048, 256, 2048, 1
giFFTattributes7	ftgen	0, 0, 4, -2, 2048,1024, 2048, 1
giFFTattributes8	ftgen	0, 0, 4, -2, 4096,1024, 4096, 1
giFFTattributes9	ftgen	0, 0, 4, -2, 8192,2048, 8192, 1
giFFTattributes10	ftgen	0, 0, 4, -2,16384,4096,16384, 1

opcode	pvscross_module,a,aakkkiiii
	a_src,a_dst,kampS,kampD,klev,iFFTsize,ioverlap,iwinsize,iwintype	xin
	f_src  	pvsanal	a_src, iFFTsize, ioverlap, iwinsize, iwintype		;ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
	f_dst  	pvsanal	a_dst, iFFTsize, ioverlap, iwinsize, iwintype		;ANALYSE AUDIO INPUT SIGNAL AND OUTPUT AN FSIG
	f_cross 	pvscross f_src, f_dst, kampS, kampD			;IMPLEMENT fsig CROSS SYNTHESIS
	aout		pvsynth f_cross                      				;RESYNTHESIZE THE f-SIGNAL AS AN AUDIO SIGNAL
			xout		aout*klev	
endop



instr	1

	katt_table	chnget	"att_table"	; FFT atribute table
	katt_table	init	5
	ktrig		changed	katt_table
	if ktrig==1 then
	 reinit update
	endif
	update:
	iFFTsize	table	0, giFFTattributes1 + i(katt_table) - 1
	ioverlap	table	1, giFFTattributes1 + i(katt_table) - 1
	iwinsize	table	2, giFFTattributes1 + i(katt_table) - 1
	iwintype	table	3, giFFTattributes1 + i(katt_table) - 1
		
	kfeedback	chnget	"FB"
	kampS		=	0;chnget	"ampS"
	kampD		=	1;chnget	"ampD"
	kporttime	linseg	0,0.001,0.02
	kmix		chnget	"mix"
	klev		chnget	"lev"
	ksrc_lev		chnget	"src_lev"
	kdst_lev		chnget	"dst_lev"
	kcross_lev		chnget	"cross_lev"
	klev		chnget	"lev"
	klisten		chnget	"listen"
	kswap		chnget	"swap"
	
	a1		inch	1
	a2		inch	2

	if kswap==0 then
	 a_src	=	a1
	 a_dst	=	a2
	else
	 a_src	=	a2
	 a_dst	=	a1
	endif
	
	aoutL		pvscross_module	a_src,a_dst,kampS,kampD,klev,iFFTsize,ioverlap,iwinsize,iwintype
	aoutR		pvscross_module	a_src,a_dst,kampS,kampD,klev,iFFTsize,ioverlap,iwinsize,iwintype

	aMixL	=	(aoutL*kcross_lev)+(a_src*ksrc_lev)+(a_dst*kdst_lev)
	aMixR	=	(aoutR*kcross_lev)+(a_src*ksrc_lev)+(a_dst*kdst_lev)

		outs	aMixL,aMixR
endin

</CsInstruments>

<CsScore>
i 1 0 [60*60*24*7]
</CsScore>

</CsoundSynthesizer>